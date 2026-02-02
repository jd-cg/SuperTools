// Copyright lostpanda. All Rights Reserved.

#include "SuperTools.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

DEFINE_LOG_CATEGORY(LogSuperTools);

#define LOCTEXT_NAMESPACE "FSuperToolsModule"

void FSuperToolsModule::StartupModule()
{
	UE_LOG(LogSuperTools, Log, TEXT("SuperTools 插件启动中..."));

	// 获取插件基础目录
	FString BaseDir = IPluginManager::Get().FindPlugin("SuperTools")->GetBaseDir();

	// 加载第三方库（如果需要）
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/SuperToolsLibrary/Win64/ExampleLibrary.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/SuperToolsLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/SuperToolsLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (ExampleLibraryHandle)
	{
		UE_LOG(LogSuperTools, Log, TEXT("第三方库加载成功: %s"), *LibraryPath);
	}
	else
	{
		UE_LOG(LogSuperTools, Warning, TEXT("第三方库加载失败: %s"), *LibraryPath);
	}

	UE_LOG(LogSuperTools, Log, TEXT("SuperTools 插件启动完成"));
}

void FSuperToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperToolsModule, SuperTools)
