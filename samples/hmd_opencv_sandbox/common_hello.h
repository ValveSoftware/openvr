#ifndef _COMMON_H
#define _COMMON_H

#include "shared/Matrices.h"
#include <string>
#include <openvr.h>

#define CONTENT_FOLDER "../hmd_opencv_sandbox_resources"

#if defined(POSIX)
#include "unistd.h"
#endif

#ifndef _WIN32
#define APIENTRY
#endif

Matrix4 ConvertSteamVRMatrixToMatrix4( const vr::HmdMatrix34_t &matPose );
extern Matrix4 gCurrentViewProjection;

void dprintf( int stream, const char *fmt, ... );

double FileTimeCached( const char * fname );
std::string FileToString( const char * fn );

#endif

