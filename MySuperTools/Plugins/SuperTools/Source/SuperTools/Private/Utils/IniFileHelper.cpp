// Copyright lostpanda. All Rights Reserved.

#include "Utils/IniFileHelper.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "SuperTools.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

FString FIniFileHelper::GetFullPath(const FString& FilePath)
{
	// If it's an absolute path, return directly
	if (FPaths::IsRelative(FilePath) == false)
	{
		return FilePath;
	}

	// Relative path, convert to absolute path under project directory
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), FilePath);
}

bool FIniFileHelper::FileExists(const FString& FilePath)
{
	FString FullPath = GetFullPath(FilePath);
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath);
}

// ==================== Read Operations ====================

FString FIniFileHelper::ReadString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

#if PLATFORM_WINDOWS
	// Use Windows API for flexible INI file reading from any path
	TCHAR Buffer[4096];
	DWORD Result = GetPrivateProfileString(
		*Section,
		*Key,
		*DefaultValue,
		Buffer,
		4096,
		*FullPath
	);

	if (Result > 0 || DefaultValue.IsEmpty())
	{
		FString Value(Buffer);
		UE_LOG(LogSuperTools, Verbose, TEXT("Read INI string: [%s]%s = %s"), *Section, *Key, *Value);
		return Value;
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI key not found, using default: [%s]%s"), *Section, *Key);
	return DefaultValue;
#else
	// Fallback for non-Windows platforms: manual parsing
	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI file not found: %s"), *FullPath);
		return DefaultValue;
	}

	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		return DefaultValue;
	}

	FString CurrentSection;
	const FString TargetSection = FString::Printf(TEXT("[%s]"), *Section);

	for (const FString& Line : Lines)
	{
		FString TrimmedLine = Line.TrimStartAndEnd();

		if (TrimmedLine.IsEmpty() || TrimmedLine.StartsWith(TEXT(";")) || TrimmedLine.StartsWith(TEXT("#")))
		{
			continue;
		}

		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			CurrentSection = TrimmedLine;
			continue;
		}

		if (CurrentSection.Equals(TargetSection, ESearchCase::IgnoreCase))
		{
			int32 EqualsIndex;
			if (TrimmedLine.FindChar(TEXT('='), EqualsIndex))
			{
				FString KeyName = TrimmedLine.Left(EqualsIndex).TrimStartAndEnd();
				if (KeyName.Equals(Key, ESearchCase::IgnoreCase))
				{
					FString Value = TrimmedLine.Mid(EqualsIndex + 1).TrimStartAndEnd();
					UE_LOG(LogSuperTools, Verbose, TEXT("Read INI string: [%s]%s = %s"), *Section, *Key, *Value);
					return Value;
				}
			}
		}
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI key not found, using default: [%s]%s"), *Section, *Key);
	return DefaultValue;
#endif
}

int32 FIniFileHelper::ReadInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

#if PLATFORM_WINDOWS
	int32 Value = GetPrivateProfileInt(
		*Section,
		*Key,
		DefaultValue,
		*FullPath
	);

	UE_LOG(LogSuperTools, Verbose, TEXT("Read INI int: [%s]%s = %d"), *Section, *Key, Value);
	return Value;
#else
	FString StringValue = ReadString(FilePath, Section, Key, FString::FromInt(DefaultValue));
	return FCString::Atoi(*StringValue);
#endif
}

float FIniFileHelper::ReadFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue)
{
	FString StringValue = ReadString(FilePath, Section, Key, FString::SanitizeFloat(DefaultValue));
	float Value = FCString::Atof(*StringValue);
	UE_LOG(LogSuperTools, Verbose, TEXT("Read INI float: [%s]%s = %f"), *Section, *Key, Value);
	return Value;
}

bool FIniFileHelper::ReadBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue)
{
	FString StringValue = ReadString(FilePath, Section, Key, DefaultValue ? TEXT("true") : TEXT("false"));

	// Support multiple boolean representations
	bool Value = StringValue.Equals(TEXT("true"), ESearchCase::IgnoreCase) ||
	             StringValue.Equals(TEXT("1")) ||
	             StringValue.Equals(TEXT("yes"), ESearchCase::IgnoreCase) ||
	             StringValue.Equals(TEXT("on"), ESearchCase::IgnoreCase);

	UE_LOG(LogSuperTools, Verbose, TEXT("Read INI bool: [%s]%s = %s"), *Section, *Key, Value ? TEXT("true") : TEXT("false"));
	return Value;
}

// ==================== Write Operations ====================

bool FIniFileHelper::WriteString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value)
{
	FString FullPath = GetFullPath(FilePath);

#if PLATFORM_WINDOWS
	// Use Windows API for flexible INI file writing to any path
	BOOL Result = WritePrivateProfileString(
		*Section,
		*Key,
		*Value,
		*FullPath
	);

	if (Result)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Write INI string: [%s]%s = %s"), *Section, *Key, *Value);
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("Failed to write INI: [%s]%s, Error: %d"), *Section, *Key, GetLastError());
	return false;
