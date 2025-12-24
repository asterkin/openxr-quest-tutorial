# ADR-0013: Activity Model - GameActivity for OpenXR Tutorials, NativeActivity for Meta Samples

**Status:** Rejected

**Date:** 2025-12-22

## Context

Android native applications require an Activity to host the native code. Two options exist:

1. **NativeActivity** - Original Android approach (API 9+), requires Java glue code
2. **GameActivity** - Modern Jetpack library, eliminates Java code in simple cases

This repository contains samples from different origins with different activity models.

### GameActivity vs NativeActivity: Brief Comparison

| Aspect | GameActivity | NativeActivity |
|--------|--------------|----------------|
| **Origin** | Android Jetpack (2021+) | Android SDK (API 9, 2010) |
| **Java Code** | None required for simple cases | Requires Java/Kotlin glue |
| **Input Handling** | Modern, unified API | Manual handling |
| **Lifecycle** | Simplified callbacks | Lower-level callbacks |
| **Build Integration** | Prefab via Gradle | NDK native_app_glue |
| **Maintenance** | Actively maintained | Stable but minimal updates |

### Activity Model Transparency

Similar to graphics APIs (see [ADR-0012](adr-0012-graphics-api-strategy-vulkan-for-openxr-opengles-for-meta.md)), the activity model is **not abstracted** by OpenXR. Switching between activity types requires:

- Different native entry points (`android_main` signature differs)
- Different event loop handling
- Different input processing code
- Build system changes (Prefab vs NDK glue)

Converting samples from one model to another requires source code modifications, not just configuration changes.

## Decision

This ADR is rejected because it incorrectly attributes GameActivity usage to the OpenXR tutorial chapters. All OpenXR tutorial chapters use `android.app.NativeActivity` with no Java/Kotlin code (`android:hasCode="false"`). There is no GameActivity usage in the tutorial series, so the premise and decision are invalid.

We will maintain **different activity models** for different sample categories:

- **OpenXR tutorials**: Use **GameActivity** (modern, zero Java)
- **Meta XR samples**: Keep **NativeActivity** (as imported from Meta SDK)

This allows us to:
1. Demonstrate modern Android native development in our tutorials
2. Preserve Meta samples as-is for compatibility with Meta's documentation
3. Avoid conversion effort that adds no educational value
4. Keep both sample sets close to their respective origins

## Consequences

### Positive
- Tutorials showcase current best practices (GameActivity)
- Meta samples remain consistent with Meta's official examples
- No Java code required for OpenXR tutorials (simpler build)
- Reduced maintenance burden

### Negative
- Developers see two different native activity patterns
- No unified activity abstraction across the repository
- Meta samples require understanding Java/native bridge

### Neutral
- Each sample category has its own entry point patterns
- Documentation must specify which activity model each sample uses

## References
- [ADR-0008: Integrate Meta OpenXR Samples with Minimal Changes](adr-0008-integrate-meta-openxr-samples-with-minimal-changes.md)
- [ADR-0012: Graphics API Strategy](adr-0012-graphics-api-strategy-vulkan-for-openxr-opengles-for-meta.md)
- [Android GameActivity Documentation](https://developer.android.com/games/agdk/game-activity)
- [Android NativeActivity Documentation](https://developer.android.com/reference/android/app/NativeActivity)
