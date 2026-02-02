// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// HTTP 请求完成委托
DECLARE_DELEGATE_ThreeParams(FOnHttpRequestComplete, bool /*bSuccess*/, int32 /*ResponseCode*/, const FString& /*ResponseContent*/);

/**
 * HTTP 助手类
 * 提供 HTTP 请求功能 (GET/POST)
 */
class SUPERTOOLS_API FHttpHelper
{
public:
	/**
	 * 发送 GET 请求 (异步)
	 * @param URL 请求 URL
	 * @param OnComplete 完成回调
	 * @param Headers 可选的请求头
	 */
	static void Get(
		const FString& URL,
		FOnHttpRequestComplete OnComplete,
		const TMap<FString, FString>& Headers = TMap<FString, FString>());

	/**
	 * 发送 POST 请求 (异步)
	 * @param URL 请求 URL
	 * @param Content 请求内容
	 * @param OnComplete 完成回调
	 * @param ContentType 内容类型 (默认: application/json)
	 * @param Headers 可选的请求头
	 */
	static void Post(
		const FString& URL,
		const FString& Content,
		FOnHttpRequestComplete OnComplete,
		const FString& ContentType = TEXT("application/json"),
		const TMap<FString, FString>& Headers = TMap<FString, FString>());

	/**
	 * 发送 PUT 请求 (异步)
	 * @param URL 请求 URL
	 * @param Content 请求内容
	 * @param OnComplete 完成回调
	 * @param ContentType 内容类型 (默认: application/json)
	 * @param Headers 可选的请求头
	 */
	static void Put(
		const FString& URL,
		const FString& Content,
		FOnHttpRequestComplete OnComplete,
		const FString& ContentType = TEXT("application/json"),
		const TMap<FString, FString>& Headers = TMap<FString, FString>());

	/**
	 * 发送 DELETE 请求 (异步)
	 * @param URL 请求 URL
	 * @param OnComplete 完成回调
	 * @param Headers 可选的请求头
	 */
	static void Delete(
		const FString& URL,
		FOnHttpRequestComplete OnComplete,
		const TMap<FString, FString>& Headers = TMap<FString, FString>());

	/**
	 * 下载文件 (异步)
	 * @param URL 文件 URL
	 * @param SavePath 保存路径
	 * @param OnComplete 完成回调 (ResponseContent 为空字符串)
	 */
	static void DownloadFile(
		const FString& URL,
		const FString& SavePath,
		FOnHttpRequestComplete OnComplete);

private:
	/**
	 * 发送 HTTP 请求的内部实现
	 */
	static void SendRequest(
		const FString& Verb,
		const FString& URL,
		const FString& Content,
		const FString& ContentType,
		const TMap<FString, FString>& Headers,
		FOnHttpRequestComplete OnComplete);
};
