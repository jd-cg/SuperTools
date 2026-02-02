// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 文件对话框助手类
 * 提供文件和文件夹对话框操作
 */
class SUPERTOOLS_API FFileDialogHelper
{
public:
	/**
	 * 打开文件对话框 (单个文件)
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param FileTypes 文件类型过滤器 (例如: "Text Files|*.txt|All Files|*.*")
	 * @param OutFilePath 输出选中的文件路径
	 * @return 是否选择了文件
	 */
	static bool OpenFileDialog(
		const FString& DialogTitle,
		const FString& DefaultPath,
		const FString& FileTypes,
		FString& OutFilePath);

	/**
	 * 打开文件对话框 (多个文件)
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param FileTypes 文件类型过滤器
	 * @param OutFilePaths 输出选中的文件路径数组
	 * @return 是否选择了文件
	 */
	static bool OpenFileDialogMultiple(
		const FString& DialogTitle,
		const FString& DefaultPath,
		const FString& FileTypes,
		TArray<FString>& OutFilePaths);

	/**
	 * 保存文件对话框
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param DefaultFileName 默认文件名
	 * @param FileTypes 文件类型过滤器
	 * @param OutFilePath 输出选中的文件路径
	 * @return 是否选择了路径
	 */
	static bool SaveFileDialog(
		const FString& DialogTitle,
		const FString& DefaultPath,
		const FString& DefaultFileName,
		const FString& FileTypes,
		FString& OutFilePath);

	/**
	 * 打开文件夹对话框
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param OutFolderPath 输出选中的文件夹路径
	 * @return 是否选择了文件夹
	 */
	static bool OpenFolderDialog(
		const FString& DialogTitle,
		const FString& DefaultPath,
		FString& OutFolderPath);
};
