#!/usr/bin/env python3
"""Remove all existing gradle/wrapper directories from individual samples."""

import shutil
from pathlib import Path

def main():
    script_dir = Path(__file__).parent
    xr_samples_dir = script_dir / "XrSamples"

    removed_count = 0

    for sample_dir in xr_samples_dir.iterdir():
        if sample_dir.is_dir():
            wrapper_dir = sample_dir / "Projects" / "Android" / "gradle"
            if wrapper_dir.exists():
                shutil.rmtree(wrapper_dir)
                print(f"[REMOVED] {sample_dir.name}/gradle")
                removed_count += 1

    print(f"\nRemoved {removed_count} gradle wrapper directories")
    print("Run gradlew.bat in any sample to auto-bootstrap with Gradle 8.13")

if __name__ == "__main__":
    main()
