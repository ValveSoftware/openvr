# OpenVR Driver Samples

## Driver Samples

`barebones` - A driver that has only the basics.

* `HmdDriverFactory`
* `IServerTrackedDeviceProvider`

`handskeletonsimulation` - A driver showing off skeleton input with position coming from the HMD position.

* `HmdDriverFactory`
* `IServerTrackedDeviceProvider`
* `IVRDriverInput (skeletal input)`
* `IVRSettings`
* `IVRServerDriverHost`
* `IVRDriverLog`
* `IVRProperties`

`simplecontroller` - A driver showing off a simple controller, and what inputs it might have.

* `HmdDriverFactory`
* `IServerTrackedDeviceProvider`
* `IVRDriverInput (boolean, scalar, haptic)`
* `IVRSettings`
* `IVRServerDriverHost`
* `IVRDriverLog`
* `IVRProperties`

## Linking to SteamVR

* Make sure you have built the sample drivers.
    * `output/drivers/<driver_name>` should be populated with a .dll at `bin/win64/<driver_name>.dll` and resources
      in `resources/`
* Navigate to your SteamVR install directory (typically `C:\Program Files (x86)\Steam\steamapps\common\SteamVR`)
* Navigate to `bin\win64`
* Run `vrpathreg.exe adddriver <path_to_samples_dir>/output/drivers/<driver_name>`
  * For example: `vrpathreg.exe adddriver <my_dir>\output\drivers\simplecontroller`
* Run SteamVR and the driver should have been loaded

## Debugging SteamVR with Visual Studio

To debug a driver with SteamVR, some extra set up is required.

A headset **must** be connected in order to debug with Visual Studio.

SteamVR is launched by running `vrstartup.exe`, which spins up child processes: `vrserver.exe` is the process where
drivers get loaded into, and the one that you need to attach a debugger to.

Visual Studio has a tool: "Microsoft child process debugging power tool", which provides functionality to be able to
launch `vrstartup.exe` when running a build and debugging `vrserver.exe`.

Download the extension
here: https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool2022

* In the top toolbar, select Debug > "<project name> Debug Properties" (bottom option in the list)
    - Select Configuration Properties > Debugging
    - In `Command`, copy and paste the path to `vrstartup.exe` executable
        + This is typically located `C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrstartup.exe`, or
          under your Steam install path
    - Click `Ok`
* In the top toolbar Select Debug > Other Debug Targets > Child Process Debugging Settings...
    - Click to enable `Enable child process debugging`
    - On the `<All other processes`, select `Do not debug`
    - Add a row to the table, in the textbox enter `vrserver.exe`
        + Ensure this row is set to `Attach debugger`
    - Click `Save`

When running the project through Visual Studio, SteamVR should open and Visual Studio should constantly stay in the
debugging state.

If Visual Studio exits the debugging state while SteamVR is still running, ensure that Child Process Debugging Settings
has been correctly filled out, and the driver is being loaded into SteamVR.

Clicking Stop in Visual Studio while SteamVR is running **will** cause SteamVR to exit in a crash state, but **will not
** cause SteamVR to enter safe mode (deactivating the driver). While this won't happen, developers **should** exit
SteamVR by closing it properly.

`.pdb` are not provided with SteamVR. A stack trace won't be provided if the driver is crashing.


