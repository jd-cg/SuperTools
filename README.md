# SuperTools

**[English](README.md) | [中文](README_CN.md)**

A universal utility framework plugin for Unreal Engine 5.7.

**Author**: lostpanda

## Overview

SuperTools provides commonly used utility functions including INI file operations and Windows API wrappers, designed to be easily extensible for game development needs.

## Features

### INI File Operations
- Read/Write string, integer, float, and boolean values
- Cross-platform support (Windows uses native API, other platforms use manual parsing)
- Blueprint-friendly interface

### Windows API Wrappers
- Window title get/set
- Screen resolution query
- Window position and size control
- Window state management (maximize, minimize, restore, bring to front)

> **Note**: Windows API functions only work on Windows platform. Other platforms return false or empty values.

### UDP Communication

**Send Functions:**

- `UdpSendBytes` - Send byte array to IP:Port
- `UdpSendString` - Send string message to IP:Port

**Simplified Receive Interface (Recommended):**

- `StartUdpReceive(Port)` - Start listening, returns handle
- `GetUdpMessage(Handle)` - Get latest message as string (auto-clears buffer)
- `GetAllUdpMessages(Handle)` - Get all messages as string array
- `StopUdpReceive(Handle)` - Stop listening

**Advanced Receive Interface:**

- `CreateUdpListener(Port, BufferSize)` - Create listener with custom buffer
- `HasUdpData(Handle)` - Check if data available
- `GetAllUdpPackets(Handle)` - Get packets with sender info (`FUdpReceivedPacket`)
- `BytesToString(Data)` - Convert byte array to string
- `DestroyUdpListener(Handle)` - Destroy listener

#### Blueprint Usage Example

```
// BeginPlay
Handle = StartUdpReceive(Port: 8080)

// Tick or Timer
if GetUdpMessage(Handle, Message, SenderIP, SenderPort):
    Print(Message)

// EndPlay
StopUdpReceive(Handle)
```

### Serial Port Communication

- Full serial port configuration (baud rate, data bits, parity, stop bits, flow control)
- Open/close/read/write operations
- Async reading with callback delegates
- Port enumeration (list available COM ports)
- Buffer management (flush, get available bytes)

> **Note**: Serial port functions are fully supported on Windows platform only. Other platforms return false or empty values.

## Project Structure

```
MySuperTools/
├── Source/MySuperTools/          # Main game module
├── Plugins/SuperTools/           # Plugin module
│   └── Source/
│       ├── SuperTools/           # Core plugin code
│       │   ├── Public/
│       │   │   ├── SuperToolsBlueprintLibrary.h
│       │   │   └── Utils/
│       │   │       ├── IniFileHelper.h
│       │   │       ├── WindowsAPIHelper.h
│       │   │       ├── UdpHelper.h
│       │   │       └── SerialPortHelper.h
│       │   └── Private/
│       └── ThirdParty/           # Third-party library integration
└── Config/                       # Configuration files
```

## Requirements

- Unreal Engine 5.7
- Visual Studio 2022 (Windows)
- Xcode (macOS)
- GCC/Clang (Linux)

## Installation

1. Clone the repository
2. Open `MySuperTools.uproject` with Unreal Engine 5.7
3. Build the project

## Usage

### Blueprint

All functions are available in Blueprints under the `SuperTools` category:

- `SuperTools|INI` - INI file operations
- `SuperTools|Windows` - Windows API functions
- `SuperTools|UDP` - UDP communication
- `SuperTools|SerialPort` - Serial port communication

### C++

```cpp
#include "SuperToolsBlueprintLibrary.h"

// Read INI value
FString Value = USuperToolsBlueprintLibrary::ReadIniString(
    TEXT("C:/Config/settings.ini"),
    TEXT("General"),
    TEXT("PlayerName"),
    TEXT("DefaultPlayer")
);

// Set window title
USuperToolsBlueprintLibrary::SetWindowTitle(TEXT("My Game"));
```

## Build Commands

```bash
# Build Editor (Development)
Build.bat  # Select option 1

# Build Game (Shipping)
Build.bat  # Select option 2

# Clean and Rebuild
Build.bat  # Select option 5
```

## License

Copyright lostpanda. All Rights Reserved.
