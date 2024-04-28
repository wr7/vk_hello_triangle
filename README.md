
My current implementation of https://vulkan-tutorial.com/Introduction. It has a few noticeable differences
 - Written in C and not C++
 - Does not use an external linear algebra library
 - Uses the build system (Meson) to automatically compile shaders, convert them into C headers, and embed them into the executable/binary
   - The tutorial instead uses shell scripts to compile the shaders and reads the shaders from external files at runtime
 - Uses a shared vertex/index buffer

## Compiling on Linux using Nix
```bash
nix-shell
source envsetup.sh
```
The program can be compiled by running `b`, can be run with `r`, and can be run in renderdoc with `re`.

## Compiling on Windows
Download and install Visual Studio Community from https://visualstudio.microsoft.com/downloads/

Download glfw from https://www.glfw.org/download
 - copy `include` into `<project_dir>/windeps/glfw`
 - copy `lib-vc2019/glfw3.lib` into `<project_dir>/windeps/glfw`

Download shaderc from https://github.com/google/shaderc/blob/main/downloads.md
 - copy `glslc.exe` into the `windeps` directory

Download and install Meson and Ninja from https://github.com/mesonbuild/meson/releases

Download and install Vulkan SDK Core from https://vulkan.lunarg.com/sdk/home

Open x64 Native Tools Command Prompt for VS and run:
```
cd <project_dir>
meson setup build --native-file native-windows.ini --backend vs
```

Open the Visual Studio sln file in `<project_dir>/build`.

Right click `Solution 'vulkanhello' (5 of 5 projects)` on the right. Select `Properties`,
and change `bce` to `vulkanhello`. Press `Apply` and `Ok`.

For `bce` and `vulkanhello` on the right, do the following:
  1. Right click and select `Properties`
  2. Click `C/C++`
  3. Click `Code Generation`
  4. Double click "Basic Runtime Checks" until it says "Default"
  5. Click `Apply` and `Ok`.

It can now be compiled and run with the play button near the top of the screen.
