// Copyright lostpanda. All Rights Reserved.

#include "SuperToolsBlueprintLibrary.h"
#include "Utils/IniFileHelper.h"
#include "Utils/WindowsAPIHelper.h"
#include "Utils/UdpHelper.h"
#include "Utils/SerialPortHelper.h"
#include "Utils/ClipboardHelper.h"
#include "Utils/FileDialogHelper.h"
#include "Utils/SystemHelper.h"
#include "Utils/DataHelper.h"
#include "Utils/JsonHelper.h"
#include "Utils/FileIOHelper.h"
#include "Utils/ScreenshotHelper.h"
#include "Utils/HttpLatentActions.h"
#include "Utils/HardwareIdHelper.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

// ==================== INI 文件操作 ====================

FString USuperToolsBlueprintLibrary::ReadIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue)
{
	return FIniFileHelper::ReadString(FilePath, Section, Key, DefaultValue);
}

int32 USuperToolsBlueprintLibrary::ReadIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue)
{
	return FIniFileHelper::ReadInt(FilePath, Section, Key, DefaultValue);
}

float USuperToolsBlueprintLibrary::ReadIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue)
{
	return FIniFileHelper::ReadFloat(FilePath, Section, Key, DefaultValue);
}

bool USuperToolsBlueprintLibrary::ReadIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue)
{
	return FIniFileHelper::ReadBool(FilePath, Section, Key, DefaultValue);
}

bool USuperToolsBlueprintLibrary::WriteIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value)
{
	return FIniFileHelper::WriteString(FilePath, Section, Key, Value);
}

bool USuperToolsBlueprintLibrary::WriteIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value)
{
	return FIniFileHelper::WriteInt(FilePath, Section, Key, Value);
}

bool USuperToolsBlueprintLibrary::WriteIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value)
{
	return FIniFileHelper::WriteFloat(FilePath, Section, Key, Value);
}

bool USuperToolsBlueprintLibrary::WriteIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value)
{
	return FIniFileHelper::WriteBool(FilePath, Section, Key, Value);
}

// ==================== Windows API 操作 ====================

#if PLATFORM_WINDOWS

bool USuperToolsBlueprintLibrary::SetWindowTitle(const FString& Title)
{
	return FWindowsAPIHelper::SetWindowTitle(nullptr, Title);
}

FString USuperToolsBlueprintLibrary::GetWindowTitle()
{
	return FWindowsAPIHelper::GetWindowTitle(nullptr);
}

bool USuperToolsBlueprintLibrary::GetScreenResolution(int32& Width, int32& Height)
{
	return FWindowsAPIHelper::GetScreenResolution(Width, Height);
}

bool USuperToolsBlueprintLibrary::SetWindowPosition(int32 X, int32 Y)
{
	return FWindowsAPIHelper::SetWindowPosition(nullptr, X, Y);
}

bool USuperToolsBlueprintLibrary::SetWindowSize(int32 Width, int32 Height)
{
	return FWindowsAPIHelper::SetWindowSize(nullptr, Width, Height);
}

bool USuperToolsBlueprintLibrary::MaximizeWindow()
{
	return FWindowsAPIHelper::MaximizeWindow(nullptr);
}

bool USuperToolsBlueprintLibrary::MinimizeWindow()
{
	return FWindowsAPIHelper::MinimizeWindow(nullptr);
}

bool USuperToolsBlueprintLibrary::RestoreWindow()
{
	return FWindowsAPIHelper::RestoreWindow(nullptr);
}

bool USuperToolsBlueprintLibrary::BringWindowToFront()
{
	return FWindowsAPIHelper::BringWindowToFront(nullptr);
}

#else // Non-Windows platforms

bool USuperToolsBlueprintLibrary::SetWindowTitle(const FString& Title)
{
	return false;
}

FString USuperToolsBlueprintLibrary::GetWindowTitle()
{
	return FString();
}

bool USuperToolsBlueprintLibrary::GetScreenResolution(int32& Width, int32& Height)
{
	Width = 0;
	Height = 0;
	return false;
}

bool USuperToolsBlueprintLibrary::SetWindowPosition(int32 X, int32 Y)
{
	return false;
}

