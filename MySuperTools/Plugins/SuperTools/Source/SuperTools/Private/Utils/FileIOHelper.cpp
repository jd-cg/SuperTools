// Copyright lostpanda. All Rights Reserved.

#include "Utils/FileIOHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/FileManager.h"

bool FFileIOHelper::ReadTextFile(const FString& FilePath, FString& OutContent)
{
	return FFileHelper::LoadFileToString(OutContent, *FilePath);
}

bool FFileIOHelper::WriteTextFile(const FString& FilePath, const FString& Content, bool bAppend)
{
	if (bAppend)
	{
		return FFileHelper::SaveStringToFile(Content, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	return FFileHelper::SaveStringToFile(Content, *FilePath);
}

bool FFileIOHelper::ReadLines(const FString& FilePath, TArray<FString>& OutLines)
{
	return FFileHelper::LoadFileToStringArray(OutLines, *FilePath);
}

bool FFileIOHelper::WriteLines(const FString& FilePath, const TArray<FString>& Lines, bool bAppend)
{
	if (bAppend)
	{
		FString Content;
		for (const FString& Line : Lines)
		{
			Content += Line + LINE_TERMINATOR;
		}
		return FFileHelper::SaveStringToFile(Content, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
	return FFileHelper::SaveStringArrayToFile(Lines, *FilePath);
}

bool FFileIOHelper::ReadBinaryFile(const FString& FilePath, TArray<uint8>& OutData)
{
	return FFileHelper::LoadFileToArray(OutData, *FilePath);
}

bool FFileIOHelper::WriteBinaryFile(const FString& FilePath, const TArray<uint8>& Data)
{
	return FFileHelper::SaveArrayToFile(Data, *FilePath);
}

bool FFileIOHelper::FileExists(const FString& FilePath)
{
	return FPaths::FileExists(FilePath);
}

bool FFileIOHelper::DirectoryExists(const FString& DirectoryPath)
{
	return FPaths::DirectoryExists(DirectoryPath);
}

bool FFileIOHelper::CreateDirectory(const FString& DirectoryPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.CreateDirectoryTree(*DirectoryPath);
}

bool FFileIOHelper::DeleteFile(const FString& FilePath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.DeleteFile(*FilePath);
}

bool FFileIOHelper::DeleteDirectory(const FString& DirectoryPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.DeleteDirectoryRecursively(*DirectoryPath);
}

bool FFileIOHelper::CopyFile(const FString& SourcePath, const FString& DestPath, bool bOverwrite)
{
	if (!bOverwrite && FPaths::FileExists(DestPath))
	{
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.CopyFile(*DestPath, *SourcePath);
}

bool FFileIOHelper::MoveFile(const FString& SourcePath, const FString& DestPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.MoveFile(*DestPath, *SourcePath);
}

int64 FFileIOHelper::GetFileSize(const FString& FilePath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.FileSize(*FilePath);
}

bool FFileIOHelper::GetFilesInDirectory(const FString& DirectoryPath, TArray<FString>& OutFiles, const FString& Extension, bool bRecursive)
{
	OutFiles.Empty();

	if (!FPaths::DirectoryExists(DirectoryPath))
	{
		return false;
	}

	IFileManager& FileManager = IFileManager::Get();
	FString SearchPath = DirectoryPath / TEXT("*");

	if (!Extension.IsEmpty())
	{
		SearchPath = DirectoryPath / TEXT("*.") + Extension;
	}

	if (bRecursive)
	{
		FileManager.FindFilesRecursive(OutFiles, *DirectoryPath, Extension.IsEmpty() ? TEXT("*") : *(TEXT("*.") + Extension), true, false);
	}
	else
	{
		FileManager.FindFiles(OutFiles, *SearchPath, true, false);
		// 转换为完整路径
		for (FString& File : OutFiles)
		{
			File = DirectoryPath / File;
		}
	}

	return true;
}

bool FFileIOHelper::GetDirectoriesInDirectory(const FString& DirectoryPath, TArray<FString>& OutDirectories, bool bRecursive)
{
	OutDirectories.Empty();

	if (!FPaths::DirectoryExists(DirectoryPath))
	{
		return false;
	}

	IFileManager& FileManager = IFileManager::Get();

	if (bRecursive)
	{
		FileManager.FindFilesRecursive(OutDirectories, *DirectoryPath, TEXT("*"), false, true);
	}
	else
	{
		FileManager.FindFiles(OutDirectories, *(DirectoryPath / TEXT("*")), false, true);
		// 转换为完整路径
		for (FString& Dir : OutDirectories)
		{
			Dir = DirectoryPath / Dir;
		}
	}

	return true;
}

FString FFileIOHelper::GetFileExtension(const FString& FilePath)
{
	return FPaths::GetExtension(FilePath);
}

FString FFileIOHelper::GetFileName(const FString& FilePath, bool bWithExtension)
{
	if (bWithExtension)
	{
		return FPaths::GetCleanFilename(FilePath);
	}
	return FPaths::GetBaseFilename(FilePath);
}

FString FFileIOHelper::GetFileDirectory(const FString& FilePath)
{
	return FPaths::GetPath(FilePath);
}
