# Simple HMD Driver

This driver provides an example on how to add a HMD device to SteamVR.

The device driver has an additional component, `IVRDisplayComponent` which provides the display information for the HMD.

Ensure that the window is in focus when running the sample to see the output. The hmd will slowly move up and down to
demonstrate providing pose data to OpenVR.

## Folder Structure

`simplehmd/` - contains resource files.

`src/` - contains source code.

## Building

Use the solution or cmake in `samples/` to build this driver.