bool USuperToolsBlueprintLibrary::SetWindowSize(int32 Width, int32 Height)
{
	return false;
}

bool USuperToolsBlueprintLibrary::MaximizeWindow()
{
	return false;
}

bool USuperToolsBlueprintLibrary::MinimizeWindow()
{
	return false;
}

bool USuperToolsBlueprintLibrary::RestoreWindow()
{
	return false;
}

bool USuperToolsBlueprintLibrary::BringWindowToFront()
{
	return false;
}

#endif // PLATFORM_WINDOWS

// ==================== UDP 通信 ====================

bool USuperToolsBlueprintLibrary::UdpSendBytes(const FString& IP, int32 Port, const TArray<uint8>& Data)
{
	return FUdpHelper::SendTo(IP, Port, Data);
}

bool USuperToolsBlueprintLibrary::UdpSendString(const FString& IP, int32 Port, const FString& Message)
{
	return FUdpHelper::SendStringTo(IP, Port, Message);
}

int32 USuperToolsBlueprintLibrary::CreateUdpListener(int32 Port, int32 MaxBufferSize)
{
	return FUdpHelper::CreateListenerWithBuffer(Port, MaxBufferSize);
}

void USuperToolsBlueprintLibrary::DestroyUdpListener(int32 Handle)
{
	FUdpHelper::DestroyListener(Handle);
}

bool USuperToolsBlueprintLibrary::IsUdpListenerValid(int32 Handle)
{
	return FUdpHelper::IsListenerValid(Handle);
}

bool USuperToolsBlueprintLibrary::HasUdpData(int32 Handle)
{
	return FUdpHelper::HasReceivedData(Handle);
}

int32 USuperToolsBlueprintLibrary::GetUdpPacketCount(int32 Handle)
{
	return FUdpHelper::GetReceivedPacketCount(Handle);
}

TArray<FUdpReceivedPacket> USuperToolsBlueprintLibrary::GetAllUdpPackets(int32 Handle)
{
	return FUdpHelper::GetAndClearReceivedPackets(Handle);
}

bool USuperToolsBlueprintLibrary::GetLatestUdpPacket(int32 Handle, FUdpReceivedPacket& OutPacket)
{
	return FUdpHelper::GetLatestPacket(Handle, OutPacket);
}

void USuperToolsBlueprintLibrary::ClearUdpBuffer(int32 Handle)
{
	FUdpHelper::ClearReceivedBuffer(Handle);
}

FString USuperToolsBlueprintLibrary::BytesToString(const TArray<uint8>& Data)
{
	if (Data.Num() == 0)
	{
		return FString();
	}

	// 将 UTF-8 字节数组转换为 FString
	FUTF8ToTCHAR Converter((const ANSICHAR*)Data.GetData(), Data.Num());
	return FString(Converter.Length(), Converter.Get());
}

// ==================== UDP 简化接口 ====================

int32 USuperToolsBlueprintLibrary::StartUdpReceive(int32 Port)
{
	return FUdpHelper::CreateListenerWithBuffer(Port, 100);
}

void USuperToolsBlueprintLibrary::StopUdpReceive(int32 Handle)
{
	FUdpHelper::DestroyListener(Handle);
}

bool USuperToolsBlueprintLibrary::GetUdpMessage(int32 Handle, FString& OutMessage, FString& OutSenderIP, int32& OutSenderPort)
{
	FUdpReceivedPacket Packet;
	if (!FUdpHelper::GetLatestPacket(Handle, Packet))
	{
		OutMessage = FString();
		OutSenderIP = FString();
		OutSenderPort = 0;
		return false;
	}

	// 清空缓冲区
	FUdpHelper::ClearReceivedBuffer(Handle);

	// 转换为字符串
	OutMessage = BytesToString(Packet.Data);
	OutSenderIP = Packet.SenderIP;
	OutSenderPort = Packet.SenderPort;
	return true;
}

TArray<FString> USuperToolsBlueprintLibrary::GetAllUdpMessages(int32 Handle)
{
	TArray<FString> Messages;
	TArray<FUdpReceivedPacket> Packets = FUdpHelper::GetAndClearReceivedPackets(Handle);

	for (const FUdpReceivedPacket& Packet : Packets)
	{
		Messages.Add(BytesToString(Packet.Data));
	}

	return Messages;
}

