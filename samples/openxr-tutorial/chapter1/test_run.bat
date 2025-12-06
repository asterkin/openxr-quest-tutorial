@echo off
REM OpenXR Tutorial Chapter 1 - Quick Test Script
REM Tests OpenXR/Vulkan setup by running Chapter 1 APK on Quest 3
REM
REM Usage:
REM   1. Build first: gradlew assembleVulkanDebug
REM   2. Then run: test_run.bat

echo.
echo ========================================
echo   OpenXR Tutorial Ch1 - Quick Test
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
if not exist app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk (
    echo [ERROR] APK not found. Build first with: gradlew assembleVulkanDebug
    exit /b 1
)
echo [OK] APK found
echo.

REM Install APK
echo [3/5] Installing APK...
adb install -r app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Installation failed
    exit /b 1
)
echo [OK] APK installed
echo.

REM Clear logs and launch
echo [4/5] Launching app...
adb logcat -c
adb shell am start -n com.example.openxr_tutorial_ch1/android.app.NativeActivity >nul 2>&1
echo [OK] App launched on Quest 3
echo.
echo NOTE: Chapter 1 native code runs instantly, but Activity stays open.
echo       Waiting 3 seconds for native code to complete...
echo.

REM Wait for native code to execute (it runs in milliseconds)
echo Waiting 3 seconds for native code to complete...
timeout /t 3 /nobreak >nul
echo.

:collect_logs
REM Collect logs
echo.
echo [5/5] Collecting logs...
echo.
echo ========== Chapter 1 Debug Logs (OpenXR_Ch1) ==========
adb logcat -d -s OpenXR_Ch1:*
echo.
echo ========== Chapter 1 App Logs (openxr_tutorial) ==========
adb logcat -d -s openxr_tutorial:*

echo.
echo ========================================
echo   Test Complete
echo ========================================
echo.
echo Expected logs (if app is working):
echo   Debug logs (OpenXR_Ch1):
echo     - "===== android_main() started ====="
echo     - "JNI thread attached"
echo     - "xrInitializeLoaderKHR obtained successfully"
echo     - "OpenXR loader initialized successfully"
echo.
echo   App logs (openxr_tutorial):
echo     - "OpenXR Tutorial Chapter 1"
echo.
echo If you see NO LOGS above, the native library is not being called.
echo.
