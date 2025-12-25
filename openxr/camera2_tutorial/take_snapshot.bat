@echo off
adb shell screencap -p /sdcard/camera2.png
adb pull /sdcard/camera2.png .\
