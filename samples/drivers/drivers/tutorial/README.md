# OpenVR Driver Tutorial

This file provides a tutorial for setting up an OpenVR Driver.

The driver we'll end up with will be a virtual controller that takes the HMD position as its pose, and has some basic
inputs. While it's possible to follow this guide without any hardware to interface with SteamVR, the end result won't be
particularly useful. Instead, this guide is intended to serve as a reference or foundation for building your own driver
for SteamVR, with common use-cases and how to implement them.

The files contained in this directory contain the resultant driver from following the tutorial. If you want to just use
the driver, you **must** run `git submodule update --init` in this directory.

## Introduction

This tutorial...

Assumes:

* An understanding of C++ and Object-Oriented Programming.
* A basic understanding of CMake.

Requires:

* Git -https://git-scm.com/download

Recommends:

* Visual Studio and MSVC - https://visualstudio.microsoft.com/
* Visual Studio Child Process Debugging Power Tool Extension - Used for debugging your driver.
    * VS2015, VS2017,
      VS2019 - https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool
    * VS2022 - https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool2022

Will:

* Provide information of how to implement certain interfaces, and which methods to call when.
* Produce a simple controller driver that can be used as the foundation for a device.
* Put angle brackets around values that are user defined. Example: `<my_driver_name>`

Won't:

* Provide documentation on the interfaces. See the Wiki for documentation. It will sometimes provide useful tips, but
  this should be complemented with the full description in the Wiki.

## Initial Setup

Create a folder where you will store your driver project. The location doesn't matter, nor does the name of _this_ root
folder.

### Adding the OpenVR SDK

We need to add the OpenVR SDK to our project, which contains necessary headers and binaries for our driver to compile
against to get it working with OpenVR runtimes.

Let's create a folder within our root project folder to store dependencies, called `lib/`.

* `mkdir lib`
* `cd lib`

Then let's add the OpenVR SDK as a submodule. Within `lib/`, run:

* `git submodule add https://github.com/ValveSoftware/openvr`

### Adding the required driver files

There are some required files we should add first to our project. We'll add this to a folder separate to the code we
will be writing.

Create a folder called `<my_driver_name>`. `<my_driver_name>` should be an all lowercase, non-whitespace string. Give it
a good name! You'll be using it in a few places.

* `mkdir <my_driver_name>`
* `cd <my_driver_name>`

#### driver.vrdrivermanifest

Create a file called `driver.vrdrivermanifest` within `<my_driver_name>`.

`driver.vrdrivermanifest` is a file that contains attributes about your driver. Fill it with the following contents:

```json
{
  "alwaysActivate": true,
  "name": "<my_driver_name>",
  "directory": "",
  "resourceOnly": false,
  "hmd_presence": []
}
```

See wiki for documentation about the file.

The value of `name` must match the same as the folder it's currently in: `<my_driver_name>`.

Your project folder structure should now look like this:

    <project_root>
    └── <my_driver_name>
        └── driver.vrdrivermanifest

## Source files

Navigate back to the root project folder.

Create a folder called `src`. This will contain our source code. In `src/`, let's create the files we're going to be
filling in:

* `hmd_driver_factory.cpp`
* `device_provider.h`
* `device_provider.cpp`
* `controller_device.h`
* `controller_device.cpp`

Your project folder structure should now look like this:

    <project_root>
    ├── <my_driver_name>
    │   └── driver.vrdrivermanifest
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp

## Creating the Driver Binary

There's two ways this tutorial outlines for creating the shared library for our driver. The first is using CMake, and
the second is using Visual Studio. The output of both will be the same.

### Create a Project using CMake

Navigate back to the root project folder.

Create a file called `CMakeLists.txt`.

This will be where we instruct CMake to generate the binary for our driver, and compile in dependencies.

Add the following to `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.7.1)

# This is your project name
set(TARGET_NAME <my_driver_name>)

# This is the name that the binary will be called (driver_<my_driver_name>)
set(DRIVER_NAME "driver_${TARGET_NAME}")

project(${TARGET_NAME})

set(OPENVR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/openvr)

# If not set, determine the platform architecture
if (NOT PLATFORM)
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(PLATFORM 64)
    else ()
        set(PLATFORM 32)
    endif ()
endif ()
message(STATUS "Compilation set for ${PLATFORM}bits architectures.")

# OpenVR compatibility checking
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DLINUX -DPOSIX)
    set(ARCH_TARGET linux64)

    if (${PLATFORM} MATCHES 32)
        message(WARNING "OpenVR x86 binaries not provided on GNU/Linux.")
    endif ()
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MACOSX_RPATH 0)
    add_definitions(-DOSX -DPOSIX)
    set(ARCH_TARGET osx32)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-D_WIN32)
    set(ARCH_TARGET win${PLATFORM})

    # Binaries path for thirdparties are not generics so we try to guess their suffixes.
    set(WINDOWS_PATH_SUFFIXES win${PLATFORM} Win${PLATFORM} x${PLATFORM})
endif ()

find_library(OPENVR_LIBRARIES
        NAMES
        openvr_api
        PATHS
        ${OPENVR_LIB_DIR}/bin
        ${OPENVR_LIB_DIR}/lib
        PATH_SUFFIXES
        osx${PLATFORM}
        linux${PLATFORM}
        win${PLATFORM}
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
        )

set(OPENVR_INCLUDE_DIR ${OPENVR_LIB_DIR}/headers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/bin/${ARCH_TARGET}>)

add_library(${DRIVER_NAME} SHARED
        src/hmd_driver_factory.cpp
        src/device_provider.h
        src/device_provider.cpp
        src/controller_device.h
        src/controller_device.cpp
        )

target_link_libraries(${DRIVER_NAME} PRIVATE ${OPENVR_LIBRARIES})
target_include_directories(${DRIVER_NAME} PRIVATE ${OPENVR_INCLUDE_DIR})

# Copy driver assets to output folder
add_custom_command(
        TARGET ${DRIVER_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
)
```

Fill in `<my_driver_name>` with the name of your driver.

The bulk of this `CMakeLists.txt` file is setting up the OpenVR SDK as a dependency. But there's a couple of extra
things:

* `set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/${DRIVER_NAME}/bin/${PLATFORM_NAME}${PROCESSOR_ARCH}>)`
    * Sets our output directory for the binary to (if on windows 64-bit) `<build_dir>/<my_driver_name>/bin/win64` or
      equivalent platform/arch on other OSs.
* `add_library(${DRIVER_NAME} SHARED src/hmd_driver_factory.cpp)`
    * Creates the shared library target that our driver is going to be built as.
* `target_link_libraries(${DRIVER_NAME} PRIVATE ${OPENVR_LIBRARIES})` links to the OpenVR SDK binaries.
* `target_include_directories(${DRIVER_NAME} PRIVATE ${OPENVR_INCLUDE_DIR})` adds the OpenVR SDK headers to our target's
  include path.
* `add_custom_command(..)`
    * Copies the contents of our required driver files to the output directory. The runtime requires the extra files to
      load the driver.

If you're using Visual Studio, create a `build` folder where our solution file will be generated. CLion does this for
you.

