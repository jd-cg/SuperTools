// Copyright lostpanda. All Rights Reserved.

#include "Utils/ClipboardHelper.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

// ==================== 文本操作 ====================

bool FClipboardHelper::CopyToClipboard(const FString& Text)
{
	FPlatformApplicationMisc::ClipboardCopy(*Text);
	return true;
}

bool FClipboardHelper::GetFromClipboard(FString& OutText)
{
	FPlatformApplicationMisc::ClipboardPaste(OutText);
	return !OutText.IsEmpty();
}

bool FClipboardHelper::HasClipboardText()
{
	FString ClipboardText;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardText);
	return !ClipboardText.IsEmpty();
}

bool FClipboardHelper::ClearClipboard()
{
	FPlatformApplicationMisc::ClipboardCopy(TEXT(""));
	return true;
}

// ==================== 图像操作 ====================

#if PLATFORM_WINDOWS

bool FClipboardHelper::CopyImageToClipboard(const FString& ImagePath)
{
	if (!FPaths::FileExists(ImagePath))
	{
		return false;
	}

	// 加载图像文件
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
	{
		return false;
	}

	// 检测图像格式
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());

	if (ImageFormat == EImageFormat::Invalid)
	{
		return false;
	}

	// 解码图像
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		return false;
	}

	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		return false;
	}

	return CopyImageDataToClipboard(RawData, ImageWrapper->GetWidth(), ImageWrapper->GetHeight());
}

bool FClipboardHelper::CopyImageDataToClipboard(const TArray<uint8>& ImageData, int32 Width, int32 Height)
{
	if (ImageData.Num() == 0 || Width <= 0 || Height <= 0)
	{
		return false;
	}

	// 创建 DIB 位图
	BITMAPINFOHEADER BitmapHeader;
	FMemory::Memzero(&BitmapHeader, sizeof(BitmapHeader));
	BitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapHeader.biWidth = Width;
	BitmapHeader.biHeight = -Height; // 负值表示自上而下
	BitmapHeader.biPlanes = 1;
	BitmapHeader.biBitCount = 32;
	BitmapHeader.biCompression = BI_RGB;
	BitmapHeader.biSizeImage = Width * Height * 4;

	// 分配全局内存
	SIZE_T DataSize = sizeof(BITMAPINFOHEADER) + BitmapHeader.biSizeImage;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, DataSize);
	if (!hGlobal)
	{
		return false;
	}

	// 复制数据到全局内存
	void* pData = GlobalLock(hGlobal);
	if (!pData)
	{
		GlobalFree(hGlobal);
		return false;
	}

	FMemory::Memcpy(pData, &BitmapHeader, sizeof(BITMAPINFOHEADER));
	FMemory::Memcpy((uint8*)pData + sizeof(BITMAPINFOHEADER), ImageData.GetData(), BitmapHeader.biSizeImage);
	GlobalUnlock(hGlobal);

	// 打开剪贴板并设置数据
	if (!OpenClipboard(nullptr))
	{
		GlobalFree(hGlobal);
		return false;
	}

	EmptyClipboard();
	HANDLE hResult = SetClipboardData(CF_DIB, hGlobal);
	CloseClipboard();

	if (!hResult)
	{
		GlobalFree(hGlobal);
		return false;
	}

	return true;
}

bool FClipboardHelper::GetImageFromClipboard(const FString& SavePath)
{
	TArray<uint8> ImageData;
	int32 Width, Height;

	if (!GetImageDataFromClipboard(ImageData, Width, Height))
	{
		return false;
	}

	// 根据扩展名确定保存格式
	FString Extension = FPaths::GetExtension(SavePath).ToLower();
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

	return FFileHelper::SaveArrayToFile(CompressedData, *SavePath);
}

bool FClipboardHelper::GetImageDataFromClipboard(TArray<uint8>& OutImageData, int32& OutWidth, int32& OutHeight)
{
	OutImageData.Empty();
	OutWidth = 0;
	OutHeight = 0;

	if (!OpenClipboard(nullptr))
	{
		return false;
	}

	HANDLE hData = GetClipboardData(CF_DIB);
	if (!hData)
	{
		CloseClipboard();
		return false;
	}

	BITMAPINFOHEADER* pBitmapHeader = (BITMAPINFOHEADER*)GlobalLock(hData);
	if (!pBitmapHeader)
	{
		CloseClipboard();
		return false;
	}

	OutWidth = pBitmapHeader->biWidth;
	OutHeight = FMath::Abs(pBitmapHeader->biHeight);

	// 计算像素数据偏移
	int32 ColorTableSize = 0;
	if (pBitmapHeader->biBitCount <= 8)
	{
		ColorTableSize = static_cast<int32>((1ULL << pBitmapHeader->biBitCount) * sizeof(RGBQUAD));
	}

	uint8* pPixelData = (uint8*)pBitmapHeader + pBitmapHeader->biSize + ColorTableSize;
	int32 RowSize = ((OutWidth * pBitmapHeader->biBitCount + 31) / 32) * 4;

	// 转换为 BGRA 格式
	OutImageData.SetNum(OutWidth * OutHeight * 4);

	bool bBottomUp = pBitmapHeader->biHeight > 0;

	for (int32 y = 0; y < OutHeight; ++y)
	{
		int32 SrcY = bBottomUp ? (OutHeight - 1 - y) : y;
		uint8* SrcRow = pPixelData + SrcY * RowSize;
		uint8* DstRow = OutImageData.GetData() + y * OutWidth * 4;

		for (int32 x = 0; x < OutWidth; ++x)
		{
			if (pBitmapHeader->biBitCount == 32)
			{
				DstRow[x * 4 + 0] = SrcRow[x * 4 + 0]; // B
				DstRow[x * 4 + 1] = SrcRow[x * 4 + 1]; // G
				DstRow[x * 4 + 2] = SrcRow[x * 4 + 2]; // R
				DstRow[x * 4 + 3] = SrcRow[x * 4 + 3]; // A
			}
			else if (pBitmapHeader->biBitCount == 24)
			{
				DstRow[x * 4 + 0] = SrcRow[x * 3 + 0]; // B
				DstRow[x * 4 + 1] = SrcRow[x * 3 + 1]; // G
				DstRow[x * 4 + 2] = SrcRow[x * 3 + 2]; // R
				DstRow[x * 4 + 3] = 255;              // A
			}
		}
	}

	GlobalUnlock(hData);
	CloseClipboard();

	return true;
}

bool FClipboardHelper::HasClipboardImage()
{
	if (!OpenClipboard(nullptr))
	{
		return false;
	}

	bool bHasImage = IsClipboardFormatAvailable(CF_DIB) || IsClipboardFormatAvailable(CF_BITMAP);
	CloseClipboard();

	return bHasImage;
}

#else // 非 Windows 平台

bool FClipboardHelper::CopyImageToClipboard(const FString& ImagePath)
{
	return false;
}

bool FClipboardHelper::CopyImageDataToClipboard(const TArray<uint8>& ImageData, int32 Width, int32 Height)
{
	return false;
}

bool FClipboardHelper::GetImageFromClipboard(const FString& SavePath)
{
	return false;
}

bool FClipboardHelper::GetImageDataFromClipboard(TArray<uint8>& OutImageData, int32& OutWidth, int32& OutHeight)
{
	OutImageData.Empty();
	OutWidth = 0;
	OutHeight = 0;
	return false;
}

bool FClipboardHelper::HasClipboardImage()
{
	return false;
}

#endif // PLATFORM_WINDOWS
