// Copyright lostpanda. All Rights Reserved.

#include "Utils/HardwareIdHelper.h"
#include "Utils/DataHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include <iphlpapi.h>
#include <intrin.h>
#include "Windows/HideWindowsPlatformTypes.h"
#pragma comment(lib, "iphlpapi.lib")
#endif

// ==================== 单项硬件ID ====================

FString FHardwareIdHelper::GetMotherboardSerial()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("baseboard get serialnumber"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetMotherboardManufacturer()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("baseboard get manufacturer"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetMotherboardProduct()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("baseboard get product"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetCpuId()
{
#if PLATFORM_WINDOWS
	int CpuInfo[4] = { -1 };
	__cpuid(CpuInfo, 1);

	return FString::Printf(TEXT("%08X%08X"), CpuInfo[3], CpuInfo[0]);
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetCpuName()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("cpu get name"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetDiskSerial()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("diskdrive get serialnumber"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetMacAddress()
{
#if PLATFORM_WINDOWS
	TArray<FString> MacAddresses = GetAllMacAddresses();
	if (MacAddresses.Num() > 0)
	{
		return MacAddresses[0];
	}
	return FString();
#else
	return FString();
#endif
}

TArray<FString> FHardwareIdHelper::GetAllMacAddresses()
{
	TArray<FString> MacAddresses;

#if PLATFORM_WINDOWS
	ULONG BufferSize = 0;
	GetAdaptersInfo(nullptr, &BufferSize);

	if (BufferSize == 0)
	{
		return MacAddresses;
	}

	TArray<uint8> Buffer;
	Buffer.SetNumUninitialized(BufferSize);
	PIP_ADAPTER_INFO AdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(Buffer.GetData());

	if (GetAdaptersInfo(AdapterInfo, &BufferSize) == NO_ERROR)
	{
		PIP_ADAPTER_INFO CurrentAdapter = AdapterInfo;
		while (CurrentAdapter)
		{
			// 只获取以太网和无线网卡的 MAC 地址
			if (CurrentAdapter->Type == MIB_IF_TYPE_ETHERNET ||
				CurrentAdapter->Type == IF_TYPE_IEEE80211)
			{
				if (CurrentAdapter->AddressLength == 6)
				{
					FString MacAddress = FString::Printf(
						TEXT("%02X:%02X:%02X:%02X:%02X:%02X"),
						CurrentAdapter->Address[0],
						CurrentAdapter->Address[1],
						CurrentAdapter->Address[2],
						CurrentAdapter->Address[3],
						CurrentAdapter->Address[4],
						CurrentAdapter->Address[5]
					);
					MacAddresses.Add(MacAddress);
				}
			}
			CurrentAdapter = CurrentAdapter->Next;
		}
	}
#endif

	return MacAddresses;
}

FString FHardwareIdHelper::GetBiosSerial()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("bios get serialnumber"));
#else
	return FString();
#endif
}

FString FHardwareIdHelper::GetSystemUuid()
{
#if PLATFORM_WINDOWS
	return ExecuteWmicCommand(TEXT("csproduct get uuid"));
#else
	return FString();
#endif
}

// ==================== 组合硬件指纹 ====================

FString FHardwareIdHelper::GetHardwareFingerprint()
{
	return GetCustomHardwareFingerprint(true, true, true, true, false);
}

FString FHardwareIdHelper::GetCustomHardwareFingerprint(
	bool bIncludeMotherboard,
	bool bIncludeCpu,
	bool bIncludeDisk,
	bool bIncludeMac,
	bool bIncludeBios)
{
	FString CombinedData;

	if (bIncludeMotherboard)
	{
		CombinedData += GetMotherboardSerial();
		CombinedData += TEXT("|");
	}

	if (bIncludeCpu)
	{
		CombinedData += GetCpuId();
		CombinedData += TEXT("|");
	}

	if (bIncludeDisk)
	{
		CombinedData += GetDiskSerial();
		CombinedData += TEXT("|");
	}

	if (bIncludeMac)
	{
		CombinedData += GetMacAddress();
		CombinedData += TEXT("|");
	}

	if (bIncludeBios)
	{
		CombinedData += GetBiosSerial();
	}

	// 如果没有任何数据，返回空字符串
	if (CombinedData.IsEmpty())
	{
		return FString();
	}

	// 使用 SHA256 生成指纹
	return FDataHelper::SHA256Hash(CombinedData);
}

FString FHardwareIdHelper::GetHardwareInfoJson()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	// 主板信息
	TSharedPtr<FJsonObject> MotherboardObj = MakeShareable(new FJsonObject);
	MotherboardObj->SetStringField(TEXT("serial"), GetMotherboardSerial());
	MotherboardObj->SetStringField(TEXT("manufacturer"), GetMotherboardManufacturer());
	MotherboardObj->SetStringField(TEXT("product"), GetMotherboardProduct());
	JsonObject->SetObjectField(TEXT("motherboard"), MotherboardObj);

	// CPU 信息
	TSharedPtr<FJsonObject> CpuObj = MakeShareable(new FJsonObject);
	CpuObj->SetStringField(TEXT("id"), GetCpuId());
	CpuObj->SetStringField(TEXT("name"), GetCpuName());
	JsonObject->SetObjectField(TEXT("cpu"), CpuObj);

	// 硬盘信息
	JsonObject->SetStringField(TEXT("diskSerial"), GetDiskSerial());

	// 网卡信息
	TArray<TSharedPtr<FJsonValue>> MacArray;
	TArray<FString> MacAddresses = GetAllMacAddresses();
	for (const FString& Mac : MacAddresses)
	{
		MacArray.Add(MakeShareable(new FJsonValueString(Mac)));
	}
	JsonObject->SetArrayField(TEXT("macAddresses"), MacArray);

	// BIOS 信息
	JsonObject->SetStringField(TEXT("biosSerial"), GetBiosSerial());

	// 系统 UUID
	JsonObject->SetStringField(TEXT("systemUuid"), GetSystemUuid());

	// 硬件指纹
	JsonObject->SetStringField(TEXT("fingerprint"), GetHardwareFingerprint());

	// 序列化为 JSON 字符串
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

// ==================== Windows 平台辅助函数 ====================

#if PLATFORM_WINDOWS
FString FHardwareIdHelper::ExecuteWmiQuery(const FString& WmiClass, const FString& PropertyName)
{
	// 使用 WMIC 命令行工具作为简化实现
	FString Command = FString::Printf(TEXT("%s get %s"), *WmiClass, *PropertyName);
	return ExecuteWmicCommand(Command);
}

FString FHardwareIdHelper::ExecuteWmicCommand(const FString& WmicCommand)
{
	FString Result;

	// 构建完整的 WMIC 命令
	FString FullCommand = FString::Printf(TEXT("wmic %s"), *WmicCommand);

	// 创建管道读取命令输出
	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;

	if (!FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
	{
		return Result;
	}

	// 执行命令
	FProcHandle ProcessHandle = FPlatformProcess::CreateProc(
		TEXT("cmd.exe"),
		*FString::Printf(TEXT("/c %s"), *FullCommand),
		false,  // bLaunchDetached
		true,   // bLaunchHidden
		true,   // bLaunchReallyHidden
		nullptr,
		0,
		nullptr,
		WritePipe,
		ReadPipe
	);

	if (ProcessHandle.IsValid())
	{
		// 等待进程完成
		FPlatformProcess::WaitForProc(ProcessHandle);

		// 读取输出
		Result = FPlatformProcess::ReadPipe(ReadPipe);

		// 关闭进程句柄
		FPlatformProcess::CloseProc(ProcessHandle);
	}

	// 关闭管道
	FPlatformProcess::ClosePipe(ReadPipe, WritePipe);

	// 清理输出结果
	// WMIC 输出通常包含标题行和空行，需要提取实际值
	TArray<FString> Lines;
	Result.ParseIntoArrayLines(Lines);

	if (Lines.Num() >= 2)
	{
		// 第一行是标题，第二行是值
		Result = Lines[1].TrimStartAndEnd();
	}
	else if (Lines.Num() == 1)
	{
		Result = Lines[0].TrimStartAndEnd();
	}
	else
	{
		Result.Empty();
	}

	return Result;
}
#endif
