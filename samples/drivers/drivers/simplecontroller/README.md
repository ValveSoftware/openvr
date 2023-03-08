# Simple Controller Driver

This driver provides an example on how to add right and left hand controller devices to SteamVR, with a couple of simple
inputs.

Both right and left hand controllers are implemented in one `ITrackedDeviceServerDriver` class, with the constructor
taking in the controller role. This is then provided to SteamVR as property of the device, and used internally to offset
the pose `x` position.

They get their tracking data from the current HMD position, with a few examples on how to manipulate the poses.

## Folder Structure

`simplecontroller/` - contains resource files.

`src/` - contains source code.

## Building

Use the solution or cmake in `samples/` to build this driver.