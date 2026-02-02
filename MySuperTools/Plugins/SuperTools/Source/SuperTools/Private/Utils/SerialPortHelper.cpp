// Copyright lostpanda. All Rights Reserved.

#include "Utils/SerialPortHelper.h"
#include "SuperTools.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

/**
 * 串口信息结构 (内部使用)
 */
struct FSerialPortInfo
{
#if PLATFORM_WINDOWS
	HANDLE PortHandle;
#else
	void* PortHandle;
#endif
	FString PortName;
	FSerialPortConfig Config;
	FOnSerialDataReceived AsyncCallback;
	FRunnableThread* AsyncThread;
	bool bAsyncRunning;
	bool bStopRequested;

	FSerialPortInfo()
		: PortHandle(nullptr)
		, AsyncThread(nullptr)
		, bAsyncRunning(false)
		, bStopRequested(false)
	{
	}
};

/**
 * 异步读取线程
 */
class FSerialAsyncReader : public FRunnable
{
public:
	FSerialAsyncReader(TSharedPtr<FSerialPortInfo> InPortInfo, int32 InHandle)
		: PortInfo(InPortInfo)
		, Handle(InHandle)
	{
	}

	virtual uint32 Run() override
	{
#if PLATFORM_WINDOWS
		TArray<uint8> Buffer;
		Buffer.SetNumUninitialized(1024);

		while (!PortInfo->bStopRequested)
		{
			DWORD BytesRead = 0;
			BOOL bSuccess = ReadFile(
				PortInfo->PortHandle,
				Buffer.GetData(),
				Buffer.Num(),
				&BytesRead,
				nullptr
			);

			if (bSuccess && BytesRead > 0)
			{
				TArray<uint8> ReceivedData;
				ReceivedData.Append(Buffer.GetData(), BytesRead);

				if (PortInfo->AsyncCallback.IsBound())
				{
					// 在游戏线程执行回调
					AsyncTask(ENamedThreads::GameThread, [this, ReceivedData]()
					{
						if (PortInfo.IsValid() && PortInfo->AsyncCallback.IsBound())
						{
							PortInfo->AsyncCallback.Execute(Handle, ReceivedData);
						}
					});
				}
			}
			else
			{
				// 短暂休眠避免 CPU 占用过高
				FPlatformProcess::Sleep(0.01f);
			}
		}
#endif
		return 0;
	}

	virtual void Stop() override
	{
		PortInfo->bStopRequested = true;
	}

private:
	TSharedPtr<FSerialPortInfo> PortInfo;
	int32 Handle;
};

// 静态成员初始化
TMap<int32, TSharedPtr<FSerialPortInfo>> FSerialPortHelper::OpenPorts;
int32 FSerialPortHelper::NextHandleId = 1;
FCriticalSection FSerialPortHelper::PortLock;

int32 FSerialPortHelper::OpenPort(const FString& PortName, const FSerialPortConfig& Config)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	// 构建端口路径
	FString PortPath = FString::Printf(TEXT("\\\\.\\%s"), *PortName);

	// 打开串口
	HANDLE PortHandle = CreateFileW(
		*PortPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (PortHandle == INVALID_HANDLE_VALUE)
	{
		DWORD Error = GetLastError();
		UE_LOG(LogSuperTools, Error, TEXT("串口打开失败: %s, 错误码: %d"), *PortName, Error);
		return -1;
	}

	// 配置串口参数
	DCB dcb;
	FMemory::Memzero(&dcb, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);

	if (!GetCommState(PortHandle, &dcb))
	{
		UE_LOG(LogSuperTools, Error, TEXT("获取串口状态失败: %s"), *PortName);
		CloseHandle(PortHandle);
		return -1;
	}

	dcb.BaudRate = Config.BaudRate;
	dcb.ByteSize = Config.DataBits;

	// 设置奇偶校验
	switch (Config.Parity)
	{
	case ESerialParity::None:	dcb.Parity = NOPARITY; break;
	case ESerialParity::Odd:	dcb.Parity = ODDPARITY; break;
	case ESerialParity::Even:	dcb.Parity = EVENPARITY; break;
	case ESerialParity::Mark:	dcb.Parity = MARKPARITY; break;
	case ESerialParity::Space:	dcb.Parity = SPACEPARITY; break;
	}

	// 设置停止位
	switch (Config.StopBits)
	{
	case ESerialStopBits::One:			dcb.StopBits = ONESTOPBIT; break;
	case ESerialStopBits::OnePointFive:	dcb.StopBits = ONE5STOPBITS; break;
	case ESerialStopBits::Two:			dcb.StopBits = TWOSTOPBITS; break;
	}

	// 流控制
	if (Config.bEnableFlowControl)
	{
		dcb.fOutxCtsFlow = 1;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
	else
	{
		dcb.fOutxCtsFlow = 0;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
	}

	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutX = 0;
	dcb.fInX = 0;

	if (!SetCommState(PortHandle, &dcb))
	{
		UE_LOG(LogSuperTools, Error, TEXT("设置串口参数失败: %s"), *PortName);
		CloseHandle(PortHandle);
		return -1;
	}

	// 设置超时
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = Config.ReadTimeoutMs;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = Config.WriteTimeoutMs;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(PortHandle, &timeouts))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("设置串口超时失败: %s"), *PortName);
	}

	// 清空缓冲区
	PurgeComm(PortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);

	// 创建端口信息
	TSharedPtr<FSerialPortInfo> PortInfo = MakeShared<FSerialPortInfo>();
	PortInfo->PortHandle = PortHandle;
	PortInfo->PortName = PortName;
	PortInfo->Config = Config;

	// 分配句柄
	int32 Handle = NextHandleId++;
	OpenPorts.Add(Handle, PortInfo);

	UE_LOG(LogSuperTools, Log, TEXT("串口打开成功: %s, 句柄: %d, 波特率: %d"), *PortName, Handle, Config.BaudRate);

	return Handle;
