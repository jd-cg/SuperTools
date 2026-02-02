// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 系统操作助手类
 * 提供系统操作功能 (启动应用、打开 URL 等)
 */
class SUPERTOOLS_API FSystemHelper
{
public:
	/**
	 * 在默认浏览器中打开 URL
	 * @param URL 要打开的 URL
	 * @return 是否操作成功
	 */
	static bool OpenURL(const FString& URL);

	/**
	 * 在文件资源管理器中打开文件夹
	 * @param FolderPath 要打开的文件夹路径
	 * @return 是否操作成功
	 */
	static bool OpenFolder(const FString& FolderPath);

	/**
	 * 使用默认应用程序打开文件
	 * @param FilePath 要打开的文件路径
	 * @return 是否操作成功
	 */
	static bool OpenFile(const FString& FilePath);

	/**
	 * 启动外部应用程序
	 * @param ExecutablePath 可执行文件路径
	 * @param Arguments 命令行参数
	 * @param bHidden 是否隐藏运行
	 * @return 是否操作成功
	 */
	static bool LaunchApplication(
		const FString& ExecutablePath,
		const FString& Arguments = TEXT(""),
		bool bHidden = false);

	/**
	 * 获取环境变量
	 * @param VariableName 环境变量名称
	 * @return 环境变量值 (如果不存在则返回空字符串)
	 */
	static FString GetEnvironmentVariable(const FString& VariableName);

	/**
	 * 获取格式化的当前时间
	 * @param Format 时间格式字符串 (默认: "%Y-%m-%d %H:%M:%S")
	 * @return 格式化的时间字符串
	 */
	static FString GetFormattedTime(const FString& Format = TEXT("%Y-%m-%d %H:%M:%S"));

	/**
	 * 获取计算机名称
	 * @return 计算机名称
	 */
	static FString GetComputerName();

	/**
	 * 获取当前用户名
	 * @return 当前用户名
	 */
	static FString GetUserName();
};
