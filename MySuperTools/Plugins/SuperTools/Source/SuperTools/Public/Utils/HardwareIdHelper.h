// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 硬件ID助手类
 * 提供获取硬件唯一标识符的功能 (主要支持 Windows 平台)
 */
class SUPERTOOLS_API FHardwareIdHelper
{
public:
	// ==================== 单项硬件ID ====================

	/**
	 * 获取主板序列号
	 * @return 主板序列号，失败返回空字符串
	 */
	static FString GetMotherboardSerial();

	/**
	 * 获取主板制造商
	 * @return 主板制造商名称
	 */
	static FString GetMotherboardManufacturer();

	/**
	 * 获取主板产品名称
	 * @return 主板产品名称
	 */
	static FString GetMotherboardProduct();

	/**
	 * 获取 CPU ID
	 * @return CPU 处理器ID
	 */
	static FString GetCpuId();

	/**
	 * 获取 CPU 名称
	 * @return CPU 型号名称
	 */
	static FString GetCpuName();

	/**
	 * 获取主硬盘序列号
	 * @return 硬盘序列号，失败返回空字符串
	 */
	static FString GetDiskSerial();

	/**
	 * 获取第一个网卡的 MAC 地址
	 * @return MAC 地址 (格式: XX:XX:XX:XX:XX:XX)
	 */
	static FString GetMacAddress();

	/**
	 * 获取所有网卡的 MAC 地址
	 * @return MAC 地址数组
	 */
	static TArray<FString> GetAllMacAddresses();

	/**
	 * 获取 BIOS 序列号
	 * @return BIOS 序列号
	 */
	static FString GetBiosSerial();

	/**
	 * 获取系统 UUID
	 * @return 系统 UUID
	 */
	static FString GetSystemUuid();

	// ==================== 组合硬件指纹 ====================

	/**
	 * 获取硬件指纹 (基于多个硬件ID的组合哈希)
	 * 组合: 主板序列号 + CPU ID + 硬盘序列号 + MAC 地址
	 * @return SHA256 哈希值作为硬件指纹
	 */
	static FString GetHardwareFingerprint();

	/**
	 * 获取自定义硬件指纹
	 * @param bIncludeMotherboard 是否包含主板信息
	 * @param bIncludeCpu 是否包含 CPU 信息
	 * @param bIncludeDisk 是否包含硬盘信息
	 * @param bIncludeMac 是否包含 MAC 地址
	 * @param bIncludeBios 是否包含 BIOS 信息
	 * @return SHA256 哈希值作为硬件指纹
	 */
	static FString GetCustomHardwareFingerprint(
		bool bIncludeMotherboard = true,
		bool bIncludeCpu = true,
		bool bIncludeDisk = true,
		bool bIncludeMac = true,
		bool bIncludeBios = false
	);

	// ==================== 硬件信息结构 ====================

	/**
	 * 获取完整的硬件信息 (JSON 格式)
	 * @return JSON 字符串包含所有硬件信息
	 */
	static FString GetHardwareInfoJson();

private:
#if PLATFORM_WINDOWS
	/**
	 * 执行 WMI 查询
	 * @param WmiClass WMI 类名
	 * @param PropertyName 属性名
	 * @return 查询结果
	 */
	static FString ExecuteWmiQuery(const FString& WmiClass, const FString& PropertyName);

	/**
	 * 执行 WMIC 命令
	 * @param WmicCommand WMIC 命令参数
	 * @return 命令输出结果
	 */
	static FString ExecuteWmicCommand(const FString& WmicCommand);
#endif
};