#else
	UE_LOG(LogSuperTools, Warning, TEXT("串口功能仅在 Windows 平台可用"));
	return -1;
#endif
}

int32 FSerialPortHelper::OpenPortSimple(const FString& PortName, int32 BaudRate)
{
	FSerialPortConfig Config;
	Config.BaudRate = BaudRate;
	return OpenPort(PortName, Config);
}

bool FSerialPortHelper::ClosePort(int32 Handle)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		UE_LOG(LogSuperTools, Warning, TEXT("串口关闭失败: 无效的句柄 %d"), Handle);
		return false;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	// 停止异步读取
	if (PortInfo->bAsyncRunning)
	{
		PortInfo->bStopRequested = true;
		if (PortInfo->AsyncThread)
		{
			PortInfo->AsyncThread->WaitForCompletion();
			delete PortInfo->AsyncThread;
			PortInfo->AsyncThread = nullptr;
		}
		PortInfo->bAsyncRunning = false;
	}

	// 关闭句柄
	if (PortInfo->PortHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(PortInfo->PortHandle);
		PortInfo->PortHandle = INVALID_HANDLE_VALUE;
	}

	FString PortName = PortInfo->PortName;
	OpenPorts.Remove(Handle);

	UE_LOG(LogSuperTools, Log, TEXT("串口已关闭: %s, 句柄: %d"), *PortName, Handle);

	return true;
#else
	return false;
#endif
}

void FSerialPortHelper::CloseAllPorts()
{
	TArray<int32> Handles;
	{
		FScopeLock Lock(&PortLock);
		OpenPorts.GetKeys(Handles);
	}

	for (int32 Handle : Handles)
	{
		ClosePort(Handle);
	}

	UE_LOG(LogSuperTools, Log, TEXT("所有串口已关闭"));
}

bool FSerialPortHelper::IsPortOpen(int32 Handle)
{
	FScopeLock Lock(&PortLock);
	return OpenPorts.Contains(Handle);
}

bool FSerialPortHelper::Write(int32 Handle, const TArray<uint8>& Data)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		UE_LOG(LogSuperTools, Warning, TEXT("串口写入失败: 无效的句柄 %d"), Handle);
		return false;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	DWORD BytesWritten = 0;
	BOOL bSuccess = WriteFile(
		PortInfo->PortHandle,
		Data.GetData(),
		Data.Num(),
		&BytesWritten,
		nullptr
	);

	if (bSuccess && BytesWritten == (DWORD)Data.Num())
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("串口写入成功: %d 字节, 句柄: %d"), BytesWritten, Handle);
		return true;
	}
	else
	{
		UE_LOG(LogSuperTools, Warning, TEXT("串口写入失败: 句柄 %d, 预期 %d 字节, 实际 %d 字节"), Handle, Data.Num(), BytesWritten);
		return false;
	}
#else
	return false;
#endif
}

bool FSerialPortHelper::WriteString(int32 Handle, const FString& Message)
{
	FTCHARToUTF8 Converter(*Message);
	TArray<uint8> Data;
	Data.Append((const uint8*)Converter.Get(), Converter.Length());
	return Write(Handle, Data);
}

TArray<uint8> FSerialPortHelper::Read(int32 Handle, int32 MaxBytes)
{
	TArray<uint8> Result;

#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		UE_LOG(LogSuperTools, Warning, TEXT("串口读取失败: 无效的句柄 %d"), Handle);
		return Result;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	Result.SetNumUninitialized(MaxBytes);

	DWORD BytesRead = 0;
	BOOL bSuccess = ReadFile(
		PortInfo->PortHandle,
		Result.GetData(),
		MaxBytes,
		&BytesRead,
		nullptr
	);

	if (bSuccess && BytesRead > 0)
	{
		Result.SetNum(BytesRead);
		UE_LOG(LogSuperTools, Verbose, TEXT("串口读取成功: %d 字节, 句柄: %d"), BytesRead, Handle);
	}
	else
	{
		Result.Empty();
	}
#endif

	return Result;
}

