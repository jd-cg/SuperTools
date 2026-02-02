// Copyright lostpanda. All Rights Reserved.

#include "Utils/FileDialogHelper.h"
#include "DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"

bool FFileDialogHelper::OpenFileDialog(
	const FString& DialogTitle,
	const FString& DefaultPath,
	const FString& FileTypes,
	FString& OutFilePath)
{
	TArray<FString> OutFiles;
	if (OpenFileDialogMultiple(DialogTitle, DefaultPath, FileTypes, OutFiles) && OutFiles.Num() > 0)
	{
		OutFilePath = OutFiles[0];
		return true;
	}
	return false;
}

bool FFileDialogHelper::OpenFileDialogMultiple(
	const FString& DialogTitle,
	const FString& DefaultPath,
	const FString& FileTypes,
	TArray<FString>& OutFilePaths)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}

	void* ParentWindowHandle = nullptr;
	if (FSlateApplication::IsInitialized())
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
		if (ParentWindow.IsValid())
		{
			ParentWindowHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	return DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		DialogTitle,
		DefaultPath,
		TEXT(""),
		FileTypes,
		EFileDialogFlags::Multiple,
		OutFilePaths);
}

bool FFileDialogHelper::SaveFileDialog(
	const FString& DialogTitle,
	const FString& DefaultPath,
	const FString& DefaultFileName,
	const FString& FileTypes,
	FString& OutFilePath)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}

	void* ParentWindowHandle = nullptr;
	if (FSlateApplication::IsInitialized())
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
		if (ParentWindow.IsValid())
		{
			ParentWindowHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	TArray<FString> OutFiles;
	bool bResult = DesktopPlatform->SaveFileDialog(
		ParentWindowHandle,
		DialogTitle,
		DefaultPath,
		DefaultFileName,
		FileTypes,
		EFileDialogFlags::None,
		OutFiles);

	if (bResult && OutFiles.Num() > 0)
	{
		OutFilePath = OutFiles[0];
		return true;
	}
	return false;
}

bool FFileDialogHelper::OpenFolderDialog(
	const FString& DialogTitle,
	const FString& DefaultPath,
	FString& OutFolderPath)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}

	void* ParentWindowHandle = nullptr;
	if (FSlateApplication::IsInitialized())
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
		if (ParentWindow.IsValid())
		{
			ParentWindowHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}
	}

	return DesktopPlatform->OpenDirectoryDialog(
		ParentWindowHandle,
		DialogTitle,
		DefaultPath,
		OutFolderPath);
}
