// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 数据处理助手类
 * 提供数据编码/解码和哈希操作
 */
class SUPERTOOLS_API FDataHelper
{
public:
	// ==================== Base64 编码 ====================

	/**
	 * 将字符串编码为 Base64
	 * @param Input 输入字符串
	 * @return Base64 编码后的字符串
	 */
	static FString Base64Encode(const FString& Input);

	/**
	 * 解码 Base64 字符串
	 * @param Input Base64 编码的字符串
	 * @param OutDecoded 输出解码后的字符串
	 * @return 是否解码成功
	 */
	static bool Base64Decode(const FString& Input, FString& OutDecoded);

	/**
	 * 将字节数组编码为 Base64
	 * @param Input 输入字节数组
	 * @return Base64 编码后的字符串
	 */
	static FString Base64EncodeBytes(const TArray<uint8>& Input);

	/**
	 * 将 Base64 解码为字节数组
	 * @param Input Base64 编码的字符串
	 * @param OutBytes 输出字节数组
	 * @return 是否解码成功
	 */
	static bool Base64DecodeToBytes(const FString& Input, TArray<uint8>& OutBytes);

	// ==================== 哈希计算 ====================

	/**
	 * 计算字符串的 MD5 哈希值
	 * @param Input 输入字符串
	 * @return MD5 哈希值 (32 字符十六进制字符串)
	 */
	static FString MD5Hash(const FString& Input);

	/**
	 * 计算字节数组的 MD5 哈希值
	 * @param Input 输入字节数组
	 * @return MD5 哈希值 (32 字符十六进制字符串)
	 */
	static FString MD5HashBytes(const TArray<uint8>& Input);

	/**
	 * 计算字符串的 SHA256 哈希值
	 * @param Input 输入字符串
	 * @return SHA256 哈希值 (64 字符十六进制字符串)
	 */
	static FString SHA256Hash(const FString& Input);

	/**
	 * 计算字节数组的 SHA256 哈希值
	 * @param Input 输入字节数组
	 * @return SHA256 哈希值 (64 字符十六进制字符串)
	 */
	static FString SHA256HashBytes(const TArray<uint8>& Input);

	/**
	 * 计算文件的 MD5 哈希值
	 * @param FilePath 文件路径
	 * @param OutHash 输出哈希值
	 * @return 是否计算成功
	 */
	static bool MD5HashFile(const FString& FilePath, FString& OutHash);

	/**
	 * 计算文件的 SHA256 哈希值
	 * @param FilePath 文件路径
	 * @param OutHash 输出哈希值
	 * @return 是否计算成功
	 */
	static bool SHA256HashFile(const FString& FilePath, FString& OutHash);
};
