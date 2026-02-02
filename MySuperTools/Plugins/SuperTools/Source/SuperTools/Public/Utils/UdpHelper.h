// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ArrayReader.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

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
 * UDP 监听器信息结构
 */
struct FUdpListenerInfo
{
	FSocket* Socket;
	FUdpSocketReceiver* Receiver;
	FOnUdpDataReceived Callback;
	int32 Port;

	FUdpListenerInfo()
		: Socket(nullptr)
		, Receiver(nullptr)
		, Port(0)
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
};