// ==================== 串口通信 ====================

int32 USuperToolsBlueprintLibrary::OpenSerialPort(const FString& PortName, int32 BaudRate)
{
	return FSerialPortHelper::OpenPortSimple(PortName, BaudRate);
}

int32 USuperToolsBlueprintLibrary::OpenSerialPortWithConfig(const FString& PortName, const FSerialPortConfig& Config)
{
	return FSerialPortHelper::OpenPort(PortName, Config);
}

bool USuperToolsBlueprintLibrary::CloseSerialPort(int32 Handle)
{
	return FSerialPortHelper::ClosePort(Handle);
}

bool USuperToolsBlueprintLibrary::IsSerialPortOpen(int32 Handle)
{
	return FSerialPortHelper::IsPortOpen(Handle);
}

bool USuperToolsBlueprintLibrary::SerialWriteBytes(int32 Handle, const TArray<uint8>& Data)
{
	return FSerialPortHelper::Write(Handle, Data);
}

bool USuperToolsBlueprintLibrary::SerialWriteString(int32 Handle, const FString& Message)
{
	return FSerialPortHelper::WriteString(Handle, Message);
}

TArray<uint8> USuperToolsBlueprintLibrary::SerialReadBytes(int32 Handle, int32 MaxBytes)
{
	return FSerialPortHelper::Read(Handle, MaxBytes);
}

FString USuperToolsBlueprintLibrary::SerialReadString(int32 Handle, int32 MaxBytes)
{
	return FSerialPortHelper::ReadString(Handle, MaxBytes);
}

int32 USuperToolsBlueprintLibrary::GetSerialBytesAvailable(int32 Handle)
{
	return FSerialPortHelper::GetBytesAvailable(Handle);
}

bool USuperToolsBlueprintLibrary::FlushSerialBuffers(int32 Handle, bool bClearInput, bool bClearOutput)
{
	return FSerialPortHelper::FlushBuffers(Handle, bClearInput, bClearOutput);
}

TArray<FString> USuperToolsBlueprintLibrary::GetAvailableSerialPorts()
{
	return FSerialPortHelper::GetAvailablePorts();
}

// ==================== 剪贴板操作 ====================

bool USuperToolsBlueprintLibrary::CopyToClipboard(const FString& Text)
{
	return FClipboardHelper::CopyToClipboard(Text);
}

bool USuperToolsBlueprintLibrary::GetFromClipboard(FString& OutText)
{
	return FClipboardHelper::GetFromClipboard(OutText);
}

bool USuperToolsBlueprintLibrary::HasClipboardText()
{
	return FClipboardHelper::HasClipboardText();
}

bool USuperToolsBlueprintLibrary::ClearClipboard()
{
	return FClipboardHelper::ClearClipboard();
}

bool USuperToolsBlueprintLibrary::CopyImageToClipboard(const FString& ImagePath)
{
	return FClipboardHelper::CopyImageToClipboard(ImagePath);
}

bool USuperToolsBlueprintLibrary::GetImageFromClipboard(const FString& SavePath)
{
	return FClipboardHelper::GetImageFromClipboard(SavePath);
}

bool USuperToolsBlueprintLibrary::HasClipboardImage()
{
	return FClipboardHelper::HasClipboardImage();
}

// ==================== 文件对话框 ====================

bool USuperToolsBlueprintLibrary::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath)
{
	return FFileDialogHelper::OpenFileDialog(DialogTitle, DefaultPath, FileTypes, OutFilePath);
}

bool USuperToolsBlueprintLibrary::OpenFileDialogMultiple(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFilePaths)
{
	return FFileDialogHelper::OpenFileDialogMultiple(DialogTitle, DefaultPath, FileTypes, OutFilePaths);
}

bool USuperToolsBlueprintLibrary::SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFileName, const FString& FileTypes, FString& OutFilePath)
{
	return FFileDialogHelper::SaveFileDialog(DialogTitle, DefaultPath, DefaultFileName, FileTypes, OutFilePath);
}

bool USuperToolsBlueprintLibrary::OpenFolderDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderPath)
{
	return FFileDialogHelper::OpenFolderDialog(DialogTitle, DefaultPath, OutFolderPath);
}

