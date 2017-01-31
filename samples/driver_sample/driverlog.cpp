//========= Copyright Valve Corporation ============//

#include "driverlog.h"

#include <stdio.h>
#include <stdarg.h>

static vr::IVRDriverLog * s_pLogFile = NULL;


bool InitDriverLog( vr::IVRDriverLog *pDriverLog )
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


