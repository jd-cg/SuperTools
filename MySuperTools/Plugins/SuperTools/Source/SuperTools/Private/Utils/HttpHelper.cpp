// Copyright lostpanda. All Rights Reserved.

#include "Utils/HttpHelper.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"

void FHttpHelper::Get(
	const FString& URL,
	FOnHttpRequestComplete OnComplete,
	const TMap<FString, FString>& Headers)
{
	SendRequest(TEXT("GET"), URL, TEXT(""), TEXT(""), Headers, OnComplete);
}

void FHttpHelper::Post(
	const FString& URL,
	const FString& Content,
	FOnHttpRequestComplete OnComplete,
	const FString& ContentType,
	const TMap<FString, FString>& Headers)
{
	SendRequest(TEXT("POST"), URL, Content, ContentType, Headers, OnComplete);
}

void FHttpHelper::Put(
	const FString& URL,
	const FString& Content,
	FOnHttpRequestComplete OnComplete,
	const FString& ContentType,
	const TMap<FString, FString>& Headers)
{
	SendRequest(TEXT("PUT"), URL, Content, ContentType, Headers, OnComplete);
}

void FHttpHelper::Delete(
	const FString& URL,
	FOnHttpRequestComplete OnComplete,
	const TMap<FString, FString>& Headers)
{
	SendRequest(TEXT("DELETE"), URL, TEXT(""), TEXT(""), Headers, OnComplete);
}

void FHttpHelper::DownloadFile(
	const FString& URL,
	const FString& SavePath,
	FOnHttpRequestComplete OnComplete)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(URL);

	// 捕获 SavePath 用于保存文件
	FString CapturedSavePath = SavePath;

	Request->OnProcessRequestComplete().BindLambda(
		[OnComplete, CapturedSavePath](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (bConnectedSuccessfully && HttpResponse.IsValid())
			{
				int32 ResponseCode = HttpResponse->GetResponseCode();
				if (ResponseCode >= 200 && ResponseCode < 300)
				{
					// 保存文件
					const TArray<uint8>& Content = HttpResponse->GetContent();
					if (FFileHelper::SaveArrayToFile(Content, *CapturedSavePath))
					{
						OnComplete.ExecuteIfBound(true, ResponseCode, TEXT(""));
						return;
					}
				}
				OnComplete.ExecuteIfBound(false, ResponseCode, TEXT(""));
			}
			else
			{
				OnComplete.ExecuteIfBound(false, 0, TEXT(""));
			}
		});

	Request->ProcessRequest();
}

void FHttpHelper::SendRequest(
	const FString& Verb,
	const FString& URL,
	const FString& Content,
	const FString& ContentType,
	const TMap<FString, FString>& Headers,
	FOnHttpRequestComplete OnComplete)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(Verb);
	Request->SetURL(URL);

	// 设置内容类型
	if (!ContentType.IsEmpty())
	{
		Request->SetHeader(TEXT("Content-Type"), ContentType);
	}

	// 设置自定义请求头
	for (const auto& Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}

	// 设置请求内容
	if (!Content.IsEmpty())
	{
		Request->SetContentAsString(Content);
	}

	// 绑定完成回调
	Request->OnProcessRequestComplete().BindLambda(
		[OnComplete](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bConnectedSuccessfully)
		{
			if (bConnectedSuccessfully && HttpResponse.IsValid())
			{
				int32 ResponseCode = HttpResponse->GetResponseCode();
				FString ResponseContent = HttpResponse->GetContentAsString();
				OnComplete.ExecuteIfBound(ResponseCode >= 200 && ResponseCode < 300, ResponseCode, ResponseContent);
			}
			else
			{
				OnComplete.ExecuteIfBound(false, 0, TEXT(""));
			}
		});

	Request->ProcessRequest();
}
