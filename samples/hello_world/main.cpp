// Minimal OpenXR Hello World for Android/Quest
// Based on https://openxr-tutorial.com/

#include <android/log.h>
#include <android_native_app_glue.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <cstring>

#define LOG_TAG "OpenXR_HelloWorld"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

struct AppState {
    XrInstance instance = XR_NULL_HANDLE;
    XrSession session = XR_NULL_HANDLE;
    XrSystemId systemId = XR_NULL_SYSTEM_ID;
    bool running = false;
};

void InitializeOpenXR(android_app* app, AppState* state) {
    // Initialize OpenXR loader on Android
    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR;
    xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR",
                          (PFN_xrVoidFunction*)&xrInitializeLoaderKHR);

    if (xrInitializeLoaderKHR) {
        XrLoaderInitInfoAndroidKHR loaderInitInfo = {XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};
        loaderInitInfo.applicationVM = app->activity->vm;
        loaderInitInfo.applicationContext = app->activity->clazz;
        xrInitializeLoaderKHR((const XrLoaderInitInfoBaseHeaderKHR*)&loaderInitInfo);
    }

    // Create OpenXR instance
    XrInstanceCreateInfo instanceCreateInfo = {XR_TYPE_INSTANCE_CREATE_INFO};
    strcpy(instanceCreateInfo.applicationInfo.applicationName, "OpenXR Hello World");
    instanceCreateInfo.applicationInfo.applicationVersion = 1;
    strcpy(instanceCreateInfo.applicationInfo.engineName, "Custom");
    instanceCreateInfo.applicationInfo.engineVersion = 1;
    instanceCreateInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

    XrResult result = xrCreateInstance(&instanceCreateInfo, &state->instance);
    if (XR_SUCCEEDED(result)) {
        LOGI("✅ OpenXR Instance created successfully!");

        // Get system (HMD)
        XrSystemGetInfo systemGetInfo = {XR_TYPE_SYSTEM_GET_INFO};
        systemGetInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

        result = xrGetSystem(state->instance, &systemGetInfo, &state->systemId);
        if (XR_SUCCEEDED(result)) {
            LOGI("✅ OpenXR System ID: %llu", (unsigned long long)state->systemId);
        } else {
            LOGE("❌ Failed to get OpenXR system: %d", result);
        }
    } else {
        LOGE("❌ Failed to create OpenXR instance: %d", result);
    }
}

void CleanupOpenXR(AppState* state) {
    if (state->session != XR_NULL_HANDLE) {
        xrDestroySession(state->session);
        state->session = XR_NULL_HANDLE;
    }
    if (state->instance != XR_NULL_HANDLE) {
        xrDestroyInstance(state->instance);
        state->instance = XR_NULL_HANDLE;
    }
    LOGI("OpenXR cleaned up");
}

// Event handler callback
static void HandleAppCmd(android_app* app, int32_t cmd) {
    AppState* state = (AppState*)app->userData;

    switch (cmd) {
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            break;
        case APP_CMD_RESUME:
            LOGI("APP_CMD_RESUME");
            break;
        case APP_CMD_PAUSE:
            LOGI("APP_CMD_PAUSE");
            break;
        case APP_CMD_STOP:
            LOGI("APP_CMD_STOP");
            break;
        case APP_CMD_DESTROY:
            LOGI("APP_CMD_DESTROY");
            state->running = false;
            break;
    }
}

void android_main(struct android_app* app) {
    LOGI("=== OpenXR Hello World Starting ===");

    AppState state = {};
    app->userData = &state;
    app->onAppCmd = HandleAppCmd;

    InitializeOpenXR(app, &state);

    state.running = true;
    int frameCount = 0;

    LOGI("Entering main loop (will auto-exit after 300 frames ~5 seconds)");

    while (state.running && frameCount < 300) {
        // Process Android events
        int events;
        android_poll_source* source;

        while (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) {
                source->process(app, source);
            }

            if (app->destroyRequested) {
                LOGI("App destroy requested");
                state.running = false;
                break;
            }
        }

        frameCount++;

        // Log progress every 60 frames (~1 second)
        if (frameCount % 60 == 0) {
            LOGI("Frame %d - Still running...", frameCount);
        }
    }

    if (frameCount >= 300) {
        LOGI("Auto-exiting after %d frames", frameCount);
    }

    CleanupOpenXR(&state);
    LOGI("=== OpenXR Hello World Exiting ===");
}
