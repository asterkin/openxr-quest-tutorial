# OpenXR Quest Tutorial

> Learning and reference repository for OpenXR development on Meta Quest 3

[![Maintenance](https://img.shields.io/badge/Maintained-yes-green.svg)](https://github.com/asterkin/openxr-quest-tutorial/graphs/commit-activity)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE.md)
![OpenXR](https://img.shields.io/badge/OpenXR-1.0.34-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Meta%20Quest%203-orange.svg)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](#)
[![CMake](https://img.shields.io/badge/CMake-3.24.3-green.svg)](#)
[![Gradle](https://img.shields.io/badge/Gradle-8.1-orange.svg)](#)
[![Android](https://img.shields.io/badge/Android-API%2034-green.svg)](#)

---

## Overview

This project offers self-contained OpenXR tutorial samples, organized by complexity from minimal setup to advanced features. Its main objective is to accelerate the onboarding process for software engineers who possess basic C/C++ familiarity but are new to the specifics of Mixed Reality technology. It also aims to provide a ready-made starting point for new development by demonstrating core OpenXR and MetaXR features in a simplified form.

This is a **tutorial and learning repository**, NOT a production development environment.

## Target Platform

- **Device**: Meta Quest 3
- **OS**: Horizon OS v81 (Android API 34)
- **Graphics**: Vulkan 1.0.3+
- **XR Runtime**: OpenXR 1.0.34

## Getting Started

Follow these steps to get started with the tutorial samples:

1.  **Set Up Your Environment**: Before you can build and run the samples, you must configure your workstation. Follow the detailed instructions in the **[Environment Setup Guide](docs/Environment_Setup.md)**.

2.  **Explore the Samples**: Once your environment is ready, browse the available samples. We now have two main collections of samples:

    *   **[OpenXR Tutorial Samples](openxr/README.md)**: Our core tutorial samples, including `hello_world` (a great place to start) and a progressive tutorial series.

    *   **[Meta Quest Samples](meta/README.md)**: Integrated official Meta OpenXR SDK samples, demonstrating Quest-specific features.**

## Documentation

For more detailed information, refer to the following documents:

- **[Environment Setup Guide](docs/Environment_Setup.md)**: Detailed workstation and toolchain prerequisites.
- **[Repository Structure](docs/Repository_Structure.md)**: An overview of the directory layout.
- **[AI Agent Instructions](CLAUDE.md)**: Guidelines for using AI assistants with this project.

---
## Contributing
Contributions are welcome! Please read our [**Contributing Guidelines**](CONTRIBUTING.md) for more details on how to get involved.

---
## License and Conduct

-   **[MIT License](LICENSE.md)**: This project is licensed under the MIT License.
-   **[Code of Conduct](CODE_OF_CONDUCT.md)**: We adhere to the Contributor Covenant code of conduct.
