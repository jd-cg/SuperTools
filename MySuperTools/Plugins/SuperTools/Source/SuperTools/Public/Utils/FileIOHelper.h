// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 文件 I/O 助手类
 * 提供文件读写操作功能
 */
class SUPERTOOLS_API FFileIOHelper
{
public:
	// ==================== 文本文件操作 ====================

	/**
	 * 读取文本文件
	 * @param FilePath 文件路径
	 * @param OutContent 输出的文件内容
	 * @return 是否读取成功
	 */
	static bool ReadTextFile(const FString& FilePath, FString& OutContent);

	/**
	 * 写入文本文件
	 * @param FilePath 文件路径
	 * @param Content 要写入的内容
	 * @param bAppend 是否追加模式 (默认: false, 覆盖)
	 * @return 是否写入成功
	 */
	static bool WriteTextFile(const FString& FilePath, const FString& Content, bool bAppend = false);

	/**
	 * 读取文本文件的所有行
	 * @param FilePath 文件路径
	 * @param OutLines 输出的行数组
	 * @return 是否读取成功
	 */
	static bool ReadLines(const FString& FilePath, TArray<FString>& OutLines);

	/**
	 * 写入多行到文本文件
	 * @param FilePath 文件路径
	 * @param Lines 要写入的行数组
	 * @param bAppend 是否追加模式
	 * @return 是否写入成功
	 */
	static bool WriteLines(const FString& FilePath, const TArray<FString>& Lines, bool bAppend = false);

	// ==================== 二进制文件操作 ====================

	/**
	 * 读取二进制文件
	 * @param FilePath 文件路径
	 * @param OutData 输出的字节数组
	 * @return 是否读取成功
	 */
	static bool ReadBinaryFile(const FString& FilePath, TArray<uint8>& OutData);

	/**
	 * 写入二进制文件
	 * @param FilePath 文件路径
	 * @param Data 要写入的字节数组
	 * @return 是否写入成功
	 */
	static bool WriteBinaryFile(const FString& FilePath, const TArray<uint8>& Data);

	// ==================== 文件/目录操作 ====================

	/**
	 * 检查文件是否存在
	 * @param FilePath 文件路径
	 * @return 文件是否存在
	 */
	static bool FileExists(const FString& FilePath);

	/**
	 * 检查目录是否存在
	 * @param DirectoryPath 目录路径
	 * @return 目录是否存在
	 */
	static bool DirectoryExists(const FString& DirectoryPath);

	/**
	 * 创建目录 (包括所有父目录)
	 * @param DirectoryPath 目录路径
	 * @return 是否创建成功
	 */
	static bool CreateDirectory(const FString& DirectoryPath);

	/**
	 * 删除文件
	 * @param FilePath 文件路径
	 * @return 是否删除成功
	 */
	static bool DeleteFile(const FString& FilePath);

	/**
	 * 删除目录 (包括所有内容)
	 * @param DirectoryPath 目录路径
	 * @return 是否删除成功
	 */
	static bool DeleteDirectory(const FString& DirectoryPath);

	/**
	 * 复制文件
	 * @param SourcePath 源文件路径
	 * @param DestPath 目标文件路径
	 * @param bOverwrite 是否覆盖已存在的文件
	 * @return 是否复制成功
	 */
	static bool CopyFile(const FString& SourcePath, const FString& DestPath, bool bOverwrite = true);

	/**
	 * 移动/重命名文件
	 * @param SourcePath 源文件路径
	 * @param DestPath 目标文件路径
	 * @return 是否移动成功
	 */
	static bool MoveFile(const FString& SourcePath, const FString& DestPath);

	/**
	 * 获取文件大小
	 * @param FilePath 文件路径
	 * @return 文件大小 (字节), 如果文件不存在返回 -1
	 */
	static int64 GetFileSize(const FString& FilePath);

	/**
	 * 获取目录中的所有文件
	 * @param DirectoryPath 目录路径
	 * @param OutFiles 输出的文件路径数组
	 * @param Extension 文件扩展名过滤 (可选, 如 "txt", 不带点)
	 * @param bRecursive 是否递归搜索子目录
	 * @return 是否获取成功
	 */
	static bool GetFilesInDirectory(const FString& DirectoryPath, TArray<FString>& OutFiles, const FString& Extension = TEXT(""), bool bRecursive = false);

	/**
	 * 获取目录中的所有子目录
	 * @param DirectoryPath 目录路径
	 * @param OutDirectories 输出的目录路径数组
	 * @param bRecursive 是否递归搜索
	 * @return 是否获取成功
	 */
	static bool GetDirectoriesInDirectory(const FString& DirectoryPath, TArray<FString>& OutDirectories, bool bRecursive = false);

	/**
	 * 获取文件扩展名
	 * @param FilePath 文件路径
	 * @return 扩展名 (不带点)
	 */
	static FString GetFileExtension(const FString& FilePath);

	/**
	 * 获取文件名 (不含路径)
	 * @param FilePath 文件路径
	 * @param bWithExtension 是否包含扩展名
	 * @return 文件名
	 */
	static FString GetFileName(const FString& FilePath, bool bWithExtension = true);

	/**
	 * 获取文件所在目录
	 * @param FilePath 文件路径
	 * @return 目录路径
	 */
	static FString GetFileDirectory(const FString& FilePath);
};
