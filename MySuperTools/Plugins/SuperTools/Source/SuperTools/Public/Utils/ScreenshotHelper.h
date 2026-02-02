// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 截图助手类
 * 提供屏幕截图功能
 */
class SUPERTOOLS_API FScreenshotHelper
{
public:
	/**
	 * 截取游戏视口并保存到文件
	 * @param FilePath 保存路径 (支持 PNG, BMP, JPG)
	 * @param bShowUI 是否包含 UI
	 * @return 是否截图成功
	 */
	static bool CaptureViewport(const FString& FilePath, bool bShowUI = true);

	/**
	 * 截取游戏视口到像素数据
	 * @param OutImageData 输出的像素数据 (BGRA 格式)
	 * @param OutWidth 输出图像宽度
	 * @param OutHeight 输出图像高度
	 * @param bShowUI 是否包含 UI
	 * @return 是否截图成功
	 */
	static bool CaptureViewportToData(TArray<uint8>& OutImageData, int32& OutWidth, int32& OutHeight, bool bShowUI = true);

	/**
	 * 请求截图 (异步, 使用引擎截图系统)
	 * @param FilePath 保存路径
	 * @param bShowUI 是否包含 UI
	 * @note 此方法是异步的, 截图会在下一帧完成
	 */
	static void RequestScreenshot(const FString& FilePath, bool bShowUI = true);

	/**
	 * 截取整个屏幕 (Windows 平台)
	 * @param FilePath 保存路径
	 * @return 是否截图成功 (非 Windows 平台返回 false)
	 */
	static bool CaptureScreen(const FString& FilePath);

	/**
	 * 截取指定区域 (Windows 平台)
	 * @param FilePath 保存路径
	 * @param X 起始 X 坐标
	 * @param Y 起始 Y 坐标
	 * @param Width 宽度
	 * @param Height 高度
	 * @return 是否截图成功 (非 Windows 平台返回 false)
	 */
	static bool CaptureRegion(const FString& FilePath, int32 X, int32 Y, int32 Width, int32 Height);
};