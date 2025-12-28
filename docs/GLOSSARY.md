# Glossary

This glossary defines key terms and concepts relevant to OpenXR development on Meta Quest 3, covering Android fundamentals, OpenXR API concepts, and specific tools used in this repository.

## A

### Action
An OpenXR abstraction for user input (e.g., "Grab", "Teleport") that isolates application logic from specific hardware devices. Actions are mapped to concrete inputs via Interaction Profiles.

### Action Set
A collection of Actions that can be enabled or disabled together based on the application's context (e.g., "Gameplay", "Menu").

### Activity (Android)
A fundamental Android component representing a single screen with a user interface. In OpenXR apps on Quest, a `NativeActivity` or `GameActivity` is typically used to handle the lifecycle and window creation.

### ADB (Android Debug Bridge)
A command-line tool used to communicate with an Android device (like the Quest 3) for debugging, installing APKs, and viewing logs.

### APK (Android Package Kit)
The file format used to distribute and install applications on the Android operating system.

## C

### Context7
A documentation service used by the AI assistants in this repository to access up-to-date API references for C++, OpenXR, and Android NDK, overcoming the knowledge cutoff of LLMs.

## G

### Gradle
The build automation tool used for Android applications. It manages dependencies, compilation, packaging, and signing of the APK.

## H

### HMD (Head-Mounted Display)
A display device worn on the head, such as the Meta Quest 3, providing a virtual or mixed reality experience.

### Horizon OS
The operating system powering Meta Quest devices, based on Android Open Source Project (AOSP) but heavily customized for spatial computing.

## I

### Interaction Profile
An OpenXR concept that defines a mapping between abstract Actions and physical buttons/sensors on a specific controller (e.g., Oculus Touch Controller).

## J

### JNI (Java Native Interface)
A framework that allows Java code running in a Java Virtual Machine (JVM) to call and be called by native applications (programs specific to a hardware and operating system platform) and libraries written in other languages such as C, C++, and Assembly. Crucial for OpenXR apps which are primarily C++ but run on the Android runtime.

## L

### Layer
In OpenXR, a source of visual content to be composited by the runtime. The most common is the `projection layer` (the 3D world), but others exist for UI (quad layers) or passthrough.

## M

### Manifest (AndroidManifest.xml)
A required XML file for every Android application that describes essential information about the app to the Android build tools, the Android operating system, and Google Play (e.g., permissions, activities, intent filters).

### MQDH (Meta Quest Developer Hub)
A desktop application provided by Meta to simplify device management, APK installation, file transfer, and performance profiling for Quest developers.

## N

### NativeActivity
A subclass of Android's `Activity` that allows the entire application to be implemented in native code (C/C++) using the NDK, bypassing most Java setup.

### NDK (Native Development Kit)
A set of tools that allows you to use C and C++ code with Android. OpenXR development on Quest is primarily done using the NDK to maximize performance.

## O

### OpenXR
An open, royalty-free standard API from the Khronos Group that provides a cross-platform engine interface for Virtual Reality (VR) and Augmented Reality (AR) devices.

## R

### Reference Space
A specific type of OpenXR Space that maps to a well-known tracking origin, such as `LOCAL` (head-relative) or `STAGE` (room-scale bounds).

### Runtime (OpenXR Runtime)
The software implementation of the OpenXR standard for a specific hardware platform (e.g., the Meta XR Runtime on Quest 3). It handles tracking, composition, and driver interactions.

## S

### Session (XrSession)
An OpenXR object representing an application's intent to display XR content. It manages the lifecycle of the rendering loop and interaction with the runtime.

### Space (XrSpace)
A coordinate system in OpenXR used to track objects (like hands or the head) relative to a reference frame.

### Swapchain
A chain of image buffers used for rendering. The application renders into one image while the runtime displays another, ensuring smooth visual output.

## V

### View
Represents a specific perspective from which the scene is rendered. For stereoscopic VR, there are typically two views (Left Eye and Right Eye) per frame.

### Vulkan
A low-overhead, cross-platform 3D graphics and computing API. It is the preferred graphics API for OpenXR development on Android due to its performance and explicit control.

## X

### XrInstance
The root object in OpenXR, storing global state and application info. Creating an instance initializes the connection to the OpenXR loader and runtime.

### XrSystem
An OpenXR abstraction representing the physical XR device (headset + controllers). The application queries the system for properties and capabilities.
