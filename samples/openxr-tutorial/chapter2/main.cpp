// Copyright 2023, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

// OpenXR Tutorial for Khronos Group

#include <DebugOutput.h>

// Include only the graphics API for this platform
#if defined(XR_USE_GRAPHICS_API_D3D11)
#include <GraphicsAPI_D3D11.h>
#endif
#if defined(XR_USE_GRAPHICS_API_D3D12)
#include <GraphicsAPI_D3D12.h>
#endif
#if defined(XR_USE_GRAPHICS_API_OPENGL)
#include <GraphicsAPI_OpenGL.h>
#endif
#if defined(XR_USE_GRAPHICS_API_OPENGL_ES)
#include <GraphicsAPI_OpenGL_ES.h>
#endif
#if defined(XR_USE_GRAPHICS_API_VULKAN)
#include <GraphicsAPI_Vulkan.h>
#endif

#include <OpenXRDebugUtils.h>

#define XR_DOCS_CHAPTER_VERSION XR_DOCS_CHAPTER_1_4

class OpenXRTutorial {
public:
    OpenXRTutorial(GraphicsAPI_Type apiType) {
    }
    ~OpenXRTutorial() = default;

    void Run() {
    }

#if defined(__ANDROID__)
public:
    // Stored pointer to the android_app structure from android_main().
    static android_app *androidApp;

    // Custom data structure that is used by PollSystemEvents().
    // Modified from https://github.com/KhronosGroup/OpenXR-SDK-Source/blob/d6b6d7a10bdcf8d4fe806b4f415fde3dd5726878/src/tests/hello_xr/main.cpp#L133C1-L189C2
    struct AndroidAppState {
        ANativeWindow *nativeWindow = nullptr;
        bool resumed = false;
    };
    static AndroidAppState androidAppState;

    // Processes the next command from the Android OS. It updates AndroidAppState.
    static void AndroidAppHandleCmd(struct android_app *app, int32_t cmd) {
        AndroidAppState *appState = (AndroidAppState *)app->userData;

        switch (cmd) {
        // There is no APP_CMD_CREATE. The ANativeActivity creates the application thread from onCreate().
        // The application thread then calls android_main().
        case APP_CMD_START: {
            break;
        }
        case APP_CMD_RESUME: {
            appState->resumed = true;
            break;
        }
        case APP_CMD_PAUSE: {
            appState->resumed = false;
            break;
        }
        case APP_CMD_STOP: {
            break;
        }
        case APP_CMD_DESTROY: {
            appState->nativeWindow = nullptr;
            break;
        }
        case APP_CMD_INIT_WINDOW: {
            appState->nativeWindow = app->window;
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            appState->nativeWindow = nullptr;
            break;
        }
        }
    }

private:
    void PollSystemEvents() {
        // Checks whether Android has requested that application should by destroyed.
        if (androidApp->destroyRequested != 0) {
            m_applicationRunning = false;
            return;
        }
        while (true) {
            // Poll and process the Android OS system events.
            struct android_poll_source *source = nullptr;
            int events = 0;
            // The timeout depends on whether the application is active.
            const int timeoutMilliseconds = (!androidAppState.resumed && !m_sessionRunning && androidApp->destroyRequested == 0) ? -1 : 0;
            if (ALooper_pollOnce(timeoutMilliseconds, nullptr, &events, (void**)&source) >= 0) {
                if (source != nullptr) {
                    source->process(androidApp, source);
                }
            } else {
                break;
            }
        }
    }
#else
    void PollSystemEvents() {
        return;
    }
#endif

private:
    bool m_applicationRunning = true;
    bool m_sessionRunning = false;
};

void OpenXRTutorial_Main(GraphicsAPI_Type apiType) {
    DebugOutput debugOutput;  // This redirects std::cerr and std::cout to the IDE's output or Android Studio's logcat.
    XR_TUT_LOG("OpenXR Tutorial Chapter 1");

    OpenXRTutorial app(apiType);
    app.Run();
}

#if defined(_WIN32) || (defined(__linux__) && !defined(__ANDROID__))
int main(int argc, char **argv) {
    OpenXRTutorial_Main(XR_TUTORIAL_GRAPHICS_API);
}
/*
int main(int argc, char **argv) {
    OpenXRTutorial_Main(OPENGL);
}
int main(int argc, char **argv) {
    OpenXRTutorial_Main(VULKAN);
}
int main(int argc, char **argv) {
    OpenXRTutorial_Main(D3D11);
}
int main(int argc, char **argv) {
    OpenXRTutorial_Main(D3D12);
}
*/
#elif (__ANDROID__)
android_app *OpenXRTutorial::androidApp = nullptr;
OpenXRTutorial::AndroidAppState OpenXRTutorial::androidAppState = {};

void android_main(struct android_app *app) {
    // Debug logging to verify we entered android_main
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "===== android_main() started =====");

    // Allow interaction with JNI and the JVM on this thread.
    // https://developer.android.com/training/articles/perf-jni#threads
    JNIEnv *env;
    app->activity->vm->AttachCurrentThread(&env, nullptr);
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "JNI thread attached");

    // https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html#XR_KHR_loader_init
    // Load xrInitializeLoaderKHR() function pointer. On Android, the loader must be initialized with variables from android_app *.
    // Without this, there's is no loader and thus our function calls to OpenXR would fail.
    XrInstance m_xrInstance = XR_NULL_HANDLE;  // Dummy XrInstance variable for OPENXR_CHECK macro.
    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR = nullptr;
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "Getting xrInitializeLoaderKHR proc address...");
    OPENXR_CHECK(xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction *)&xrInitializeLoaderKHR), "Failed to get InstanceProcAddr for xrInitializeLoaderKHR.");
    if (!xrInitializeLoaderKHR) {
        __android_log_print(ANDROID_LOG_ERROR, "OpenXR_Ch1", "xrInitializeLoaderKHR is NULL - EXITING");
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "xrInitializeLoaderKHR obtained successfully");

    // Fill out an XrLoaderInitInfoAndroidKHR structure and initialize the loader for Android.
    XrLoaderInitInfoAndroidKHR loaderInitializeInfoAndroid{XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};
    loaderInitializeInfoAndroid.applicationVM = app->activity->vm;
    loaderInitializeInfoAndroid.applicationContext = app->activity->clazz;
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "Initializing OpenXR loader for Android...");
    OPENXR_CHECK(xrInitializeLoaderKHR((XrLoaderInitInfoBaseHeaderKHR *)&loaderInitializeInfoAndroid), "Failed to initialize Loader for Android.");
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "OpenXR loader initialized successfully");

    // Set userData and Callback for PollSystemEvents().
    app->userData = &OpenXRTutorial::androidAppState;
    app->onAppCmd = OpenXRTutorial::AndroidAppHandleCmd;

    OpenXRTutorial::androidApp = app;
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "Calling OpenXRTutorial_Main() with graphics API: VULKAN");
    OpenXRTutorial_Main(XR_TUTORIAL_GRAPHICS_API);
    __android_log_print(ANDROID_LOG_INFO, "OpenXR_Ch1", "===== android_main() finished =====");
}
/*
    OpenXRTutorial_Main(OPENGL_ES);
}
    OpenXRTutorial_Main(VULKAN);
}
*/
#endif
