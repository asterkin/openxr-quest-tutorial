package com.example.openxrcamera2tutorial;

import android.Manifest;
import android.app.NativeActivity;
import android.content.pm.PackageManager;
import android.os.Bundle;

public class Camera2TutorialActivity extends NativeActivity {
    private static final int REQUEST_CAMERA_PERMISSION = 1;
    private static final String HEADSET_CAMERA_PERMISSION = "horizonos.permission.HEADSET_CAMERA";

    static {
        System.loadLibrary("OpenXRCamera2Tutorial");
    }

    private static native void nativeOnPermissionResult(boolean granted);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        checkCameraPermissions();
    }

    private void checkCameraPermissions() {
        boolean hasCameraPermission = checkSelfPermission(Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED;
        boolean hasHeadsetPermission = checkSelfPermission(HEADSET_CAMERA_PERMISSION) == PackageManager.PERMISSION_GRANTED;

        if (hasCameraPermission && hasHeadsetPermission) {
            nativeOnPermissionResult(true);
        } else {
            requestPermissions(new String[] {
                Manifest.permission.CAMERA,
                HEADSET_CAMERA_PERMISSION
            }, REQUEST_CAMERA_PERMISSION);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == REQUEST_CAMERA_PERMISSION) {
            boolean allGranted = true;
            for (int result : grantResults) {
                if (result != PackageManager.PERMISSION_GRANTED) {
                    allGranted = false;
                    break;
                }
            }
            nativeOnPermissionResult(allGranted);
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }
}
