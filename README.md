# OpenXR Quest Tutorial

> Learning and reference repository for OpenXR development on Meta Quest 3

---

## Overview

This repository contains consolidated OpenXR samples from multiple sources, focused on **Quest flavor of NDK+OpenXR development**. This is a **tutorial and learning repository**, NOT a production development environment.

## Target Platform

- **Device**: Meta Quest 3
- **OS**: Horizon OS v81 (Android API 34)
- **Graphics**: Vulkan 1.0.3+
- **XR Runtime**: OpenXR 1.0.34

## Repository Structure

```
openxr-quest-tutorial/
├── README.md                    # This file
├── CLAUDE.md                    # AI agent instructions
├── docs/                        # Detailed documentation
│   ├── Project_Plan.md          # Architecture and phase breakdown
│   ├── Environment_Setup.md     # SDK installation guide (TBD)
│   └── Build_Deploy_Guide.md    # Build/deploy workflows (TBD)
└── samples/                     # Sample projects (TBD)
    ├── hello_xr/                # Khronos OpenXR sample
    └── meta/                    # Meta Quest samples
```

## Documentation

- **AI Agent Instructions**: See [CLAUDE.md](CLAUDE.md)
- **Project Architecture**: See [docs/Project_Plan.md](docs/Project_Plan.md)
- **Environment Setup**: See [docs/Environment_Setup.md](docs/Environment_Setup.md) (TBD)

## Prerequisites

Before working with this repository, you need:

1. **OpenXR SDK** (built from source for Android)
2. **Meta XR SDK** (OVR OpenXR Mobile SDK 81.0)
3. **Android NDK** (r27+)
4. **Meta Quest 3** with Developer Mode enabled

See [docs/Project_Plan.md](docs/Project_Plan.md) for detailed setup instructions.

## Getting Started

This repository is in initial setup phase. See [docs/Project_Plan.md](docs/Project_Plan.md) for the complete architecture and implementation plan.

## Current Status

**Phase**: Phase 0 - Repository Foundation
**Progress**: Initial structure created

Next steps:
1. Complete SDK installation and documentation
2. Extract hello_xr sample
3. Extract Meta samples
4. Create build/deploy automation

---

**Created**: 2025-12-03
**Focus**: Quest-specific OpenXR/NDK development
**Type**: Tutorial and learning repository
