//========= Copyright Valve Corporation ============//

#include "driverlog.h"

#include <stdio.h>
#include <stdarg.h>

static vr::IDriverLog * s_pLogFile = NULL;


bool InitDriverLog( vr::IDriverLog *pDriverLog )
{
	if( s_pLogFile )
		return false;
	s_pLogFile = pDriverLog;
	return s_pLogFile != NULL;
}

void CleanupDriverLog()
{
	s_pLogFile = NULL;
}

static void DriverLogVarArgs( const char *pMsgFormat, va_list args )
{
	char buf[1024];
#if defined( WIN32 )
	vsprintf_s( buf, pMsgFormat, args );
#else
	vsnprintf( buf, sizeof(buf), pMsgFormat, args );
#endif

	if( s_pLogFile )
		s_pLogFile->Log( buf );
}


void DriverLog( const char *pMsgFormat, ... )
{
	va_list args;
	va_start( args, pMsgFormat );

	DriverLogVarArgs( pMsgFormat, args );

	va_end(args);
}


void DebugDriverLog( const char *pMsgFormat, ... )
{
#ifdef _DEBUG
	va_list args;
	va_start( args, pMsgFormat );

	DriverLogVarArgs( pMsgFormat, args );

	va_end(args);
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString( vr::IClientDriverHost *pClientHost, vr::TrackedDeviceIndex_t unDevice, vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *peError )
{
	uint32_t unRequiredBufferLen = pClientHost->GetStringTrackedDeviceProperty( unDevice, prop, NULL, 0, peError );
	if( unRequiredBufferLen == 0 )
		return "";

	char *pchBuffer = new char[ unRequiredBufferLen ];
	unRequiredBufferLen = pClientHost->GetStringTrackedDeviceProperty( unDevice, prop, pchBuffer, unRequiredBufferLen, peError );
	std::string sResult = pchBuffer;
	delete [] pchBuffer;
	return sResult;
}


