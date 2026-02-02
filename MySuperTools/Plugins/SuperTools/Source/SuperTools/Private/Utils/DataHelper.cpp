// Copyright lostpanda. All Rights Reserved.

#include "Utils/DataHelper.h"
#include "Misc/Base64.h"
#include "Misc/SecureHash.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// Windows CryptoAPI SHA256 支持
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <wincrypt.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

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
	TArray<uint8> Bytes;
	FTCHARToUTF8 Converter(*Input);
	Bytes.Append((uint8*)Converter.Get(), Converter.Length());
	return SHA256HashBytes(Bytes);
}

FString FDataHelper::SHA256HashBytes(const TArray<uint8>& Input)
{
#if PLATFORM_WINDOWS
	// 使用 Windows CryptoAPI 计算真正的 SHA256
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	FString Result;

	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, Input.GetData(), Input.Num(), 0))
			{
				DWORD HashLen = 32; // SHA256 = 32 bytes
				uint8 Hash[32];
				if (CryptGetHashParam(hHash, HP_HASHVAL, Hash, &HashLen, 0))
				{
					Result = BytesToHex(Hash, 32);
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}

	// 如果 CryptoAPI 失败，回退到 SHA1
	if (Result.IsEmpty())
	{
		FSHAHash Hash;
		FSHA1 Sha1Gen;
		Sha1Gen.Update(Input.GetData(), Input.Num());
		Sha1Gen.Final();
		Sha1Gen.GetHash(Hash.Hash);
		Result = Hash.ToString();
	}

	return Result;
#else
	// 非 Windows 平台使用 SHA1
	FSHAHash Hash;
	FSHA1 Sha1Gen;
	Sha1Gen.Update(Input.GetData(), Input.Num());
	Sha1Gen.Final();
	Sha1Gen.GetHash(Hash.Hash);
	return Hash.ToString();
#endif
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
