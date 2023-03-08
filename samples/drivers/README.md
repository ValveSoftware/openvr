# OpenVR Driver Samples

## CMake

The `CMakeLists.txt` found in this folder shows how to link OpenVR libraries.

These are added to the targets in each sample.

## Samples

`drivers/` - driver samples. See `drivers/` for information on each of the drivers.

* `barebones`
* `handskeletonsimulation`
* `simplecontroller`

`utils/` - utility samples, that can be copied to your own project for ease of development.

* `driverlog`
* `vrmath`

## Building

There are two options that can be used to build the samples: [CMake](#building-with-cmake)
or [Visual Studio](#building-with-visual-studio).

This directory contains both a CMakeLists.txt file which can be used to create a Visual Studio solution, or you can use
the Visual Studio solution provided in this directory.

### Building with CMake

To create a solution for the samples, run (in this directory):

* `cmake -A x64 -B build`
* Navigate into `build/` and open the solution.
* Build the project.

### Building with Visual Studio

Open the `vs-openvr_samples.sln` file in Visual Studio (2019 or 2022).

Build the project by pressing `Ctrl + Shift + B`.

## Build Artifacts

Building with either CMake or Visual Studio will output the build artifacts to the `output/` directory.

See `drivers/` for more documentation on linking the drivers to SteamVR.

Building the project will output the driver binary to `output/drivers/<driver_name>/bin/win64/driver_<driver_name>.dll`