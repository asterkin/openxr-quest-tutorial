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
  - **Shaders/** - GLSL/HLSL shader files
  - **thirdparty/** - Third-party dependencies shared across chapters
  - **chapter1/** - Self-contained Gradle project: Introduction & Setup
  - **chapter3/** - Self-contained Gradle project: GraphicsAPI enhancements & swapchains
  - *Additional chapters planned* - See [openxr-tutorial/README.md](../samples/openxr-tutorial/README.md)
- **meta/** - Meta Quest-specific samples from OVR SDK (planned)
