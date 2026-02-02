// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSuperTools, Log, All);

/**
 * SuperTools 插件模块
 * 提供通用工具函数和框架功能
 */
class FSuperToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** 第三方库句柄 */
	void*	ExampleLibraryHandle;
};
