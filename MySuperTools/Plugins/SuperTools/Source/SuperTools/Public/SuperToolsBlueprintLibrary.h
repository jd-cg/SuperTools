// Copyright lostpanda. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/SerialPortHelper.h"
#include "Utils/UdpHelper.h"
#include "Utils/ClipboardHelper.h"
#include "Utils/FileDialogHelper.h"
#include "Utils/SystemHelper.h"
#include "Utils/DataHelper.h"
#include "SuperToolsBlueprintLibrary.generated.h"

/**
 * SuperTools 蓝图函数库
 * 提供 INI 文件操作和 Windows API 功能的蓝图接口
 */
UCLASS()
class SUPERTOOLS_API USuperToolsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ==================== INI 文件操作 ====================

	/**
	 * 从 INI 文件读取字符串
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的字符串值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Read INI String", Keywords = "ini read string 读取 字符串"))
	static FString ReadIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& DefaultValue = TEXT(""));

	/**
	 * 从 INI 文件读取整数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的整数值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Read INI Int", Keywords = "ini read int 读取 整数"))
	static int32 ReadIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 DefaultValue = 0);

	/**
	 * 从 INI 文件读取浮点数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的浮点数值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Read INI Float", Keywords = "ini read float 读取 浮点数"))
	static float ReadIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float DefaultValue = 0.0f);

	/**
	 * 从 INI 文件读取布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param DefaultValue 默认值
	 * @return 读取的布尔值
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Read INI Bool", Keywords = "ini read bool 读取 布尔"))
	static bool ReadIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool DefaultValue = false);

	/**
	 * 向 INI 文件写入字符串
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Write INI String", Keywords = "ini write string 写入 字符串"))
	static bool WriteIniString(const FString& FilePath, const FString& Section, const FString& Key, const FString& Value);

	/**
	 * 向 INI 文件写入整数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Write INI Int", Keywords = "ini write int 写入 整数"))
	static bool WriteIniInt(const FString& FilePath, const FString& Section, const FString& Key, int32 Value);

	/**
	 * 向 INI 文件写入浮点数
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Write INI Float", Keywords = "ini write float 写入 浮点数"))
	static bool WriteIniFloat(const FString& FilePath, const FString& Section, const FString& Key, float Value);

	/**
	 * 向 INI 文件写入布尔值
	 * @param FilePath INI 文件路径
	 * @param Section 节名称
	 * @param Key 键名称
	 * @param Value 要写入的值
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|INI", meta = (DisplayName = "Write INI Bool", Keywords = "ini write bool 写入 布尔"))
	static bool WriteIniBool(const FString& FilePath, const FString& Section, const FString& Key, bool Value);

	// ==================== Windows API 操作 ====================
	// 注意: 这些函数仅在 Windows 平台有效，其他平台返回失败或空值

	/**
	 * 设置窗口标题
	 * @param Title 新的窗口标题
	 * @return 是否设置成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Set Window Title", Keywords = "window title set 窗口 标题 设置"))
	static bool SetWindowTitle(const FString& Title);

	/**
	 * 获取窗口标题
	 * @return 窗口标题 (非 Windows 平台返回空字符串)
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Windows", meta = (DisplayName = "Get Window Title", Keywords = "window title get 窗口 标题 获取"))
	static FString GetWindowTitle();

	/**
	 * 获取屏幕分辨率
	 * @param Width 输出宽度
	 * @param Height 输出高度
	 * @return 是否获取成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Windows", meta = (DisplayName = "Get Screen Resolution", Keywords = "screen resolution get 屏幕 分辨率 获取"))
	static bool GetScreenResolution(int32& Width, int32& Height);

	/**
	 * 设置窗口位置
	 * @param X X 坐标
	 * @param Y Y 坐标
	 * @return 是否设置成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Set Window Position", Keywords = "window position set 窗口 位置 设置"))
	static bool SetWindowPosition(int32 X, int32 Y);

	/**
	 * 设置窗口大小
	 * @param Width 宽度
	 * @param Height 高度
	 * @return 是否设置成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Set Window Size", Keywords = "window size set 窗口 大小 设置"))
	static bool SetWindowSize(int32 Width, int32 Height);

	/**
	 * 最大化窗口
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Maximize Window", Keywords = "window maximize 窗口 最大化"))
	static bool MaximizeWindow();

	/**
	 * 最小化窗口
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Minimize Window", Keywords = "window minimize 窗口 最小化"))
	static bool MinimizeWindow();

	/**
	 * 恢复窗口
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Restore Window", Keywords = "window restore 窗口 恢复"))
	static bool RestoreWindow();

	/**
	 * 将窗口置于最前
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Windows", meta = (DisplayName = "Bring Window To Front", Keywords = "window front bring 窗口 置顶"))
	static bool BringWindowToFront();

	// ==================== UDP 通信 ====================
	// 提供 UDP 数据发送功能

	/**
	 * 发送 UDP 字节数据
	 * @param IP 目标 IP 地址
	 * @param Port 目标端口
	 * @param Data 要发送的字节数据
	 * @return 是否发送成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "UDP Send Bytes", Keywords = "udp send bytes 发送 字节"))
	static bool UdpSendBytes(const FString& IP, int32 Port, const TArray<uint8>& Data);

	/**
	 * 发送 UDP 字符串消息
	 * @param IP 目标 IP 地址
	 * @param Port 目标端口
	 * @param Message 要发送的字符串消息
	 * @return 是否发送成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "UDP Send String", Keywords = "udp send string 发送 字符串"))
	static bool UdpSendString(const FString& IP, int32 Port, const FString& Message);

	/**
	 * 创建 UDP 监听器
	 * @param Port 监听端口
	 * @param MaxBufferSize 最大缓冲区大小 (默认 100)
	 * @return 监听器句柄, -1 表示创建失败
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Create UDP Listener", Keywords = "udp listener create 监听器 创建"))
	static int32 CreateUdpListener(int32 Port, int32 MaxBufferSize = 100);

	/**
	 * 销毁 UDP 监听器
	 * @param Handle 监听器句柄
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Destroy UDP Listener", Keywords = "udp listener destroy 监听器 销毁"))
	static void DestroyUdpListener(int32 Handle);

	/**
	 * 检查 UDP 监听器是否有效
	 * @param Handle 监听器句柄
	 * @return 是否有效
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|UDP", meta = (DisplayName = "Is UDP Listener Valid", Keywords = "udp listener valid 监听器 有效"))
	static bool IsUdpListenerValid(int32 Handle);

	/**
	 * 检查是否有接收到的 UDP 数据
	 * @param Handle 监听器句柄
	 * @return 是否有数据
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|UDP", meta = (DisplayName = "Has UDP Data", Keywords = "udp data has 数据 有"))
	static bool HasUdpData(int32 Handle);

	/**
	 * 获取接收到的 UDP 数据包数量
	 * @param Handle 监听器句柄
	 * @return 数据包数量
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|UDP", meta = (DisplayName = "Get UDP Packet Count", Keywords = "udp packet count 数据包 数量"))
	static int32 GetUdpPacketCount(int32 Handle);

	/**
	 * 获取并清空所有接收到的 UDP 数据包
	 * @param Handle 监听器句柄
	 * @return 接收到的数据包数组
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Get All UDP Packets", Keywords = "udp packet get all 数据包 获取"))
	static TArray<FUdpReceivedPacket> GetAllUdpPackets(int32 Handle);

	/**
	 * 获取最新的 UDP 数据包
	 * @param Handle 监听器句柄
	 * @param OutPacket 输出数据包
	 * @return 是否成功获取
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Get Latest UDP Packet", Keywords = "udp packet get latest 数据包 最新"))
	static bool GetLatestUdpPacket(int32 Handle, FUdpReceivedPacket& OutPacket);

	/**
	 * 清空 UDP 接收缓冲区
	 * @param Handle 监听器句柄
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Clear UDP Buffer", Keywords = "udp buffer clear 缓冲区 清空"))
	static void ClearUdpBuffer(int32 Handle);

	/**
	 * 将字节数组转换为字符串 (UTF-8)
	 * @param Data 字节数组
	 * @return 转换后的字符串
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|UDP", meta = (DisplayName = "Bytes To String", Keywords = "bytes string convert 字节 字符串 转换"))
	static FString BytesToString(const TArray<uint8>& Data);

	// ==================== UDP 简化接口 ====================
	// 更易用的 UDP 接收接口，适合大多数使用场景

	/**
	 * 开始 UDP 接收 (简化版)
	 * @param Port 监听端口
	 * @return 句柄, -1 表示失败
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Start UDP Receive", Keywords = "udp receive start 接收 开始"))
	static int32 StartUdpReceive(int32 Port);

	/**
	 * 停止 UDP 接收
	 * @param Handle 句柄
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Stop UDP Receive", Keywords = "udp receive stop 接收 停止"))
	static void StopUdpReceive(int32 Handle);

	/**
	 * 获取 UDP 消息 (字符串格式，自动清空缓冲区)
	 * @param Handle 句柄
	 * @param OutMessage 输出消息
	 * @param OutSenderIP 输出发送方 IP
	 * @param OutSenderPort 输出发送方端口
	 * @return 是否有消息
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Get UDP Message", Keywords = "udp message get 消息 获取"))
	static bool GetUdpMessage(int32 Handle, FString& OutMessage, FString& OutSenderIP, int32& OutSenderPort);

	/**
	 * 获取所有 UDP 消息 (字符串数组)
	 * @param Handle 句柄
	 * @return 消息字符串数组
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|UDP", meta = (DisplayName = "Get All UDP Messages", Keywords = "udp messages get all 消息 获取 所有"))
	static TArray<FString> GetAllUdpMessages(int32 Handle);

	// ==================== 串口通信 ====================
	// 注意: 串口功能仅在 Windows 平台完整可用

	/**
	 * 打开串口 (简化版)
	 * @param PortName 端口名称 (如 "COM1", "COM3")
	 * @param BaudRate 波特率 (默认 9600)
	 * @return 串口句柄, -1 表示打开失败
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Open Serial Port", Keywords = "serial port open 串口 打开"))
	static int32 OpenSerialPort(const FString& PortName, int32 BaudRate = 9600);

	/**
	 * 打开串口 (完整配置)
	 * @param PortName 端口名称
	 * @param Config 串口配置
	 * @return 串口句柄, -1 表示打开失败
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Open Serial Port With Config", Keywords = "serial port open config 串口 打开 配置"))
	static int32 OpenSerialPortWithConfig(const FString& PortName, const FSerialPortConfig& Config);

	/**
	 * 关闭串口
	 * @param Handle 串口句柄
	 * @return 是否关闭成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Close Serial Port", Keywords = "serial port close 串口 关闭"))
	static bool CloseSerialPort(int32 Handle);

	/**
	 * 检查串口是否打开
	 * @param Handle 串口句柄
	 * @return 是否打开
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|SerialPort", meta = (DisplayName = "Is Serial Port Open", Keywords = "serial port open check 串口 打开 检查"))
	static bool IsSerialPortOpen(int32 Handle);

	/**
	 * 向串口写入字节数据
	 * @param Handle 串口句柄
	 * @param Data 要写入的数据
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Serial Write Bytes", Keywords = "serial port write bytes 串口 写入 字节"))
	static bool SerialWriteBytes(int32 Handle, const TArray<uint8>& Data);

	/**
	 * 向串口写入字符串
	 * @param Handle 串口句柄
	 * @param Message 要写入的字符串
	 * @return 是否写入成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Serial Write String", Keywords = "serial port write string 串口 写入 字符串"))
	static bool SerialWriteString(int32 Handle, const FString& Message);

	/**
	 * 从串口读取字节数据
	 * @param Handle 串口句柄
	 * @param MaxBytes 最大读取字节数
	 * @return 读取到的数据
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Serial Read Bytes", Keywords = "serial port read bytes 串口 读取 字节"))
	static TArray<uint8> SerialReadBytes(int32 Handle, int32 MaxBytes = 1024);

	/**
	 * 从串口读取字符串
	 * @param Handle 串口句柄
	 * @param MaxBytes 最大读取字节数
	 * @return 读取到的字符串
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Serial Read String", Keywords = "serial port read string 串口 读取 字符串"))
	static FString SerialReadString(int32 Handle, int32 MaxBytes = 1024);

	/**
	 * 获取串口接收缓冲区中的可用字节数
	 * @param Handle 串口句柄
	 * @return 可用字节数, -1 表示错误
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|SerialPort", meta = (DisplayName = "Get Serial Bytes Available", Keywords = "serial port bytes available 串口 可用 字节"))
	static int32 GetSerialBytesAvailable(int32 Handle);

	/**
	 * 清空串口缓冲区
	 * @param Handle 串口句柄
	 * @param bClearInput 是否清空输入缓冲区
	 * @param bClearOutput 是否清空输出缓冲区
	 * @return 是否成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Flush Serial Buffers", Keywords = "serial port flush buffer 串口 清空 缓冲区"))
	static bool FlushSerialBuffers(int32 Handle, bool bClearInput = true, bool bClearOutput = true);

	/**
	 * 获取系统中可用的串口列表
	 * @return 可用串口名称列表
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|SerialPort", meta = (DisplayName = "Get Available Serial Ports", Keywords = "serial port list available 串口 列表 可用"))
	static TArray<FString> GetAvailableSerialPorts();

	// ==================== 剪贴板操作 ====================

	/**
	 * 复制文本到剪贴板
	 * @param Text 要复制的文本
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Clipboard", meta = (DisplayName = "Copy To Clipboard", Keywords = "clipboard copy text 剪贴板 复制 文本"))
	static bool CopyToClipboard(const FString& Text);

	/**
	 * 从剪贴板获取文本
	 * @param OutText 输出的文本
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Clipboard", meta = (DisplayName = "Get From Clipboard", Keywords = "clipboard paste get text 剪贴板 粘贴 获取 文本"))
	static bool GetFromClipboard(FString& OutText);

	/**
	 * 检查剪贴板是否有文本
	 * @return 剪贴板是否包含文本
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Clipboard", meta = (DisplayName = "Has Clipboard Text", Keywords = "clipboard has text 剪贴板 有 文本"))
	static bool HasClipboardText();

	/**
	 * 清空剪贴板
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Clipboard", meta = (DisplayName = "Clear Clipboard", Keywords = "clipboard clear 剪贴板 清空"))
	static bool ClearClipboard();

	/**
	 * 复制图像文件到剪贴板
	 * @param ImagePath 图像文件路径 (支持 PNG, BMP, JPG)
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Clipboard", meta = (DisplayName = "Copy Image To Clipboard", Keywords = "clipboard copy image 剪贴板 复制 图像 图片"))
	static bool CopyImageToClipboard(const FString& ImagePath);

	/**
	 * 从剪贴板获取图像并保存到文件
	 * @param SavePath 保存路径 (支持 PNG, BMP, JPG)
	 * @return 是否操作成功 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Clipboard", meta = (DisplayName = "Get Image From Clipboard", Keywords = "clipboard paste get image 剪贴板 粘贴 获取 图像 图片"))
	static bool GetImageFromClipboard(const FString& SavePath);

	/**
	 * 检查剪贴板是否有图像
	 * @return 剪贴板是否包含图像 (非 Windows 平台返回 false)
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Clipboard", meta = (DisplayName = "Has Clipboard Image", Keywords = "clipboard has image 剪贴板 有 图像 图片"))
	static bool HasClipboardImage();

	// ==================== 文件对话框 ====================

	/**
	 * 打开文件对话框 (单个文件)
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param FileTypes 文件类型过滤器
	 * @param OutFilePath 输出选中的文件路径
	 * @return 是否选择了文件
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|FileDialog", meta = (DisplayName = "Open File Dialog", Keywords = "file dialog open 文件 对话框 打开"))
	static bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFilePath);

	/**
	 * 打开文件对话框 (多个文件)
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param FileTypes 文件类型过滤器
	 * @param OutFilePaths 输出选中的文件路径数组
	 * @return 是否选择了文件
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|FileDialog", meta = (DisplayName = "Open File Dialog Multiple", Keywords = "file dialog open multiple 文件 对话框 打开 多个"))
	static bool OpenFileDialogMultiple(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFilePaths);

	/**
	 * 保存文件对话框
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param DefaultFileName 默认文件名
	 * @param FileTypes 文件类型过滤器
	 * @param OutFilePath 输出选中的文件路径
	 * @return 是否选择了路径
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|FileDialog", meta = (DisplayName = "Save File Dialog", Keywords = "file dialog save 文件 对话框 保存"))
	static bool SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFileName, const FString& FileTypes, FString& OutFilePath);

	/**
	 * 打开文件夹对话框
	 * @param DialogTitle 对话框标题
	 * @param DefaultPath 默认路径
	 * @param OutFolderPath 输出选中的文件夹路径
	 * @return 是否选择了文件夹
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|FileDialog", meta = (DisplayName = "Open Folder Dialog", Keywords = "folder dialog open 文件夹 对话框 打开"))
	static bool OpenFolderDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderPath);

	// ==================== 系统操作 ====================

	/**
	 * 在默认浏览器中打开 URL
	 * @param URL 要打开的 URL
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|System", meta = (DisplayName = "Open URL", Keywords = "url open browser 打开 浏览器"))
	static bool OpenURL(const FString& URL);

	/**
	 * 在文件资源管理器中打开文件夹
	 * @param FolderPath 要打开的文件夹路径
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|System", meta = (DisplayName = "Open Folder In Explorer", Keywords = "folder open explorer 文件夹 打开 资源管理器"))
	static bool OpenFolderInExplorer(const FString& FolderPath);

	/**
	 * 使用默认应用程序打开文件
	 * @param FilePath 要打开的文件路径
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|System", meta = (DisplayName = "Open File With Default App", Keywords = "file open default app 文件 打开 默认 应用"))
	static bool OpenFileWithDefaultApp(const FString& FilePath);

	/**
	 * 启动外部应用程序
	 * @param ExecutablePath 可执行文件路径
	 * @param Arguments 命令行参数
	 * @param bHidden 是否隐藏运行
	 * @return 是否操作成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|System", meta = (DisplayName = "Launch Application", Keywords = "launch app application 启动 应用 程序"))
	static bool LaunchApplication(const FString& ExecutablePath, const FString& Arguments = TEXT(""), bool bHidden = false);

	/**
	 * 获取环境变量
	 * @param VariableName 环境变量名称
	 * @return 环境变量值
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|System", meta = (DisplayName = "Get Environment Variable", Keywords = "environment variable get 环境变量 获取"))
	static FString GetEnvVariable(const FString& VariableName);

	/**
	 * 获取格式化的当前时间
	 * @param Format 时间格式字符串
	 * @return 格式化的时间字符串
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|System", meta = (DisplayName = "Get Formatted Time", Keywords = "time format get 时间 格式化 获取"))
	static FString GetFormattedTime(const FString& Format = TEXT("%Y-%m-%d %H:%M:%S"));

	/**
	 * 获取计算机名称
	 * @return 计算机名称
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|System", meta = (DisplayName = "Get Computer Name", Keywords = "computer name get 计算机 名称 获取"))
	static FString GetComputerName();

	/**
	 * 获取当前用户名
	 * @return 当前用户名
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|System", meta = (DisplayName = "Get User Name", Keywords = "user name get 用户名 获取"))
	static FString GetCurrentUserName();

	// ==================== 数据处理 ====================

	/**
	 * 将字符串编码为 Base64
	 * @param Input 输入字符串
	 * @return Base64 编码后的字符串
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Data", meta = (DisplayName = "Base64 Encode", Keywords = "base64 encode 编码"))
	static FString Base64Encode(const FString& Input);

	/**
	 * 解码 Base64 字符串
	 * @param Input Base64 编码的字符串
	 * @param OutDecoded 输出解码后的字符串
	 * @return 是否解码成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Data", meta = (DisplayName = "Base64 Decode", Keywords = "base64 decode 解码"))
	static bool Base64Decode(const FString& Input, FString& OutDecoded);

	/**
	 * 计算字符串的 MD5 哈希值
	 * @param Input 输入字符串
	 * @return MD5 哈希值 (32 字符十六进制字符串)
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Data", meta = (DisplayName = "MD5 Hash", Keywords = "md5 hash 哈希"))
	static FString MD5Hash(const FString& Input);

	/**
	 * 计算字符串的 SHA256 哈希值
	 * @param Input 输入字符串
	 * @return SHA256 哈希值 (64 字符十六进制字符串)
	 */
	UFUNCTION(BlueprintPure, Category = "SuperTools|Data", meta = (DisplayName = "SHA256 Hash", Keywords = "sha256 hash 哈希"))
	static FString SHA256Hash(const FString& Input);

	/**
	 * 计算文件的 MD5 哈希值
	 * @param FilePath 文件路径
	 * @param OutHash 输出哈希值
	 * @return 是否计算成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Data", meta = (DisplayName = "MD5 Hash File", Keywords = "md5 hash file 哈希 文件"))
	static bool MD5HashFile(const FString& FilePath, FString& OutHash);

	/**
	 * 计算文件的 SHA256 哈希值
	 * @param FilePath 文件路径
	 * @param OutHash 输出哈希值
	 * @return 是否计算成功
	 */
	UFUNCTION(BlueprintCallable, Category = "SuperTools|Data", meta = (DisplayName = "SHA256 Hash File", Keywords = "sha256 hash file 哈希 文件"))
	static bool SHA256HashFile(const FString& FilePath, FString& OutHash);
};
