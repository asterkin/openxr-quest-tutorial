# ADR-0003: Use OVR OpenXR Mobile SDK for Meta Extensions

**Status**: Accepted

## Context

Meta provides multiple SDK packages for Quest development. For a tutorial repository focused on native OpenXR development, we must choose which Meta SDK to use for Quest-specific features.

### Available Options

1. **Meta XR Core SDK** (Native)
   - Core OpenXR extensions (C/C++ headers and libraries)
   - Extensions: `FB_passthrough`, `FB_hand_tracking`, `FB_body_tracking`, etc.
   - Size: ~50 MB
   - Minimal package, headers only

2. **Meta XR SDK All-in-One UPM** (Unity)
   - Unity Package Manager format
   - Includes Core SDK + Unity assets
   - Size: ~500 MB
   - Unity-only

3. **OVR OpenXR Mobile SDK** (Full Native SDK with Samples)
   - Core SDK + SampleXrFramework + sample projects
   - Size: ~200 MB
   - Includes Meta XR Core extensions plus reference implementation samples

## Decision

We will use **OVR OpenXR Mobile SDK** as the source for Meta-specific OpenXR extensions and reference samples.

**Scope**: Native C++ development only. Unity tutorials are out of scope and may be addressed in a separate tutorial repository.

### Rationale

1. **Complete Learning Resource**: Includes both API headers and working sample implementations
2. **Native Focus**: Designed for NDK/C++ development (matches tutorial scope)
3. **Reference Framework**: Provides `SampleXrFramework` that demonstrates best practices
4. **Extension Coverage**: Includes all Quest-specific OpenXR extensions (`FB_*` vendor extensions)
5. **Adaptation Source**: Samples can be adapted and simplified for tutorial purposes

### What OVR OpenXR Mobile SDK Provides

- Meta OpenXR extension headers (`XR_FB_passthrough.h`, `XR_FB_hand_tracking.h`, etc.)
- Core SDK libraries for Meta extensions
- SampleXrFramework (reusable OpenXR boilerplate)
- Sample projects demonstrating Meta-specific features

### Installation Strategy

**TBD** - Installation approach (workstation-level vs per-project) requires further investigation to avoid duplication between tutorial and production development environments.

## Consequences

### Advantages

- Comprehensive reference material for Meta-specific OpenXR features
- Working sample code to understand extension usage patterns
- Native C++ focus aligns with tutorial goals
- Can adapt Meta samples while preserving educational clarity

### Disadvantages

- Additional SDK to manage alongside Android SDK/NDK
- Must manually track SDK updates for new extensions
- Installation strategy not yet determined

### Unity Support

**Out of scope**. Meta XR SDK All-in-One UPM is Unity-specific and excluded from this native NDK tutorial repository. Unity tutorials may be addressed separately in the future.

## References

- [Meta OpenXR SDK Downloads](https://developers.meta.com/horizon/downloads/package/oculus-openxr-mobile-sdk/)
- [Meta OpenXR Mobile SDK Documentation](https://developers.meta.com/horizon/documentation/native/android/mobile-intro)
- [Project Plan - SDK Strategy](../Project_Plan.md)
- [ADR-0002](adr-0002-workstation-level-prerequisite-installation.md) - Workstation-level installation strategy
