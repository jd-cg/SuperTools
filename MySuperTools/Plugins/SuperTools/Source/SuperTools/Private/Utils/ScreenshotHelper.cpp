// Copyright lostpanda. All Rights Reserved.

#include "Utils/ScreenshotHelper.h"
#include "Engine/GameViewportClient.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "UnrealClient.h"
#include "Engine/Engine.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

bool FScreenshotHelper::CaptureViewport(const FString& FilePath, bool bShowUI)
{
	TArray<uint8> ImageData;
	int32 Width, Height;

	if (!CaptureViewportToData(ImageData, Width, Height, bShowUI))
	{
		return false;
	}

	// 根据扩展名确定保存格式
	FString Extension = FPaths::GetExtension(FilePath).ToLower();
	EImageFormat SaveFormat = EImageFormat::PNG;
	if (Extension == TEXT("bmp"))
	{
		SaveFormat = EImageFormat::BMP;
	}
	else if (Extension == TEXT("jpg") || Extension == TEXT("jpeg"))
	{
		SaveFormat = EImageFormat::JPEG;
	}

	// 编码图像
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(SaveFormat);

	if (!ImageWrapper.IsValid() || !ImageWrapper->SetRaw(ImageData.GetData(), ImageData.Num(), Width, Height, ERGBFormat::BGRA, 8))
	{
		return false;
	}

	TArray64<uint8> CompressedData = ImageWrapper->GetCompressed(100);
	if (CompressedData.Num() == 0)
	{
		return false;
	}

	return FFileHelper::SaveArrayToFile(CompressedData, *FilePath);
}

bool FScreenshotHelper::CaptureViewportToData(TArray<uint8>& OutImageData, int32& OutWidth, int32& OutHeight, bool bShowUI)
{
	OutImageData.Empty();
	OutWidth = 0;
	OutHeight = 0;

	if (!GEngine || !GEngine->GameViewport)
	{
		return false;
	}

	FViewport* Viewport = GEngine->GameViewport->Viewport;
	if (!Viewport)
	{
		return false;
	}

	TArray<FColor> Bitmap;
	if (!Viewport->ReadPixels(Bitmap))
	{
		return false;
	}

	OutWidth = Viewport->GetSizeXY().X;
	OutHeight = Viewport->GetSizeXY().Y;

	// 转换为 BGRA 格式
	OutImageData.SetNum(OutWidth * OutHeight * 4);
	for (int32 i = 0; i < Bitmap.Num(); ++i)
	{
		OutImageData[i * 4 + 0] = Bitmap[i].B;
		OutImageData[i * 4 + 1] = Bitmap[i].G;
		OutImageData[i * 4 + 2] = Bitmap[i].R;
		OutImageData[i * 4 + 3] = Bitmap[i].A;
	}

	return true;
}

void FScreenshotHelper::RequestScreenshot(const FString& FilePath, bool bShowUI)
{
	FScreenshotRequest::RequestScreenshot(FilePath, bShowUI, false);
}

#if PLATFORM_WINDOWS

bool FScreenshotHelper::CaptureScreen(const FString& FilePath)
{
	int32 ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int32 ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	return CaptureRegion(FilePath, 0, 0, ScreenWidth, ScreenHeight);
}

bool FScreenshotHelper::CaptureRegion(const FString& FilePath, int32 X, int32 Y, int32 Width, int32 Height)
{
	if (Width <= 0 || Height <= 0)
	{
		return false;
	}

	// 获取屏幕 DC
	HDC hScreenDC = GetDC(nullptr);
	if (!hScreenDC)
	{
		return false;
	}

	// 创建兼容 DC 和位图
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, Width, Height);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

	// 复制屏幕内容
	BitBlt(hMemoryDC, 0, 0, Width, Height, hScreenDC, X, Y, SRCCOPY);

	// 获取位图数据
	BITMAPINFOHEADER BitmapHeader;
	FMemory::Memzero(&BitmapHeader, sizeof(BitmapHeader));
	BitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapHeader.biWidth = Width;
	BitmapHeader.biHeight = -Height; // 负值表示自上而下
	BitmapHeader.biPlanes = 1;
	BitmapHeader.biBitCount = 32;
	BitmapHeader.biCompression = BI_RGB;

	TArray<uint8> ImageData;
	ImageData.SetNum(Width * Height * 4);

	GetDIBits(hMemoryDC, hBitmap, 0, Height, ImageData.GetData(), (BITMAPINFO*)&BitmapHeader, DIB_RGB_COLORS);

	// 清理资源
	SelectObject(hMemoryDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(nullptr, hScreenDC);

	// 保存图像
	FString Extension = FPaths::GetExtension(FilePath).ToLower();
	EImageFormat SaveFormat = EImageFormat::PNG;
	if (Extension == TEXT("bmp"))
	{
		SaveFormat = EImageFormat::BMP;
	}
	else if (Extension == TEXT("jpg") || Extension == TEXT("jpeg"))
	{
		SaveFormat = EImageFormat::JPEG;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(SaveFormat);

	if (!ImageWrapper.IsValid() || !ImageWrapper->SetRaw(ImageData.GetData(), ImageData.Num(), Width, Height, ERGBFormat::BGRA, 8))
	{
		return false;
	}

	TArray64<uint8> CompressedData = ImageWrapper->GetCompressed(100);
	if (CompressedData.Num() == 0)
	{
		return false;
	}

	return FFileHelper::SaveArrayToFile(CompressedData, *FilePath);
}

#else // 非 Windows 平台

bool FScreenshotHelper::CaptureScreen(const FString& FilePath)
{
	return false;
}

bool FScreenshotHelper::CaptureRegion(const FString& FilePath, int32 X, int32 Y, int32 Width, int32 Height)
{
	return false;
}

#endif // PLATFORM_WINDOWS
