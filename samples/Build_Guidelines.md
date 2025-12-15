# OpenXR Samples Build Guidelines

This document provides common instructions for building, running, and troubleshooting the OpenXR samples in this repository.

## Standard Workflow

1.  **Clean the Project**
    Before starting a new build, it's recommended to clean the project to avoid potential issues. From within a specific sample's directory, run:
    ```powershell
    .\gradlew.bat clean
    ```

2.  **Build and Install**
    To build the sample and install it on your connected Quest 3, run:
    ```powershell
    .\gradlew.bat installDebug
    ```
    *   **Note**: Ensure your Quest 3 is connected via USB. The first time you run the sample, you may need to find it in the **"Unknown Sources"** list in your app library. Subsequently, it should appear in your main app menu.

3.  **Launch the Application (Optional)**
    For samples like `hello_world` that don't require immediate visual interaction, you can launch the application directly from your development machine without wearing the Quest headset. This is particularly useful for verifying logs.
    To launch, use the Android Debug Bridge (ADB):
    ```powershell
    adb shell am start -n com.example.openxr.helloworld/android.app.NativeActivity
    ```
    *   **Note**: The package name (`com.example.openxr.helloworld`) and activity (`android.app.NativeActivity`) might vary between samples. Refer to the specific sample's `AndroidManifest.xml` for accurate details.

4.  **Uninstall the Sample**
    To remove the sample from your device, run:
    ```powershell
    .\gradlew.bat uninstallDebug
    ```

5.  **View Device Logs**
    To view the application logs from your device in real-time, use the Android Debug Bridge (ADB):
    ```powershell
    adb logcat
    ```
    *   **Tip**: You can filter the output to show only messages from your specific sample. For example: `adb logcat -s "hello_world"`. Check the sample's own `README.md` for specific log tags.

## Troubleshooting

*   **Build Fails or Freezes**: If a build fails or seems stuck, you can force-stop the Gradle daemon and try cleaning the project again.
    ```powershell
    .\gradlew.bat --stop
    .\gradlew.bat clean
    ```

*   **Major Build Issues**: For more persistent issues, especially after changing file or folder structures, manually delete the `.cxx` and `build` directories from the sample's folder before attempting to build again.
 