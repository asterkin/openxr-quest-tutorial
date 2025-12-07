# OpenXR Quest Tutorial - Implementation Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

**Completed**: Phases 0, 1, 2, 3
**In Progress**: [Phase 4: OpenXR Tutorial Migration](#phase-4-openxr-tutorial-migration)
**Next**: [Phase 5: Meta SDK Samples](#phase-5-meta-sdk-samples)

## Implementation Phases

### Phase 0: Prerequisites

**Status**: ✅ Completed

- Install development tools (Android Studio, NDK r27, Java 17, Python 3.14)
- Configure environment (see [Environment_Setup.md](Environment_Setup.md))
- OpenXR SDK managed via Maven + CMake FetchContent

### Phase 1: Repository Foundation

**Status**: ✅ Completed

- Repository structure established
- Documentation framework created
- ADR infrastructure added ([docs/adrs/](adrs/))
- Build configuration established

### Phase 2: hello_world Sample

**Status**: ✅ Completed

**Documentation**: [samples/hello_world/](../samples/hello_world/)

### Phase 3: hello_xr Sample

**Status**: ✅ Completed (2024-12-04)

**Source**: [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr)

**Steps**:
1. Copy source files from OpenXR-SDK-Source release-1.1.54
2. Update CMakeLists.txt (CMake FetchContent for headers)
3. Update build.gradle (Maven dependency for loader)
4. Fix missing gfxwrapper files
5. Update GLAD to GLAD2 with OpenGL ES support
6. Add Quest metadata to AndroidManifest.xml
7. Create automated test scripts (test_run.bat, adb_cleanup.bat)
8. Document integration changes (INTEGRATION_NOTES.md)

**Result**: Standalone hello_xr sample building and running on Quest 3

**Documentation**: [samples/hello_xr/](../samples/hello_xr/)

### Phase 4: OpenXR Tutorial Migration

**Status**: 🔧 In Progress

**Source**: [openxr-tutorial.com Android/Vulkan Tutorial](https://openxr-tutorial.com/android/vulkan/1-introduction.html)

**Goal**: Adapt the progressive tutorial series for Quest 3 with our build system

**Steps**:
- [x] Implement and test Chapter 1 on Quest 3 (setup - coordinate with hello_world/hello_xr patterns)
- [x] Implement and test Chapter 2 on Quest 3 (OpenXR instance, session, events) - **Complete 2024-12-07**
- [ ] Implement and test Chapter 3 on Quest 3 (graphics rendering) - **In Progress**
- [ ] Implement and test Chapter 4 on Quest 3
- [ ] Implement and test Chapter 5 on Quest 3
- [ ] (Continue for remaining chapters)
- [ ] Identify duplications between samples
- [ ] Introduce common infrastructure if applicable

**Expected Output**: Progressive tutorial samples in `samples/openxr-tutorial/`

**Chapter 2 Implementation**: See [samples/openxr-tutorial/chapter2/](../samples/openxr-tutorial/chapter2/) for completed implementation with full documentation.

---

#### Chapter 3: Graphics Rendering (Detailed Plan)

**Status**: ⏳ Planned

**Source**: [openxr-tutorial.com - Chapter 3: Graphics](https://openxr-tutorial.com/android/vulkan/3-graphics.html)

**Goal**: Add complete rendering pipeline with swapchain management, stereo rendering, and shader-based geometry rendering

**Expected Location**: `samples/openxr-tutorial/chapter3/`

##### Core Graphics Features to Implement

1. **View Configuration & Environment Blend**
   - Enumerate view configurations (stereo/mono)
   - Query view configuration views (FOV, recommended resolution)
   - Enumerate environment blend modes (opaque/additive/alpha-blend)
   - Select appropriate blend mode for VR

2. **Swapchain Management**
   - Enumerate swapchain formats (color and depth)
   - Create color swapchains per view (2 for stereo)
   - Create depth swapchains per view
   - Enumerate swapchain images (acquire VkImage handles)
   - Acquire → Wait → Release swapchain synchronization

3. **Reference Space Creation**
   - Create `XR_REFERENCE_SPACE_TYPE_LOCAL` space
   - Understand coordinate system: +Y up, +X right, -Z forward
   - Use for view pose queries

4. **Frame Rendering Loop**
   - Implement `xrWaitFrame()` → `xrBeginFrame()` → `xrEndFrame()` pattern
   - Query view poses with `xrLocateViews()`
   - Handle `shouldRender` flag from frame state
   - Submit composition layers to compositor

5. **Vulkan Rendering Infrastructure**
   - Image layout management (color: OPTIMAL, depth: DEPTH_STENCIL_OPTIMAL)
   - Pipeline creation (shaders, vertex input, rasterization)
   - Vertex/index buffers for cube geometry
   - Uniform buffers for camera constants and model transforms
   - Command buffer recording for stereo views

6. **Shader System**
   - GLSL to SPIR-V compilation via CMake
   - Vertex shader: MVP transformation, per-vertex normals
   - Fragment shader: Simple lighting/coloring
   - Shader loading from APK assets

##### Implementation Steps

**Step 1: Project Setup** (Similar to Chapter 2) - ✅ **Complete**
- [x] Create `samples/openxr-tutorial/chapter3/` directory
- [x] Copy and adapt `app/build.gradle` from Chapter 2
- [x] Copy and adapt `CMakeLists.txt` from Chapter 2
- [x] Update `settings.gradle` to include chapter3 module
- [x] Create symlink to `Common/` directory (uses relative paths in CMakeLists.txt)
- [x] Copy `AndroidManifest.xml` with appropriate package name (`com.example.openxr_tutorial_ch3`)

**Step 2: GraphicsAPI Enhancements** - ✅ **Complete**
- [x] Add `GetDepthFormat()` method to GraphicsAPI_Vulkan
- [x] Add `GetViewConfigurationViews()` support
- [x] Add swapchain creation with proper Vulkan formats
- [x] Implement image view creation for swapchain images
- [x] Add `SelectSwapchainFormat()` with priority: `VK_FORMAT_B8G8R8A8_SRGB` → `VK_FORMAT_R8G8B8A8_SRGB` → UNORM variants

**Step 3: OpenXRTutorial Class Extensions** - ✅ **Complete**

New member variables:
- [x] `std::vector<XrViewConfigurationView> m_viewConfigurationViews`
- [x] `XrSpace m_localSpace` (reference space)
- [x] `std::vector<SwapchainInfo> m_colorSwapchains` (per view)
- [x] `std::vector<SwapchainInfo> m_depthSwapchains` (per view)
- [x] `XrEnvironmentBlendMode m_environmentBlendMode`

New data structures:
- [x] `SwapchainInfo` struct (XrSwapchain, format, std::vector<void*> imageViews)
- [x] `RenderLayerInfo` struct (XrCompositionLayerProjection, std::vector<XrCompositionLayerProjectionView>)

New methods:
- [x] `GetViewConfigurationViews()` - Query stereo view specs
- [x] `GetEnvironmentBlendModes()` - Get blend mode support
- [x] `CreateSwapchains()` - Create color/depth swapchains
- [x] `DestroySwapchains()` - Cleanup swapchains
- [x] `CreateReferenceSpace()` - Create LOCAL space
- [x] `DestroyReferenceSpace()` - Destroy space
- [x] `CreateResources()` - Initialize rendering resources
- [x] `DestroyResources()` - Cleanup rendering resources
- [x] `RenderFrame()` - Main rendering function
- [x] `RenderLayer()` - Render single projection layer
- [x] `RenderCuboid()` - Render cube geometry

**Step 4: Rendering Resources**
- [ ] Vertex buffer with cube positions (8 vertices, 3 floats each)
- [ ] Index buffer with cube triangles (36 indices for 12 triangles)
- [ ] Uniform buffer for `CameraConstants` (view-projection matrix, model matrix, color)
- [ ] Pipeline state object (shaders + vertex layout + rasterization state)
- [ ] Descriptor sets for uniform buffer binding

**Step 5: Shader Implementation**

Create `shaders/` directory with:
- [ ] `VertexShader.glsl`:
  ```glsl
  layout(binding = 0) uniform CameraConstants {
      mat4 viewProj;
      mat4 model;
      vec4 color;
  };
  layout(location = 0) in vec3 inPosition;
  layout(location = 0) out vec4 outColor;

  void main() {
      gl_Position = viewProj * model * vec4(inPosition, 1.0);
      outColor = color;
  }
  ```

- [ ] `PixelShader.glsl` (Fragment Shader):
  ```glsl
  layout(location = 0) in vec4 inColor;
  layout(location = 0) out vec4 outColor;

  void main() {
      outColor = inColor;
  }
  ```

- [ ] CMake integration for GLSL → SPIR-V compilation
  - Use `glslangValidator` or `glslc` compiler
  - Generate `.spv` files at build time
  - Package into APK assets

**Step 6: Frame Rendering Logic**

Main loop changes:
- [ ] Call `xrWaitFrame()` to get frame timing and `shouldRender`
- [ ] Call `xrBeginFrame()` to signal render start
- [ ] If `shouldRender`:
  - [ ] Call `xrLocateViews()` to get per-view poses and FOVs
  - [ ] Acquire swapchain images (color + depth)
  - [ ] Wait for swapchain image availability
  - [ ] Render each view to its swapchain
  - [ ] Release swapchain images
  - [ ] Build `XrCompositionLayerProjection` with projection views
- [ ] Call `xrEndFrame()` with composition layers

**Step 7: Build System Updates**

CMakeLists.txt additions:
- [ ] Add shader compilation custom commands
- [ ] Link additional Vulkan utilities if needed
- [ ] Include shader output directory in APK assets

build.gradle updates:
- [ ] Ensure `assets/` directory included in APK
- [ ] Verify shader `.spv` files packaged correctly

**Step 8: Testing Infrastructure**
- [ ] Create `test_run.bat` (adapted from Chapter 2)
- [ ] Create `adb_cleanup.bat` (adapted from Chapter 2)
- [ ] Update package ID in scripts to `com.example.openxr_tutorial_ch3`

**Step 9: Documentation**

Create comprehensive documentation:
- [ ] `README.md`:
  - Overview of graphics features
  - Build and deployment instructions
  - Expected behavior (colored cube rendering in stereo)
  - Troubleshooting (shader compilation, swapchain errors)
  - Differences from original tutorial

- [ ] `INTEGRATION_NOTES.md`:
  - Build system changes from original
  - Quest 3 platform adaptations
  - Shader compilation process
  - Swapchain format selection rationale
  - Vulkan-specific considerations (image layouts, synchronization)
  - Known differences (Vulkan-only, no D3D/OpenGL)
  - Verification checklist

- [ ] Code comments:
  - Document swapchain acquisition flow
  - Explain view pose transformation
  - Clarify projection layer submission

##### Platform-Specific Considerations

**AndroidManifest.xml**:
- Same structure as Chapter 2
- Update package name to `com.example.openxr_tutorial_ch3`
- Update library name to `openxr_tutorial_ch3`
- Keep `com.oculus.vr.focusaware` flag (no controller input yet)
- Vulkan 1.0.3 requirement unchanged

**Logging**:
- Reuse Android logging macros from `Common/DebugOutput.h`
- Log swapchain creation details (format, dimensions)
- Log view configuration (count, FOV)
- Log frame timing metrics (optional)

**Vulkan Specifics for Quest 3**:
- Prefer `VK_FORMAT_B8G8R8A8_SRGB` for color (Quest native format)
- Use `VK_FORMAT_D32_SFLOAT` or `VK_FORMAT_D16_UNORM` for depth
- Enable multiview rendering (optional optimization for later)
- Verify image layouts match OpenXR requirements

##### Expected Build Artifacts

After implementation:
```
chapter3/
├── app/
│   ├── build.gradle
│   ├── src/main/
│   │   ├── AndroidManifest.xml
│   │   └── assets/
│   │       └── shaders/
│   │           ├── VertexShader.spv
│   │           └── PixelShader.spv
│   └── build/outputs/apk/vulkan/debug/
│       └── app-vulkan-debug.apk  (~2MB with shaders)
├── CMakeLists.txt
├── main.cpp  (~750 lines, up from 465 in Chapter 2)
├── shaders/
│   ├── VertexShader.glsl
│   └── PixelShader.glsl
├── Common/  (symlink to ../Common)
├── test_run.bat
├── adb_cleanup.bat
├── README.md
└── INTEGRATION_NOTES.md
```

##### Expected Runtime Behavior

When running on Quest 3:
- OpenXR session created (inherited from Chapter 2)
- Swapchains created: 2 color + 2 depth (stereo)
- Reference space: LOCAL (gravity-aligned)
- Rendering: Two colored cubes (one per eye) floating in space
- Frame rate: 72 Hz or 90 Hz (Quest 3 refresh rates)
- Environment: Black background (opaque blend mode)
- No input handling yet (controllers visible but not used)

##### Verification Checklist

- [ ] APK builds without errors
- [ ] Shaders compile to SPIR-V successfully
- [ ] Native library includes rendering code (`libopenxr_tutorial_ch3.so` ~1.5MB)
- [ ] App launches on Quest 3
- [ ] Stereo rendering visible (two different views per eye)
- [ ] Cube geometry renders correctly (no artifacts)
- [ ] Head tracking updates view poses
- [ ] Frame timing stable (no dropped frames)
- [ ] Logs show swapchain creation (format, dimensions)
- [ ] App exits cleanly

##### Common Integration Challenges (Anticipated)

Based on Chapter 1 and 2 experience:

1. **Shader Compilation**:
   - CMake may not find `glslangValidator` by default
   - Solution: Document shader compiler installation or include precompiled `.spv` files

2. **Swapchain Format Selection**:
   - Runtime may not support tutorial's preferred formats
   - Solution: Implement fallback chain (SRGB → UNORM)

3. **Image Layout Transitions**:
   - Vulkan requires explicit layout transitions
   - Solution: Ensure GraphicsAPI handles layouts correctly per OpenXR spec

4. **Depth Buffer Usage**:
   - Quest may optimize rendering if depth not used
   - Solution: Actually use depth testing in pipeline state

5. **Asset Packaging**:
   - Shader `.spv` files may not package into APK
   - Solution: Verify `assets/` directory configuration in build.gradle

6. **Build Size Increase**:
   - Rendering code + shaders increase APK size
   - Expected: ~1.5-2MB (up from ~1MB in Chapter 2)

##### Lessons from Previous Chapters to Apply

**From Chapter 1 (hello_xr)**:
- Use CMake FetchContent for OpenXR headers (✓ already established)
- Maven dependency for OpenXR loader (✓ already established)
- Include GLAD2 if OpenGL ES variant added (N/A for Chapter 3)

**From Chapter 2**:
- Consistent package naming: `com.example.openxr_tutorial_chX`
- Unified logging tag: `"openxr_tutorial"`
- Reuse Common/ directory structure (✓)
- `com.oculus.vr.focusaware` metadata (✓)
- Automated test scripts (✓)
- Comprehensive INTEGRATION_NOTES.md (✓)

**New for Chapter 3**:
- Shader build system integration
- Asset pipeline for binary shaders
- Vulkan resource management patterns
- Swapchain lifecycle documentation

##### Timeline Estimate

Based on Chapter 2 experience (~1 day):

- **Setup & Build System**: 1-2 hours
- **GraphicsAPI Extensions**: 2-3 hours (swapchains, formats)
- **OpenXRTutorial Rendering Methods**: 3-4 hours (main complexity)
- **Shader Implementation**: 1-2 hours (simple shaders)
- **Testing & Debugging**: 2-3 hours
- **Documentation**: 2-3 hours

**Total**: 1-2 days (assuming no major blockers)

##### Success Criteria

Chapter 3 is complete when:
- ✅ APK builds and deploys successfully
- ✅ Cube renders in stereo with correct perspective
- ✅ Head tracking updates view smoothly
- ✅ No OpenXR or Vulkan validation errors
- ✅ Documentation matches Chapter 2 quality standards
- ✅ All verification checklist items pass
- ✅ Ready to proceed to Chapter 4 (extended features)

---

---

### Phase 5: Meta SDK Samples

**Status**: ⏳ Planned

**Source**: OVR OpenXR Mobile SDK 81.0 samples

**Goal**: Import Meta-specific OpenXR extension samples

**Key Samples** (selection TBD):
- XrPassthrough
- XrHandsFB
- XrControllers
- XrSpatialAnchor
- (Additional samples as needed)

**Steps**:
1. Copy SampleXrFramework to `samples/meta/_framework/`
2. Select and copy relevant Meta samples
3. Update CMakeLists.txt for each sample
4. Update build.gradle configurations
5. Add to root settings.gradle
6. Document Meta extensions used

**Expected Output**: Meta extension samples in `samples/meta/`

### Phase 6: Automation & Documentation

**Status**: ⏳ Planned

**Goal**: Python automation scripts and comprehensive documentation

**Steps**:
1. Create `scripts/build.py` (context-aware building)
2. Create `scripts/deploy.py` (context-aware deployment)
3. Create `scripts/setup_env.py` (environment verification)
4. Complete documentation in `docs/`
5. Add sample-specific analysis documents

**Expected Output**: Automated workflows and complete documentation
