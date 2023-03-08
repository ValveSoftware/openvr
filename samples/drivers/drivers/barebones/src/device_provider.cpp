//============ Copyright (c) Valve Corporation, All rights reserved. ============
#include "device_provider.h"

#include "driverlog.h"

//-----------------------------------------------------------------------------
// Purpose: This is called by vrserver after it receives a pointer back from HmdDriverFactory.
// You should do your resources allocations here (**not** in the constructor).
//-----------------------------------------------------------------------------
vr::EVRInitError MyDeviceProvider::Init( vr::IVRDriverContext *pDriverContext )
{
	// We need to initialise our driver context to make calls to the server.
	// OpenVR provides a macro to do this for us.
	VR_INIT_SERVER_DRIVER_CONTEXT( pDriverContext );

	// Init has been called. Let's just log to acknowledge. You'll see this
	// in the logs (SteamVR hamburger menu > Developer > Web Console)

	DriverLog( "Init called on MyDeviceProvider!" );

	return vr::VRInitError_None;
}

//-----------------------------------------------------------------------------
// Purpose: Tells the runtime which version of the API we are targeting.
// Helper variables in the header you're using contain this information, which can be returned here.
//-----------------------------------------------------------------------------
const char *const *MyDeviceProvider::GetInterfaceVersions()
{
	return vr::k_InterfaceVersions;
}

//-----------------------------------------------------------------------------
// Purpose: This function is deprecated and never called. But, it must still be defined, or we can't compile.
//-----------------------------------------------------------------------------
bool MyDeviceProvider::ShouldBlockStandbyMode()
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: This is called in the main loop of vrserver.
// Drivers *can* do work here, but should ensure this work is relatively inexpensive.
// A good thing to do here is poll for events from the runtime or applications
//-----------------------------------------------------------------------------
void MyDeviceProvider::RunFrame()
{
}

//-----------------------------------------------------------------------------
// Purpose: This function is called when the system enters a period of inactivity.
// The devices might want to turn off their displays or go into a low power mode to preserve them.
//-----------------------------------------------------------------------------
void MyDeviceProvider::EnterStandby()
{
}

//-----------------------------------------------------------------------------
// Purpose: This function is called after the system has been in a period of inactivity, and is waking up again.
// Turn back on the displays or devices here.
//-----------------------------------------------------------------------------
void MyDeviceProvider::LeaveStandby()
{
}

//-----------------------------------------------------------------------------
// Purpose: This function is called just before the driver is unloaded from vrserver.
// Drivers should free whatever resources they have acquired over the session here.
// Any calls to the server is guaranteed to be valid before this, but not after it has been called.
//-----------------------------------------------------------------------------
void MyDeviceProvider::Cleanup()
{
}