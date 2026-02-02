// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ArrayReader.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "UdpHelper.generated.h"

// Forward declarations
class FSocket;
class FUdpSocketReceiver;

/**
 * UDP 数据接收回调委托
 * @param SenderIP 发送方 IP 地址
 * @param SenderPort 发送方端口
 * @param Data 接收到的数据
 */
DECLARE_DELEGATE_ThreeParams(FOnUdpDataReceived, const FString& /*SenderIP*/, int32 /*SenderPort*/, const TArray<uint8>& /*Data*/);

/**
 * UDP 接收数据包结构体 (蓝图可用)
 */
USTRUCT(BlueprintType)
struct SUPERTOOLS_API FUdpReceivedPacket
{
	GENERATED_BODY()

	/** 发送方 IP 地址 */
	UPROPERTY(BlueprintReadOnly, Category = "UDP")
	FString SenderIP;

	/** 发送方端口 */
	UPROPERTY(BlueprintReadOnly, Category = "UDP")
	int32 SenderPort = 0;

	/** 接收到的数据 */
	UPROPERTY(BlueprintReadOnly, Category = "UDP")
	TArray<uint8> Data;

	/** 接收时间戳 */
	UPROPERTY(BlueprintReadOnly, Category = "UDP")
	FDateTime Timestamp;

	FUdpReceivedPacket()
		: SenderPort(0)
		, Timestamp(FDateTime::Now())
	{
	}

	FUdpReceivedPacket(const FString& InSenderIP, int32 InSenderPort, const TArray<uint8>& InData)
		: SenderIP(InSenderIP)
		, SenderPort(InSenderPort)
		, Data(InData)
		, Timestamp(FDateTime::Now())
	{
	}
};

/**
 * UDP 监听器信息结构
 */
struct FUdpListenerInfo
{
	FSocket* Socket;
	FUdpSocketReceiver* Receiver;
	FOnUdpDataReceived Callback;
	int32 Port;

	/** 接收数据缓冲区 (用于蓝图轮询) */
	TArray<FUdpReceivedPacket> ReceivedBuffer;

	/** 缓冲区锁 */
	FCriticalSection BufferLock;

	/** 最大缓冲区大小 */
	int32 MaxBufferSize;

	FUdpListenerInfo()
		: Socket(nullptr)
		, Receiver(nullptr)
		, Port(0)
		, MaxBufferSize(100)
	{
	}
};

/**
 * UDP 通信辅助类
 * 提供 UDP 数据发送和接收功能
 */
class SUPERTOOLS_API FUdpHelper
{
public:
	// ==================== 发送功能 ====================

	/**
	 * 发送 UDP 数据包
	 * @param IP 目标 IP 地址
	 * @param Port 目标端口
	 * @param Data 要发送的数据
	 * @return 是否发送成功
	 */
	static bool SendTo(const FString& IP, int32 Port, const TArray<uint8>& Data);

	/**
	 * 发送 UDP 字符串消息
	 * @param IP 目标 IP 地址
	 * @param Port 目标端口
	 * @param Message 要发送的字符串消息
	 * @return 是否发送成功
	 */
	static bool SendStringTo(const FString& IP, int32 Port, const FString& Message);

	// ==================== 接收功能 ====================

	/**
	 * 创建 UDP 监听器
	 * @param Port 监听端口
	 * @param Callback 数据接收回调
	 * @return 监听器句柄 (用于后续管理), -1 表示创建失败
	 */
	static int32 CreateListener(int32 Port, const FOnUdpDataReceived& Callback);

	/**
	 * 销毁指定的 UDP 监听器
	 * @param Handle 监听器句柄
	 */
	static void DestroyListener(int32 Handle);

	/**
	 * 销毁所有 UDP 监听器
	 */
	static void DestroyAllListeners();

	/**
	 * 检查监听器是否存在
	 * @param Handle 监听器句柄
	 * @return 是否存在
	 */
	static bool IsListenerValid(int32 Handle);

	/**
	 * 获取活跃的监听器数量
	 * @return 监听器数量
	 */
	static int32 GetActiveListenerCount();

	// ==================== 蓝图轮询接口 ====================

	/**
	 * 创建 UDP 监听器 (蓝图版本，使用缓冲区)
	 * @param Port 监听端口
	 * @param MaxBufferSize 最大缓冲区大小 (默认 100)
	 * @return 监听器句柄, -1 表示创建失败
	 */
	static int32 CreateListenerWithBuffer(int32 Port, int32 MaxBufferSize = 100);

	/**
	 * 检查是否有接收到的数据
	 * @param Handle 监听器句柄
	 * @return 是否有数据
	 */
	static bool HasReceivedData(int32 Handle);

	/**
	 * 获取接收缓冲区中的数据包数量
	 * @param Handle 监听器句柄
	 * @return 数据包数量
	 */
	static int32 GetReceivedPacketCount(int32 Handle);

	/**
	 * 获取并清空所有接收到的数据包
	 * @param Handle 监听器句柄
	 * @return 接收到的数据包数组
	 */
	static TArray<FUdpReceivedPacket> GetAndClearReceivedPackets(int32 Handle);

	/**
	 * 获取最新的一个数据包 (不清空缓冲区)
	 * @param Handle 监听器句柄
	 * @param OutPacket 输出数据包
	 * @return 是否成功获取
	 */
	static bool GetLatestPacket(int32 Handle, FUdpReceivedPacket& OutPacket);

	/**
	 * 清空接收缓冲区
	 * @param Handle 监听器句柄
	 */
	static void ClearReceivedBuffer(int32 Handle);

private:
	/** 监听器映射表 */
	static TMap<int32, TSharedPtr<FUdpListenerInfo>> Listeners;

	/** 下一个可用的句柄 ID */
	static int32 NextHandleId;

	/** 线程安全锁 */
	static FCriticalSection ListenerLock;

	/**
	 * UDP 数据接收处理函数
	 * @param Socket 接收数据的 Socket
	 * @param Data 接收到的数据
	 * @param Handle 监听器句柄
	 */
	static void HandleDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Sender, int32 Handle);

	/**
	 * UDP 数据接收处理函数 (缓冲区版本)
	 * @param Data 接收到的数据
	 * @param Sender 发送方端点
	 * @param Handle 监听器句柄
	 */
	static void HandleDataReceivedToBuffer(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Sender, int32 Handle);
};
