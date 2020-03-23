#include "common_hello.h"
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <map>
#include <shared_mutex>
#include "os_generic.h"
#include "hmd_opencv_sandbox.h"

#if defined( WINDOWS ) || defined( WIN32 )
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifndef WIN32
#define statstruct stat
#define _stat stat
#else
#define statstruct _stat64i32
#endif


Matrix4 gCurrentViewProjection;

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
Matrix4 ConvertSteamVRMatrixToMatrix4( const vr::HmdMatrix34_t &matPose )
{
	Matrix4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}


//-----------------------------------------------------------------------------
// Purpose: Outputs a set of optional arguments to debugging output, using
//          the printf format setting specified in fmt*.
//-----------------------------------------------------------------------------
void dprintf( int stream, const char *fmt, ... )
{
	va_list args;
	char buffer[2048];

	va_start( args, fmt );
	vsprintf_s( buffer, fmt, args );
	va_end( args );

	if ( stream == 0 )
	{
		printf( "%s", buffer );
		if( APP ) APP->m_trmErrors.Append( buffer );
	}
	else
	{
		if ( APP ) APP->m_trmProfile.Append( buffer );
	}


	OutputDebugStringA( buffer );
}




static void CacheMapThread();
//Cache check area
std::thread * g_thdCacheChecker;
bool        g_bCreatedThreadCheck;
std::map< std::string, double > g_mFileTimeCacheMap;
std::map< std::string, bool > g_mCheckFileSet;
std::mutex g_mFileTimeCacheMapMutex;
og_sema_t g_mFileTimeRequestMutex;

static void CacheMapThread()
{
	do
	{
		//Alow time between refreshes.
		OGUSleep( 10000 );
		OGLockSema( g_mFileTimeRequestMutex );

		std::string currentfile = "";
		do
		{
			{
				std::unique_lock<std::mutex> ulm( g_mFileTimeCacheMapMutex );
				std::map< std::string, bool >::iterator it;
				if ( currentfile.length() == 0 )
					it = g_mCheckFileSet.begin();
				else
				{
					it = g_mCheckFileSet.find( currentfile );
					it++;
				}
				if ( it == g_mCheckFileSet.end() ) break;
				currentfile = it->first;
				it->second = false;
			}

			double dTime = -1;
			struct statstruct result;
			if ( _stat( currentfile.c_str(), &result ) == 0 )
				dTime = (double)result.st_mtime;

			{
				std::unique_lock<std::mutex> ulm( g_mFileTimeCacheMapMutex );
				g_mFileTimeCacheMap[currentfile] = dTime;
			}
		} while ( 1 );
	} while ( 1 );
}

double FileTimeCached( const char * fn )
{
	if ( !g_bCreatedThreadCheck )
	{
		g_bCreatedThreadCheck = 1;
		g_thdCacheChecker = new std::thread( CacheMapThread );
	}

	std::unique_lock<std::mutex> ulm( g_mFileTimeCacheMapMutex );
	g_mCheckFileSet[fn] = true;
	OGUnlockSema( g_mFileTimeRequestMutex );
	double dTime = g_mFileTimeCacheMap[fn];
	return dTime;
}





std::string FileToString( const char * fn )
{
	std::ifstream t( fn );
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}
