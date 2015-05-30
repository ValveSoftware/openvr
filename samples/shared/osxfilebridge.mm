//========= Copyright Valve Corporation ============//
#include <Foundation/Foundation.h>
#include <Foundation/NSPathUtilities.h>
#include <CoreFoundation/CFBase.h>

#include <string>

#include "osxfilebridge.h"

std::string GetHomeDir( )
{
    std::string sHomeDir;
    @autoreleasepool {
        NSString *pnsHomeDir = NSHomeDirectory();
        sHomeDir.assign([pnsHomeDir UTF8String]);
    }
    return sHomeDir;
}