#else
	// Fallback for non-Windows platforms: load, modify, save
	TArray<FString> Lines;
	FFileHelper::LoadFileToStringArray(Lines, *FullPath); // OK if file doesn't exist

	FString TargetSection = FString::Printf(TEXT("[%s]"), *Section);
	FString NewLine = FString::Printf(TEXT("%s=%s"), *Key, *Value);

	bool bFoundSection = false;
	bool bFoundKey = false;
	int32 SectionEndIndex = -1;

	for (int32 i = 0; i < Lines.Num(); ++i)
	{
		FString TrimmedLine = Lines[i].TrimStartAndEnd();

		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			if (bFoundSection && !bFoundKey)
			{
				// Insert before next section
				Lines.Insert(NewLine, i);
				bFoundKey = true;
				break;
			}
			bFoundSection = TrimmedLine.Equals(TargetSection, ESearchCase::IgnoreCase);
			if (bFoundSection)
			{
				SectionEndIndex = i;
			}
		}
		else if (bFoundSection)
		{
			SectionEndIndex = i;
			int32 EqualsIndex;
			if (TrimmedLine.FindChar(TEXT('='), EqualsIndex))
			{
				FString KeyName = TrimmedLine.Left(EqualsIndex).TrimStartAndEnd();
				if (KeyName.Equals(Key, ESearchCase::IgnoreCase))
				{
					Lines[i] = NewLine;
					bFoundKey = true;
					break;
				}
			}
		}
	}

	if (!bFoundSection)
	{
		// Add new section at end
		if (Lines.Num() > 0 && !Lines.Last().IsEmpty())
		{
			Lines.Add(TEXT(""));
		}
		Lines.Add(TargetSection);
		Lines.Add(NewLine);
	}
	else if (!bFoundKey)
	{
		// Add key after section
		Lines.Insert(NewLine, SectionEndIndex + 1);
	}

	bool bSuccess = FFileHelper::SaveStringArrayToFile(Lines, *FullPath);
	if (bSuccess)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Write INI string: [%s]%s = %s"), *Section, *Key, *Value);
	}
	return bSuccess;
#endif
}

bool FIniFileHelper::WriteInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value)
{
	return WriteString(FilePath, Section, Key, FString::FromInt(Value));
}

bool FIniFileHelper::WriteFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value)
{
	return WriteString(FilePath, Section, Key, FString::SanitizeFloat(Value));
}

bool FIniFileHelper::WriteBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value)
{
	return WriteString(FilePath, Section, Key, Value ? TEXT("true") : TEXT("false"));
}

// ==================== Delete Operations ====================

bool FIniFileHelper::RemoveKey(const FString& FilePath, const FString& Section, const FString& Key)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI file not found: %s"), *FullPath);
		return false;
	}

#if PLATFORM_WINDOWS
	// Pass NULL as value to delete the key
	BOOL Result = WritePrivateProfileString(
		*Section,
		*Key,
		NULL,
		*FullPath
	);

	if (Result)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Remove INI key: [%s]%s"), *Section, *Key);
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("Failed to remove INI key: [%s]%s"), *Section, *Key);
	return false;
#else
	// Fallback: load, remove, save
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		return false;
	}

	FString TargetSection = FString::Printf(TEXT("[%s]"), *Section);
	bool bInSection = false;

	for (int32 i = Lines.Num() - 1; i >= 0; --i)
	{
		FString TrimmedLine = Lines[i].TrimStartAndEnd();

		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			bInSection = TrimmedLine.Equals(TargetSection, ESearchCase::IgnoreCase);
		}
		else if (bInSection)
		{
			int32 EqualsIndex;
			if (TrimmedLine.FindChar(TEXT('='), EqualsIndex))
			{
				FString KeyName = TrimmedLine.Left(EqualsIndex).TrimStartAndEnd();
				if (KeyName.Equals(Key, ESearchCase::IgnoreCase))
				{
					Lines.RemoveAt(i);
					break;
				}
			}
		}
	}

	bool bSuccess = FFileHelper::SaveStringArrayToFile(Lines, *FullPath);
	if (bSuccess)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Remove INI key: [%s]%s"), *Section, *Key);
	}
	return bSuccess;
#endif
}

bool FIniFileHelper::RemoveSection(const FString& FilePath, const FString& Section)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI file not found: %s"), *FullPath);
		return false;
	}

#if PLATFORM_WINDOWS
	// Pass NULL as key to delete the entire section
	BOOL Result = WritePrivateProfileString(
		*Section,
		NULL,
		NULL,
		*FullPath
	);

	if (Result)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Remove INI section: [%s]"), *Section);
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("Failed to remove INI section: [%s]"), *Section);
	return false;
