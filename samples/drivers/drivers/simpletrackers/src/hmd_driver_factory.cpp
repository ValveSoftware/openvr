//============ Copyright (c) Valve Corporation, All rights reserved. ============
#include "device_provider.h"
#include "openvr_driver.h"
#include <cstring>

#if defined( _WIN32 )
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined( __GNUC__ ) || defined( COMPILER_GCC ) || defined( __APPLE__ )
#define HMD_DLL_EXPORT extern "C" __attribute__( ( visibility( "default" ) ) )
#define HMD_DLL_IMPORT extern "C"
#else
#error "Unsupported Platform."
#endif

MyDeviceProvider device_provider;

//-----------------------------------------------------------------------------
// Purpose: This is exported from the shared library to be called as the entry point into the driver by vrserver.
// You should return a point to your IServerTrackedDeviceProvider here, as well as optionally a watchdog (see other
// samples).
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory( const char *pInterfaceName, int *pReturnCode )
{
	// This is where we return our device provider, if the HmdDriverFactory call asks for it.
	if ( 0 == strcmp( vr::IServerTrackedDeviceProvider_Version, pInterfaceName ) )
	{
		return &device_provider;
	}

	// Otherwise tell the runtime that we don't have this interface.
	if ( pReturnCode )
		*pReturnCode = vr::VRInitError_Init_InterfaceNotFound;

	return NULL;
}