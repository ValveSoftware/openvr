#ifndef OPENVR_SAMPLES_SHARED_COMPAT_H_
#define OPENVR_SAMPLES_SHARED_COMPAT_H_

#include <cstdio>
#include <cstring>
#include <cassert>

// Handle non standard code.
#ifndef _WIN32

#include <cstdbool>
#include <unistd.h>
#include <type_traits>

#define sprintf_s   snprintf
#define vsprintf_s  sprintf
#define _stricmp    strcmp
#define stricmp     strcmp
#define strcpy_s(dst, n, src)   int(strncpy(dst, src, n) != nullptr)
#define fopen_s(fd, path, mode) int((*fd = fopen(path, mode)) != nullptr)
#define _vsnprintf_s(buffer, size, fmt, ap)  vsnprintf(buffer, size, fmt, ap)
#define OutputDebugStringA(x) fprintf(stderr, "%s\n", x)

typedef int errno_t;

template < typename T, size_t N >
size_t _countof( T ( & arr )[ N ] )
{
    return std::extent< T[ N ] >::value;
}

#endif  // _WIN32

#endif  // OPENVR_SAMPLES_SHARED_COMPAT_H_
