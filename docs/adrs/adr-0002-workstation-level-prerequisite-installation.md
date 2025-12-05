# ADR-0002: Workstation-Level Prerequisite Installation

**Status**: Accepted

## Context

When establishing a development environment for OpenXR Quest development, we must decide where to install prerequisites (SDKs, tools, build systems). This decision affects not only this tutorial repository but all Quest 3 native development workflows on the same workstation.

### The Question

Should prerequisites be:
1. **Repository-scoped**: Installed in each project directory (vendored dependencies)
2. **User-scoped**: Installed per user account (`~/.local`, `AppData\Local`)
3. **Workstation-scoped**: Installed globally at system level (`Program Files`, shared paths)

### Environmental Context

The tutorial repository requires:
- Android SDK Platform 34, Build-Tools 34.0.0
- Android NDK r27 (27.2.12479018)
- Java 17, Gradle 8.x, CMake 3.22+
- Android Studio, Meta Quest Developer Hub
- Python 3.14

**Total disk footprint**: ~10 GB for full development environment

### Key Constraint

These prerequisites are needed for **both tutorial learning and production development** (InnoVision SDK, customer applications). Installing per-repository would duplicate 10 GB for each project.

### OpenXR SDK Exception

OpenXR SDK is **not** installed at workstation level. Instead:
- **Headers**: Downloaded by CMake during build (FetchContent from GitHub)
- **Loader**: Provided as Maven dependency (`org.khronos.openxr:openxr_loader_for_android`)
- No manual installation required

## Decision

We will **install all prerequisites at workstation level when possible, otherwise at user scope**.

### Rationale

1. **Single source of truth**: One SDK installation serves all projects (tutorial + production)
2. **Production-ready**: Same environment for tutorials and customer applications
3. **Efficient disk usage**: 10 GB total, not 10 GB per project
4. **Team consistency**: All developers have identical paths and versions
5. **Standard tooling**: Matches Android Studio and industry conventions

### Installation Strategy

- **System-level (workstation)**: When tool installer defaults to `Program Files` and requires admin privileges
- **User-level**: When tool installer defaults to `AppData\Local` and works without admin
- **Never repository-level**: SDKs are not vendored or checked into Git

Specific installation paths and procedures are documented in [Environment_Setup.md](../Environment_Setup.md).

## Consequences

### Advantages

- Faster onboarding: Install SDKs once, run any sample
- Lower disk usage across multiple projects
- Consistent environment between tutorial and production development
- Skills transfer: Learning environment matches production deployment
- Simplified documentation and maintenance

### Disadvantages

- All projects on the same workstation must use compatible SDK versions
- Admin privileges required for system-level installations
- Manual version management (no automatic SDK switching between projects)

### Mitigations

- Use NDK side-by-side installs to support multiple versions
- Document exact SDK versions in Environment_Setup.md
- Gradle build scripts validate NDK version at build time

## References

- [Environment Setup Guide](../Environment_Setup.md) - Complete installation instructions
- [Project Plan](../Project_Plan.md#core-principles) - Principle #1: "Computer-Level SDKs"
- [ADR-0001](adr-0001-consolidated-tutorial-repository-for-openxr-quest-samples.md) - Tutorial repository structure
