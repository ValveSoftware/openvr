//========= Copyright Valve Corporation ============//
#pragma once

#include <stdint.h>
#include <string>

typedef void *SharedLibHandle;

SharedLibHandle SharedLib_Load( const char *pchPath, std::string *pErrStr = nullptr );
void *SharedLib_GetFunction( SharedLibHandle lib, const char *pchFunctionName);
void SharedLib_Unload( SharedLibHandle lib );


