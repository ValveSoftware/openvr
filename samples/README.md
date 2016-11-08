# OpenVR Samples: CMake build guide.

## Support

|      Configuration      |    32     |    64     |
| ----------------------- |:---------:|----------:|
| GNU/Linux   (gcc 4.8+)  |   OK      |    OK     |
| Windows     (msvc 12)   |   OK      |    OK     |
| OSX         (clang)     |   ~       |    ~      |


## Dependencies

* Qt 5.x
* GLEW 1.11+
* SDL 2
* OpenVR

The only general external dependency is Qt5.

On GNU/Linux, you will need GLEW 1.11 (available on most package manager).

On Windows x64, you will need the SDL2 runtime library.

## Build

**Important:**
*you might need to specify Qt5 cmake module path manually when generating the cache for the first time using the __CMAKE_PREFIX_PATH__ macro (see examples below).*

We will use the command-line on Unix and [Git Bash](https://git-for-windows.github.io/) on Windows.

First, move from the repository root to the samples directory to create a build directory:
```
cd samples
mkdir build; cd build
```

Then, depending on your system:

### Unix

Generate the CMake cache using Makefile:
```
cmake .. -G Makefile -DCMAKE_PREFIX_PATH=/opt/Qt/5.6/gcc_64/lib/cmake -DCMAKE_BUILD_TYPE=Release
```

To build type:
```
make -j4
```


### Windows

Here we will use MSVC 12 for x64 platform:
```
cmake .. -G "Visual Studio 12 2013 Win64" -DCMAKE_PREFIX_PATH=C:/Qt/5.6/msvc2013_64/lib/cmake
```

Alternatively, to force the compilation on x86 you can use the PLATFORM property (be sure to use the right generator and QT binaries):
```
cmake .. -G "Visual Studio 12 2013" -DCMAKE_PREFIX_PATH=C:/Qt/5.6/msvc2013/lib/cmake -DPLATFORM=32
```

To build, simply type:
```
cmake --build . --target all --config Release
```

*Note : with CMake, the build configuration type (ie. Debug, Release) is set at build time on Windows and at cache generation time on Unix.*

---
