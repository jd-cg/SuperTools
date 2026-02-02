// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Engine/LatentActionManager.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

/**
 * HTTP GET 请求的 Latent Action
 * 用于在 Blueprint 中实现异步 HTTP 请求
 */
class FHttpGetLatentAction : public FPendingLatentAction
{
public:
	FString URL;
	FString& OutResponse;
	int32& OutResponseCode;
	bool& bOutSuccess;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bRequestSent;
	bool bRequestComplete;

	FHttpGetLatentAction(
		const FString& InURL,
		FString& InOutResponse,
		int32& InOutResponseCode,
		bool& InOutSuccess,
		const FLatentActionInfo& LatentInfo);

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("HTTP GET: %s"), *URL);
	}
#endif

private:
	void SendRequest();
};

/**
 * HTTP POST 请求的 Latent Action
 */
class FHttpPostLatentAction : public FPendingLatentAction
{
public:
	FString URL;
	FString Content;
	FString ContentType;
	FString& OutResponse;
	int32& OutResponseCode;
	bool& bOutSuccess;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bRequestSent;
	bool bRequestComplete;

	FHttpPostLatentAction(
		const FString& InURL,
		const FString& InContent,
		const FString& InContentType,
		FString& InOutResponse,
		int32& InOutResponseCode,
		bool& InOutSuccess,
		const FLatentActionInfo& LatentInfo);

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("HTTP POST: %s"), *URL);
	}
#endif

private:
	void SendRequest();
};

/**
 * HTTP 文件下载的 Latent Action
 */
class FHttpDownloadLatentAction : public FPendingLatentAction
{
public:
	FString URL;
	FString SavePath;
	int32& OutResponseCode;
	bool& bOutSuccess;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bRequestSent;
	bool bRequestComplete;

	FHttpDownloadLatentAction(
		const FString& InURL,
		const FString& InSavePath,
		int32& InOutResponseCode,
		bool& InOutSuccess,
		const FLatentActionInfo& LatentInfo);

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("HTTP Download: %s"), *URL);
	}
#endif

private:
	void SendRequest();
};
