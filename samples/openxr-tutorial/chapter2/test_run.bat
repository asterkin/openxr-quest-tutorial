@echo off
REM OpenXR Tutorial Chapter 2 - Quick Test Script
REM Tests OpenXR instance, session, and event loop on Quest 3
REM
REM Usage:
REM   1. Build first: gradlew assembleVulkanDebug
REM   2. Then run: test_run.bat

echo.
echo ========================================
echo   OpenXR Tutorial Ch2 - Quick Test
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
adb shell am start -n com.example.openxr_tutorial_ch2/android.app.NativeActivity >nul 2>&1
echo [OK] App launched on Quest 3
echo.
echo NOTE: Chapter 2 creates OpenXR session and enters event loop.
echo       App will display black screen (no rendering yet).
echo       Waiting 5 seconds for initialization...
echo.

REM Wait for session initialization
echo Waiting 5 seconds for OpenXR session to initialize...
timeout /t 5 /nobreak >nul
echo.

:collect_logs
REM Collect logs
echo.
echo [5/5] Collecting logs...
echo.
echo ========== Chapter 2 Application Logs ==========
adb logcat -d -s openxr_tutorial:*

echo.
echo ========================================
echo   Test Complete
echo ========================================
echo.
echo Expected logs (if app is working):
echo   Application logs (openxr_tutorial):
echo     - "redirected cout" / "Testing cout redirect"
echo     - "OpenXR Tutorial Chapter 2"
echo     - "OpenXR Runtime: Oculus - ..." (Meta Quest runtime version)
echo     - "OPENXR: Reference Space Change pending for Session: 0x..."
echo     - Debug messenger verbose logs (loader trampoline/terminator)
echo.
echo   App should:
echo     - Not crash during startup
echo     - Create instance and session successfully
echo     - Enter main event loop
echo     - Display black screen (rendering is Chapter 3)
echo.
echo If you see NO LOGS above, the native library is not being called.
echo.
echo To stop the app: Use Quest menu or run adb_cleanup.bat
echo.
