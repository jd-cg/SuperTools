# SuperTools

**[English](README.md) | [中文](README_CN.md)**

基于 Unreal Engine 5.7 的通用工具框架插件。

**作者**: lostpanda

## 概述

SuperTools 提供游戏开发常用的工具函数，包括文件操作、网络通信、系统工具等。所有函数都支持蓝图调用，设计为易于扩展。

## 功能特性

### INI 文件操作

- 读写字符串、整数、浮点数和布尔值
- 跨平台支持（Windows 使用原生 API，其他平台使用手动解析）
- 蓝图友好接口

### Windows API 封装

- 获取/设置窗口标题
- 查询屏幕分辨率
- 窗口位置和大小控制
- 窗口状态管理（最大化、最小化、恢复、置顶）

> **注意**: Windows API 函数仅在 Windows 平台有效。其他平台返回 false 或空值。

### UDP 通信

**发送功能:**

- `UdpSendBytes` - 向 IP:Port 发送字节数组
- `UdpSendString` - 向 IP:Port 发送字符串消息

**简化接收接口 (推荐):**

- `StartUdpReceive(Port)` - 开始监听，返回句柄
- `GetUdpMessage(Handle)` - 获取最新消息（字符串格式，自动清空缓冲区）
- `GetAllUdpMessages(Handle)` - 获取所有消息（字符串数组）
- `StopUdpReceive(Handle)` - 停止监听

**高级接收接口:**

- `CreateUdpListener(Port, BufferSize)` - 创建自定义缓冲区大小的监听器
- `HasUdpData(Handle)` - 检查是否有数据
- `GetAllUdpPackets(Handle)` - 获取包含发送方信息的数据包
- `BytesToString(Data)` - 将字节数组转换为字符串
- `DestroyUdpListener(Handle)` - 销毁监听器

### 串口通信

- 完整的串口配置（波特率、数据位、奇偶校验、停止位、流控制）
- 打开/关闭/读取/写入操作
- 端口枚举（列出可用 COM 端口）
- 缓冲区管理（清空、获取可用字节数）

> **注意**: 串口功能仅在 Windows 平台完整支持。

### 剪贴板操作

- `CopyToClipboard` - 复制文本到剪贴板
- `GetFromClipboard` - 从剪贴板获取文本
- `HasClipboardText` - 检查剪贴板是否有文本
- `ClearClipboard` - 清空剪贴板
- `CopyImageToClipboard` - 复制图片文件到剪贴板（仅 Windows）
- `GetImageFromClipboard` - 保存剪贴板图片到文件（仅 Windows）
- `HasClipboardImage` - 检查剪贴板是否有图片（仅 Windows）

### 文件对话框操作

- `OpenFileDialog` - 打开单文件选择对话框
- `OpenFileDialogMultiple` - 打开多文件选择对话框
- `SaveFileDialog` - 打开保存文件对话框
- `OpenFolderDialog` - 打开文件夹选择对话框

### 系统操作

- `OpenURL` - 在默认浏览器中打开 URL
- `OpenFolderInExplorer` - 在文件资源管理器中打开文件夹
- `OpenFileWithDefaultApp` - 使用默认应用程序打开文件
- `LaunchApplication` - 启动外部应用程序
- `GetEnvVariable` - 获取环境变量
- `GetFormattedTime` - 获取格式化的当前时间
- `GetComputerName` - 获取计算机名称
- `GetCurrentUserName` - 获取当前用户名

### 数据处理

- `Base64Encode` / `Base64Decode` - Base64 编码/解码
- `MD5Hash` / `MD5HashFile` - MD5 哈希计算
- `SHA256Hash` / `SHA256HashFile` - SHA256 哈希计算（Windows 使用 CryptoAPI）

### JSON 操作

- `JsonGetString` / `JsonGetInt` / `JsonGetFloat` / `JsonGetBool` - 从 JSON 获取值（支持嵌套路径如 "data.user.name"）
- `JsonGetStringArray` - 从 JSON 获取字符串数组
- `MakeJsonString` - 创建简单的键值对 JSON
- `MapToJson` - 将 Map 转换为 JSON 字符串
- `JsonToMap` - 将 JSON 字符串解析为 Map

### 文件 I/O 操作