// ==================== 系统操作 ====================

bool USuperToolsBlueprintLibrary::OpenURL(const FString& URL)
{
	return FSystemHelper::OpenURL(URL);
}

bool USuperToolsBlueprintLibrary::OpenFolderInExplorer(const FString& FolderPath)
{
	return FSystemHelper::OpenFolder(FolderPath);
}

bool USuperToolsBlueprintLibrary::OpenFileWithDefaultApp(const FString& FilePath)
{
	return FSystemHelper::OpenFile(FilePath);
}

bool USuperToolsBlueprintLibrary::LaunchApplication(const FString& ExecutablePath, const FString& Arguments, bool bHidden)
{
	return FSystemHelper::LaunchApplication(ExecutablePath, Arguments, bHidden);
}

FString USuperToolsBlueprintLibrary::GetEnvVariable(const FString& VariableName)
{
	return FSystemHelper::GetEnvironmentVariable(VariableName);
}

FString USuperToolsBlueprintLibrary::GetFormattedTime(const FString& Format)
{
	return FSystemHelper::GetFormattedTime(Format);
}

FString USuperToolsBlueprintLibrary::GetComputerName()
{
	return FSystemHelper::GetComputerName();
}

FString USuperToolsBlueprintLibrary::GetCurrentUserName()
{
	return FSystemHelper::GetUserName();
}

// ==================== 数据处理 ====================

FString USuperToolsBlueprintLibrary::Base64Encode(const FString& Input)
{
	return FDataHelper::Base64Encode(Input);
}

bool USuperToolsBlueprintLibrary::Base64Decode(const FString& Input, FString& OutDecoded)
{
	return FDataHelper::Base64Decode(Input, OutDecoded);
}

FString USuperToolsBlueprintLibrary::MD5Hash(const FString& Input)
{
	return FDataHelper::MD5Hash(Input);
}

FString USuperToolsBlueprintLibrary::SHA256Hash(const FString& Input)
{
	return FDataHelper::SHA256Hash(Input);
}

bool USuperToolsBlueprintLibrary::MD5HashFile(const FString& FilePath, FString& OutHash)
{
	return FDataHelper::MD5HashFile(FilePath, OutHash);
}

bool USuperToolsBlueprintLibrary::SHA256HashFile(const FString& FilePath, FString& OutHash)
{
	return FDataHelper::SHA256HashFile(FilePath, OutHash);
}

// ==================== JSON 操作 ====================

FString USuperToolsBlueprintLibrary::JsonGetString(const FString& JsonString, const FString& FieldName, const FString& DefaultValue)
{
	return FJsonHelper::GetStringField(JsonString, FieldName, DefaultValue);
}

int32 USuperToolsBlueprintLibrary::JsonGetInt(const FString& JsonString, const FString& FieldName, int32 DefaultValue)
{
	return FJsonHelper::GetIntField(JsonString, FieldName, DefaultValue);
}

float USuperToolsBlueprintLibrary::JsonGetFloat(const FString& JsonString, const FString& FieldName, float DefaultValue)
{
	return FJsonHelper::GetFloatField(JsonString, FieldName, DefaultValue);
}

bool USuperToolsBlueprintLibrary::JsonGetBool(const FString& JsonString, const FString& FieldName, bool DefaultValue)
{
	return FJsonHelper::GetBoolField(JsonString, FieldName, DefaultValue);
}

bool USuperToolsBlueprintLibrary::JsonGetStringArray(const FString& JsonString, const FString& FieldName, TArray<FString>& OutArray)
{
	return FJsonHelper::GetStringArrayField(JsonString, FieldName, OutArray);
}

FString USuperToolsBlueprintLibrary::MakeJsonString(const FString& Key, const FString& Value)
{
	return FJsonHelper::MakeJsonString(Key, Value);
}

FString USuperToolsBlueprintLibrary::MapToJson(const TMap<FString, FString>& StringMap, bool bPrettyPrint)
{
	return FJsonHelper::MapToJsonString(StringMap, bPrettyPrint);
}

bool USuperToolsBlueprintLibrary::JsonToMap(const FString& JsonString, TMap<FString, FString>& OutMap)
{
	return FJsonHelper::JsonStringToMap(JsonString, OutMap);
}