#else
	// Fallback: load, remove section, save
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		return false;
	}

	FString TargetSection = FString::Printf(TEXT("[%s]"), *Section);
	bool bInSection = false;
	int32 SectionStart = -1;

	for (int32 i = 0; i < Lines.Num(); ++i)
	{
		FString TrimmedLine = Lines[i].TrimStartAndEnd();

		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			if (bInSection)
			{
				// End of target section
				Lines.RemoveAt(SectionStart, i - SectionStart);
				break;
			}
			if (TrimmedLine.Equals(TargetSection, ESearchCase::IgnoreCase))
			{
				bInSection = true;
				SectionStart = i;
			}
		}
	}

	// If section was at end of file
	if (bInSection && SectionStart >= 0)
	{
		Lines.RemoveAt(SectionStart, Lines.Num() - SectionStart);
	}

	bool bSuccess = FFileHelper::SaveStringArrayToFile(Lines, *FullPath);
	if (bSuccess)
	{
		UE_LOG(LogSuperTools, Log, TEXT("Remove INI section: [%s]"), *Section);
	}
	return bSuccess;
#endif
}

// ==================== Query Operations ====================

bool FIniFileHelper::GetKeys(const FString& FilePath, const FString& Section, TArray<FString>& OutKeys)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI file not found: %s"), *FullPath);
		return false;
	}

#if PLATFORM_WINDOWS
	// Use Windows API to get all keys in a section
	TCHAR Buffer[32768];
	DWORD Result = GetPrivateProfileString(
		*Section,
		NULL,  // NULL key returns all keys
		TEXT(""),
		Buffer,
		32768,
		*FullPath
	);

	if (Result > 0)
	{
		OutKeys.Empty();
		TCHAR* Current = Buffer;
		while (*Current)
		{
			OutKeys.Add(FString(Current));
			Current += FCString::Strlen(Current) + 1;
		}
		UE_LOG(LogSuperTools, Verbose, TEXT("Get INI keys for [%s], count: %d"), *Section, OutKeys.Num());
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("INI section not found or empty: [%s]"), *Section);
	return false;
#else
	// Manual parsing for non-Windows platforms
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("Cannot read INI file: %s"), *FullPath);
		return false;
	}

	OutKeys.Empty();
	FString CurrentSection;
	const FString TargetSection = FString::Printf(TEXT("[%s]"), *Section);

	for (const FString& Line : Lines)
	{
		FString TrimmedLine = Line.TrimStartAndEnd();

		if (TrimmedLine.IsEmpty() || TrimmedLine.StartsWith(TEXT(";")) || TrimmedLine.StartsWith(TEXT("#")))
		{
			continue;
		}

		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			CurrentSection = TrimmedLine;
			continue;
		}

		if (CurrentSection.Equals(TargetSection, ESearchCase::IgnoreCase))
		{
			int32 EqualsIndex;
			if (TrimmedLine.FindChar(TEXT('='), EqualsIndex))
			{
				FString Key = TrimmedLine.Left(EqualsIndex).TrimStartAndEnd();
				if (!Key.IsEmpty())
				{
					OutKeys.AddUnique(Key);
				}
			}
		}
	}

	if (OutKeys.Num() > 0)
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("Get INI keys for [%s], count: %d"), *Section, OutKeys.Num());
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("INI section not found or empty: [%s]"), *Section);
	return false;
#endif
}

bool FIniFileHelper::GetSections(const FString& FilePath, TArray<FString>& OutSections)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI file not found: %s"), *FullPath);
		return false;
	}

#if PLATFORM_WINDOWS
	// Use Windows API to get all sections
	TCHAR Buffer[32768];
	DWORD Result = GetPrivateProfileSectionNames(
		Buffer,
		32768,
		*FullPath
	);

	if (Result > 0)
	{
		OutSections.Empty();
		TCHAR* Current = Buffer;
		while (*Current)
		{
			OutSections.Add(FString(Current));
			Current += FCString::Strlen(Current) + 1;
		}
		UE_LOG(LogSuperTools, Verbose, TEXT("Get INI sections, count: %d"), OutSections.Num());
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("INI file has no sections: %s"), *FullPath);
	return false;
#else
	// Manual parsing for non-Windows platforms
	TArray<FString> Lines;
	if (!FFileHelper::LoadFileToStringArray(Lines, *FullPath))
	{
		return false;
	}

	OutSections.Empty();
	for (const FString& Line : Lines)
	{
		FString TrimmedLine = Line.TrimStartAndEnd();
		if (TrimmedLine.StartsWith(TEXT("[")) && TrimmedLine.EndsWith(TEXT("]")))
		{
			FString SectionName = TrimmedLine.Mid(1, TrimmedLine.Len() - 2);
			OutSections.AddUnique(SectionName);
		}
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("Get INI sections, count: %d"), OutSections.Num());
	return OutSections.Num() > 0;
#endif
}
