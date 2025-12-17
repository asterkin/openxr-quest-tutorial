# Chapter 2

Quest/Android/Vulkan port of Chapter 2 from the OpenXR Tutorial series.

- Original tutorial: [Chapter 2: OpenXR Setup](https://openxr-tutorial.com//android/vulkan/2-setup.html#openxr-setup)

---

## Features Demonstrated

This chapter focuses on the core OpenXR setup and lifecycle management. Key features include:

-   **OpenXR Instance Creation**: Demonstrates the initialization of the OpenXR runtime, including the use of extensions like `XR_EXT_debug_utils` for debugging and obtaining the `System Id`.
-   **OpenXR Session Creation**: Explains how to establish an active connection to the XR device through the creation of an `XrSession`.
-   **Event Loop Polling**: Highlights the importance of `xrPollEvent` for handling runtime events and managing the `XrSessionState`, including starting (`xrBeginSession`) and ending (`xrEndSession`) an XR session.

