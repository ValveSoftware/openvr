print ("""//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: This is a flattened version of the OpenVR_API interfaces.
// This file is auto-generated, do not edit it.
//
//=============================================================================
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellAPI.h>
#endif

#if defined(OSX)
#include <sys/syslimits.h>
#include <signal.h>
#define MAX_PATH PATH_MAX
#elif defined(LINUX)
#include <linux/limits.h>
#include <signal.h>
#define MAX_PATH PATH_MAX
#elif defined(WIN32)
#else
#error
#endif

#ifdef POSIX
#include <fcntl.h>
#include <dlfcn.h> // dlopen,dlclose, et al
#include <unistd.h>
#include <stdarg.h>
#define HMODULE void *
#define GetProcAddress dlsym
#define OutputDebugString( pchMsg ) fputs( pchMsg, stderr )
#define _strnicmp strncasecmp
//#include "../../common/linuxhelpers_nodeps.h"
#endif

#ifdef OSX
#include <sys/sysctl.h>
#endif

#include <stdlib.h>
#include <assert.h>

#include "../headers/openvr.h"
#include "openvr_capi.h"

class FnTableRegistration
{
public:
	FnTableRegistration( const char *pchInterfaceName, void *pInterface )
	{
		char buffer[255];
		sprintf(buffer, "FnTable:%s", pchInterfaceName);
		m_pInterfaceRegistration = new GenericInterfaceRegistration( buffer, pInterface );
	}
	~FnTableRegistration()
	{
		delete m_pInterfaceRegistration;
	}
private:
	GenericInterfaceRegistration *m_pInterfaceRegistration;
};

""")


import json
import sys
with open('../headers/openvr_api.json') as data_file:
	data = json.load(data_file)

import api_shared
api_shared.outputfntablefuncs('vr', data)
api_shared.outputfntabledecls('vr', data)
api_shared.outputfntableinit('vr', data)
api_shared.outputfntableaccess('vr', data)

