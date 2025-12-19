#!/usr/bin/env python3
"""Update all individual sample gradlew.bat files with auto-bootstrapping."""

from pathlib import Path

GRADLEW_BAT_CONTENT = r'''@rem
@rem Copyright 2015 the original author or authors.
@rem
@rem Licensed under the Apache License, Version 2.0 (the "License");
@rem you may not use this file except in compliance with the License.
@rem You may obtain a copy of the License at
@rem
@rem      https://www.apache.org/licenses/LICENSE-2.0
@rem
@rem Unless required by applicable law or agreed to in writing, software
@rem distributed under the License is distributed on an "AS IS" BASIS,
@rem WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@rem See the License for the specific language governing permissions and
@rem limitations under the License.
@rem
@rem SPDX-License-Identifier: Apache-2.0
@rem

@if "%DEBUG%"=="" @echo off
@rem ##########################################################################
@rem
@rem  Gradle startup script for Windows
@rem
@rem ##########################################################################

@rem Set local scope for the variables with windows NT shell
if "%OS%"=="Windows_NT" setlocal

set DIRNAME=%~dp0
if "%DIRNAME%"=="" set DIRNAME=.
@rem This is normally unused
set APP_BASE_NAME=%~n0
set APP_HOME=%DIRNAME%

@rem Resolve any "." and ".." in APP_HOME to make it shorter.
for %%i in ("%APP_HOME%") do set APP_HOME=%%~fi

@rem Add default JVM options here. You can also use JAVA_OPTS and GRADLE_OPTS to pass JVM options to this script.
set DEFAULT_JVM_OPTS="-Xmx64m" "-Xms64m"

@rem Find java.exe
if defined JAVA_HOME goto findJavaFromJavaHome

set JAVA_EXE=java.exe
%JAVA_EXE% -version >NUL 2>&1
if %ERRORLEVEL% equ 0 goto execute

echo. 1>&2
echo ERROR: JAVA_HOME is not set and no 'java' command could be found in your PATH. 1>&2
echo. 1>&2
echo Please set the JAVA_HOME variable in your environment to match the 1>&2
echo location of your Java installation. 1>&2

goto fail

:findJavaFromJavaHome
set JAVA_HOME=%JAVA_HOME:"=%
set JAVA_EXE=%JAVA_HOME%/bin/java.exe

if exist "%JAVA_EXE%" goto execute

echo. 1>&2
echo ERROR: JAVA_HOME is set to an invalid directory: %JAVA_HOME% 1>&2
echo. 1>&2
echo Please set the JAVA_HOME variable in your environment to match the 1>&2
echo location of your Java installation. 1>&2

goto fail

:execute
@rem Bootstrap Gradle wrapper if needed
set WRAPPER_DIR=%APP_HOME%\gradle\wrapper
set WRAPPER_JAR=%WRAPPER_DIR%\gradle-wrapper.jar
set WRAPPER_PROPERTIES=%WRAPPER_DIR%\gradle-wrapper.properties
set GRADLE_VERSION=8.13

@rem Create wrapper directory if it doesn't exist
if not exist "%WRAPPER_DIR%" (
    echo Initializing Gradle wrapper...
    mkdir "%WRAPPER_DIR%"
)

@rem Download gradle-wrapper.jar if missing
if not exist "%WRAPPER_JAR%" (
    echo Downloading gradle-wrapper.jar...
    %SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe -Command "& {Invoke-WebRequest -Uri 'https://github.com/gradle/gradle/raw/v%GRADLE_VERSION%.0/gradle/wrapper/gradle-wrapper.jar' -OutFile '%WRAPPER_JAR%'}"
    if %ERRORLEVEL% neq 0 (
        echo ERROR: Failed to download gradle-wrapper.jar 1>&2
        goto fail
    )
)

@rem Create gradle-wrapper.properties if missing
if not exist "%WRAPPER_PROPERTIES%" (
    echo Creating gradle-wrapper.properties...
    (
        echo distributionBase=GRADLE_USER_HOME
        echo distributionPath=wrapper/dists
        echo distributionUrl=https\://services.gradle.org/distributions/gradle-%GRADLE_VERSION%-bin.zip
        echo networkTimeout=10000
        echo validateDistributionUrl=true
        echo zipStoreBase=GRADLE_USER_HOME
        echo zipStorePath=wrapper/dists
    ) > "%WRAPPER_PROPERTIES%"
)

@rem Setup the command line

set CLASSPATH=%APP_HOME%\gradle\wrapper\gradle-wrapper.jar


@rem Execute Gradle
"%JAVA_EXE%" %DEFAULT_JVM_OPTS% %JAVA_OPTS% %GRADLE_OPTS% "-Dorg.gradle.appname=%APP_BASE_NAME%" -classpath "%CLASSPATH%" org.gradle.wrapper.GradleWrapperMain %*

:end
@rem End local scope for the variables with windows NT shell
if %ERRORLEVEL% equ 0 goto mainEnd

:fail
rem Set variable GRADLE_EXIT_CONSOLE if you need the _script_ return code instead of
rem the _cmd.exe /c_ return code!
set EXIT_CODE=%ERRORLEVEL%
if %EXIT_CODE% equ 0 set EXIT_CODE=1
if not ""=="%GRADLE_EXIT_CONSOLE%" exit %EXIT_CODE%
exit /b %EXIT_CODE%

:mainEnd
if "%OS%"=="Windows_NT" endlocal

:omega
'''

def main():
    script_dir = Path(__file__).parent
    xr_samples_dir = script_dir / "XrSamples"

    updated_count = 0

    for sample_dir in xr_samples_dir.iterdir():
        if sample_dir.is_dir():
            gradlew_path = sample_dir / "Projects" / "Android" / "gradlew.bat"
            if gradlew_path.parent.exists():
                with open(gradlew_path, 'w', encoding='utf-8', newline='\r\n') as f:
                    f.write(GRADLEW_BAT_CONTENT)
                print(f"[OK] {sample_dir.name}")
                updated_count += 1

    print(f"\nUpdated {updated_count} gradlew.bat files")

if __name__ == "__main__":
    main()
