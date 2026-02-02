// Copyright lostpanda. All Rights Reserved.

#include "Utils/UdpHelper.h"
#include "SuperTools.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "SocketSubsystem.h"
#include "Sockets.h"

// 静态成员初始化
TMap<int32, TSharedPtr<FUdpListenerInfo>> FUdpHelper::Listeners;
int32 FUdpHelper::NextHandleId = 1;
FCriticalSection FUdpHelper::ListenerLock;

bool FUdpHelper::SendTo(const FString& IP, int32 Port, const TArray<uint8>& Data)
{
	if (Data.Num() == 0)
	{
		UE_LOG(LogSuperTools, Warning, TEXT("UDP 发送失败: 数据为空"));
		return false;
	}

	// 创建临时 Socket 用于发送
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		UE_LOG(LogSuperTools, Error, TEXT("UDP 发送失败: 无法获取 Socket 子系统"));
		return false;
	}

	FSocket* Socket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("UDP Send Socket"), false);
	if (!Socket)
	{
		UE_LOG(LogSuperTools, Error, TEXT("UDP 发送失败: 无法创建 Socket"));
		return false;
	}

	// 解析目标地址
	FIPv4Address TargetAddress;
	if (!FIPv4Address::Parse(IP, TargetAddress))
	{
		UE_LOG(LogSuperTools, Error, TEXT("UDP 发送失败: 无效的 IP 地址 %s"), *IP);
		SocketSubsystem->DestroySocket(Socket);
		return false;
	}

	FIPv4Endpoint Endpoint(TargetAddress, Port);

	// 发送数据
	int32 BytesSent = 0;
	bool bSuccess = Socket->SendTo(Data.GetData(), Data.Num(), BytesSent, *Endpoint.ToInternetAddr());

	if (bSuccess && BytesSent == Data.Num())
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("UDP 发送成功: %d 字节到 %s:%d"), BytesSent, *IP, Port);
	}
	else
	{
		UE_LOG(LogSuperTools, Warning, TEXT("UDP 发送失败: 目标 %s:%d, 预期 %d 字节, 实际发送 %d 字节"), *IP, Port, Data.Num(), BytesSent);
		bSuccess = false;
	}

	// 清理 Socket
	SocketSubsystem->DestroySocket(Socket);

	return bSuccess;
}

bool FUdpHelper::SendStringTo(const FString& IP, int32 Port, const FString& Message)
{
	// 将字符串转换为 UTF-8 字节数组
	FTCHARToUTF8 Converter(*Message);
	TArray<uint8> Data;
	Data.Append((const uint8*)Converter.Get(), Converter.Length());

	return SendTo(IP, Port, Data);
}

int32 FUdpHelper::CreateListener(int32 Port, const FOnUdpDataReceived& Callback)
{
	FScopeLock Lock(&ListenerLock);

	// 创建监听 Socket
	FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);

	FSocket* Socket = FUdpSocketBuilder(TEXT("UDP Listener"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(65535)
		.Build();

	if (!Socket)
	{
		UE_LOG(LogSuperTools, Error, TEXT("UDP 监听器创建失败: 无法绑定到端口 %d"), Port);
		return -1;
	}

	// 创建监听器信息
	TSharedPtr<FUdpListenerInfo> ListenerInfo = MakeShared<FUdpListenerInfo>();
	ListenerInfo->Socket = Socket;
	ListenerInfo->Callback = Callback;
	ListenerInfo->Port = Port;

	// 分配句柄
	int32 Handle = NextHandleId++;

	// 创建接收器
	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	ListenerInfo->Receiver = new FUdpSocketReceiver(
		Socket,
		ThreadWaitTime,
		*FString::Printf(TEXT("UDP Receiver %d"), Handle)
	);

	// 绑定接收回调
	ListenerInfo->Receiver->OnDataReceived().BindStatic(&FUdpHelper::HandleDataReceived, Handle);

	// 启动接收器
	ListenerInfo->Receiver->Start();

	// 保存到映射表
	Listeners.Add(Handle, ListenerInfo);

	UE_LOG(LogSuperTools, Log, TEXT("UDP 监听器创建成功: 句柄 %d, 端口 %d"), Handle, Port);

	return Handle;
}

void FUdpHelper::DestroyListener(int32 Handle)
{
	FScopeLock Lock(&ListenerLock);

	TSharedPtr<FUdpListenerInfo>* ListenerInfoPtr = Listeners.Find(Handle);
	if (!ListenerInfoPtr || !ListenerInfoPtr->IsValid())
	{
		UE_LOG(LogSuperTools, Warning, TEXT("UDP 监听器销毁失败: 无效的句柄 %d"), Handle);
		return;
	}

	TSharedPtr<FUdpListenerInfo> ListenerInfo = *ListenerInfoPtr;

	// 停止并删除接收器
	if (ListenerInfo->Receiver)
	{
		ListenerInfo->Receiver->Stop();
		delete ListenerInfo->Receiver;
		ListenerInfo->Receiver = nullptr;
	}

	// 关闭并销毁 Socket
	if (ListenerInfo->Socket)
	{
		ListenerInfo->Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerInfo->Socket);
		ListenerInfo->Socket = nullptr;
	}

	// 从映射表移除
	Listeners.Remove(Handle);

	UE_LOG(LogSuperTools, Log, TEXT("UDP 监听器已销毁: 句柄 %d"), Handle);
}

void FUdpHelper::DestroyAllListeners()
{
	TArray<int32> Handles;
	{
		FScopeLock Lock(&ListenerLock);
		Listeners.GetKeys(Handles);
	}

	// 逐个销毁，避免死锁
	for (int32 Handle : Handles)
	{
		DestroyListener(Handle);
	}

	UE_LOG(LogSuperTools, Log, TEXT("所有 UDP 监听器已销毁"));
}

bool FUdpHelper::IsListenerValid(int32 Handle)
{
	FScopeLock Lock(&ListenerLock);
	return Listeners.Contains(Handle);
}

int32 FUdpHelper::GetActiveListenerCount()
{
	FScopeLock Lock(&ListenerLock);
	return Listeners.Num();
}

void FUdpHelper::HandleDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Sender, int32 Handle)
{
	FOnUdpDataReceived CallbackCopy;
	FString SenderIP;
	int32 SenderPort;
	TArray<uint8> ReceivedData;

	{
		FScopeLock Lock(&ListenerLock);

		TSharedPtr<FUdpListenerInfo>* ListenerInfoPtr = Listeners.Find(Handle);
		if (!ListenerInfoPtr || !ListenerInfoPtr->IsValid())
		{
			return;
		}

		TSharedPtr<FUdpListenerInfo> ListenerInfo = *ListenerInfoPtr;

		// 提取发送方信息
		SenderIP = Sender.Address.ToString();
		SenderPort = Sender.Port;

		// 复制数据 - 从 FArrayReader 读取
		int64 DataSize = Data->TotalSize();
		if (DataSize > 0)
		{
			ReceivedData.SetNumUninitialized(DataSize);
			Data->Seek(0);
			Data->Serialize(ReceivedData.GetData(), DataSize);
		}

		// 复制回调
		CallbackCopy = ListenerInfo->Callback;
	}

	// 在锁外执行回调，避免死锁
	if (CallbackCopy.IsBound())
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("UDP 接收数据: %d 字节, 来自 %s:%d"), ReceivedData.Num(), *SenderIP, SenderPort);
		CallbackCopy.Execute(SenderIP, SenderPort, ReceivedData);
	}
}
