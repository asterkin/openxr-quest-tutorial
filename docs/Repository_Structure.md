# Repository Structure

```
openxr-quest-tutorial/
├── README.md                    # Project overview
├── CLAUDE.md                    # AI agent instructions
├── .claude/                     # Claude Code configuration
│   └── skills/                  # Claude Code skills
├── docs/                        # Detailed documentation
│   ├── Repository_Structure.md  # This file - directory layout
│   ├── Project_Plan.md          # Architecture and phase breakdown
│   ├── Environment_Setup.md     # SDK installation guide
│   ├── Build_Deploy_Guide.md    # Build/deploy workflows (TBD)
│   └── adrs/                    # Architecture Decision Records
└── samples/                     # Sample projects
    ├── hello_world/             # Minimal OpenXR Hello World
    ├── hello_xr/                # Khronos OpenXR sample
    ├── openxr-tutorial/         # Progressive tutorial from openxr-tutorial.com
    │   ├── Common/              # Shared utility files for all chapters
    │   ├── chapter1/            # Introduction & Setup
    │   └── ...                  # Additional chapters (planned)
    └── meta/                    # Meta Quest samples (planned)
```

## Directory Descriptions

### Root Level

- **README.md** - Human-readable project overview and quick navigation
- **CLAUDE.md** - AI agent instructions and development guidelines

### .claude/

Claude Code configuration and extensions:

- **skills/** - Custom Claude Code skills for project workflows
  - **adr/** - Architecture Decision Record creation skill

### docs/

Detailed documentation for developers and AI agents:

- **Repository_Structure.md** - This file; directory layout and organization
- **Project_Plan.md** - Complete architecture, phase breakdown, and implementation roadmap
- **Environment_Setup.md** - SDK installation and development environment configuration
- **Build_Deploy_Guide.md** - Build system documentation and deployment workflows
- **adrs/** - Architecture Decision Records documenting significant technical decisions

### samples/

Tutorial samples organized by complexity and source:

- **hello_world/** - Minimal OpenXR application demonstrating basic Quest 3 setup
- **hello_xr/** - Khronos OpenXR conformance sample adapted for Quest
- **openxr-tutorial/** - Container for progressive tutorial series from openxr-tutorial.com
  - **Common/** - Shared utility files for all chapters (DebugOutput, GraphicsAPI, OpenXRDebugUtils)
  - **thirdparty/** - Third-party dependencies shared across chapters
  - **chapter1/** - Self-contained Gradle project: Introduction & Setup
  - *Additional chapters planned* - Each chapter is a standalone Gradle project
- **meta/** - Meta Quest-specific samples from OVR SDK (planned)

#### OpenXR Tutorial Structure Details

The `openxr-tutorial/` directory uses a **shared resources + standalone chapters** pattern:

```
openxr-tutorial/
├── README.md                  # Tutorial overview
├── Common/                    # Shared C++ utilities (referenced by all chapters)
│   ├── DebugOutput.h
│   ├── GraphicsAPI.h/.cpp
│   ├── GraphicsAPI_Vulkan.h/.cpp
│   ├── OpenXRDebugUtils.h/.cpp
│   └── ...
├── thirdparty/                # Shared third-party dependencies
│   └── ...
└── chapter1/                  # Self-contained Gradle project
    ├── app/                   # Android application module
    │   ├── build.gradle
    │   └── src/main/
    │       ├── AndroidManifest.xml
    │       └── res/
    ├── build.gradle           # Root build configuration
    ├── settings.gradle        # Gradle project settings
    ├── CMakeLists.txt         # Native build (references ../Common/)
    ├── main.cpp               # Chapter implementation
    ├── gradle/                # Gradle wrapper files
    ├── gradlew                # Gradle wrapper script (Unix)
    └── gradlew.bat            # Gradle wrapper script (Windows)
```

**Key Design Decisions:**
- Each chapter is a **standalone Gradle project** (can build independently)
- Each chapter has its **own Gradle wrapper** (matches hello_world/hello_xr pattern)
- Chapters **share** Common/ and thirdparty/ via CMake relative paths (`../Common/`)
- No parent-level Gradle build (openxr-tutorial/ is just a container)
- Build command: `cd chapter1 && gradlew assembleVulkanDebug`
