// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 剪贴板助手类
 * 提供剪贴板操作功能 (复制/粘贴文本和图像)
 */
class SUPERTOOLS_API FClipboardHelper
{
public:
	// ==================== 文本操作 ====================

	/**
	 * 复制文本到剪贴板
	 * @param Text 要复制的文本
	 * @return 是否操作成功
	 */
	static bool CopyToClipboard(const FString& Text);

	/**
	 * 从剪贴板获取文本
	 * @param OutText 输出的文本
	 * @return 是否操作成功
	 */
	static bool GetFromClipboard(FString& OutText);

	/**
	 * 检查剪贴板是否有文本
	 * @return 剪贴板是否包含文本
	 */
	static bool HasClipboardText();

	/**
	 * 清空剪贴板
	 * @return 是否操作成功
	 */
	static bool ClearClipboard();

	// ==================== 图像操作 ====================
	// 注意: 图像操作仅在 Windows 平台可用

	/**
	 * 复制图像文件到剪贴板
	 * @param ImagePath 图像文件路径 (支持 PNG, BMP, JPG)
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	static bool CopyImageToClipboard(const FString& ImagePath);

	/**
	 * 复制图像数据到剪贴板
	 * @param ImageData 图像像素数据 (BGRA 格式)
	 * @param Width 图像宽度
	 * @param Height 图像高度
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	static bool CopyImageDataToClipboard(const TArray<uint8>& ImageData, int32 Width, int32 Height);

	/**
	 * 从剪贴板获取图像并保存到文件
	 * @param SavePath 保存路径 (支持 PNG, BMP)
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	static bool GetImageFromClipboard(const FString& SavePath);

	/**
	 * 从剪贴板获取图像数据
	 * @param OutImageData 输出图像像素数据 (BGRA 格式)
	 * @param OutWidth 输出图像宽度
	 * @param OutHeight 输出图像高度
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	static bool GetImageDataFromClipboard(TArray<uint8>& OutImageData, int32& OutWidth, int32& OutHeight);

	/**
	 * 检查剪贴板是否有图像
	 * @return 剪贴板是否包含图像 (非 Windows 平台返回 false)
	 */
	static bool HasClipboardImage();
};