// ==================== 文件 I/O ====================

bool USuperToolsBlueprintLibrary::ReadTextFile(const FString& FilePath, FString& OutContent)
{
	return FFileIOHelper::ReadTextFile(FilePath, OutContent);
}

bool USuperToolsBlueprintLibrary::WriteTextFile(const FString& FilePath, const FString& Content, bool bAppend)
{
	return FFileIOHelper::WriteTextFile(FilePath, Content, bAppend);
}

bool USuperToolsBlueprintLibrary::ReadFileLines(const FString& FilePath, TArray<FString>& OutLines)
{
	return FFileIOHelper::ReadLines(FilePath, OutLines);
}

bool USuperToolsBlueprintLibrary::DoesFileExist(const FString& FilePath)
{
	return FFileIOHelper::FileExists(FilePath);
}

bool USuperToolsBlueprintLibrary::DoesDirectoryExist(const FString& DirectoryPath)
{
	return FFileIOHelper::DirectoryExists(DirectoryPath);
}

bool USuperToolsBlueprintLibrary::CreateDirectoryPath(const FString& DirectoryPath)
{
	return FFileIOHelper::CreateDirectory(DirectoryPath);
}

bool USuperToolsBlueprintLibrary::DeleteFileAtPath(const FString& FilePath)
{
	return FFileIOHelper::DeleteFile(FilePath);
}

bool USuperToolsBlueprintLibrary::CopyFileToPath(const FString& SourcePath, const FString& DestPath, bool bOverwrite)
{
	return FFileIOHelper::CopyFile(SourcePath, DestPath, bOverwrite);
}

int64 USuperToolsBlueprintLibrary::GetFileSizeBytes(const FString& FilePath)
{
	return FFileIOHelper::GetFileSize(FilePath);
}

bool USuperToolsBlueprintLibrary::GetFilesInDir(const FString& DirectoryPath, TArray<FString>& OutFiles, const FString& Extension, bool bRecursive)
{
	return FFileIOHelper::GetFilesInDirectory(DirectoryPath, OutFiles, Extension, bRecursive);
}

FString USuperToolsBlueprintLibrary::GetFileExt(const FString& FilePath)
{
	return FFileIOHelper::GetFileExtension(FilePath);
}

FString USuperToolsBlueprintLibrary::GetFileNameFromPath(const FString& FilePath, bool bWithExtension)
{
	return FFileIOHelper::GetFileName(FilePath, bWithExtension);
}

bool USuperToolsBlueprintLibrary::WriteFileLines(const FString& FilePath, const TArray<FString>& Lines, bool bAppend)
{
	return FFileIOHelper::WriteLines(FilePath, Lines, bAppend);
}

bool USuperToolsBlueprintLibrary::ReadBinaryFile(const FString& FilePath, TArray<uint8>& OutData)
{
	return FFileIOHelper::ReadBinaryFile(FilePath, OutData);
}

bool USuperToolsBlueprintLibrary::WriteBinaryFile(const FString& FilePath, const TArray<uint8>& Data)
{
	return FFileIOHelper::WriteBinaryFile(FilePath, Data);
}

bool USuperToolsBlueprintLibrary::DeleteDirectoryAtPath(const FString& DirectoryPath)
{
	return FFileIOHelper::DeleteDirectory(DirectoryPath);
}

bool USuperToolsBlueprintLibrary::MoveFileToPath(const FString& SourcePath, const FString& DestPath)
{
	return FFileIOHelper::MoveFile(SourcePath, DestPath);
}

bool USuperToolsBlueprintLibrary::GetDirectoriesInDir(const FString& DirectoryPath, TArray<FString>& OutDirectories, bool bRecursive)
{
	return FFileIOHelper::GetDirectoriesInDirectory(DirectoryPath, OutDirectories, bRecursive);
}

FString USuperToolsBlueprintLibrary::GetFileDir(const FString& FilePath)
{
	return FFileIOHelper::GetFileDirectory(FilePath);
}

// ==================== 截图 ====================

bool USuperToolsBlueprintLibrary::CaptureViewportToFile(const FString& FilePath, bool bShowUI)
{
	return FScreenshotHelper::CaptureViewport(FilePath, bShowUI);
}

