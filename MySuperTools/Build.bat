@echo off
setlocal enabledelayedexpansion

:: ============================================
:: SuperTools Build Script
:: Author: lostpanda
:: ============================================

:: Configure UE engine path (modify according to your installation)
set UE_ROOT=D:\Ue\UE\UE_5.7

:: Project configuration
set PROJECT_NAME=MySuperTools
set PROJECT_FILE=%~dp0%PROJECT_NAME%.uproject

:: Check if project file exists
if not exist "%PROJECT_FILE%" (
    echo [ERROR] Project file not found: %PROJECT_FILE%
    pause
    exit /b 1
)

:: Check UE engine path
if not exist "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" (
    echo [ERROR] UE engine build script not found
    echo Please check UE_ROOT path: %UE_ROOT%
    pause
    exit /b 1
)

:: Display menu
:menu
cls
echo ============================================
echo   SuperTools Build Tool
echo ============================================
echo.
echo   Project: %PROJECT_NAME%
echo   Engine:  %UE_ROOT%
echo.
echo   [1] Build Editor (Development)
echo   [2] Build Editor (DebugGame)
echo   [3] Build Game (Development)
echo   [4] Build Game (Shipping)
echo   [5] Clean and Rebuild Editor
echo   [6] Generate VS Project Files
echo   [7] Open Project
echo   [0] Exit
echo.
echo ============================================

set /p choice=Select option [0-7]:

if "%choice%"=="1" goto build_editor_dev
if "%choice%"=="2" goto build_editor_debug
if "%choice%"=="3" goto build_game_dev
if "%choice%"=="4" goto build_game_shipping
if "%choice%"=="5" goto clean_build
if "%choice%"=="6" goto generate_project
if "%choice%"=="7" goto open_project
if "%choice%"=="0" goto end

echo [ERROR] Invalid selection, please try again
timeout /t 2 >nul
goto menu

:build_editor_dev
echo.
echo [BUILD] Editor - Development configuration...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" %PROJECT_NAME%Editor Win64 Development -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
goto build_done

:build_editor_debug
echo.
echo [BUILD] Editor - DebugGame configuration...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" %PROJECT_NAME%Editor Win64 DebugGame -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
goto build_done

:build_game_dev
echo.
echo [BUILD] Game - Development configuration...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" %PROJECT_NAME% Win64 Development -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
goto build_done

:build_game_shipping
echo.
echo [BUILD] Game - Shipping configuration...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" %PROJECT_NAME% Win64 Shipping -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
goto build_done

:clean_build
echo.
echo [CLEAN] Removing Intermediate and Binaries directories...
echo.
if exist "%~dp0Intermediate" rd /s /q "%~dp0Intermediate"
if exist "%~dp0Binaries" rd /s /q "%~dp0Binaries"
if exist "%~dp0Plugins\SuperTools\Intermediate" rd /s /q "%~dp0Plugins\SuperTools\Intermediate"
if exist "%~dp0Plugins\SuperTools\Binaries" rd /s /q "%~dp0Plugins\SuperTools\Binaries"
echo [CLEAN] Done
echo.
echo [BUILD] Editor - Development configuration...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" %PROJECT_NAME%Editor Win64 Development -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
goto build_done

:generate_project
echo.
echo [GENERATE] Visual Studio project files...
echo.
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="%PROJECT_FILE%" -game -engine
goto build_done

:open_project
echo.
echo [OPEN] Launching Unreal Editor...
start "" "%PROJECT_FILE%"
goto menu

:build_done
echo.
if %ERRORLEVEL% EQU 0 (
    echo ============================================
    echo   [SUCCESS] Build completed!
    echo ============================================
) else (
    echo ============================================
    echo   [FAILED] Build error, code: %ERRORLEVEL%
    echo ============================================
)
echo.
pause
goto menu

:end
echo.
echo Goodbye!
endlocal
exit /b 0
