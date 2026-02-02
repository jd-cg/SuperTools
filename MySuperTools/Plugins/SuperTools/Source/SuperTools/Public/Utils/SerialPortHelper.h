// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerialPortHelper.generated.h"

/**
 * 串口奇偶校验类型
 */
UENUM(BlueprintType)
enum class ESerialParity : uint8
{
	None	UMETA(DisplayName = "无校验"),
	Odd		UMETA(DisplayName = "奇校验"),
	Even	UMETA(DisplayName = "偶校验"),
	Mark	UMETA(DisplayName = "标记"),
	Space	UMETA(DisplayName = "空格")
};

/**
 * 串口停止位类型
 */
UENUM(BlueprintType)
enum class ESerialStopBits : uint8
{
	One			UMETA(DisplayName = "1位"),
	OnePointFive UMETA(DisplayName = "1.5位"),
	Two			UMETA(DisplayName = "2位")
};

/**
 * 串口配置结构
 */
USTRUCT(BlueprintType)
struct SUPERTOOLS_API FSerialPortConfig
{
	GENERATED_BODY()

	/** 波特率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	int32 BaudRate = 9600;

	/** 数据位 (5-8) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort", meta = (ClampMin = "5", ClampMax = "8"))
	int32 DataBits = 8;

	/** 奇偶校验 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	ESerialParity Parity = ESerialParity::None;

	/** 停止位 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	ESerialStopBits StopBits = ESerialStopBits::One;

	/** 是否启用硬件流控制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	bool bEnableFlowControl = false;

	/** 读取超时 (毫秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	int32 ReadTimeoutMs = 100;

	/** 写入超时 (毫秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SerialPort")
	int32 WriteTimeoutMs = 100;
};

/**
 * 串口数据接收回调委托
 * @param Handle 串口句柄
 * @param Data 接收到的数据
 */
DECLARE_DELEGATE_TwoParams(FOnSerialDataReceived, int32 /*Handle*/, const TArray<uint8>& /*Data*/);

// Forward declaration
struct FSerialPortInfo;

/**
 * 串口通信辅助类
 * 提供串口打开、关闭、读写等功能
 * 注意: 完整功能仅在 Windows 平台可用，其他平台返回失败或空值
 */
class SUPERTOOLS_API FSerialPortHelper
{
public:
	// ==================== 端口管理 ====================

	/**
	 * 打开串口
	 * @param PortName 端口名称 (如 "COM1", "COM3")
	 * @param Config 串口配置
	 * @return 串口句柄 (用于后续操作), -1 表示打开失败
	 */
	static int32 OpenPort(const FString& PortName, const FSerialPortConfig& Config);

	/**
	 * 使用默认配置打开串口
	 * @param PortName 端口名称
	 * @param BaudRate 波特率 (默认 9600)
	 * @return 串口句柄, -1 表示打开失败
	 */
	static int32 OpenPortSimple(const FString& PortName, int32 BaudRate = 9600);

	/**
	 * 关闭串口
	 * @param Handle 串口句柄
	 * @return 是否关闭成功
	 */
	static bool ClosePort(int32 Handle);

	/**
	 * 关闭所有打开的串口
	 */
	static void CloseAllPorts();

	/**
	 * 检查串口是否打开
	 * @param Handle 串口句柄
	 * @return 是否打开
	 */
	static bool IsPortOpen(int32 Handle);

	// ==================== 数据操作 ====================

	/**
	 * 写入数据到串口
	 * @param Handle 串口句柄
	 * @param Data 要写入的数据
	 * @return 是否写入成功
	 */
	static bool Write(int32 Handle, const TArray<uint8>& Data);

	/**
	 * 写入字符串到串口
	 * @param Handle 串口句柄
	 * @param Message 要写入的字符串
	 * @return 是否写入成功
	 */
	static bool WriteString(int32 Handle, const FString& Message);

	/**
	 * 从串口读取数据
	 * @param Handle 串口句柄
	 * @param MaxBytes 最大读取字节数
	 * @return 读取到的数据
	 */
	static TArray<uint8> Read(int32 Handle, int32 MaxBytes = 1024);

	/**
	 * 从串口读取字符串
	 * @param Handle 串口句柄
	 * @param MaxBytes 最大读取字节数
	 * @return 读取到的字符串
	 */
	static FString ReadString(int32 Handle, int32 MaxBytes = 1024);

	/**
	 * 获取串口接收缓冲区中的可用字节数
	 * @param Handle 串口句柄
	 * @return 可用字节数, -1 表示错误
	 */
	static int32 GetBytesAvailable(int32 Handle);

	/**
	 * 清空串口缓冲区
	 * @param Handle 串口句柄
	 * @param bClearInput 是否清空输入缓冲区
	 * @param bClearOutput 是否清空输出缓冲区
	 * @return 是否成功
	 */
	static bool FlushBuffers(int32 Handle, bool bClearInput = true, bool bClearOutput = true);

	// ==================== 异步接收 ====================

	/**
	 * 启动异步读取
	 * @param Handle 串口句柄
	 * @param Callback 数据接收回调
	 * @return 是否启动成功
	 */
	static bool StartAsyncRead(int32 Handle, const FOnSerialDataReceived& Callback);

	/**
	 * 停止异步读取
	 * @param Handle 串口句柄
	 */
	static void StopAsyncRead(int32 Handle);

	// ==================== 工具函数 ====================

	/**
	 * 获取系统中可用的串口列表
	 * @return 可用串口名称列表 (如 ["COM1", "COM3", "COM5"])
	 */
	static TArray<FString> GetAvailablePorts();

	/**
	 * 获取打开的串口数量
	 * @return 打开的串口数量
	 */
	static int32 GetOpenPortCount();

private:
	/** 串口信息映射表 */
	static TMap<int32, TSharedPtr<FSerialPortInfo>> OpenPorts;

	/** 下一个可用的句柄 ID */
	static int32 NextHandleId;

	/** 线程安全锁 */
	static FCriticalSection PortLock;
};
