// Copyright lostpanda. All Rights Reserved.

#include "Utils/HttpLatentActions.h"
#include "HttpModule.h"

// ==================== HTTP GET Latent Action ====================

FHttpGetLatentAction::FHttpGetLatentAction(
	const FString& InURL,
	FString& InOutResponse,
	int32& InOutResponseCode,
	bool& InOutSuccess,
	const FLatentActionInfo& LatentInfo)
	: URL(InURL)
	, OutResponse(InOutResponse)
	, OutResponseCode(InOutResponseCode)
	, bOutSuccess(InOutSuccess)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, bRequestSent(false)
	, bRequestComplete(false)
{
	// 初始化输出值
	OutResponse = TEXT("");
	OutResponseCode = 0;
	bOutSuccess = false;
}

void FHttpGetLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!bRequestSent)
	{
		SendRequest();
		bRequestSent = true;
	}

	if (bRequestComplete)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
	}
}

void FHttpGetLatentAction::SendRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(URL);

	// LatentActionManager 拥有此 Action，在完成前不会被销毁
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (bConnectedSuccessfully && HttpResponse.IsValid())
			{
				OutResponseCode = HttpResponse->GetResponseCode();
				OutResponse = HttpResponse->GetContentAsString();
				bOutSuccess = (OutResponseCode >= 200 && OutResponseCode < 300);
			}
			else
			{
				OutResponseCode = 0;
				OutResponse = TEXT("");
				bOutSuccess = false;
			}
			bRequestComplete = true;
		});

	Request->ProcessRequest();
}

// ==================== HTTP POST Latent Action ====================

FHttpPostLatentAction::FHttpPostLatentAction(
	const FString& InURL,
	const FString& InContent,
	const FString& InContentType,
	FString& InOutResponse,
	int32& InOutResponseCode,
	bool& InOutSuccess,
	const FLatentActionInfo& LatentInfo)
	: URL(InURL)
	, Content(InContent)
	, ContentType(InContentType)
	, OutResponse(InOutResponse)
	, OutResponseCode(InOutResponseCode)
	, bOutSuccess(InOutSuccess)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, bRequestSent(false)
	, bRequestComplete(false)
{
	OutResponse = TEXT("");
	OutResponseCode = 0;
	bOutSuccess = false;
}

void FHttpPostLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!bRequestSent)
	{
		SendRequest();
		bRequestSent = true;
	}

	if (bRequestComplete)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
	}
}

void FHttpPostLatentAction::SendRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetContentAsString(Content);

	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (bConnectedSuccessfully && HttpResponse.IsValid())
			{
				OutResponseCode = HttpResponse->GetResponseCode();
				OutResponse = HttpResponse->GetContentAsString();
				bOutSuccess = (OutResponseCode >= 200 && OutResponseCode < 300);
			}
			else
			{
				OutResponseCode = 0;
				OutResponse = TEXT("");
				bOutSuccess = false;
			}
			bRequestComplete = true;
		});

	Request->ProcessRequest();
}

// ==================== HTTP Download Latent Action ====================

FHttpDownloadLatentAction::FHttpDownloadLatentAction(
	const FString& InURL,
	const FString& InSavePath,
	int32& InOutResponseCode,
	bool& InOutSuccess,
	const FLatentActionInfo& LatentInfo)
	: URL(InURL)
	, SavePath(InSavePath)
	, OutResponseCode(InOutResponseCode)
	, bOutSuccess(InOutSuccess)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, bRequestSent(false)
	, bRequestComplete(false)
{
	OutResponseCode = 0;
	bOutSuccess = false;
}

void FHttpDownloadLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!bRequestSent)
	{
		SendRequest();
		bRequestSent = true;
	}

	if (bRequestComplete)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
	}
}

void FHttpDownloadLatentAction::SendRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(URL);

	FString CapturedSavePath = SavePath;

	Request->OnProcessRequestComplete().BindLambda(
		[this, CapturedSavePath](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (bConnectedSuccessfully && HttpResponse.IsValid())
			{
				OutResponseCode = HttpResponse->GetResponseCode();
				if (OutResponseCode >= 200 && OutResponseCode < 300)
				{
					const TArray<uint8>& ResponseContent = HttpResponse->GetContent();
					bOutSuccess = FFileHelper::SaveArrayToFile(ResponseContent, *CapturedSavePath);
				}
				else
				{
					bOutSuccess = false;
				}
			}
			else
			{
				OutResponseCode = 0;
				bOutSuccess = false;
			}
			bRequestComplete = true;
		});

	Request->ProcessRequest();
}