- `ReadTextFile` / `WriteTextFile` - 读写文本文件
- `ReadFileLines` / `WriteFileLines` - 按行读写文本文件
- `ReadBinaryFile` / `WriteBinaryFile` - 读写二进制文件
- `DoesFileExist` / `DoesDirectoryExist` - 检查文件/目录是否存在
- `CreateDirectoryPath` - 创建目录（包括父目录）
- `DeleteFileAtPath` / `DeleteDirectoryAtPath` - 删除文件/目录
- `CopyFileToPath` / `MoveFileToPath` - 复制/移动文件
- `GetFileSizeBytes` - 获取文件大小
- `GetFilesInDir` / `GetDirectoriesInDir` - 列出文件/目录
- `GetFileExt` / `GetFileNameFromPath` / `GetFileDir` - 路径工具函数

### 截图操作

- `CaptureViewportToFile` - 捕获游戏视口到文件
- `RequestScreenshotToFile` - 请求异步截图（使用引擎截图系统）
- `CaptureScreenToFile` - 捕获整个屏幕（仅 Windows）
- `CaptureRegionToFile` - 捕获屏幕区域（仅 Windows）

### HTTP 操作（异步/延迟）

- `HttpGet` - 异步 HTTP GET 请求
- `HttpPost` - 异步 HTTP POST 请求
- `HttpDownloadFile` - 异步文件下载

> **注意**: HTTP 函数是延迟动作，在蓝图中异步执行。

## 项目结构

```text
MySuperTools/
├── Source/MySuperTools/          # 主游戏模块
├── Plugins/SuperTools/           # 插件模块
│   └── Source/
│       ├── SuperTools/           # 核心插件代码
│       │   ├── Public/
│       │   │   ├── SuperToolsBlueprintLibrary.h
│       │   │   └── Utils/
│       │   │       ├── IniFileHelper.h
│       │   │       ├── WindowsAPIHelper.h
│       │   │       ├── UdpHelper.h
│       │   │       ├── SerialPortHelper.h
│       │   │       ├── ClipboardHelper.h
│       │   │       ├── FileDialogHelper.h
│       │   │       ├── SystemHelper.h
│       │   │       ├── DataHelper.h
│       │   │       ├── JsonHelper.h
│       │   │       ├── FileIOHelper.h
│       │   │       ├── ScreenshotHelper.h
│       │   │       ├── HttpHelper.h
│       │   │       └── HttpLatentActions.h
│       │   └── Private/
│       └── ThirdParty/           # 第三方库集成
└── Config/                       # 配置文件
```

## 环境要求

- Unreal Engine 5.7
- Visual Studio 2022 (Windows)
- Xcode (macOS)
- GCC/Clang (Linux)

## 安装

1. 克隆仓库
2. 使用 Unreal Engine 5.7 打开 `MySuperTools.uproject`
3. 构建项目

## 使用方法

### 蓝图

所有函数都可在蓝图中使用，位于 `SuperTools` 分类下：

- `SuperTools|INI` - INI 文件操作
- `SuperTools|Windows` - Windows API 函数
- `SuperTools|UDP` - UDP 通信
- `SuperTools|SerialPort` - 串口通信
- `SuperTools|Clipboard` - 剪贴板操作
- `SuperTools|FileDialog` - 文件对话框操作
- `SuperTools|System` - 系统操作
- `SuperTools|Data` - 数据处理（Base64、MD5、SHA256）
- `SuperTools|JSON` - JSON 操作
- `SuperTools|FileIO` - 文件 I/O 操作
- `SuperTools|Screenshot` - 截图操作
- `SuperTools|HTTP` - HTTP 操作

### C++

```cpp
#include "SuperToolsBlueprintLibrary.h"

// 读取 INI 值
FString Value = USuperToolsBlueprintLibrary::ReadIniString(
    TEXT("C:/Config/settings.ini"),
    TEXT("General"),
    TEXT("PlayerName"),
    TEXT("DefaultPlayer")
);

// 设置窗口标题
USuperToolsBlueprintLibrary::SetWindowTitle(TEXT("我的游戏"));

// 读取 JSON 值
FString Name = USuperToolsBlueprintLibrary::JsonGetString(
    JsonString,
    TEXT("data.user.name"),
    TEXT("Unknown")
);

// HTTP GET（在蓝图中使用延迟动作）
// 在蓝图中使用 HttpGet 节点进行异步请求
```

## 构建命令

```bash
# 构建编辑器版本 (Development)
Build.bat  # 选择选项 1

# 构建游戏版本 (Shipping)
Build.bat  # 选择选项 2

# 清理并重新构建
Build.bat  # 选择选项 5
```

## 许可证

Copyright lostpanda. All Rights Reserved.
