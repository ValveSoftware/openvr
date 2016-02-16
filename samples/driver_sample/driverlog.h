//========= Copyright Valve Corporation ============//

#ifndef DRIVERLOG_H
#define DRIVERLOG_H

#pragma once

#include <string>
#include <openvr_driver.h>

extern void DriverLog( const char *pchFormat, ... );


// --------------------------------------------------------------------------
// Purpose: Write to the log file only in debug builds
// --------------------------------------------------------------------------
extern void DebugDriverLog( const char *pchFormat, ... );


extern bool InitDriverLog( vr::IDriverLog *pDriverLog );
extern void CleanupDriverLog();


/** Retrieves a string property from the IClientHost interface */
std::string GetTrackedDeviceString( vr::IClientDriverHost *pClientHost, vr::TrackedDeviceIndex_t unDevice, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *peError = NULL );



#endif // DRIVERLOG_H