Run:

* `mkdir build`
* `cd build`
* `cmake ..`

This should generate a solution file you can open in Visual Studio to edit the driver source code.

Our project folder structure should now look like this:

    <project_root>
    ├── <my_driver_name>
    │   └── driver.vrdrivermanifest
    ├── build
    │   └── <my_driver_name>.sln
    ├── CMakeLists.txt
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp

### Creating a Project using Visual Studio

Make sure you've created the files listed in [Source Files](#Source-Files). We'll be using them.

#### Creating the Visual Studio Project

In Visual Studio, Click "Create New Project":

![image](https://user-images.githubusercontent.com/39023874/213766831-7ce35f93-2a1e-4279-950b-f87ec5316015.png)

Select `Empty Project`:

![image](https://user-images.githubusercontent.com/39023874/213772711-328d7e68-698f-4d5f-ae50-f3cd94887565.png)

Your "Project name" should be `<my_driver_name>`.

Point `Location` to where the folder in which `src/` and `<my_driver_name>/` are located.

Check `Place solution and project in the same directory`.

![image](https://user-images.githubusercontent.com/39023874/213773190-aee36653-783f-433d-b3a1-2cef7e6ae120.png)

Click "Create".

Visual Studio might place your `.sln` and `.vcxproj` files within `<my_driver_name>/`. If they are, move them up one
directory, so the files are within the same folder that contains `src/` and `<my_driver_name>/`.

Right click your project named `<my_driver_name>`. Select `Add > Existing Item...`

![image](https://user-images.githubusercontent.com/39023874/213773704-f933383b-1e37-43ed-b1a8-d8d2052609a2.png)

In file explorer, navigate into your `src/` folder and highlight all the files.

![image](https://user-images.githubusercontent.com/39023874/213773871-04bac90f-5e83-4808-88ec-49965b45894a.png)

Click `Add`.

Your files should now appear in the Solution Explorer under the Project.

Our project folder structure should now look like this:

    <project_root>
    ├── <my_driver_name>.sln
    ├── <my_driver_name>.vcxproj
    ├── <my_driver_name>
    │   └── driver.vrdrivermanifest
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp

#### Setting up the build

Right-click the Project again. Select `Properties`.

Make sure "Configuration" is set to `All Configurations`, and "Platform" is set to `All Platforms`.

---

Navigate to `Configuration Properties > General`.

Set "Output Directory" to: `$(SolutionDir)build\$(ProjectName)\bin\win$(PlatformArchitecture)`.

Set "Target Name" to: `driver_$(ProjectName)`.

Set "Configuration Type" to: `Dynamic Library (.dll)`.

![image](https://user-images.githubusercontent.com/39023874/213774640-37a8d169-ea96-4b4d-ae29-9bc3cac8274f.png)

This will make our shared library binary output to where the runtime expects it to
be (`<my_driver_name>/bin/win64/driver_<my_driver_name>.dll`).

Click Apply.

---

Navigate to `C/C++ > General`.

Set "Additional Include Directories" to: `$(SolutionDir)lib/openvr/headers`.

![image](https://user-images.githubusercontent.com/39023874/213774862-eefa9bfe-abad-4107-a15b-1dab86ac5e4e.png)

This will allow us to `#include` the required OpenVR headers for building. Make sure you've got the OpenVR SDK
downloaded and extracted to `lib/openvr/`.

Click Apply.

---

Navigate to `Linker > General`.

Set "Additional Library Directories" to `$(SolutionDir)lib\openvr\lib\win$(PlatformArchitecture)`.

![image](https://user-images.githubusercontent.com/39023874/213775413-91489cf2-df98-4e2f-879d-3da7d9d006b1.png)

This will set the linker to look for the OpenVR SDK libraries in this location. It helps with keeping the next section
tidy.

Click Apply.

---

Navigate to `Linker > Input`.

Set "Additional Dependencies" to: `openvr_api.lib`.

![image](https://user-images.githubusercontent.com/39023874/213775744-2fee83aa-4b5e-4142-b2f9-917fbe2e1ecc.png)

This will link the required OpenVR static libraries into our driver binary.

Click Apply.

---

Navigate to `Build Events > Post-Build Event`.

Set "Command Line" to: `XCOPY "$(ProjectDir)$(ProjectName)" "$(SolutionDir)build\$(ProjectName)" /S /Y /I`.

![image](https://user-images.githubusercontent.com/39023874/213777341-b3d94d3c-7712-4e17-9f54-6a98c5de30c6.png)

This will copy our other required driver files into the driver ready to be loaded by the runtime.

Click OK.

---

Press `Ctrl + Shift + B`. Your project should build successfully and the output should be placed
in `build/<my_driver_name>`.

## IServerTrackedDeviceProvider

We're first going to implement our driver's `IServerTrackedDeviceProvider` interface. This interface provides us with a
way to spin up our own processes and devices when the driver is loaded, and some useful methods which the runtime can
call when the state of the runtime changes.

The interfaces in OpenVR are abstract classes: they provide a set of pure virtual functions (functions with no
definition and overrideable) and so we **must** implement all the methods in the interface.

### Implementing IServerTrackedDeviceProvider

First, in `device_provider.h`, we'll define our `DeviceProvider` class:

```cpp
#pragma once

#include "openvr_driver.h"

class DeviceProvider : public vr::IServerTrackedDeviceProvider {
    public:
        // Inherited via IServerTrackedDeviceProvider
        vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext) override;
        void Cleanup() override;
        const char * const *GetInterfaceVersions() override;
        void RunFrame() override;
        bool ShouldBlockStandbyMode() override;
        void EnterStandby() override;
        void LeaveStandby() override;
};
```

We first include `openvr_driver.h` which contains the interface declarations for the interfaces we want to implement.

Note the use of `public` in `class DeviceProvider : public vr::IServerTrackedDeviceProvider` and in defining the
methods.

This overrides all the methods in `IServerTrackedDeviceProvider` for us to implement on our own.

Next, in `device_provider.cpp`, we'll implement the methods:

```cpp
#include "device_provider.h"

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

    vr::VRDriverLog()->Log("Hello world!");

    return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
    return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {

}

bool DeviceProvider::ShouldBlockStandbyMode() {
    return false;
}

void DeviceProvider::EnterStandby() {

}

void DeviceProvider::LeaveStandby() {

}
```

Here, we first include our `device_provider.h` header file with the declarations of the methods we'll be defining
in `device_provider.cpp`.

Our `Init` method is called when SteamVR starts up. It takes in a `IVRDriverContext` pointer, which we must use to
initialize our driver context. This can be done with a macro provided by the OpenVR API
called `VR_INIT_SERVER_DRIVER_CONTEXT`. We then return `VRInitError_None` to indicate that we've successfully
initialized.

We then log to the runtime log files. Live logs can be found by opening the SteamVR Web
Console (`SteamVR Hamburger Menu > Developer Options > Web Console`).

Our `Cleanup` method is called when SteamVR exits. It cleans up our driver context that we initialized in `Init`.

`GetInterfaceVersions` tells OpenVR which interface versions we implemented. OpenVR is backwards compatible, and
supports all previous versions of interfaces, so we need to tell it which versions we support. For this OpenVR provides
an array of interface versions of the current version of OpenVR you are targeting, called `k_InterfaceVersions`.

`ShouldBlockStandbyMode` is deprecated and doesn't do anything.

We'll be adding more to this later!

## HMDDriverFactory

HMDDriverFactory is a function that is exported from your driver's shared library and is used as an entry point by the
runtime to reach into your driver and find the interfaces you implement.

In `hmd_driver_factory.cpp`, add the following:

```c++
#include "device_provider.h"

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif

//Global, single interface of our device provider.
DeviceProvider device_provider;

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
	if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
		return &device_provider;
	}

	if (pReturnCode) {
		*pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
	}

	return nullptr;
}
```

First, we include our `device_provider.h` header, to get our implementation of `IServerTrackedDeviceProvider`.

Next, we define a macro that expands to the platform specific method for exporting a function from the shared library.

Then, we create a stack allocated instance of our `DeviceProvider` class, which we'll be adding to the runtime.

After, we create a function that returns `void *` that is called `HmdDriverFactory`. It takes in two parameters:

* `const char *pInterfaceName` - The name of the current interface being requested.
* `int *pReturnCode` - A pointer to an `EVRInitError` that might occur when we try to fetch our interface.

When a `pInterfaceName` is called that we support, we return a pointer to our implementation.

For now, we only want to check when `IServerTrackedDeviceProvider_Version` is requested, where we'll return our
implementation of `IServerTrackedDeviceProvider`.

`IServerTrackedDeviceProvider_Version` is the interface version of the current `IServerTrackedDeviceProvider` interface
in the version of the OpenVR SDK you are currently targeting.

If we don't support the interface, we set the `pReturnCode` to `VRInitError_Init_InterfaceNotFound` and
return `nullptr` to tell the runtime we don't support that interface.

We now have a minimal driver that can be loaded by the runtime!

## Building

We now have a minimal driver that can be built and later loaded into the runtime.

Build the driver in Visual Studio by pressing `ctrl + shift + b`. There should be no errors.

Your driver will be build to `build/<my_driver_name>`. The contents of your build folder should now look like this:

    <project_root>
    └── build
        └── <my_driver_name>
            ├── bin
            │   └── win64
            │       └── <my_driver_name>.dll
            └── driver.vrdrivermanifest

## Linking to SteamVR

Navigate to your SteamVR install directory (typically `C:\Program Files (x86)\Steam\steamapps\common\SteamVR`)

Navigate to `bin\win64`

Run `vrpathreg.exe adddriver "<path_to_project_dir>/build/<my_driver_name>"`

If you go to `<AppData>/Local/openvr/openvrpaths.vrpath` you should see your driver listed under `external_drivers`.

Run SteamVR! There won't be anything displayed in the SteamVR Monitor, but you navigate to the SteamVR Web Console
(`SteamVR Hamburger Menu > Developer Options > Web Console`) and you should see your driver's log of `Hello World`
from `IServerTrackedDeviceProvider::Init`.

If there's a lot of logs, try searching at the top for `<my_driver_name>` or `hello`. Logs are also written
to `C:\Program Files (x86)\Steam\logs\vrserver.txt`.

The structure of a log line is `<date> <driver_name>: <log_message>`. For
example, `Fri Jan 13 2023 17:46:20.237 - sample: Hello world!`.

For this example, the driver is called `sample`, but you should search for whatever your driver is called.

![image](https://user-images.githubusercontent.com/39023874/213327011-35538730-dede-4ccc-b3f8-88a79a3618e5.png)

### Debugging drivers

Being able to debug drivers requires some extra setup.

You **must** have Visual Studio and Visual Studio Child Process Debugging Power Tool Extension installed.
See [Introduction](#introduction) for installation links.

In Visual Studio with the extension installed, navigate
to `Debug > Other Debug Targets > Child Process Debugging Settings...`.

Click `Enable child process debugging`.

In the table, add a new entry. In the Process name column, add `vrserver.exe`.

In the `vrserver.exe` row, make sure the action is set to `Attach debugger`.

In the `<All other processeses>` row, make sure the action is set to `Do not debug`.

Click save.

![image](https://user-images.githubusercontent.com/39023874/213326226-09f3e750-1e63-486c-a1ac-9327a852c1c9.png)

Next, we want it so when we click the green launch button, SteamVR launches.

Make sure you set the project to build in `Debug` mode, for `x64`.

Next to the green play and `Local Windows Debugger` text, there is a small down arrow, click it, and
select `ALL_BUILD debug properties`.

* If `ALL_BUILD` is not the option, in the solution explorer right
  click `Solution '<my_driver_name>'` `Properties > Startup Project`. Select `Single startup project` and set
  to `ALL_BUILD`.
* Now try again.

Make sure `Configuration` and `Platform` are `Active`.

Select `Configuration Properites > Debugging`. In `Command`, replace
with: `C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrstartup.exe`.

Click `OK`.

![image](https://user-images.githubusercontent.com/39023874/213326391-51da1454-c87e-499c-a068-c35b2cbd3a7c.png)

Now, when you press `Local Windows Debugger`, SteamVR should launch and your driver should be loaded.

Before launching SteamVR, if you place a breakpoint on the `vr::VRDriverLog()` call in `device_provider.cpp`, Visual
Studio should let you single step through your driver.

## Creating a Device Driver

A single device type (such as an Index controller, Vive wand) is represented through the `ITrackedDeviceServerDriver`
interface.

We'll be making a simple controller in this tutorial, which we'll implement through this interface.

Unlike how our `IServerTrackDeviceProvider` interface is called from the runtime, we choose when we add
our `ITrackDeviceServerDriver` implementations to the runtime. We'll do this
in `IServerTrackDeviceProvider::Activate`.

But first, let's implement our `ITrackDeviceServerDriver`.

In `controller_device.h`, add:

```cpp
#pragma once

#include "openvr_driver.h"

class ControllerDevice : public vr::ITrackedDeviceServerDriver {
public:
    ControllerDevice(vr::ETrackedControllerRole role);

    // Inherited via ITrackedDeviceServerDriver
    virtual vr::EVRInitError Activate(uint32_t unObjectId) override;
    virtual void Deactivate() override;
    virtual void EnterStandby() override;
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override;
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
    virtual vr::DriverPose_t GetPose() override;

private:
    vr::ETrackedControllerRole role_;
    vr::TrackedDeviceIndex_t device_id_;
};
```

We implement a constructor for this class, that takes in `vr::ETrackedControllerRole`.

Our `ControllerDevice` will be the same class we initialize for both the left and right controller, so we need some way
of knowing which controller is left and which is right.

We'll be the ones creating an instance of `ControllerDevice`, so we'll be the ones passing in that parameter later.

Note the use of `public` in `class ControllerDevice : public vr::ITrackedDeviceServerDriver` and in defining the
methods.

Next, we'll define these methods in `controller_device.cpp`:

```cpp
#include "controller_device.h"

ControllerDevice::ControllerDevice(vr::ETrackedControllerRole role) : role_(role), device_id_(vr::k_unTrackedDeviceIndexInvalid) {};

vr::EVRInitError ControllerDevice::Activate(uint32_t unObjectId) {
    vr::VRDriverLog()->Log("ControllerDevice::Activate");
    
    device_id_ = unObjectId;
    
    return vr::VRInitError_None;
}

void ControllerDevice::Deactivate() {
}

void ControllerDevice::EnterStandby() {
}

void* ControllerDevice::GetComponent(const char* pchComponentNameAndVersion) {
    return nullptr;
}

void ControllerDevice::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
    if(unResponseBufferSize >= 1)
        pchResponseBuffer[0] = 0;
}

vr::DriverPose_t ControllerDevice::GetPose() {
    return vr::DriverPose_t();
}
```

Our `Activate` method is called after the device is added to the runtime (more on this later).

We can log a message to the console to let us know that our controller was activated. We then also store our unique
device id in `device_id_`, we'll be needing it later.

`Deactivate` is called when the runtime begins exiting.

We won't have any "components" to return in `GetComponents` so we should just return `nullptr`.

`DebugRequest` can be called by the application to request information from the driver. We won't implement anything
here as we don't have anything currently to debug.

`GetPose` isn't called by the runtime, but is a helpful method we can implement to calculate the pose of the controller.
We'll
fill this in the next section.

### Setting the controller role

We have to inform the runtime of the role of our controller (whether it's a left or right hand). We'll do this
in `Activate`.

Device properties are stored within containers, that are specific to each device in the system. We first need to
retrieve the correct container for the device we want to set the properties for, then we can set the properties.

```c++
vr::EVRInitError ControllerDevice::Activate(uint32_t unObjectId) {
    vr::VRDriverLog()->Log("ControllerDevice::Activate");
    
    const vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer(unObjectId);
    vr::VRProperties()->SetInt32Property(container, vr::Prop_ControllerRoleHint_Int32, role_);
    
    device_id_ = unObjectId;
    return vr::VRInitError_None;
}
```

We set the property `Prop_ControllerRoleHint_Int32` to our `role_` which will be
either `TrackedControllerRole_RightHand` or `TrackedControllerRole_LeftHand` from being set in the `DeviceProvider`.

Properties that are available to be set are stored within `ETrackedDeviceProperty`. You can find more documentation on
this in the wiki.

### Adding a model number

We can also set a specific model number for our controller. This is useful if we want to have different models of the
same role in the same driver as it provides a way to differentiate them when setting properties such as icons.

Set the property `Prop_ModelNumber_String` to set the model number of your device.

```c++
vr::EVRInitError ControllerDevice::Activate(uint32_t unObjectId) {
...
    const vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer(unObjectId);
...
    vr::VRProperties()->SetStringProperty( container, vr::Prop_ModelNumber_String, "<my_controller_model_number>" );
...
    return vr::VRInitError_None;
}
```

### Creating Poses for the Device

A pose represents the current state of a device in space at a given moment. It contains the position and
orientation of the device, as well as other information such as velocity and angular velocity, and the current tracking
status.

Because we don't have any actual hardware we're using for this tutorial, we'll base our pose off the HMD for reference.

We'll be setting our pose in `ControllerDevice::GetPose`.

`DriverPose_t` is the struct OpenVR expects the pose to be returned in. There's a couple of fields that need to be set
up correctly for the pose to be valid:

```c++
vr::DriverPose_t pose = { 0 };

pose.poseIsValid = true;
pose.result = vr::TrackingResult_Running_OK;
pose.deviceIsConnected = true;

pose.qWorldFromDriverRotation.w = 1.f;
pose.qDriverFromHeadRotation.w = 1.f;

pose.qRotation.w = 1.f;
```

Documentation for this struct is provided in the Wiki.

The quaternion members **must** have a valid `w` component, or our controller won't show up. For now, we'll set it to
the identity quaternion.

Likewise, we should also tell the runtime that our pose is valid, the device is connected, and we're tracking as
expected, otherwise the runtime will set our controller to not active.

While this is a valid pose, it's located at the origin (probably in the floor) if we tried to use it (
although, we haven't yet submitted the pose to the runtime, so our controller won't currently show up).

To make our controllers show up in a more sensible place, we can use the position of the HMD to offset the controller to
slightly in-front of the user.

To get the pose of the HMD we can use:

```c++
vr::TrackedDevicePose_t hmd_pose{};

vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0.f, &hmd_pose, 1);
```

`GetRawTrackedDevicePoses` takes in an array of `TrackedDevicePose_t` structs, and fills each index with associated pose
of the device at that index in the system.

The HMD pose **will** always be at index 0, so we can just pass in a pointer to a single `TrackedDevicePose_t` struct
instead of an array.

`vr::TrackedDevicePose_t` contains the position and orientation of the device in a `vr::HmdMatrix34_t` struct: a 3x4
matrix.

The last column of the matrix is the position of the HMD. We'll be using this (and optionally the rotation later):

```c++
pose.vecPosition[0] = role_ == vr::TrackedControllerRole_LeftHand
		? hmd_pose.mDeviceToAbsoluteTracking.m[0][3] - 0.2f
		: hmd_pose.mDeviceToAbsoluteTracking.m[0][3] + 0.2f; // set the x position offset based on whether we are a left or right hand (so the controllers don't appear on top of eachother). This will
                                                          // make it look like two controllers appearing side by side, 0.4 metres apart.

pose.vecPosition[1] = hmd_pose.mDeviceToAbsoluteTracking.m[1][3];		// no vertical offset
pose.vecPosition[2] = hmd_pose.mDeviceToAbsoluteTracking.m[2][3] - 0.5f; // make the controller appear half a metre in front of the hmd (-z is forward in space)
```

The pose we submit is in a right-handed coordinate system. Make your thumb, index and middle fingers point at right
angles to each-other. Point your thumb to the right, your index finger upwards, and your middle finger towards you. Your
thumb is the positive x-axis, index is positive y, and middle is positive z (+z is towards you).

The code above sets the position of the controller to be 0.2 metres to the left or right of the HMD (0.2 metres to the
left for the left controller, 0.2 metres to the right for the right controller), and 0.5 metres in front of the HMD.

Finally, we need to just return our pose from `GetPose`. Together, our `ControllerDevice::GetPose` method should now
look like this:

```c++
vr::DriverPose_t ControllerDevice::GetPose() {
    vr::DriverPose_t pose = { 0 };

    pose.poseIsValid = true;
    pose.result = vr::TrackingResult_Running_OK;
    pose.deviceIsConnected = true;
    
    pose.qWorldFromDriverRotation.w = 1.f;
    pose.qDriverFromHeadRotation.w = 1.f;
    
    pose.qRotation.w = 1.f;
    
    vr::TrackedDevicePose_t hmd_pose{};
    vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0.f, &hmd_pose, 1);

    pose.vecPosition[0] = my_controller_role_ == vr::TrackedControllerRole_LeftHand
                                ? hmd_position.v[0] - 0.2f
                                : hmd_position.v[0] + 0.2f;
    
    pose.vecPosition[1] = hmd_position.v[1];
    pose.vecPosition[2] = hmd_position.v[2] - 0.5f;

    return pose;
}

```

#### Adding orientation to the pose

We can also add the orientation of the HMD to our device. Doing this requires a little more work, and is optional.

As mentioned, `GetRawTrackedDevicePoses` gives pose orientation and position in a `vr::HmdMatrix34_t` struct.

The first 3 columns of the matrix represent a 3x3 rotation matrix for the current HMD orientation. But, we need to
convert this to a quaternion to submit it to OpenVR.

`samples/util/vrmath.h` provides utility functions for this:

```c++
// 3x3 or 3x4 matrix
template < class T >
vr::HmdQuaternion_t HmdQuaternion_FromMatrix( const T &matrix )
{
	vr::HmdQuaternion_t q{};

	q.w = sqrt( fmax( 0, 1 + matrix.m[ 0 ][ 0 ] + matrix.m[ 1 ][ 1 ] + matrix.m[ 2 ][ 2 ] ) ) / 2;
	q.x = sqrt( fmax( 0, 1 + matrix.m[ 0 ][ 0 ] - matrix.m[ 1 ][ 1 ] - matrix.m[ 2 ][ 2 ] ) ) / 2;
	q.y = sqrt( fmax( 0, 1 - matrix.m[ 0 ][ 0 ] + matrix.m[ 1 ][ 1 ] - matrix.m[ 2 ][ 2 ] ) ) / 2;
	q.z = sqrt( fmax( 0, 1 - matrix.m[ 0 ][ 0 ] - matrix.m[ 1 ][ 1 ] + matrix.m[ 2 ][ 2 ] ) ) / 2;

	q.x = copysign( q.x, matrix.m[ 2 ][ 1 ] - matrix.m[ 1 ][ 2 ] );
	q.y = copysign( q.y, matrix.m[ 0 ][ 2 ] - matrix.m[ 2 ][ 0 ] );
	q.z = copysign( q.z, matrix.m[ 1 ][ 0 ] - matrix.m[ 0 ][ 1 ] );

	return q;
}
```

So this can become:

```c++
const vr::HmdQuaternion_t hmd_orientation = HmdQuaternion_FromMatrix(hmd_pose.mDeviceToAbsoluteTracking);
```

Now, we can add this to our pose:

```c++
pose.qRotation = hmd_orientation;
```

Altogether, our `GetPose` should now look like this:

```c++
vr::DriverPose_t ControllerDevice::GetPose() {
	vr::DriverPose_t pose = { 0 };

	pose.poseIsValid = true;
	pose.result = vr::TrackingResult_Running_OK;
	pose.deviceIsConnected = true;

	pose.qWorldFromDriverRotation.w = 1.f;
	pose.qDriverFromHeadRotation.w = 1.f;

	pose.qRotation.w = 1.f;

	vr::TrackedDevicePose_t hmd_pose{};
	vr::VRServerDriverHost()->GetRawTrackedDevicePoses(0.f, &hmd_pose, 1);

	const vr::HmdQuaternion_t hmd_orientation = HmdQuaternion_FromMatrix(hmd_pose.mDeviceToAbsoluteTracking);
	pose.qRotation = hmd_orientation;

	pose.vecPosition[0] = role_ == vr::TrackedControllerRole_LeftHand
		? hmd_pose.mDeviceToAbsoluteTracking.m[0][3] - 0.2f
		: hmd_pose.mDeviceToAbsoluteTracking.m[0][3] + 0.2f;

	pose.vecPosition[1] = hmd_pose.mDeviceToAbsoluteTracking.m[1][3];
	pose.vecPosition[2] = hmd_pose.mDeviceToAbsoluteTracking.m[2][3] - 0.5f;

	return pose;
}
```

### Submitting the pose

We currently don't have a good place to update the pose.

For the tutorial, we'll update the pose in `RunFrame`. In
reality, you shouldn't really do this in `RunFrame`, firstly as it is dependent on the framerate, and secondly because
it
runs in the main loop of the runtime, so blocking here causes the runtime to also block.

A recommended approach is to create another thread that updates the pose at a set interval, or when you receive data
from the device. You can check out `samples/drivers/simplecontroller` for an implementation of this.

`ITrackedDeviceServerDriver` doesn't have a `RunFrame`, so we'll make one in our derived class and call the method from
our `IServerTrackedDeviceProvider` implementation (this will be done in the next section).

```c++
class ControllerDevice : public vr::ITrackedDeviceServerDriver {
  public:
    ...
    void RunFrame();
    ...
};
```

Now that we have a pose, we need to submit it to OpenVR. We can do this by
calling `vr::VRServerDriverHost()->TrackedDevicePoseUpdated`. We'll make this call in `ControllerDevice::RunFrame`:

```c++
void ControllerDevice::RunFrame() {
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(device_id_, GetPose(), sizeof(vr::DriverPose_t));
}
```

## Adding the device to the runtime

Now that we have a simple device all setup, we can add this to the runtime.

First, we should store pointers to our `ControllerDevice` implementations within `DeviceProvider`:

Add `my_left_device_` and `my_right_device_` as a `std::unique_ptr<ControllerDevice>` to the `DeviceProvider` class:

```c++
#pragma once
#include <memory>

#include "controller_device.h"
#include "openvr_driver.h"

class DeviceProvider : public vr::IServerTrackedDeviceProvider {
    ...
private:
    std::unique_ptr<ControllerDevice> my_left_device_;
    std::unique_ptr<ControllerDevice> my_right_device_;
};
```

Unlike our `IServerTrackedDeviceProvider` implementation, which gets loaded by the runtime on startup, we can choose
when to add our devices to the runtime.

When we're ready to add devices to the runtime, we need to call `IVRServerDriverHost::TrackedDeviceAdded` in
our `DeviceProvider`. This will tell the runtime that our driver has a new device to add, and queue it to be activated.

We'll activate the device in `DeviceProvider::Init`, so the devices are added to the runtime on startup, but they can be
added at anytime during the session (perhaps when you detect the controller connected).

```c++
void DeviceProvider::Init() {
    my_left_device_ = std::make_unique<ControllerDevice>(vr::TrackedControllerRole_LeftHand);
    vr::VRServerDriverHost()->TrackedDeviceAdded("<my_left_serial_number>",
                                                vr::TrackedDeviceClass_Controller,
                                                my_left_device_.get());
    
    my_right_device_ = std::make_unique<ControllerDevice>(vr::TrackedControllerRole_RightHand);
    vr::VRServerDriverHost()->TrackedDeviceAdded("<my_right_serial_number>",
                                                vr::TrackedDeviceClass_Controller,
                                                my_right_device_.get());
}
```

Each device added to the runtime needs a **unique** serial number. This **must** be a unique string for each device.

We then need to tell the runtime what class of device this is. In this case, we're adding a controller, so we pass
in `vr::TrackedDeviceClass_Controller`.

Finally, we pass in a pointer to the device we've just instantiated.

Finally, we should also add a call to `ControllerDevice::RunFrame` in `DeviceProvider::RunFrame`, so our poses can be
updated.

```c++
void DeviceProvider::RunFrame() {
    if(my_left_device_ != nullptr) {
        my_left_device_->RunFrame();
    }
    
    if(my_right_device_ != nullptr) {
        my_right_device_->RunFrame();
    }
}
```

Starting SteamVR, you should see gamepad-like models floating somewhere in front of the HMD!

Make sure that you're in the empty SteamVR universe when testing; we haven't set up our device to be used in
applications yet.

![image](https://user-images.githubusercontent.com/39023874/213331869-5450e4a8-6125-4858-baee-06d684ce41a2.png)

You should also see two new icons in SteamVR. These are your controller devices. We'll get onto configuring icons later.
They should be a solid gradient color. If they aren't, then you have something wrong with your pose.

If these aren't appearing, then you have a problem adding the device to SteamVR. Make sure that the driver is enabled (
SteamVR Settings > Startup/Shutdown > Manage addons). You'll need to enable advanced options to get this option.

## Device Inputs

Now that we have a device set up, we can add some inputs to it.

We tell the runtime what physical inputs our device has, which can then be bound in the runtime to in-game actions (like
shooting a gun, or jumping).

To do this, we add an input profile to the driver. This is a JSON file that describes the physical inputs our device
has.

We'll add this to `<my_driver_name>/resources/input/<my_device_name>_profile.json`:

```json
{
  "jsonid": "input_profile",
  "controller_type": "<my_device_name>",
  "input_bindingui_mode": "controller_handed",
  "input_source": {
    "/input/a": {
      "type": "button",
      "touch": true,
      "binding_image_point": [
        80,
        60
      ],
      "order": 1
    },
    "/input/trigger": {
      "type": "trigger",
      "binding_image_point": [
        250,
        60
      ],
      "order": 2
    },
    "/input/joystick": {
      "type": "joystick",
      "click": true,
      "binding_image_point": [
        60,
        60
      ],
      "order": 3
    },
    "/output/haptic": {
      "type": "vibration",
      "binding_image_point": [
        300,
        150
      ],
      "order": 4
    }
  }
}
```

`controller_type` is the name of your current device, such as `vive_controller` or `knuckles` (index controllers). This
should be a short, non-whitespace string that is used more for development purposes than displaying to users. You can
set a human-readable name in the localization files.

`input_bindingui_mode` is the mode that the input binding UI will use. This will be `controller_handed` as our device is
a controller that can be held in hand.

`input_source` is a list of inputs that our device has. Each input has its path as they key, and an object as the value
giving information about that input.

Note the `click` or `touch` properties on `my_joystick`, `my_a` and `my_trigger`. These tell the runtime that the input
source has the capability of sensing click or touch events.

input source paths **should** follow: `<input/output>/<my_input_source_name>`. This should be a physical "input source",
on your device, such as a single button, joystick, trigger, touchpad, etc.

See documentation for input device types.

Devices also have one available haptic vibration component, which we specify with `/output/haptic`.

Your project file should now look like this:

    <project_root>
    ├── <my_driver_name>
    │   ├── driver.vrdrivermanifest
    │   └── resources
    │       └── input
    │           └── <my_device_name>_profile.json
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp

### Creating Handles to Inputs

Now that we have created some inputs in our input profile, we can then create handles to them to be able to update them
when our device updates.

To do this, we need to create a `vr::VRInputComponentHandle_t` for each input we want to update.

We'll make an array for all of our input handles, and have an enum to access all of them:

```c++
#include <array>

enum InputHandles {
	kInputHandle_A_click,
	kInputHandle_A_touch,
	kInputHandle_trigger_value,
	kInputHandle_trigger_click,
	kInputHandle_joystick_x,
	kInputHandle_joystick_y,
    kInputHandle_joystick_click,
	kInputHandle_haptic,
	kInputHandle_COUNT
};

class ControllerDevice : public vr::ITrackedDeviceServerDriver {
    ...
private:
    std::array<vr::VRInputComponentHandle_t, INPUT_COUNT> my_input_handles_;
};
```

We need to create handles to each individual input component (`x` and `y` for the joystick, `click` and `touch` for
the `A` button, etc.).

Handles to inputs are stored within a container specific to each device. We first need to get this container to fetch
the correct handles we should update for each input component.

We also need to tell the runtime where our input profile is located. We can do this, and also create the components in
the `ControllerDevice::Activate` method:

```c++
void ControllerDevice::Activate(uint32_t unObjectId) {
    ...
    
    // Properties are stored in containers, usually one container per device index. We need to get this container to set
	// The properties we want, so we call this to retrieve a handle to it.
	vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer( my_controller_index_ );
    
    // Tell the runtime where our input profile is located
    vr::VRProperties()->SetStringProperty(container, vr::Prop_InputProfilePath_String,
                                          "{<my_driver_name>}/resources/input/<my_device_name>_profile.json");
    
	vr::VRDriverInput()->CreateBooleanComponent(container, "/input/a/click", &my_input_handles_[kInputHandle_A_click]);
	vr::VRDriverInput()->CreateBooleanComponent(container, "/input/a/touch", &my_input_handles_[kInputHandle_A_touch]);

	vr::VRDriverInput()->CreateScalarComponent(container, "/input/trigger/value", &my_input_handles_[kInputHandle_trigger_value],
		vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedOneSided);
	vr::VRDriverInput()->CreateBooleanComponent(container, "/input/trigger/click", &my_input_handles_[kInputHandle_trigger_click]);

	vr::VRDriverInput()->CreateScalarComponent(container, "/input/joystick/x", &my_input_handles_[kInputHandle_joystick_x],
		vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
	vr::VRDriverInput()->CreateScalarComponent(container, "/input/joystick/y", &my_input_handles_[kInputHandle_joystick_y],
		vr::VRScalarType_Absolute, vr::VRScalarUnits_NormalizedTwoSided);
	vr::VRDriverInput()->CreateBooleanComponent(container, "/input/joystick/click", &my_input_handles_[kInputHandle_joystick_click]);

	vr::VRDriverInput()->CreateHapticComponent(container, "/output/haptic", &my_input_handles_[kInputHandle_haptic]);
}
```

Having the driver name wrapped in curly brackets expands to the full path of the root driver folder. You can use these
for setting path properties.

### Updating Inputs

We now have handles to our inputs, which we can update whenever we like.

In reality, inputs **should** be updated whenever you receive new data from your device, but as we don't have a physical
device for this tutorial, we'll update it in `RunFrame` again.

We can implement the `RunFrame` method now to update our inputs:

```c++
void ControllerDevice::RunFrame() {
    ...
    vr::VRDriverInput()->UpdateBooleanComponent(my_input_handles_[MY_A_CLICK], 1, 0.0);
    vr::VRDriverInput()->UpdateBooleanComponent(my_input_handles_[MY_A_TOUCH], 1, 0.0);
    vr::VRDriverInput()->UpdateScalarComponent(my_input_handles_[MY_TRIGGER_VALUE], 0.5, 0.0);
    vr::VRDriverInput()->UpdateScalarComponent(my_input_handles_[MY_JOYSTICK_X], 0.5, 0.0);
    vr::VRDriverInput()->UpdateScalarComponent(my_input_handles_[MY_JOYSTICK_Y], 0.5, 0.0);
    vr::VRDriverInput()->UpdateBooleanComponent(my_input_handles_[MY_JOYSTICK_CLICK], 1, 0.0);
}
```

We provide the handle to the input we want to update, the value we want to update it to, and the time since the last
input.

### Testing Inputs

We can now test the inputs in VR to ensure they're working. SteamVR provides a test controller interface which will show
our data visually:

![image](https://user-images.githubusercontent.com/39023874/213428665-ee0e1484-88c0-4901-99b5-e9d2660c03f9.png)

![image](https://user-images.githubusercontent.com/39023874/213429004-c7816114-9ffa-493c-9526-d20a1d1eb518.png)

### Haptic outputs

Haptic outputs happen on an event from the application. We can poll for these events
with `IVRServerDriverHost::PollNextEvent`.

We should only poll for events in one place (i.e. not in each device class) because events are scoped to each driver,
and are popped off the queue when they are polled. This means that polling for events in each device class will cause
only one device to receive each event.

So, we'll poll in `IServerTrackedDeviceProvider::RunFrame` and then pass the event to each device class to handle.

We'll add a `HandleEvent` method to our `ControllerDevice` class:

```c++
class ControllerDevice : public vr::ITrackedDeviceServerDriver {
public:
    ...
    void HandleEvent(const vr::VREvent_t &vrevent);
};
```

And call this in `DeviceProvider::RunFrame`:

```c++
void DeviceProvider::RunFrame() {
    vr::VREvent_t vrevent;
    while (vr::VRServerDriverHost()->PollNextEvent(&vrevent, sizeof(vrevent))) {
        my_left_device_->HandleEvent(vrevent);
        my_right_device_->HandleEvent(vrevent);
    }
    ...
}
```

We can now implement the `HandleEvent` method:

```c++
void ControllerDevice::HandleEvent(const vr::VREvent_t& vrevent) {
	switch (vrevent.eventType) {
        case vr::VREvent_Input_HapticVibration: {
            if (vrevent.data.hapticVibration.componentHandle == my_input_handles_[kInputHandle_haptic]) {
                vr::VRDriverLog()->Log("Buzz!");
            }
            break;
        }
	}
}
```

See the samples and documentation for an implementation of haptic events.

## Bindings

Now that we've set up our inputs, we need to tell the runtime how to map them to in-game actions. We do this with
binding profiles.

Binding profiles can be configured in the SteamVR Bindings Interface, which is accessible from the SteamVR settings
menu (SteamVR Settings > Controller > Show Old Controller Binding UI).

Select the application you want to configure bindings for in the list. We'll use Half-Life: Alyx as an example:

![image](https://user-images.githubusercontent.com/39023874/213430276-fe47564c-c0bd-409e-9948-fb78199e10e7.png)

Next, make sure your controller is selected to configure bindings for.

![image](https://user-images.githubusercontent.com/39023874/213423790-5a90a0fe-e1ff-4c6f-adff-f7bbaaf0da5b.png)

Click `Create New Binding`.

The next interface should look like this:
![image](https://user-images.githubusercontent.com/39023874/213431003-81307084-1c17-4cfe-8f77-d3fb7b5107e9.png)

Across the top are tabs which split up actions into certain groups, to make it easier to work with bindings. They are
defined by the application.

Hovering over the tab displays actions that have not yet been bound. Be sure to fill out all
the required bindings.

To create a binding, click the `+` next to an input, select what sort of input it is, then select the action to bind.

![image](https://user-images.githubusercontent.com/39023874/213431710-bae74617-0d0a-4ff9-b40d-edca37d35bbe.png)

![image](https://user-images.githubusercontent.com/39023874/213431750-83fdfbdd-ec7c-4ae4-aa9a-67bc81b5e1f2.png)

![image](https://user-images.githubusercontent.com/39023874/213431861-993f6daa-cc7e-46d7-b1f3-45048056066a.png)

Go through and do this for all the actions you want to bind. You **must** bind all required bindings, or you won't be
able to save the bindings profile.

Once you've bound all your inputs to actions for the application, click `Save Personal Binding`. This will save it
to `Documents\steamvr\input`.
Navigate back and make sure the binding profile you've just set is active.

### Default bindings

You can ship these bindings with your driver, providing them as a default binding profile if the user has not already
configured one.

Binding profiles you've created live in `Documents\steamvr\input`. Copy the appropriate bindings file (check the appId
of the application).

Copy and paste the bindings profile to `<my_driver_name>/resources/input/default_bindings`.

Your file structure should now look like this:

    <project_root>
    ├── <my_driver_name>
    │   ├── driver.vrdrivermanifest
    │   └── resources
    │       └── input
    │           ├── <my_device_name>_profile.json
    │           └── default_bindings
    │               └── steam.app.<appId>.<my_controller_name>.json
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp

We specify our default bindings in the input profile.

Add a `default_bindings` field to the input profile:

```json
{
  "default_bindings": [
    {
      "app_key": "steam.app.<appId>",
      "binding_url": "default_bindings/steam.app.<appId>.<my_controller_name>.json"
    }
  ]
}
```

For example, for Half-Life: Alyx for a device called `sample_controller`, this would look like:

```json
{
  "default_bindings": [
    {
      "app_key": "steam.app.546560",
      "binding_url": "default_bindings/steam.app.546560_sample_controller.json"
    }
  ]
}
```

Our input profile should now look like this:

```json
{
  "jsonid": "input_profile",
  "controller_type": "<my_device_name>",
  "input_bindingui_mode": "controller_handed",
  "input_source": {
    "/input/a": {
      "type": "button",
      "touch": true,
      "binding_image_point": [
        80,
        60
      ],
      "order": 1
    },
    "/input/trigger": {
      "type": "trigger",
      "binding_image_point": [
        250,
        60
      ],
      "order": 2
    },
    "/input/joystick": {
      "type": "joystick",
      "click": true,
      "binding_image_point": [
        60,
        60
      ],
      "order": 3
    },
    "/output/haptic": {
      "type": "vibration",
      "binding_image_point": [
        300,
        150
      ],
      "order": 4
    }
  },
  "default_bindings": [
    {
      "app_key": "steam.app.546560",
      "binding_url": "default_bindings/steam.app.546560_sample_controller.json"
    }
  ]
}
```

Append to the `default_bindings` array to add additional bindings for the controller.

## Device Icons

Currently, we have default icons that represent our controller in the SteamVR monitor. We can customise these icons to
help users know the status of each of their device's in the system.

We'll add our icons to the `resources/icons` folder. We can name them anything.

### Recommended Icons

There's a few icons that we **should** have for our device. These are:

* Device in normal operation
    * This should be a solid, coloured icon that provides a visual representation of the device.

* Device off
    * A shaded-out visual representation of the device.
* Device alert
    * A visual representation of the device in a ready status with an alert icon on top. This represents whether the
      device has some information it needs to tell the user, such as needing a firmware update.
* Device searching
    * This should be a "breathing" animation as a .gif that fades in and out to show the device has not yet established
      tracking.
* Device searching alert
    * This should be a "breathing" animation as a .gif that fades in and out to show the device has not yet established
      tracking, but with an alert icon on top representing the device some information it needs to tell the user, such
      as needing a firmware update.
* Low battery
    * The device has low battery. This should be a visual representation of the device displaying the device has low
      battery

Icons must be one of the following sizes:

* HMD: 50x32 or 100x64 (see below) png/gif
* Other (Controller, tracker, etc.): 32x32 or 64x64 (see below) png/gif

An icon's filename instructs the runtime on the dimensions of the icon to use. Append `@2x` to the filename to have a
100x64 HMD icon or 64x64 controller icon. For example:

* `my_hmd_icon.png` - 50x32 icon **must** be used.
* `my_hmd_icon@2x.png` - 100x64 icon **must** be used.

Icons **must** be a colored green-blue gradient across the image. Icons **will** be formatted appropriately to include
this gradient if they do not have it. These gradients are generated by the runtime, and are placed into the same folder
the icon is in, with  `.b4bfb144` appended to the filename.

We'll add icons to the `resources/icons` folder. We can name them anything, and specify which icons link to which
properties later on.

The files specified within this directory `sample/resources/icons` are:

* `controller_status_standby.png`
* `controller_status_error.png`
* `controller_status_off.png`
* `controller_status_ready.png`
* `controller_status_ready_alert.png`
* `controller_status_ready_low.png`
* `controller_status_searching.gif`
* `controller_status_searching_alert.gif`

We'll now create a new file in `resources/` called `driver.vrresources`, where we can specify which icons link to what
properties of the device.

In `<my_driver_name>/resources/driver.vrresources` add:

```json
{
  "jsonid": "vrresources",
  "statusicons": {
    "Controller": {
      "Prop_NamedIconPathDeviceOff_String": "{<my_driver_name>}/icons/controller_status_off.png",
      "Prop_NamedIconPathDeviceSearching_String": "{<my_driver_name>}/icons/controller_status_searching.gif",
      "Prop_NamedIconPathDeviceSearchingAlert_String": "{<my_driver_name>}/icons/controller_status_searching_alert.gif",
      "Prop_NamedIconPathDeviceReady_String": "{<my_driver_name>}/icons/controller_status_ready.png",
      "Prop_NamedIconPathDeviceReadyAlert_String": "{<my_driver_name>}/icons/controller_status_ready_alert.png",
      "Prop_NamedIconPathDeviceNotReady_String": "{<my_driver_name>}/icons/controller_status_error.png",
      "Prop_NamedIconPathDeviceStandby_String": "{<my_driver_name>}/icons/controller_status_standby.png",
      "Prop_NamedIconPathDeviceAlertLow_String": "{<my_driver_name>}/icons/controller_status_ready_low.png"
    }
  }
}
```

`<my_driver_name>` wrapped in curly brackets (i.e. `{<my_driver_name>}` specifies a wildcard to the absolute directory
of the driver folder.)

As we have the same icons for both the right and left-handed controllers, we can use the generic key `Controller` to
specify the icons for both controllers.

If you want to specify icons for the right and left-handed controllers separately, you can use the keys `LeftController`
and `RightController` instead:

```json
{
  "jsonid": "vrresources",
  "statusicons": {
    "LeftController": {
      "Prop_NamedIconPathDeviceOff_String": "{<my_driver_name>}/icons/controller_status_off.png",
      "Prop_NamedIconPathDeviceSearching_String": "{<my_driver_name>}/icons/controller_status_searching.gif",
      "Prop_NamedIconPathDeviceSearchingAlert_String": "{<my_driver_name>}/icons/controller_status_searching_alert.gif",
      "Prop_NamedIconPathDeviceReady_String": "{<my_driver_name>}/icons/controller_status_ready.png",
      "Prop_NamedIconPathDeviceReadyAlert_String": "{<my_driver_name>}/icons/controller_status_ready_alert.png",
      "Prop_NamedIconPathDeviceNotReady_String": "{<my_driver_name>}/icons/controller_status_error.png",
      "Prop_NamedIconPathDeviceStandby_String": "{<my_driver_name>}/icons/controller_status_standby.png",
      "Prop_NamedIconPathDeviceAlertLow_String": "{<my_driver_name>}/icons/controller_status_ready_low.png"
    },
    "RightController": {
      "Prop_NamedIconPathDeviceOff_String": "{<my_driver_name>}/icons/controller_status_off.png",
      "Prop_NamedIconPathDeviceSearching_String": "{<my_driver_name>}/icons/controller_status_searching.gif",
      "Prop_NamedIconPathDeviceSearchingAlert_String": "{<my_driver_name>}/icons/controller_status_searching_alert.gif",
      "Prop_NamedIconPathDeviceReady_String": "{<my_driver_name>}/icons/controller_status_ready.png",
      "Prop_NamedIconPathDeviceReadyAlert_String": "{<my_driver_name>}/icons/controller_status_ready_alert.png",
      "Prop_NamedIconPathDeviceNotReady_String": "{<my_driver_name>}/icons/controller_status_error.png",
      "Prop_NamedIconPathDeviceStandby_String": "{<my_driver_name>}/icons/controller_status_standby.png",
      "Prop_NamedIconPathDeviceAlertLow_String": "{<my_driver_name>}/icons/controller_status_ready_low.png"
    }
  }
}
```

In reality, each of icon paths would be different for both `LeftController` and `RightController`.

Or you can specify icons per model number:

```json
{
  "jsonid": "vrresources",
  "statusicons": {
    "<my_model_number>": {
      "Prop_NamedIconPathDeviceOff_String": "{<my_driver_name>}/icons/controller_status_off.png",
      "Prop_NamedIconPathDeviceSearching_String": "{<my_driver_name>}/icons/controller_status_searching.gif",
      "Prop_NamedIconPathDeviceSearchingAlert_String": "{<my_driver_name>}/icons/controller_status_searching_alert.gif",
      "Prop_NamedIconPathDeviceReady_String": "{<my_driver_name>}/icons/controller_status_ready.png",
      "Prop_NamedIconPathDeviceReadyAlert_String": "{<my_driver_name>}/icons/controller_status_ready_alert.png",
      "Prop_NamedIconPathDeviceNotReady_String": "{<my_driver_name>}/icons/controller_status_error.png",
      "Prop_NamedIconPathDeviceStandby_String": "{<my_driver_name>}/icons/controller_status_standby.png",
      "Prop_NamedIconPathDeviceAlertLow_String": "{<my_driver_name>}/icons/controller_status_ready_low.png"
    }
  }
}
```

Your file structure should now look like this:

    <project_root>
    ├── <my_driver_name>
    │   ├── driver.vrdrivermanifest
    │   └── resources
    │       ├── input
    │       │   ├── <my_device_name>_profile.json
    │       │   └── default_bindings
    │       │       └── steam.app.<appId>.<my_controller_name>.json
    │       ├── icons
    │       │   └── icon files...
    │       └── driver.vrresources
    └── src
        ├── hmd_driver_factory.cpp
        ├── device_provider.h
        ├── device_provider.cpp
        ├── controller_device.h
        └── controller_device.cpp
