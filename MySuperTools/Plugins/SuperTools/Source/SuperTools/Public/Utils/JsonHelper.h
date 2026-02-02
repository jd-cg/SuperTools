// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * JSON 助手类
 * 提供 JSON 解析和序列化功能
 */
class SUPERTOOLS_API FJsonHelper
{
public:
	// ==================== 解析 ====================

	/**
	 * 解析 JSON 字符串
	 * @param JsonString JSON 字符串
	 * @param OutJsonObject 输出的 JSON 对象
	 * @return 是否解析成功
	 */
	static bool ParseJson(const FString& JsonString, TSharedPtr<FJsonObject>& OutJsonObject);

	/**
	 * 从 JSON 字符串获取字符串值
	 * @param JsonString JSON 字符串
	 * @param FieldName 字段名称 (支持点分隔的路径, 如 "data.user.name")
	 * @param DefaultValue 默认值
	 * @return 字段值
	 */
	static FString GetStringField(const FString& JsonString, const FString& FieldName, const FString& DefaultValue = TEXT(""));

	/**
	 * 从 JSON 字符串获取整数值
	 * @param JsonString JSON 字符串
	 * @param FieldName 字段名称 (支持点分隔的路径)
	 * @param DefaultValue 默认值
	 * @return 字段值
	 */
	static int32 GetIntField(const FString& JsonString, const FString& FieldName, int32 DefaultValue = 0);

	/**
	 * 从 JSON 字符串获取浮点值
	 * @param JsonString JSON 字符串
	 * @param FieldName 字段名称 (支持点分隔的路径)
	 * @param DefaultValue 默认值
	 * @return 字段值
	 */
	static float GetFloatField(const FString& JsonString, const FString& FieldName, float DefaultValue = 0.0f);

	/**
	 * 从 JSON 字符串获取布尔值
	 * @param JsonString JSON 字符串
	 * @param FieldName 字段名称 (支持点分隔的路径)
	 * @param DefaultValue 默认值
	 * @return 字段值
	 */
	static bool GetBoolField(const FString& JsonString, const FString& FieldName, bool DefaultValue = false);

	/**
	 * 从 JSON 字符串获取字符串数组
	 * @param JsonString JSON 字符串
	 * @param FieldName 字段名称 (支持点分隔的路径)
	 * @param OutArray 输出数组
	 * @return 是否获取成功
	 */
	static bool GetStringArrayField(const FString& JsonString, const FString& FieldName, TArray<FString>& OutArray);

	// ==================== 序列化 ====================

	/**
	 * 创建空的 JSON 对象
	 * @return JSON 对象
	 */
	static TSharedPtr<FJsonObject> CreateJsonObject();

	/**
	 * 设置 JSON 对象的字符串字段
	 * @param JsonObject JSON 对象
	 * @param FieldName 字段名称
	 * @param Value 字段值
	 */
	static void SetStringField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FString& Value);

	/**
	 * 设置 JSON 对象的整数字段
	 * @param JsonObject JSON 对象
	 * @param FieldName 字段名称
	 * @param Value 字段值
	 */
	static void SetIntField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, int32 Value);

	/**
	 * 设置 JSON 对象的浮点字段
	 * @param JsonObject JSON 对象
	 * @param FieldName 字段名称
	 * @param Value 字段值
	 */
	static void SetFloatField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, float Value);

	/**
	 * 设置 JSON 对象的布尔字段
	 * @param JsonObject JSON 对象
	 * @param FieldName 字段名称
	 * @param Value 字段值
	 */
	static void SetBoolField(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, bool Value);

	/**
	 * 将 JSON 对象序列化为字符串
	 * @param JsonObject JSON 对象
	 * @param bPrettyPrint 是否格式化输出
	 * @return JSON 字符串
	 */
	static FString JsonObjectToString(TSharedPtr<FJsonObject> JsonObject, bool bPrettyPrint = false);

	// ==================== 简化接口 ====================

	/**
	 * 创建简单的键值对 JSON 字符串
	 * @param Key 键
	 * @param Value 值
	 * @return JSON 字符串
	 */
	static FString MakeJsonString(const FString& Key, const FString& Value);

	/**
	 * 从 Map 创建 JSON 字符串
	 * @param StringMap 字符串映射
	 * @param bPrettyPrint 是否格式化输出
	 * @return JSON 字符串
	 */
	static FString MapToJsonString(const TMap<FString, FString>& StringMap, bool bPrettyPrint = false);

	/**
	 * 从 JSON 字符串解析为 Map
	 * @param JsonString JSON 字符串
	 * @param OutMap 输出的字符串映射
	 * @return 是否解析成功
	 */
	static bool JsonStringToMap(const FString& JsonString, TMap<FString, FString>& OutMap);

private:
	/**
	 * 根据路径获取嵌套的 JSON 对象
	 * @param RootObject 根 JSON 对象
	 * @param Path 点分隔的路径
	 * @param OutFieldName 输出最终字段名
	 * @return 包含目标字段的 JSON 对象
	 */
	static TSharedPtr<FJsonObject> GetNestedObject(TSharedPtr<FJsonObject> RootObject, const FString& Path, FString& OutFieldName);
};
