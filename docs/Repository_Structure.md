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
│   └── adrs/                    # Architecture Decision Records
├── meta/                        # Integrated Meta Quest samples (originally from OVR SDK)
│   ├── INTEGRATION_NOTES.md     # Integration notes for Meta samples
│   ├── README.md                # Overview of Meta samples
│   ├── OpenXR/                  # Meta OpenXR SDK headers
│   └── Samples/                 # Original Meta OpenXR SDK samples
│       ├── 1stParty/            # First-party dependencies (OVR)
│       ├── 3rdParty/            # Third-party dependencies
│       ├── SampleXrFramework/   # Meta's sample framework
│       └── XrSamples/           # Individual Meta samples (e.g., XrPassthrough)
└── openxr/                      # Our OpenXR tutorial samples
    ├── README.md                # High-level overview of all samples
    ├── Build_Guidelines.md      # Common build and deployment instructions
    ├── hello_world/             # Minimal OpenXR Hello World
    ├── hello_xr/                # Khronos OpenXR sample
    ├── camera2_tutorial/        # Standalone Camera2 + OpenXR scaffold
    └── tutorial/                # Progressive tutorial from openxr-tutorial.com
        ├── Common/              # Shared utility files for all chapters
        ├── Chapter1/            # Introduction & Setup
        └── ...                  # Additional chapters
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

-   **README.md** - Primary entry point for documentation overview
-   **Repository_Structure.md** - This file; directory layout and organization
-   **Project_Plan.md** - Complete architecture, phase breakdown, and implementation roadmap
-   **Environment_Setup.md** - SDK installation and development environment configuration
-   **adrs/** - Architecture Decision Records documenting significant technical decisions

### meta/

Integrated Meta Quest-specific samples from the official OVR SDK. These samples are integrated with minimal modifications, retaining their original build system and architecture as documented in ADR-0008.

- **INTEGRATION_NOTES.md** - Notes specific to the integration of Meta samples.
- **README.md** - Overview and quick start guide for the integrated Meta samples.
- **OpenXR/** - Contains Meta OpenXR SDK headers and related files necessary for building the samples.
- **Samples/** - The original directory structure of the Meta OpenXR SDK examples.
  - **1stParty/** - First-party libraries and utilities provided by Meta (e.g., OVR libraries).
  - **3rdParty/** - Third-party dependencies used by Meta's samples.
  - **SampleXrFramework/** - Meta's proprietary framework used by their samples.
  - **XrSamples/** - Individual OpenXR samples demonstrating various Meta-specific extensions and features (e.g., passthrough, hand tracking).

### openxr/

Our core OpenXR tutorial samples, following the established build blueprint (Vulkan-first, NativeActivity).

- **README.md** - High-level overview of all tutorial samples within this directory.
- **Build_Guidelines.md** - Common instructions for building and running these samples.
- **hello_world/** - Minimal OpenXR application demonstrating basic Quest 3 setup.
- **hello_xr/** - Khronos OpenXR conformance sample adapted for Quest.
- **camera2_tutorial/** - Standalone OpenXR + Vulkan scaffold for the Camera2 passthrough tutorial.
- **tutorial/** - Container for a progressive tutorial series, providing step-by-step learning modules.
  - **Common/** - Shared utility files (e.g., DebugOutput, GraphicsAPI, OpenXRDebugUtils) used across tutorial chapters.
  - **Shaders/** - GLSL/HLSL shader files used in various tutorial chapters.
  - **thirdparty/** - Third-party dependencies shared across tutorial chapters.
  - **Chapter1/** - Self-contained Gradle project for the introductory chapter.
  - *Additional chapters planned* - Future chapters will be added here, each as a self-contained project.
