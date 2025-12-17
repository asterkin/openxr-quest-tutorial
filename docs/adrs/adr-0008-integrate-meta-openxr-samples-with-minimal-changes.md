# ADR-0008: Integrate Meta OpenXR Samples with Minimal Changes

**Status:** Accepted

**Date:** 2025-12-17

## Context

This tutorial repository has established a streamlined build system for its core OpenXR samples (ADR-0005, ADR-0007), focusing on a Vulkan-first rendering approach and the modern Android `GameActivity` framework.

Phase 5 of the project plan requires demonstrating Meta-specific OpenXR extensions (e.g., `XR_FB_passthrough`, `XR_FB_hand_tracking_mesh`). The official Meta OpenXR SDK provides a comprehensive set of samples for these features.

Initial analysis suggested these samples might require significant adaptation to fit our established build blueprint due to their use of OpenGL ES, `NativeActivity`, and a custom `SampleXrFramework`. However, further testing revealed that the Meta samples compile and run efficiently "out of the box," with sub-minute build times. This makes their direct integration a viable and attractive option.

The primary integration challenge is not technical incompatibility but structural organization. The Meta samples are deeply nested within their original repository structure, which is not ideal for a flattened, easy-to-navigate tutorial project.

## Decision

We will **integrate the official Meta OpenXR SDK samples directly into this repository with minimal modifications.**

Instead of adapting the samples to our existing build blueprint, we will preserve their original architecture and build system. The main effort will be focused on reorganizing and flattening the file structure to make the samples more accessible.

### Integration Strategy

1.  **Relocate and Flatten:** The entire Meta `Samples` directory will be moved into the repository. The deeply nested project structure will be flattened to improve clarity and reduce path lengths.
2.  **Preserve Original Build System:** The samples will continue to use their own Gradle-based build system. We will not port them to our primary CMake-based blueprint.
3.  **Maintain Original Rendering and Framework:** The samples will retain their OpenGL ES rendering pipeline and their dependency on the `SampleXrFramework`. No porting to Vulkan or removal of the framework will be undertaken at this stage.
4.  **Incremental Streamlining:** This integration provides a stable starting point. Future work will focus on incrementally aligning versions (Gradle, NDK, C++), but the initial goal is simply to incorporate the working samples.

### Alternatives Considered

**Option 1: Adapt Meta Samples to Established Blueprint (Rejected)**
- **Pros:** Would create a completely consistent developer experience, with all samples using Vulkan and `GameActivity`.
- **Cons:** Requires a massive, time-consuming effort to reimplement the rendering logic for every sample in Vulkan. It would diverge significantly from the official vendor reference, making future updates difficult.

**Option 2: Skip Meta Samples Entirely (Rejected)**
- **Pros:** No integration work required.
- **Cons:** Fails to cover critical Quest-specific features, which is a primary goal of this tutorial repository.

## Rationale

1.  **Speed of Delivery:** This approach delivers a rich set of functional, advanced samples to learners immediately, without a lengthy development cycle.
2.  **Low Initial Effort:** Since the samples work "out of the box," the integration effort is minimal and focused on organization rather than complex engineering.
3.  **Value of Reference Implementation:** Preserving the original samples provides learners with a direct view of Meta's official, "vendor-blessed" implementation, which is a valuable learning resource in itself.
4.  **Establishes a Stable Baseline:** By integrating the samples as-is, we create a solid foundation for future, incremental improvements and version alignments without the risk of a large, complex porting project.

## Consequences

### Positive
- A comprehensive suite of samples for Meta-specific features is now available.
- The fast build times of the original Meta samples are maintained.
- Learners can directly compare our core tutorial samples with an official vendor implementation.

### Negative
- **Architectural Inconsistency:** The repository now contains two distinct architectures and build systems (our core Vulkan/GameActivity samples vs. Meta's GLES/NativeActivity samples).
- **Increased Cognitive Load:** Learners must understand the context and differences between the two sets of samples.
- **Dependency Inheritance:** We inherit the `SampleXrFramework` and other dependencies from the Meta samples.

### Neutral
- The tutorial evolves into a "collection of examples" that showcases different valid approaches to building OpenXR applications on Quest.
- The architectural differences can serve as a practical lesson in framework design and trade-offs.

## References

- [Meta OpenXR SDK Repository](https://github.com/meta-quest/Meta-OpenXR-SDK)
- [ADR-0003: Use OVR OpenXR Mobile SDK for Meta Extensions](adr-0003-use-ovr-openxr-mobile-sdk-for-meta-extensions.md)
- [ADR-0005: OpenXR SDK Dependency Management](adr-0005-openxr-sdk-dependency-management-via-maven-and-fetchcontent.md)
- [ADR-0007: Build and Run Architecture](adr-0007-build-and-run-architecture.md)
