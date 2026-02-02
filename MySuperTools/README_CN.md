# SuperTools

**[English](README.md) | [中文](README_CN.md)**

基于 Unreal Engine 5.7 的通用工具框架插件。

**作者**: lostpanda

## 概述

SuperTools 提供常用的工具函数，包括 INI 文件操作和 Windows API 封装，设计为易于扩展以满足游戏开发需求。

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

## 项目结构

```
MySuperTools/
├── Source/MySuperTools/          # 主游戏模块
├── Plugins/SuperTools/           # 插件模块
│   └── Source/
│       ├── SuperTools/           # 核心插件代码
│       │   ├── Public/
│       │   │   ├── SuperToolsBlueprintLibrary.h
│       │   │   └── Utils/
│       │   │       ├── IniFileHelper.h
│       │   │       └── WindowsAPIHelper.h
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
