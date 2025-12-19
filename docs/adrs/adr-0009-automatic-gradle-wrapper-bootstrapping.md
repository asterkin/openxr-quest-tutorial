# ADR-0009: Automatic Gradle Wrapper Bootstrapping

**Status:** Accepted

**Date:** 2025-12-18

## Context

The standard Gradle wrapper mechanism requires three components to be committed to version control:
- `gradlew` / `gradlew.bat` (bootstrap scripts)
- `gradle/wrapper/gradle-wrapper.jar` (~60KB binary)
- `gradle/wrapper/gradle-wrapper.properties` (configuration)

This repository contains multiple independent Gradle projects:
- `openxr/hello_world/`
- `openxr/hello_xr/`
- `openxr/tutorial/` (root + 6 chapters)
- `meta/Samples/` (17+ samples)

Committing the gradle-wrapper.jar to each project results in:
1. **Repository bloat**: Multiple copies of the same ~60KB binary
2. **Maintenance overhead**: Each project's wrapper must be updated separately when upgrading Gradle
3. **Onboarding friction**: New contributors may encounter version mismatches if wrappers are inconsistent

The tutorial nature of this repository prioritizes ease of onboarding over offline build capability.

## Decision

We will **modify the `gradlew.bat` scripts to automatically download the Gradle wrapper JAR and generate the properties file if they are missing**.

### Implementation

The bootstrap logic is inserted into each `gradlew.bat` at the `:execute` label:

```batch
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
```

### Repository Configuration

1. **gitignore**: Add `**/gradle/wrapper/` to `.gitignore`
2. **Version control**: Commit only the modified `gradlew.bat` scripts
3. **Centralized version**: Gradle version is defined once in each `gradlew.bat`

### Alternatives Considered

**Option 1: Standard Gradle Wrapper (Commit JAR files) - Rejected**
- **Pros:** Works offline, verified/signed binaries, industry standard
- **Cons:** Repository bloat, maintenance overhead across many projects, version drift

**Option 2: Global Gradle Installation Requirement - Rejected**
- **Pros:** No wrapper files needed
- **Cons:** Requires pre-installed Gradle, version conflicts, poor onboarding experience

**Option 3: Single Shared Wrapper (Monorepo Style) - Rejected**
- **Pros:** Single copy of wrapper files
- **Cons:** Requires restructuring to true monorepo, breaks standalone sample usage

## Rationale

1. **Clone and Run**: New contributors can clone the repository and run `.\gradlew.bat assembleDebug` immediately without additional setup
2. **Smaller Repository**: Eliminates ~60KB × N redundant binary files
3. **Single Source of Truth**: Gradle version defined in one place per project hierarchy
4. **CI/CD Friendly**: Pipelines don't need pre-installed Gradle or cached wrapper files
5. **Tutorial Focus**: Prioritizes learning experience over production requirements

## Consequences

### Positive
- Zero-setup build experience for new contributors
- Smaller git repository size
- Consistent Gradle version across all samples (defined in scripts)
- Simplified CI/CD pipeline configuration

### Negative
- **Requires Internet**: First build must download wrapper JAR (~60KB)
- **Non-Standard**: Deviates from Gradle's expected pattern; may surprise experienced users
- **Platform Dependency**: Uses PowerShell on Windows (full path mitigates PATH issues)
- **External Dependency**: Downloads from GitHub (reliable but external)

### Neutral
- The downloaded JAR is cached locally in `gradle/wrapper/` (gitignored)
- Subsequent builds use the cached JAR without network access

## Hierarchy

The composite build structure enables building all samples from a single command:

```
openxr/
├── gradlew.bat              ← builds all OpenXR samples
├── settings.gradle.kts
├── hello_world/
│   └── gradlew.bat          ← standalone build
├── hello_xr/
│   └── gradlew.bat          ← standalone build
└── tutorial/
    ├── gradlew.bat          ← builds all chapters
    └── Chapter1-6/
        └── (use parent gradlew or standalone)
```

## References

- [Gradle Wrapper Documentation](https://docs.gradle.org/current/userguide/gradle_wrapper.html)
- [ADR-0007: Build and Run Architecture](adr-0007-build-and-run-architecture.md)
- [Project Plan - Phase 6: Upgrade to Latest Tool Versions](../Project_Plan.md)
