# Hand Skeleton Simulation Driver

This driver provides left and right hand controllers that interact with the Skeletal Input System.

The hands have the ability to curl and splay.

See the video below for an example:

https://user-images.githubusercontent.com/39023874/213689379-169ad66e-3498-4fb3-ada0-5a2096b8ef66.mp4

While the hand skeletons are built programmatically, it is often simpler (and recommended) to load this data from an
external 3D model file.

Both right and left hand controllers are implemented in one `ITrackedDeviceServerDriver` class, with the constructor
taking in the controller role. This is then provided to SteamVR as property of the device, and used internally to offset
the pose `x` position.

They get their tracking data from the current HMD position, with a few examples on how to manipulate the poses.

## Info on the Skeletal Input API

The Skeletal Input API is designed to be used with common industry tools, such as Maya, to make it easier to move
content from 3D editors into VR.

The way that FBX handles conversion to a different coordinate system is to transform the root bone (wrist), then
counter-transform the root's children to account for the root's change, but then leave the local coordinate systems of
the remaining bones as-is

This means that the metacarpals will be rotated 90 degrees from the wrist if trying to build a skeleton
programmatically. You will see this rotation being applied in `ComputeBoneTransformMetacarpal`
within `src/hand_simulation.cpp`.

## Folder Structure

`src/hand_skeleton_simulation.cpp` contains code on how to build up a hand skeleton programmatically, with curl and
splay components.

It is often simpler (and more maintainable) to load this data from an external file that can be updated with 3D
modelling software to change the animations.

`handskeletonsimulation/` - contains resource files.

`src/` - contains source code.

Refer to the `samples/` README for how to build this driver.  

