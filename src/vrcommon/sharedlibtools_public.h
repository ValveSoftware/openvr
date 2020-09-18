//========= Copyright Valve Corporation ============//
#pragma once

#include <stdint.h>

typedef void *SharedLibHandle;

SharedLibHandle SharedLib_Load( const char *pchPath, uint32_t *pErrorCode = nullptr );
void *SharedLib_GetFunction( SharedLibHandle lib, const char *pchFunctionName);
void SharedLib_Unload( SharedLibHandle lib );


