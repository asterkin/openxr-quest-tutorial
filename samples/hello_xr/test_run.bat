@echo off
REM OpenXR Hello XR - Quick Test Script
REM Tests full hello_xr sample with Vulkan rendering
REM
REM Usage:
REM   1. Build first: gradlew assembleVulkanDebug
REM   2. Then run: test_run.bat

echo.
echo ========================================
echo   OpenXR Hello XR - Quick Test
echo ========================================
echo.

REM Check if Quest is connected
echo [1/4] Checking for Quest device...
adb devices | findstr /C:"device" >nul
if %errorlevel% neq 0 (
    echo [ERROR] No Quest device found. Connect your Quest and enable USB debugging.
    exit /b 1
)
echo [OK] Quest device connected
echo.

REM Check if APK exists
echo [2/4] Checking for APK...
if not exist build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk (
    echo [ERROR] APK not found. Build first with: gradlew assembleVulkanDebug
    exit /b 1
)
echo [OK] APK found
echo.

REM Install APK
echo [3/5] Installing APK...
adb install -r build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Installation failed
    exit /b 1
)
echo [OK] APK installed
echo.

REM Clear logs and launch
echo [4/5] Launching app (put on headset)...
adb logcat -c
adb shell am start -n org.khronos.openxr.hello_xr.vulkan/android.app.NativeActivity >nul 2>&1
echo [OK] App launched
echo.
echo NOTE: This app renders cubes in VR. Put on headset to see them.
echo       App runs continuously until you exit.
echo.

REM Stream logs
echo [5/5] Streaming logs (Ctrl+C to stop)...
echo.
adb logcat -s "hello_xr:*"