void USuperToolsBlueprintLibrary::RequestScreenshotToFile(const FString& FilePath, bool bShowUI)
{
	FScreenshotHelper::RequestScreenshot(FilePath, bShowUI);
}

bool USuperToolsBlueprintLibrary::CaptureScreenToFile(const FString& FilePath)
{
	return FScreenshotHelper::CaptureScreen(FilePath);
}

bool USuperToolsBlueprintLibrary::CaptureRegionToFile(const FString& FilePath, int32 X, int32 Y, int32 Width, int32 Height)
{
	return FScreenshotHelper::CaptureRegion(FilePath, X, Y, Width, Height);
}

// ==================== HTTP 请求 ====================

void USuperToolsBlueprintLibrary::HttpGet(UObject* WorldContextObject, const FString& URL, FString& OutResponse, int32& OutResponseCode, bool& bOutSuccess, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FHttpGetLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
				new FHttpGetLatentAction(URL, OutResponse, OutResponseCode, bOutSuccess, LatentInfo));
		}
	}
}

void USuperToolsBlueprintLibrary::HttpPost(UObject* WorldContextObject, const FString& URL, const FString& Content, const FString& ContentType, FString& OutResponse, int32& OutResponseCode, bool& bOutSuccess, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FHttpPostLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FString ActualContentType = ContentType.IsEmpty() ? TEXT("application/json") : ContentType;
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
				new FHttpPostLatentAction(URL, Content, ActualContentType, OutResponse, OutResponseCode, bOutSuccess, LatentInfo));
		}
	}
}

void USuperToolsBlueprintLibrary::HttpDownloadFile(UObject* WorldContextObject, const FString& URL, const FString& SavePath, int32& OutResponseCode, bool& bOutSuccess, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FHttpDownloadLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
				new FHttpDownloadLatentAction(URL, SavePath, OutResponseCode, bOutSuccess, LatentInfo));
		}
	}
}

// ==================== 硬件ID ====================

FString USuperToolsBlueprintLibrary::GetMotherboardSerial()
{
	return FHardwareIdHelper::GetMotherboardSerial();
}

FString USuperToolsBlueprintLibrary::GetMotherboardManufacturer()
{
	return FHardwareIdHelper::GetMotherboardManufacturer();
}

FString USuperToolsBlueprintLibrary::GetMotherboardProduct()
{
	return FHardwareIdHelper::GetMotherboardProduct();
}

FString USuperToolsBlueprintLibrary::GetCpuId()
{
	return FHardwareIdHelper::GetCpuId();
}

FString USuperToolsBlueprintLibrary::GetCpuName()
{
	return FHardwareIdHelper::GetCpuName();
}

FString USuperToolsBlueprintLibrary::GetDiskSerial()
{
	return FHardwareIdHelper::GetDiskSerial();
}

FString USuperToolsBlueprintLibrary::GetMacAddress()
{
	return FHardwareIdHelper::GetMacAddress();
}

TArray<FString> USuperToolsBlueprintLibrary::GetAllMacAddresses()
{
	return FHardwareIdHelper::GetAllMacAddresses();
}

FString USuperToolsBlueprintLibrary::GetBiosSerial()
{
	return FHardwareIdHelper::GetBiosSerial();
}

FString USuperToolsBlueprintLibrary::GetSystemUuid()
{
	return FHardwareIdHelper::GetSystemUuid();
}

FString USuperToolsBlueprintLibrary::GetHardwareFingerprint()
{
	return FHardwareIdHelper::GetHardwareFingerprint();
}

FString USuperToolsBlueprintLibrary::GetCustomHardwareFingerprint(bool bIncludeMotherboard, bool bIncludeCpu, bool bIncludeDisk, bool bIncludeMac, bool bIncludeBios)
{
	return FHardwareIdHelper::GetCustomHardwareFingerprint(bIncludeMotherboard, bIncludeCpu, bIncludeDisk, bIncludeMac, bIncludeBios);
}

FString USuperToolsBlueprintLibrary::GetHardwareInfoJson()
{
	return FHardwareIdHelper::GetHardwareInfoJson();
}
