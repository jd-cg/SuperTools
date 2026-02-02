# SuperTools

**[English](README.md) | [中文](README_CN.md)**

A universal utility framework plugin for Unreal Engine 5.7.

**Author**: lostpanda

## Overview

SuperTools provides commonly used utility functions for game development, including file operations, network communication, system utilities, and more. All functions are Blueprint-friendly and designed to be easily extensible.

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
- `GetAllUdpPackets(Handle)` - Get packets with sender info
- `BytesToString(Data)` - Convert byte array to string
- `DestroyUdpListener(Handle)` - Destroy listener

### Serial Port Communication

- Full serial port configuration (baud rate, data bits, parity, stop bits, flow control)
- Open/close/read/write operations
- Port enumeration (list available COM ports)
- Buffer management (flush, get available bytes)

> **Note**: Serial port functions are fully supported on Windows platform only.

### Clipboard Operations

- `CopyToClipboard` - Copy text to clipboard
- `GetFromClipboard` - Get text from clipboard
- `HasClipboardText` - Check if clipboard has text
- `ClearClipboard` - Clear clipboard
- `CopyImageToClipboard` - Copy image file to clipboard (Windows only)
- `GetImageFromClipboard` - Save clipboard image to file (Windows only)
- `HasClipboardImage` - Check if clipboard has image (Windows only)

### File Dialog Operations

- `OpenFileDialog` - Open single file selection dialog
- `OpenFileDialogMultiple` - Open multiple file selection dialog
- `SaveFileDialog` - Open save file dialog
- `OpenFolderDialog` - Open folder selection dialog

### System Operations

- `OpenURL` - Open URL in default browser
- `OpenFolderInExplorer` - Open folder in file explorer
- `OpenFileWithDefaultApp` - Open file with default application
- `LaunchApplication` - Launch external application
- `GetEnvVariable` - Get environment variable
- `GetFormattedTime` - Get formatted current time
- `GetComputerName` - Get computer name
- `GetCurrentUserName` - Get current user name

### Data Processing

- `Base64Encode` / `Base64Decode` - Base64 encoding/decoding
- `MD5Hash` / `MD5HashFile` - MD5 hash calculation
- `SHA256Hash` / `SHA256HashFile` - SHA256 hash calculation (Windows uses CryptoAPI)

### JSON Operations

- `JsonGetString` / `JsonGetInt` / `JsonGetFloat` / `JsonGetBool` - Get values from JSON (supports nested paths like "data.user.name")
- `JsonGetStringArray` - Get string array from JSON
- `MakeJsonString` - Create simple key-value JSON
- `MapToJson` - Convert Map to JSON string
- `JsonToMap` - Parse JSON string to Map

### File I/O Operations

- `ReadTextFile` / `WriteTextFile` - Read/write text files
- `ReadFileLines` / `WriteFileLines` - Read/write text files by lines
- `ReadBinaryFile` / `WriteBinaryFile` - Read/write binary files
- `DoesFileExist` / `DoesDirectoryExist` - Check file/directory existence
- `CreateDirectoryPath` - Create directory (including parents)
- `DeleteFileAtPath` / `DeleteDirectoryAtPath` - Delete file/directory
- `CopyFileToPath` / `MoveFileToPath` - Copy/move files
- `GetFileSizeBytes` - Get file size
- `GetFilesInDir` / `GetDirectoriesInDir` - List files/directories
- `GetFileExt` / `GetFileNameFromPath` / `GetFileDir` - Path utilities

### Screenshot Operations

- `CaptureViewportToFile` - Capture game viewport to file
- `RequestScreenshotToFile` - Request async screenshot (uses engine screenshot system)
- `CaptureScreenToFile` - Capture entire screen (Windows only)
- `CaptureRegionToFile` - Capture screen region (Windows only)

### HTTP Operations (Async/Latent)

- `HttpGet` - Async HTTP GET request
- `HttpPost` - Async HTTP POST request
- `HttpDownloadFile` - Async file download

> **Note**: HTTP functions are latent actions that work asynchronously in Blueprints.

## Project Structure

```text
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
- `SuperTools|Clipboard` - Clipboard operations
- `SuperTools|FileDialog` - File dialog operations
- `SuperTools|System` - System operations
- `SuperTools|Data` - Data processing (Base64, MD5, SHA256)
- `SuperTools|JSON` - JSON operations
- `SuperTools|FileIO` - File I/O operations
- `SuperTools|Screenshot` - Screenshot operations
- `SuperTools|HTTP` - HTTP operations

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

// Read JSON value
FString Name = USuperToolsBlueprintLibrary::JsonGetString(
    JsonString,
    TEXT("data.user.name"),
    TEXT("Unknown")
);

// HTTP GET (in Blueprint or with latent action)
// Use HttpGet node in Blueprint for async requests
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
