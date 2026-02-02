// Copyright lostpanda. All Rights Reserved.

#include "SuperToolsBlueprintLibrary.h"
#include "Utils/IniFileHelper.h"
#include "Utils/WindowsAPIHelper.h"

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

#endif // PLATFORM_WINDOWS
