// Copyright lostpanda. All Rights Reserved.

#include "SuperToolsBlueprintLibrary.h"
#include "Utils/IniFileHelper.h"
#include "Utils/WindowsAPIHelper.h"
#include "Utils/UdpHelper.h"
#include "Utils/SerialPortHelper.h"

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
