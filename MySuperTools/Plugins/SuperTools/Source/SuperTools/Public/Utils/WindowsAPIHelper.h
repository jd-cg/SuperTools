// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Windows API 助手类
 * 提供常用的 Windows API 封装功能
 * 注意: 此类仅在 Windows 平台可用
 */
class SUPERTOOLS_API FWindowsAPIHelper
{
public:
#if PLATFORM_WINDOWS
	/**
	 * 设置窗口标题
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @param Title 新的窗口标题
	 * @return 是否设置成功
	 */
	static bool SetWindowTitle(void* WindowHandle, const FString& Title);

	/**
	 * 获取窗口标题
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @return 窗口标题
	 */
	static FString GetWindowTitle(void* WindowHandle);

	/**
	 * 设置屏幕分辨率
	 * @param Width 宽度
	 * @param Height 高度
	 * @param BitsPerPixel 色深 (默认 32)
	 * @param RefreshRate 刷新率 (默认 0 表示使用当前刷新率)
	 * @return 是否设置成功
	 */
	static bool SetScreenResolution(int32 Width, int32 Height, int32 BitsPerPixel = 32, int32 RefreshRate = 0);

	/**
	 * 获取当前屏幕分辨率
	 * @param OutWidth 输出宽度
	 * @param OutHeight 输出高度
	 * @return 是否获取成功
	 */
	static bool GetScreenResolution(int32& OutWidth, int32& OutHeight);

	/**
	 * 获取主显示器的工作区大小 (不包括任务栏)
	 * @param OutWidth 输出宽度
	 * @param OutHeight 输出高度
	 * @return 是否获取成功
	 */
	static bool GetWorkAreaSize(int32& OutWidth, int32& OutHeight);

	/**
	 * 设置窗口位置
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @param X X 坐标
	 * @param Y Y 坐标
	 * @return 是否设置成功
	 */
	static bool SetWindowPosition(void* WindowHandle, int32 X, int32 Y);

	/**
	 * 设置窗口大小
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @param Width 宽度
	 * @param Height 高度
	 * @return 是否设置成功
	 */
	static bool SetWindowSize(void* WindowHandle, int32 Width, int32 Height);

	/**
	 * 获取窗口位置和大小
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @param OutX 输出 X 坐标
	 * @param OutY 输出 Y 坐标
	 * @param OutWidth 输出宽度
	 * @param OutHeight 输出高度
	 * @return 是否获取成功
	 */
	static bool GetWindowRect(void* WindowHandle, int32& OutX, int32& OutY, int32& OutWidth, int32& OutHeight);

	/**
	 * 最大化窗口
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @return 是否操作成功
	 */
	static bool MaximizeWindow(void* WindowHandle);

	/**
	 * 最小化窗口
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @return 是否操作成功
	 */
	static bool MinimizeWindow(void* WindowHandle);

	/**
	 * 恢复窗口 (从最大化或最小化状态恢复)
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @return 是否操作成功
	 */
	static bool RestoreWindow(void* WindowHandle);

	/**
	 * 显示/隐藏窗口
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @param bShow true 显示, false 隐藏
	 * @return 是否操作成功
	 */
	static bool ShowWindow(void* WindowHandle, bool bShow);

	/**
	 * 将窗口置于最前
	 * @param WindowHandle 窗口句柄 (如果为 nullptr,则使用主窗口)
	 * @return 是否操作成功
	 */
	static bool BringWindowToFront(void* WindowHandle);

	/**
	 * 获取主窗口句柄
	 * @return 主窗口句柄
	 */
	static void* GetMainWindowHandle();

	/**
	 * 获取桌面窗口句柄
	 * @return 桌面窗口句柄
	 */
	static void* GetDesktopWindowHandle();

	/**
	 * 获取系统指标
	 * @param Index 指标索引 (参考 Windows API SM_* 常量)
	 * @return 指标值
	 */
	static int32 GetSystemMetrics(int32 Index);

#endif // PLATFORM_WINDOWS

private:
#if PLATFORM_WINDOWS
	/**
	 * 获取有效的窗口句柄 (如果传入 nullptr,返回主窗口句柄)
	 * @param WindowHandle 输入的窗口句柄
	 * @return 有效的窗口句柄
	 */
	static void* GetValidWindowHandle(void* WindowHandle);
#endif
};
