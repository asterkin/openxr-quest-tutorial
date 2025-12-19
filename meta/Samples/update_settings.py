#!/usr/bin/env python3
"""Update all individual sample settings.gradle files with plugin management."""

from pathlib import Path

SETTINGS_GRADLE_TEMPLATE = '''/*
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
pluginManagement {{
    repositories {{
        google()
        mavenCentral()
        gradlePluginPortal()
    }}
}}
dependencyResolutionManagement {{
    repositoriesMode.set(RepositoriesMode.PREFER_PROJECT)
    repositories {{
        google()
        mavenCentral()
    }}
}}
rootProject.name = "{project_name}"
'''

def main():
    script_dir = Path(__file__).parent
    xr_samples_dir = script_dir / "XrSamples"

    updated_count = 0

    for sample_dir in xr_samples_dir.iterdir():
        if sample_dir.is_dir():
            settings_path = sample_dir / "Projects" / "Android" / "settings.gradle"
            if settings_path.parent.exists():
                content = SETTINGS_GRADLE_TEMPLATE.format(project_name=sample_dir.name)
                with open(settings_path, 'w', encoding='utf-8', newline='\n') as f:
                    f.write(content)
                print(f"[OK] {sample_dir.name}")
                updated_count += 1

    print(f"\nUpdated {updated_count} settings.gradle files")

if __name__ == "__main__":
    main()
