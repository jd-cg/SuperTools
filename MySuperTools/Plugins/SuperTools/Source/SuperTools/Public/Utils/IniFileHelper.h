// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * INI 文件读写助手类
 * 提供简单易用的 INI 文件操作接口
 */
class SUPERTOOLS_API FIniFileHelper
{
public:
	/**
	 * 从 INI 文件读取字符串值
	 * @param FilePath INI 文件路径 (相对于项目目录或绝对路径)
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值 (如果键不存在)
	 * @return 读取的字符串值
	 */
	static FString ReadString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue = TEXT(""));

	/**
	 * 从 INI 文件读取整数值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的整数值
	 */
	static int32 ReadInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue = 0);

	/**
	 * 从 INI 文件读取浮点数值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的浮点数值
	 */
	static float ReadFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue = 0.0f);

	/**
	 * 从 INI 文件读取布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的布尔值
	 */
	static bool ReadBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue = false);

	/**
	 * 向 INI 文件写入字符串值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	static bool WriteString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value);

	/**
	 * 向 INI 文件写入整数值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	static bool WriteInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value);

	/**
	 * 向 INI 文件写入浮点数值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	static bool WriteFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value);

	/**
	 * 向 INI 文件写入布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	static bool WriteBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value);

	/**
	 * 检查 INI 文件是否存在
	 * @param FilePath INI 文件路径
	 * @return 文件是否存在
	 */
	static bool FileExists(const FString& FilePath);

	/**
	 * 删除 INI 文件中的某个键
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @return 是否删除成功
	 */
	static bool RemoveKey(const FString& FilePath, const FString& Section, const FString& Key);

	/**
	 * 删除 INI 文件中的整个节
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @return 是否删除成功
	 */
	static bool RemoveSection(const FString& FilePath, const FString& Section);

	/**
	 * 获取 INI 文件中某个节的所有键
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param OutKeys 输出的键数组
	 * @return 是否获取成功
	 */
	static bool GetKeys(const FString& FilePath, const FString& Section, TArray<FString>& OutKeys);

	/**
	 * 获取 INI 文件中的所有节名称
	 * @param FilePath INI 文件路径
	 * @param OutSections 输出的节名称数组
	 * @return 是否获取成功
	 */
	static bool GetSections(const FString& FilePath, TArray<FString>& OutSections);

private:
	/**
	 * 获取完整的文件路径
	 * @param FilePath 输入的文件路径
	 * @return 完整的绝对路径
	 */
	static FString GetFullPath(const FString& FilePath);
};
