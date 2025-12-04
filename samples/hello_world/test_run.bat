@echo off
REM OpenXR Hello World - Quick Test Script
REM Tests OpenXR SDK setup by running pre-built APK on Quest
REM
REM Usage:
REM   1. Build first: gradlew assembleDebug
REM   2. Then run: test_run.bat

echo.
echo ========================================
echo   OpenXR Hello World - Quick Test
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
if not exist build\outputs\apk\debug\hello_world-debug.apk (
    echo [ERROR] APK not found. Build first with: gradlew assembleDebug
    exit /b 1
)
echo [OK] APK found
echo.

REM Install APK
echo [3/5] Installing APK...
adb install -r build\outputs\apk\debug\hello_world-debug.apk >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Installation failed
    exit /b 1
)
echo [OK] APK installed
echo.

REM Clear logs and launch
echo [4/5] Launching app...
adb logcat -c
adb shell am start -n com.example.openxr.helloworld/android.app.NativeActivity >nul 2>&1
echo [OK] App launched (will auto-exit in ~5 seconds)
echo.

REM Wait and collect logs
echo [5/5] Collecting logs...
timeout /t 6 /nobreak >nul
adb logcat -d -s "OpenXR_HelloWorld:*"

echo.
echo ========================================
echo   Test Complete
echo ========================================
echo.
echo Check for:
echo   - "OpenXR Instance created successfully"
echo   - "OpenXR System ID" (Quest detected)
echo   - "Auto-exiting after 300 frames"
echo.
