import pathlib
p=pathlib.Path(r'openxr/camera2_tutorial/app/src/main/cpp/GraphicsAPI_Vulkan.cpp')
lines=p.read_text(encoding='utf-8',errors='replace').splitlines()
s=int('415')
e=int('430')
e=min(e, len(lines))
[print(f'{i:4d}: {lines[i-1]!r}') for i in range(s, e+1)]
