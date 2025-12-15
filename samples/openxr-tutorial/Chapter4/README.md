# Chapter 4

Quest/Android/Vulkan port of Chapter 4 from the OpenXR Tutorial series.

- Original tutorial: [Chapter 4: Interactions](https://openxr-tutorial.com//android/vulkan/4-actions.html#interactions)

---

## Features Demonstrated

This chapter introduces the OpenXR Interaction System for handling user input from controllers. Key features include:

-   **Action System**:
    -   Defining abstract actions (`XrAction`) and grouping them into action sets (`XrActionSet`).
    -   Creating actions for various input types, such as boolean (button presses), float (triggers), and poses (controller position and orientation).
-   **Interaction Profiles and Bindings**:
    -   Suggesting bindings to the runtime, which map physical controller inputs (e.g., the A button on an Oculus Touch controller) to the application's abstract actions.
    -   Handling different interaction profiles to support various controller types in a cross-platform manner.
-   **Input State Synchronization**:
    -   Attaching action sets to the session.
    -   Syncing action data each frame (`xrSyncActions`) to get the latest input state.
-   **Rendering Controller Poses**: Using pose actions to retrieve the real-world position and orientation of the controllers and render them in the virtual scene.

