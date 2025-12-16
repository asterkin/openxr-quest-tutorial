# ADR-0008: Adapt Meta OpenXR Samples Using Established Build Blueprint

**Status:** Proposed

**Date:** 2025-12-16

## Context

This tutorial repository has established a streamlined build system for OpenXR samples (ADR-0005, ADR-0007) that achieves:
- Fast build times (sub-minute for incremental builds)
- Vulkan-first rendering approach
- GameActivity-based Android integration (Java/Kotlin reserved for minimal Android-side code only when required for permissions/system integration, if any)
- OpenXR loader from Maven + headers via FetchContent

Phase 5 demonstrates Meta-specific OpenXR extensions (XR_FB_*, XR_META_*) via the Meta OpenXR SDK samples. However, these samples present integration challenges:

1. **Graphics API Mismatch**: Meta samples use OpenGL ES exclusively; our established samples use Vulkan
2. **Android Integration Mismatch**: Meta samples apply the older `NativeActivity` combined with explicit Java glue, whereas our project uses the modern, C++-centric `GameActivity` (part of the official Android Game Development Kit).
3. **Framework Dependency**: Meta samples depend on `SampleXrFramework`, a heavyweight abstraction layer
4. **Build System Complexity**: Meta's build system optimizes for their full sample suite, not standalone learning

### Alternatives Considered

**Option 1: Adopt Meta Build System As-Is**
- Pro: Vendor-blessed, minimal adaptation effort
- Con: Longer build times, OpenGL-only, heavy dependencies, inconsistent with existing samples

**Option 2: Skip Meta Samples Entirely**
- Pro: No adaptation work, pure portable OpenXR
- Con: Misses Quest-specific features (passthrough, hand tracking mesh, spatial anchors)

**Option 3: Adapt Meta Samples to Established Blueprint (Chosen)**
- Pro: Consistent fast builds, Vulkan rendering, educational focus maintained
- Con: Diverges from vendor reference, requires Vulkan reimplementation of rendering

## Decision

We will **adapt Meta OpenXR SDK samples** to use our established build blueprint rather than adopting Meta's build system wholesale.

### Adaptation Strategy

1.  **Isolate Extension Logic**: Analyze the Meta samples to isolate the specific OpenXR function calls and data structures required for a feature (e.g., `xrCreatePassthroughFB`).
2.  **Reimplement Rendering in Vulkan**: Port the visual aspects of each sample from OpenGL ES to our existing Vulkan rendering framework.
3.  **Integrate into Build System**: Add required Meta SDK headers and source files to the CMake target using the established `FetchContent` dependency model.
4.  **Maintain `GameActivity` Entry Point**: Ensure all samples continue to use the standard Android `GameActivity` without introducing new Java code.
5.  **Use Direct API Calls**: Interact with the OpenXR API directly, completely omitting Meta's `SampleXrFramework` unless a feature is impossible/too costly without some portion of it.

### Scope

**In Scope**:
- XR_FB_passthrough (passthrough layers)
- XR_FB_hand_tracking_mesh (hand tracking with mesh)
- XR_FB_spatial_entity (spatial anchors)
- Other XR_FB_*/XR_META_* extensions as needed

**Out of Scope**:
- SampleXrFramework adoption
- OpenGL ES rendering paths
- Meta's NativeActivity patterns

### Rationale

1. **Educational Consistency**: Learners experience uniform build/run workflow across all samples
2. **Vulkan Alignment**: Vulkan is the modern graphics API; Vulkan-first is our chosen baseline and aligns with modern Android XR directions
3. **Portability Foundation**: OpenXR extensions can be conditionally compiled; rendering layer is swappable
4. **Build Performance**: Fast iteration enables faster learning cycles
5. **Reduced Dependencies**: Minimal framework dependencies improve understanding of core OpenXR concepts

## Consequences

### Positive
- Consistent developer experience across all tutorial samples
- Fast build times maintained for Meta extension samples
- Vulkan expertise transfers to other OpenXR platforms (AndroidXR, etc.)
- Samples demonstrate pure OpenXR extension usage without framework abstraction

### Negative
- Initial adaptation effort per sample (Vulkan rendering reimplementation)
- Divergence from Meta's reference implementation may introduce subtle differences
- Cannot directly compare behavior with upstream Meta samples
- Must maintain Vulkan rendering code ourselves

### Neutral
- Meta samples serve as API reference, not copy-paste source
- Requires understanding both OpenGL ES (to read) and Vulkan (to write)
- Future Meta SDK updates require manual review for API changes

### Licensing/Attribution
- preserve license notices for complete you copy/paste portions of Meta samples
- mention Meta samples as a source of inspiration in case of complete rewrite

## References

- [Meta OpenXR SDK Repository](https://github.com/meta-quest/Meta-OpenXR-SDK)
- [ADR-0003: Use OVR OpenXR Mobile SDK for Meta Extensions](adr-0003-use-ovr-openxr-mobile-sdk-for-meta-extensions.md)
- [ADR-0005: OpenXR SDK Dependency Management](adr-0005-openxr-sdk-dependency-management-via-maven-and-fetchcontent.md)
- [ADR-0007: Build and Run Architecture](adr-0007-build-and-run-architecture.md)
