// Copyright lostpanda. All Rights Reserved.

#include "Utils/SystemHelper.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

bool FSystemHelper::OpenURL(const FString& URL)
{
	FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	return true;
}

bool FSystemHelper::OpenFolder(const FString& FolderPath)
{
	if (!FPaths::DirectoryExists(FolderPath))
	{
		return false;
	}
	FPlatformProcess::ExploreFolder(*FolderPath);
	return true;
}

bool FSystemHelper::OpenFile(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}
	FPlatformProcess::LaunchFileInDefaultExternalApplication(*FilePath);
	return true;
}

bool FSystemHelper::LaunchApplication(
	const FString& ExecutablePath,
	const FString& Arguments,
	bool bHidden)
{
	if (!FPaths::FileExists(ExecutablePath))
	{
		return false;
	}

	FProcHandle Handle = FPlatformProcess::CreateProc(
		*ExecutablePath,
		*Arguments,
		true,    // bLaunchDetached
		false,   // bLaunchHidden
		bHidden, // bLaunchReallyHidden
		nullptr, // OutProcessID
		0,       // PriorityModifier
		nullptr, // OptionalWorkingDirectory
		nullptr  // PipeWriteChild
	);

	return Handle.IsValid();
}

FString FSystemHelper::GetEnvironmentVariable(const FString& VariableName)
{
	return FPlatformMisc::GetEnvironmentVariable(*VariableName);
}

FString FSystemHelper::GetFormattedTime(const FString& Format)
{
	FDateTime Now = FDateTime::Now();
	return Now.ToString(*Format);
}

FString FSystemHelper::GetComputerName()
{
	return FPlatformProcess::ComputerName();
}

FString FSystemHelper::GetUserName()
{
	return FPlatformProcess::UserName();
}
