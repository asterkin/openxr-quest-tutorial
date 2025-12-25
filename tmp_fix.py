import pathlib, re
path = pathlib.Path(r'openxr/camera2_tutorial/app/src/main/cpp/GraphicsAPI_Vulkan.cpp')
text = path.read_text(encoding='utf-8', errors='replace')
