from __future__ import annotations

import argparse
import re
import shutil
import zipfile
from pathlib import Path


VULKAN_INCLUDE = "GraphicsAPI_Vulkan.h"
COMMON_CMAKE_INCLUDE = 'include("../Common/CMakeLists.txt")'
SHADERS_CMAKE_INCLUDE = 'include("../Shaders/CMakeLists.txt")'

# Patterns that indicate shader compilation in original CMakeLists.txt
SHADER_PATTERNS = [
    "GLSL_SHADERS",
    "glsl_spv_shader",
    "../Shaders/",
    "SHADER_DEST",
]
GRADLE_SNIPPET = """plugins {
    id 'com.android.application'
}
apply from: "${rootDir}/../Common/app/build.gradle"
"""

OLD_UNINSTALL_SNIPPET = """// Restart vrshell after uninstall to clear stale icon
tasks.matching { it.name.startsWith("uninstall") }.configureEach {
    doLast {
        exec { commandLine 'adb', 'shell', 'am', 'force-stop', 'com.oculus.vrshell' }
        exec { commandLine 'adb', 'shell', 'monkey', '-p', 'com.oculus.vrshell', '1' }
    }
}
"""

NEW_UNINSTALL_SNIPPET = """// Restart vrshell after uninstall to clear stale icon
def execOps = project.services.get(org.gradle.process.ExecOperations)

tasks.matching { it.name.startsWith("uninstall") }.configureEach {
    doLast {
        execOps.exec { spec ->
            spec.commandLine 'adb', 'shell', 'am', 'force-stop', 'com.oculus.vrshell'
        }
        execOps.exec { spec ->
            spec.commandLine 'adb', 'shell', 'monkey', '-p', 'com.oculus.vrshell', '1'
        }
    }
}
"""


def extract_chapter(chapter_dir: Path, zip_path: Path) -> bool:
    """Remove existing chapter dir and extract only its contents from the zip."""
    if not zip_path.exists():
        print(f"Missing zip: {zip_path}")
        return False

    if chapter_dir.exists():
        shutil.rmtree(chapter_dir)
        print(f"Removed existing {chapter_dir}")

    chapter_prefix = f"{chapter_dir.name}/"
    with zipfile.ZipFile(zip_path) as zf:
        members = [m for m in zf.namelist() if m.startswith(chapter_prefix)]
        if not members:
            print(f"No {chapter_dir.name} directory in {zip_path}")
            return False
        for member in members:
            zf.extract(member, path=chapter_dir.parent)
    print(f"Extracted {chapter_dir.name} from {zip_path.name}")
    return True


def clean_main_includes(main_cpp: Path) -> None:
    """Keep only the Vulkan GraphicsAPI include."""
    if not main_cpp.exists():
        print(f"Skipped (missing): {main_cpp}")
        return

    lines = main_cpp.read_text(encoding="utf-8").splitlines()
    pattern = re.compile(r"#include\s*<\s*(GraphicsAPI_[^>]+)>")

    new_lines = []
    for line in lines:
        match = pattern.match(line.strip())
        if match:
            include_name = match.group(1)
            if include_name != VULKAN_INCLUDE:
                continue
        new_lines.append(line)

    if new_lines != lines:
        main_cpp.write_text("\n".join(new_lines) + "\n", encoding="utf-8")
        print(f"Updated {main_cpp}")
    else:
        print(f"No changes needed in {main_cpp}")


def has_shader_compilation(content: str) -> bool:
    """Check if CMakeLists.txt content includes shader compilation."""
    return any(pattern in content for pattern in SHADER_PATTERNS)


def update_cmake(cmake_file: Path) -> None:
    """Standardize CMakeLists to minimal chapter stub."""
    if not cmake_file.exists():
        print(f"Skipped (missing): {cmake_file}")
        return

    # Check original content for shader compilation before overwriting
    original_content = cmake_file.read_text(encoding="utf-8")
    needs_shaders = has_shader_compilation(original_content)

    # Build template with optional shader include
    shader_line = f"{SHADERS_CMAKE_INCLUDE}\n" if needs_shaders else ""

    template = f"""cmake_minimum_required(VERSION 3.22.1)
project("${{PROJECT_NAME}}")

# Main application files
set(APP_HEADERS
    # Main headers will be added here
)

set(APP_SOURCE
    main.cpp
)
include("../Common/CMakeLists.txt")
{shader_line}"""
    cmake_file.write_text(template, encoding="utf-8")

    shader_status = " (with shaders)" if needs_shaders else ""
    print(f"Updated {cmake_file}{shader_status}")


def update_app_gradle(app_gradle: Path) -> None:
    """Replace app/build.gradle with shared include."""
    app_gradle.parent.mkdir(parents=True, exist_ok=True)
    app_gradle.write_text(GRADLE_SNIPPET, encoding="utf-8")
    print(f"Updated {app_gradle}")


def update_common_uninstall_block(common_gradle: Path) -> None:
    """Swap deprecated exec closures for ExecOperations in the shared Gradle file."""
    if not common_gradle.exists():
        print(f"Skipped (missing): {common_gradle}")
        return

    content = common_gradle.read_text(encoding="utf-8")

    if "project.services.get(org.gradle.process.ExecOperations)" in content:
        print(f"No changes needed in {common_gradle}")
        return

    if OLD_UNINSTALL_SNIPPET not in content:
        print(f"Uninstall task block not found in {common_gradle}")
        return

    common_gradle.write_text(content.replace(OLD_UNINSTALL_SNIPPET, NEW_UNINSTALL_SNIPPET), encoding="utf-8")
    print(f"Updated uninstall task block in {common_gradle}")


def remove_gradle_properties(gradle_props: Path) -> None:
    """Remove chapter-level gradle.properties so top-level one is used."""
    if gradle_props.exists():
        gradle_props.unlink()
        print(f"Removed {gradle_props}")
    else:
        print(f"Skipped (missing): {gradle_props}")


def process_chapter(chapter_dir: Path, zip_path: Path) -> None:
    """Refresh a chapter from its zip and apply required tweaks."""
    print(f"Processing {chapter_dir.name}...")
    if not extract_chapter(chapter_dir, zip_path):
        return

    clean_main_includes(chapter_dir / "main.cpp")
    update_cmake(chapter_dir / "CMakeLists.txt")
    update_app_gradle(chapter_dir / "app" / "build.gradle")
    remove_gradle_properties(chapter_dir / "gradle.properties")


def resolve_targets(root: Path, chapters: list[str]) -> list[tuple[Path, Path]]:
    """Return list of (chapter_dir, zip_path) pairs."""
    targets: list[tuple[Path, Path]] = []
    if chapters:
        for name in chapters:
            stem = Path(name).stem
            targets.append((root / stem, root / f"{stem}.zip"))
    else:
        for zip_path in sorted(root.glob("Chapter*.zip")):
            targets.append((root / zip_path.stem, zip_path))
    return targets


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Reset OpenXR tutorial chapters from zips and apply minimal tweaks."
    )
    parser.add_argument(
        "chapters",
        nargs="*",
        help="Specific chapter names (e.g., Chapter1). Defaults to all Chapter*.zip files.",
    )
    args = parser.parse_args()

    root = Path(__file__).resolve().parent
    targets = resolve_targets(root, args.chapters)

    if not targets:
        print("No Chapter*.zip files found.")
        return

    update_common_uninstall_block(root / "Common" / "app" / "build.gradle")

    for chapter_dir, zip_path in targets:
        process_chapter(chapter_dir, zip_path)


if __name__ == "__main__":
    main()
