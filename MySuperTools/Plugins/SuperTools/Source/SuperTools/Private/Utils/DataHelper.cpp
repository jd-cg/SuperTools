// Copyright lostpanda. All Rights Reserved.

#include "Utils/DataHelper.h"
#include "Misc/Base64.h"
#include "Misc/SecureHash.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// ==================== Base64 编码 ====================

FString FDataHelper::Base64Encode(const FString& Input)
{
	return FBase64::Encode(Input);
}

bool FDataHelper::Base64Decode(const FString& Input, FString& OutDecoded)
{
	return FBase64::Decode(Input, OutDecoded);
}

FString FDataHelper::Base64EncodeBytes(const TArray<uint8>& Input)
{
	return FBase64::Encode(Input);
}

bool FDataHelper::Base64DecodeToBytes(const FString& Input, TArray<uint8>& OutBytes)
{
	return FBase64::Decode(Input, OutBytes);
}

// ==================== 哈希计算 ====================

FString FDataHelper::MD5Hash(const FString& Input)
{
	return FMD5::HashAnsiString(*Input);
}

FString FDataHelper::MD5HashBytes(const TArray<uint8>& Input)
{
	FMD5 Md5Gen;
	Md5Gen.Update(Input.GetData(), Input.Num());
	uint8 Digest[16];
	Md5Gen.Final(Digest);
	return BytesToHex(Digest, 16);
}

FString FDataHelper::SHA256Hash(const FString& Input)
{
	FSHAHash Hash;
	FSHA256 Sha256Gen;
	TArray<uint8> Bytes;
	FTCHARToUTF8 Converter(*Input);
	Bytes.Append((uint8*)Converter.Get(), Converter.Length());
	Sha256Gen.Update(Bytes.GetData(), Bytes.Num());
	Sha256Gen.Final(Hash.Hash);
	return Hash.ToString();
}

FString FDataHelper::SHA256HashBytes(const TArray<uint8>& Input)
{
	FSHAHash Hash;
	FSHA256 Sha256Gen;
	Sha256Gen.Update(Input.GetData(), Input.Num());
	Sha256Gen.Final(Hash.Hash);
	return Hash.ToString();
}

bool FDataHelper::MD5HashFile(const FString& FilePath, FString& OutHash)
{
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		return false;
	}

	OutHash = MD5HashBytes(FileData);
	return true;
}

bool FDataHelper::SHA256HashFile(const FString& FilePath, FString& OutHash)
{
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		return false;
	}

	OutHash = SHA256HashBytes(FileData);
	return true;
}
