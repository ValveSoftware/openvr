//========= Copyright Valve Corporation ============//
#include <vrcore/sharedlibtools_public.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(POSIX)
#include <dlfcn.h>
#endif


SharedLibHandle SharedLib_Load( const char *pchPath, uint32_t *pErrorCode )
{
	SharedLibHandle pHandle = nullptr;
#if defined( _WIN32)
	pHandle = ( SharedLibHandle )LoadLibraryEx( pchPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
#elif defined(POSIX)
	pHandle = (SharedLibHandle) dlopen(pchPath, RTLD_LOCAL|RTLD_NOW);
#endif

	if ( pErrorCode )
	{
		if ( pHandle == nullptr )
		{
#if defined( _WIN32)
			*pErrorCode = ( uint32_t ) GetLastError();
#elif defined(POSIX)
			*pErrorCode = 1;
#endif
		}
		else
		{
			*pErrorCode = 0;
		}
	}

	return pHandle;
}

void *SharedLib_GetFunction( SharedLibHandle lib, const char *pchFunctionName)
{
#if defined( _WIN32)
	return (void*)GetProcAddress( (HMODULE)lib, pchFunctionName );
#elif defined(POSIX)
	return dlsym( lib, pchFunctionName );
#endif
}


void SharedLib_Unload( SharedLibHandle lib )
{
	if ( !lib )
		return;
#if defined( _WIN32)
	FreeLibrary( (HMODULE)lib );
#elif defined(POSIX)
	dlclose( lib );
#endif
}


