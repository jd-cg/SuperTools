// Copyright lostpanda. All Rights Reserved.

#include "Utils/IniFileHelper.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/PlatformFileManager.h"
#include "SuperTools.h"

FString FIniFileHelper::GetFullPath(const FString& FilePath)
{
	// 如果是绝对路径,直接返回
	if (FPaths::IsRelative(FilePath) == false)
	{
		return FilePath;
	}

	// 相对路径,转换为项目目录下的绝对路径
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir(), FilePath);
}

bool FIniFileHelper::FileExists(const FString& FilePath)
{
	FString FullPath = GetFullPath(FilePath);
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath);
}

FString FIniFileHelper::ReadString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return DefaultValue;
	}

	FString Value;
	if (GConfig->GetString(*Section, *Key, Value, FullPath))
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("从 INI 读取字符串: [%s]%s = %s"), *Section, *Key, *Value);
		return Value;
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI 键不存在,使用默认值: [%s]%s"), *Section, *Key);
	return DefaultValue;
}

int32 FIniFileHelper::ReadInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return DefaultValue;
	}

	int32 Value;
	if (GConfig->GetInt(*Section, *Key, Value, FullPath))
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("从 INI 读取整数: [%s]%s = %d"), *Section, *Key, Value);
		return Value;
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI 键不存在,使用默认值: [%s]%s"), *Section, *Key);
	return DefaultValue;
}

float FIniFileHelper::ReadFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return DefaultValue;
	}

	float Value;
	if (GConfig->GetFloat(*Section, *Key, Value, FullPath))
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("从 INI 读取浮点数: [%s]%s = %f"), *Section, *Key, Value);
		return Value;
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI 键不存在,使用默认值: [%s]%s"), *Section, *Key);
	return DefaultValue;
}

bool FIniFileHelper::ReadBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return DefaultValue;
	}

	bool Value;
	if (GConfig->GetBool(*Section, *Key, Value, FullPath))
	{
		UE_LOG(LogSuperTools, Verbose, TEXT("从 INI 读取布尔值: [%s]%s = %s"), *Section, *Key, Value ? TEXT("true") : TEXT("false"));
		return Value;
	}

	UE_LOG(LogSuperTools, Verbose, TEXT("INI 键不存在,使用默认值: [%s]%s"), *Section, *Key);
	return DefaultValue;
}

bool FIniFileHelper::WriteString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value)
{
	FString FullPath = GetFullPath(FilePath);

	GConfig->SetString(*Section, *Key, *Value, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("写入 INI 字符串: [%s]%s = %s"), *Section, *Key, *Value);
	return true;
}

bool FIniFileHelper::WriteInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value)
{
	FString FullPath = GetFullPath(FilePath);

	GConfig->SetInt(*Section, *Key, Value, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("写入 INI 整数: [%s]%s = %d"), *Section, *Key, Value);
	return true;
}

bool FIniFileHelper::WriteFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value)
{
	FString FullPath = GetFullPath(FilePath);

	GConfig->SetFloat(*Section, *Key, Value, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("写入 INI 浮点数: [%s]%s = %f"), *Section, *Key, Value);
	return true;
}

bool FIniFileHelper::WriteBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value)
{
	FString FullPath = GetFullPath(FilePath);

	GConfig->SetBool(*Section, *Key, Value, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("写入 INI 布尔值: [%s]%s = %s"), *Section, *Key, Value ? TEXT("true") : TEXT("false"));
	return true;
}

bool FIniFileHelper::RemoveKey(const FString& FilePath, const FString& Section, const FString& Key)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return false;
	}

	GConfig->RemoveKey(*Section, *Key, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("删除 INI 键: [%s]%s"), *Section, *Key);
	return true;
}

bool FIniFileHelper::RemoveSection(const FString& FilePath, const FString& Section)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return false;
	}

	GConfig->EmptySection(*Section, FullPath);
	GConfig->Flush(false, FullPath);

	UE_LOG(LogSuperTools, Log, TEXT("删除 INI 节: [%s]"), *Section);
	return true;
}

bool FIniFileHelper::GetKeys(const FString& FilePath, const FString& Section, TArray<FString>& OutKeys)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return false;
	}

	FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*Section, false, true, FullPath);
	if (ConfigSection)
	{
		OutKeys.Empty();
		for (auto& Pair : *ConfigSection)
		{
			OutKeys.Add(Pair.Key.ToString());
		}
		UE_LOG(LogSuperTools, Verbose, TEXT("获取 INI 节 [%s] 的键,共 %d 个"), *Section, OutKeys.Num());
		return true;
	}

	UE_LOG(LogSuperTools, Warning, TEXT("INI 节不存在: [%s]"), *Section);
	return false;
}

bool FIniFileHelper::GetSections(const FString& FilePath, TArray<FString>& OutSections)
{
	FString FullPath = GetFullPath(FilePath);

	if (!FileExists(FullPath))
	{
		UE_LOG(LogSuperTools, Warning, TEXT("INI 文件不存在: %s"), *FullPath);
		return false;
	}

	GConfig->GetSectionNames(FullPath, OutSections);
	UE_LOG(LogSuperTools, Verbose, TEXT("获取 INI 文件的所有节,共 %d 个"), OutSections.Num());
	return true;
}
