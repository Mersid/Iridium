# Iridium

---

Iridium is a simple, lightweight, and portable ray-tracer written in C++.

## Features

* Support for various primitives
  * Spheres
  * Triangles
  * Planes (parallelograms)
* Complex objects from .off files
* Support for custom materials
* Acceleration structures (bounding volume hierarchy)
* Point lights with various colours and intensities
* Support for various camera types
  * Perspective
  * Orthographic
* Arbitrary camera transformations
* Support for multiple image formats
  * PNG
  * PPM (no compression, but handles extremely large renders better)
* Support for various rendering techniques
  * Blinn-Phong shading
  * Shadows
  * Reflections
  * Depth of field
* Scene mechanism to configure object positions, materials, and lights
* Multithreaded rendering


## Compiling

Create build directory
```bash
mkdir build
cd build
```

### On Linux
On Linux, you will need a C++ compiler, CMake, and Make.
For Debian-based distributions, you can install them with:
```bash
sudo apt install build-essential cmake
```

Generate CMake files
```bash
# For debug builds, use:
cmake ..

# For release builds, use:
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Build the project
```bash
# Replace 32 with the number of threads you want to use to compile the project
make -j 32
```

### On Windows
On Windows, you will need Visual Studio with the "Desktop development with C++" workload, as well as CMake.
You can find the latest version of CMake [here](https://cmake.org/download/).
You can find the latest version of Visual Studio [here](https://visualstudio.microsoft.com/downloads/).



#### With Ninja
With Ninja, you will need to ensure that Ninja is in your PATH and that it is the default toolchain.

Generate CMake files
```bash
# For debug builds, use:
cmake -GNinja ..

# For release builds, use:
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
```

If the detected toolchain is GNU (perhaps from having MinGW installed), you can force Ninja to use the MSVC toolchain by running the `vcvarsall.bat` script from the Visual Studio installation directory before running CMake.
For Visual Studio Enterprise 2022, this file is located at `C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat`.


Build the project.
```bash
# Replace 32 with the number of threads you want to use to compile the project
cmake --build . -j 32
```

#### With Visual Studio
With Visual Studio, the build configuration is specified during the build phase. To generate the solution files, run:
```bash
cmake ..
```

Build the project
```bash
# For debug builds, use:
cmake --build . --config Debug

# For release builds, use:
cmake --build . --config Release
```

## Usage
Iridium is a command-line program. The parameters can be configured using a YAML-based scene file.
By default, Iridium will look for `data/scene.yaml`. To override this, simply pass the path to the scene file as the first argument.
```bash
# If on Windows, don't forget to append .exe!
Iridium
```

### Example
```bash
Iridium data/scene.yml
```

## Gallery
![combined](https://github.com/Mersid/Iridium/assets/30786211/a47a0591-35bf-4eb0-8250-5d3aa11081fc)
The default scene that is generated when run without any parameters. This corresponds to `data/scene.yml`.
