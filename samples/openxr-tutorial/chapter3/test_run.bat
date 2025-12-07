@echo off
REM OpenXR Tutorial Chapter 3 - Quick Test Script
REM Tests GraphicsAPI, swapchains, and rendering on Quest 3
REM
REM Usage:
REM   1. Build first: gradlew assembleVulkanDebug
REM   2. Then run: test_run.bat

echo.
echo ========================================
echo   OpenXR Tutorial Ch3 - Quick Test
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
adb shell am start -n com.example.openxr_tutorial_ch3/android.app.NativeActivity >nul 2>&1
echo [OK] App launched on Quest 3
echo.
echo NOTE: Chapter 3 implements GraphicsAPI, swapchains, and rendering.
echo       App should display rendered cubes in stereo (floor and table).
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
echo ========== Chapter 3 Application Logs ==========
adb logcat -d -s openxr_tutorial:* 2>&1

echo.
echo NOTE: If no logs appear above, the logcat buffer may be full.
echo       Try running: adb logcat -c ^&^& adb shell am start -n com.example.openxr_tutorial_ch3/android.app.NativeActivity ^&^& timeout /t 3 ^&^& adb logcat -d -s openxr_tutorial:*

echo.
echo ========================================
echo   Test Complete
echo ========================================
echo.
echo Expected logs (if app is working):
echo   Application logs (openxr_tutorial):
echo     - "OpenXR Tutorial Chapter 3"
echo     - "OpenXR Runtime: Oculus - ..." (Meta Quest runtime version)
echo     - Swapchain creation messages
echo     - Rendering initialization
echo     - Debug messenger verbose logs
echo.
echo   App should:
echo     - Not crash during startup
echo     - Create instance, session, and swapchains successfully
echo     - Render stereo view with colored cubes (floor and table)
echo     - Display smooth 72Hz/90Hz rendering
echo.
echo If you see NO LOGS above, the native library is not being called.
echo.
echo To stop the app: Use Quest menu or run adb_cleanup.bat
echo.