FString FSerialPortHelper::ReadString(int32 Handle, int32 MaxBytes)
{
	TArray<uint8> Data = Read(Handle, MaxBytes);
	if (Data.Num() > 0)
	{
		// 添加 null 终止符
		Data.Add(0);
		return FString(UTF8_TO_TCHAR(Data.GetData()));
	}
	return FString();
}

int32 FSerialPortHelper::GetBytesAvailable(int32 Handle)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		return -1;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	COMSTAT comStat;
	DWORD errors;
	if (ClearCommError(PortInfo->PortHandle, &errors, &comStat))
	{
		return comStat.cbInQue;
	}
#endif
	return -1;
}

bool FSerialPortHelper::FlushBuffers(int32 Handle, bool bClearInput, bool bClearOutput)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		return false;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	DWORD flags = 0;
	if (bClearInput) flags |= PURGE_RXCLEAR;
	if (bClearOutput) flags |= PURGE_TXCLEAR;

	return PurgeComm(PortInfo->PortHandle, flags) != 0;
#else
	return false;
#endif
}

bool FSerialPortHelper::StartAsyncRead(int32 Handle, const FOnSerialDataReceived& Callback)
{
#if PLATFORM_WINDOWS
	FScopeLock Lock(&PortLock);

	TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
	if (!PortInfoPtr || !PortInfoPtr->IsValid())
	{
		UE_LOG(LogSuperTools, Warning, TEXT("启动异步读取失败: 无效的句柄 %d"), Handle);
		return false;
	}

	TSharedPtr<FSerialPortInfo> PortInfo = *PortInfoPtr;

	if (PortInfo->bAsyncRunning)
	{
		UE_LOG(LogSuperTools, Warning, TEXT("异步读取已在运行: 句柄 %d"), Handle);
		return false;
	}

	PortInfo->AsyncCallback = Callback;
	PortInfo->bStopRequested = false;
	PortInfo->bAsyncRunning = true;

	FSerialAsyncReader* Reader = new FSerialAsyncReader(PortInfo, Handle);
	PortInfo->AsyncThread = FRunnableThread::Create(
		Reader,
		*FString::Printf(TEXT("SerialAsyncReader_%d"), Handle),
		0,
		TPri_Normal
	);

	UE_LOG(LogSuperTools, Log, TEXT("异步读取已启动: 句柄 %d"), Handle);

	return true;
#else
	return false;
#endif
}

void FSerialPortHelper::StopAsyncRead(int32 Handle)
{
#if PLATFORM_WINDOWS
	TSharedPtr<FSerialPortInfo> PortInfo;
	FRunnableThread* ThreadToWait = nullptr;

	{
		FScopeLock Lock(&PortLock);

		TSharedPtr<FSerialPortInfo>* PortInfoPtr = OpenPorts.Find(Handle);
		if (!PortInfoPtr || !PortInfoPtr->IsValid())
		{
			return;
		}

		PortInfo = *PortInfoPtr;

		if (!PortInfo->bAsyncRunning)
		{
			return;
		}

		PortInfo->bStopRequested = true;
		ThreadToWait = PortInfo->AsyncThread;
	}

	// Wait outside the lock to avoid deadlock
	if (ThreadToWait)
	{
		ThreadToWait->WaitForCompletion();

		FScopeLock Lock(&PortLock);
		delete PortInfo->AsyncThread;
		PortInfo->AsyncThread = nullptr;
		PortInfo->bAsyncRunning = false;
	}

	UE_LOG(LogSuperTools, Log, TEXT("异步读取已停止: 句柄 %d"), Handle);
#endif
}

TArray<FString> FSerialPortHelper::GetAvailablePorts()
{
	TArray<FString> Ports;

#if PLATFORM_WINDOWS
	// 枚举 COM1 到 COM256
	for (int32 i = 1; i <= 256; ++i)
	{
		FString PortName = FString::Printf(TEXT("COM%d"), i);
		FString PortPath = FString::Printf(TEXT("\\\\.\\%s"), *PortName);

		HANDLE hPort = CreateFileW(
			*PortPath,
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr
		);

		if (hPort != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hPort);
			Ports.Add(PortName);
		}
		else
		{
			DWORD Error = GetLastError();
			// ERROR_ACCESS_DENIED 表示端口存在但被占用
			if (Error == ERROR_ACCESS_DENIED)
			{
				Ports.Add(PortName + TEXT(" (占用)"));
			}
		}
	}
#endif

	return Ports;
}

int32 FSerialPortHelper::GetOpenPortCount()
{
	FScopeLock Lock(&PortLock);
	return OpenPorts.Num();
}
