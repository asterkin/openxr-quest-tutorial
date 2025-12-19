#!/usr/bin/env python3
"""Update all Meta XR sample build.gradle files to use AGP 8.13.2 and modern configuration."""

import os
from pathlib import Path

# Sample configurations: (folder_name, applicationId, cmake_target)
SAMPLES = [
    ("XrPassthroughOcclusion", "com.oculus.xrpassthroughocclusion", "xrpassthroughocclusion"),
    ("XrVirtualKeyboard", "com.oculus.sdk.xrvirtualkeyboard", "xrvirtualkeyboard"),
    ("XrPassthrough", "com.oculus.xrpassthrough", "xrpassthrough"),
    ("XrCompositor_NativeActivity", "com.oculus.sdk.xrcompositor", "xrcompositor"),
    ("XrControllers", "com.oculus.sdk.xrcontrollers", "xrcontrollers"),
    ("XrSceneSharing", "com.oculus.sdk.scenesharing", "scenesharing"),
    ("XrSpaceWarp", "com.oculus.sdk.xrspacewarp", "xrspacewarp"),
    ("XrSceneModel", "com.oculus.sdk.scenemodel", "scenemodel"),
    ("XrMicrogestures", "com.oculus.sdk.xrmicrogestures", "xrmicrogestures"),
    ("XrSpatialAnchor", "com.oculus.sdk.spatialanchor", "spatialanchor"),
    ("XrHandsAndControllers", "com.oculus.xrsamples.xrhands_and_controllers", "xrsamples_xrhands_and_controllers"),
    ("XrBodyFaceEyeSocial", "com.oculus.xrsamples.xrbodyfaceeyesocial", "xrbodyfaceeyesocial"),
    ("XrColorSpaceFB", "com.oculus.sdk.xrcolorspacefb", "xrsamples_xrcolorspacefb"),
    ("XrDynamicObjects", "com.oculus.sdk.xrdynamicobjects", "xrdynamicobjects"),
    ("XrHandDataSource", "com.oculus.sdk.xrhanddatasource", "xrhanddatasource"),
    ("XrInput", "com.oculus.xrsamples.xrinput", "xrsamples_xrinput"),
    ("XrHandsFB", "com.oculus.sdk.xrhandsfb", "xrhandsfb"),
    ("XrColocationDiscovery", "com.oculus.sdk.xrcolocationdiscovery", "xrcolocationdiscovery"),
    ("XrHandTrackingWideMotionMode", "com.oculus.sdk.xrhandtrackingwidemotionmode", "xrhandtrackingwidemotionmode"),
]

BUILD_GRADLE_TEMPLATE = '''/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * Licensed under the Oculus SDK License Agreement (the "License");
 * you may not use the Oculus SDK except in compliance with the License,
 * which is provided at the time of installation or download, or which
 * otherwise accompanies this software in either electronic or hard copy form.
 *
 * You may obtain a copy of the License at
 * https://developer.oculus.com/licenses/oculussdk/
 *
 * Unless required by applicable law or agreed to in writing, the Oculus SDK
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

plugins {{
    id 'com.android.application'
}}

android {{
    compileSdk = 34
    ndkVersion = "29.0.14206865"
    namespace = "{applicationId}"

    defaultConfig {{
        applicationId "{applicationId}"
        minSdk = 29
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        ndk {{
            abiFilters = ['arm64-v8a']
        }}

        externalNativeBuild {{
            cmake {{
                arguments = ['-DANDROID_STL=c++_shared']
                cppFlags = ['-std=c++20']
                targets "{cmake_target}"
            }}
        }}
    }}

    sourceSets {{
        main {{
            manifest.srcFile 'AndroidManifest.xml'
            java.srcDirs = ['../../java']
            assets.srcDirs = ['../../assets']
            res.srcDirs = ['../../res']
        }}
    }}

    buildTypes {{
        debug {{
            debuggable = true
            jniDebuggable = true
        }}

        release {{
            debuggable = false
            minifyEnabled = false
        }}
    }}

    externalNativeBuild {{
        cmake {{
            path = file('../../../../CMakeLists.txt')
            version = "3.22.1"
        }}
    }}

    compileOptions {{
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }}

    buildFeatures {{
        prefab = true
    }}

    packaging {{
        jniLibs {{
            useLegacyPackaging = true
        }}
    }}
}}

dependencies {{
    // OpenXR loader from Maven Central
    implementation 'org.khronos.openxr:openxr_loader_for_android:1.1.54'
}}

// Restart vrshell after uninstall to clear stale icon (zombie icon cleanup)
def execOps = project.services.get(org.gradle.process.ExecOperations)

tasks.matching {{ it.name.startsWith("uninstall") }}.configureEach {{
    doLast {{
        execOps.exec {{ spec ->
            spec.commandLine 'adb', 'shell', 'am', 'force-stop', 'com.oculus.vrshell'
        }}
        execOps.exec {{ spec ->
            spec.commandLine 'adb', 'shell', 'monkey', '-p', 'com.oculus.vrshell', '1'
        }}
    }}
}}
'''

def main():
    script_dir = Path(__file__).parent
    xr_samples_dir = script_dir / "XrSamples"

    updated_count = 0

    for folder_name, application_id, cmake_target in SAMPLES:
        build_gradle_path = xr_samples_dir / folder_name / "Projects" / "Android" / "build.gradle"

        if not build_gradle_path.parent.exists():
            print(f"[SKIP] {folder_name}: Directory not found")
            continue

        content = BUILD_GRADLE_TEMPLATE.format(
            applicationId=application_id,
            cmake_target=cmake_target
        )

        with open(build_gradle_path, 'w', encoding='utf-8', newline='\n') as f:
            f.write(content)

        print(f"[OK] {folder_name}")
        updated_count += 1

    print(f"\nUpdated {updated_count} build.gradle files")

if __name__ == "__main__":
    main()
