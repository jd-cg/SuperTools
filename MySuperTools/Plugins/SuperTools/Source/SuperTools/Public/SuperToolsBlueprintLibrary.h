// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SuperToolsBlueprintLibrary.generated.h"

/**
 * SuperTools 蓝图函数库
 * 提供 INI 文件操作和 Windows API 功能的蓝图接口
 */
UCLASS()
class SUPERTOOLS_API USuperToolsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ==================== INI 文件操作 ====================

	/**
	 * 从 INI 文件读取字符串
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的字符串值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static FString ReadIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue = TEXT(""));

	/**
	 * 从 INI 文件读取整数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的整数值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static int32 ReadIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue = 0);

	/**
	 * 从 INI 文件读取浮点数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的浮点数值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static float ReadIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue = 0.0f);

	/**
	 * 从 INI 文件读取布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的布尔值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static bool ReadIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue = false);

	/**
	 * 向 INI 文件写入字符串
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static bool WriteIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value);

	/**
	 * 向 INI 文件写入整数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static bool WriteIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value);

	/**
	 * 向 INI 文件写入浮点数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static bool WriteIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value);

	/**
	 * 向 INI 文件写入布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI")
	static bool WriteIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value);

	// ==================== Windows API 操作 ====================

#if PLATFORM_WINDOWS
	/**
	 * 设置窗口标题
	 * @param Title 新的窗口标题
	 * @return 是否设置成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool SetWindowTitle(const FString& Title);

	/**
	 * 获取窗口标题
	 * @return 窗口标题
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static FString GetWindowTitle();

	/**
	 * 获取屏幕分辨率
	 * @param Width 输出宽度
	 * @param Height 输出高度
	 * @return 是否获取成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool GetScreenResolution(int32& Width, int32& Height);

	/**
	 * 设置窗口位置
	 * @param X X 坐标
	 * @param Y Y 坐标
	 * @return 是否设置成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool SetWindowPosition(int32 X, int32 Y);

	/**
	 * 设置窗口大小
	 * @param Width 宽度
	 * @param Height 高度
	 * @return 是否设置成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool SetWindowSize(int32 Width, int32 Height);

	/**
	 * 最大化窗口
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool MaximizeWindow();

	/**
	 * 最小化窗口
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool MinimizeWindow();

	/**
	 * 恢复窗口
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool RestoreWindow();

	/**
	 * 将窗口置于最前
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows")
	static bool BringWindowToFront();
#endif
};
