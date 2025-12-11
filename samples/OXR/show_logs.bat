@echo off
setlocal

rem Detect chapter number from current directory name (expects ChapterX)
for %%A in ("%CD%") do set "BASE=%%~nA"
set "NUM=%BASE:Chapter=%"
if "%NUM%"=="%BASE%" (
    echo Could not detect chapter number from "%BASE%".
    echo Run this from a ChapterX directory.
    exit /b 1
)

set "PATTERN=OpenXR Tutorial Chapter %NUM%"
echo Filtering logcat for "%PATTERN%"...

if exist "%SystemRoot%\\System32\\findstr.exe" (
    adb logcat -d | "%SystemRoot%\\System32\\findstr.exe" /I /C:"%PATTERN%"
    exit /b %ERRORLEVEL%
)

for %%P in (powershell pwsh) do (
    %%P -NoLogo -Command "adb logcat -d ^| Select-String -SimpleMatch -Pattern '%PATTERN%'" 2>nul
    if not errorlevel 1 exit /b 0
)

echo No suitable filter tool found. Install PowerShell or ensure findstr is available.
exit /b 1
