// Copyright lostpanda. All Rights Reserved.

#include "Utils/WindowsAPIHelper.h"
#include "SuperTools.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "Misc/App.h"
#include "GenericPlatform/GenericApplication.h"
#include "Framework/Application/SlateApplication.h"

void* FWindowsAPIHelper::GetValidWindowHandle(void* WindowHandle)
{
	if (WindowHandle != nullptr)
	{
		return WindowHandle;
	}

	// 尝试获取主窗口句柄
	return GetMainWindowHandle();
}

void* FWindowsAPIHelper::GetMainWindowHandle()
{
	if (FSlateApplication::IsInitialized())
	{
		TSharedPtr<SWindow> MainWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
		if (MainWindow.IsValid())
		{
			return MainWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	// 如果 Slate 未初始化,尝试使用 FindWindow
	HWND hwnd = FindWindowW(NULL, *FString(FApp::GetProjectName()));
	return hwnd;
}

void* FWindowsAPIHelper::GetDesktopWindowHandle()
{
	return GetDesktopWindow();
}

bool FWindowsAPIHelper::SetWindowTitle(void* WindowHandle, const FString& Title)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (SetWindowTextW(hwnd, *Title))
	{
		UE_LOG(LogSuperTools, Log, TEXT("设置窗口标题: %s"), *Title);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("设置窗口标题失败"));
	return false;
}

FString FWindowsAPIHelper::GetWindowTitle(void* WindowHandle)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return FString();
	}

	WCHAR buffer[256];
	int length = GetWindowTextW(hwnd, buffer, 256);
	if (length > 0)
	{
		return FString(buffer);
	}

	return FString();
}

bool FWindowsAPIHelper::SetScreenResolution(int32 Width, int32 Height, int32 BitsPerPixel, int32 RefreshRate)
{
	DEVMODEW devMode;
	ZeroMemory(&devMode, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);
	devMode.dmPelsWidth = Width;
	devMode.dmPelsHeight = Height;
	devMode.dmBitsPerPel = BitsPerPixel;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	if (RefreshRate > 0)
	{
		devMode.dmDisplayFrequency = RefreshRate;
		devMode.dmFields |= DM_DISPLAYFREQUENCY;
	}

	LONG result = ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN);
	if (result == DISP_CHANGE_SUCCESSFUL)
	{
		UE_LOG(LogSuperTools, Log, TEXT("设置屏幕分辨率: %dx%d @ %d 位色深"), Width, Height, BitsPerPixel);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("设置屏幕分辨率失败,错误码: %d"), result);
	return false;
}

bool FWindowsAPIHelper::GetScreenResolution(int32& OutWidth, int32& OutHeight)
{
	OutWidth = GetSystemMetrics(SM_CXSCREEN);
	OutHeight = GetSystemMetrics(SM_CYSCREEN);

	UE_LOG(LogSuperTools, Verbose, TEXT("当前屏幕分辨率: %dx%d"), OutWidth, OutHeight);
	return true;
}

bool FWindowsAPIHelper::GetWorkAreaSize(int32& OutWidth, int32& OutHeight)
{
	RECT workArea;
	if (SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0))
	{
		OutWidth = workArea.right - workArea.left;
		OutHeight = workArea.bottom - workArea.top;
		UE_LOG(LogSuperTools, Verbose, TEXT("工作区大小: %dx%d"), OutWidth, OutHeight);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("获取工作区大小失败"));
	return false;
}

bool FWindowsAPIHelper::SetWindowPosition(void* WindowHandle, int32 X, int32 Y)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (SetWindowPos(hwnd, NULL, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER))
	{
		UE_LOG(LogSuperTools, Log, TEXT("设置窗口位置: (%d, %d)"), X, Y);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("设置窗口位置失败"));
	return false;
}

bool FWindowsAPIHelper::SetWindowSize(void* WindowHandle, int32 Width, int32 Height)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (SetWindowPos(hwnd, NULL, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER))
	{
		UE_LOG(LogSuperTools, Log, TEXT("设置窗口大小: %dx%d"), Width, Height);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("设置窗口大小失败"));
	return false;
}

bool FWindowsAPIHelper::GetWindowRect(void* WindowHandle, int32& OutX, int32& OutY, int32& OutWidth, int32& OutHeight)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	RECT rect;
	if (::GetWindowRect(hwnd, &rect))
	{
		OutX = rect.left;
		OutY = rect.top;
		OutWidth = rect.right - rect.left;
		OutHeight = rect.bottom - rect.top;
		UE_LOG(LogSuperTools, Verbose, TEXT("窗口矩形: (%d, %d, %d, %d)"), OutX, OutY, OutWidth, OutHeight);
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("获取窗口矩形失败"));
	return false;
}

bool FWindowsAPIHelper::MaximizeWindow(void* WindowHandle)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (::ShowWindow(hwnd, SW_MAXIMIZE))
	{
		UE_LOG(LogSuperTools, Log, TEXT("最大化窗口"));
		return true;
	}

	return false;
}

bool FWindowsAPIHelper::MinimizeWindow(void* WindowHandle)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (::ShowWindow(hwnd, SW_MINIMIZE))
	{
		UE_LOG(LogSuperTools, Log, TEXT("最小化窗口"));
		return true;
	}

	return false;
}

bool FWindowsAPIHelper::RestoreWindow(void* WindowHandle)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (::ShowWindow(hwnd, SW_RESTORE))
	{
		UE_LOG(LogSuperTools, Log, TEXT("恢复窗口"));
		return true;
	}

	return false;
}

bool FWindowsAPIHelper::ShowWindow(void* WindowHandle, bool bShow)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	::ShowWindow(hwnd, bShow ? SW_SHOW : SW_HIDE);
	UE_LOG(LogSuperTools, Log, TEXT("%s窗口"), bShow ? TEXT("显示") : TEXT("隐藏"));
	return true;
}

bool FWindowsAPIHelper::BringWindowToFront(void* WindowHandle)
{
	HWND hwnd = (HWND)GetValidWindowHandle(WindowHandle);
	if (hwnd == nullptr)
	{
		UE_LOG(LogSuperTools, Error, TEXT("无法获取窗口句柄"));
		return false;
	}

	if (SetForegroundWindow(hwnd))
	{
		UE_LOG(LogSuperTools, Log, TEXT("将窗口置于最前"));
		return true;
	}

	UE_LOG(LogSuperTools, Error, TEXT("将窗口置于最前失败"));
	return false;
}

int32 FWindowsAPIHelper::GetSystemMetrics(int32 Index)
{
	return ::GetSystemMetrics(Index);
}

#endif // PLATFORM_WINDOWS
