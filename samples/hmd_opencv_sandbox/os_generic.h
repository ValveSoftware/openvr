#ifndef _OS_GENERIC_H
#define _OS_GENERIC_H
/*
	"osgeneric" Generic, platform independent tool for the following operations:

	Delay functions:
		void OGSleep( int is );
		void OGUSleep( int ius );

	Getting current time (may be time from program start, boot, or epoc)
		double OGGetAbsoluteTime();
		double OGGetFileTime( const char * file );

	Thread functions
		og_thread_t OGCreateThread( void * (routine)( void * ), void * parameter );
		void * OGJoinThread( og_thread_t ot );
		void OGCancelThread( og_thread_t ot );

	Mutex functions, used for protecting data structures.
		 (recursive on platforms where available.)
		og_mutex_t OGCreateMutex();
		void OGLockMutex( og_mutex_t om );
		void OGUnlockMutex( og_mutex_t om );
		void OGDeleteMutex( og_mutex_t om );

//Always a semaphore (not recursive)
// og_sema_t OGCreateSema(); //Create a semaphore, comes locked initially.  NOTE: Max count is 32767
//  void OGLockSema( og_sema_t os );
//  int OGGetSema( og_sema_t os );  //if <0 there was a failure.
//  void OGUnlockSema( og_sema_t os );
//  void OGDeleteSema( og_sema_t os );



   Copyright (c) 2011-2012,2013,2016,2018 <>< Charles Lohr
	This file may be licensed under the MIT/x11 license or the NewBSD license.

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of this file.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

	Date Stamp: 2018-03-25: Switched to header-only format.
*/

//Options:
//	OSG_NOINLINE  << Do this if you don't want to use it in the normal way
//	OSG_IMPL_FNS  << If not inlining, then you must have this defined in
//		something that is compiled

#if defined( OSG_NOINLINE ) || defined( OSG_IMPL_FNS )
#define OSG_FUNCTION_PREFIX
#else
#define OSG_FUNCTION_PREFIX static inline
#endif

//Threads and Mutices
typedef void* og_thread_t;
typedef void* og_mutex_t;
typedef void* og_sema_t;

#if !defined( OSG_IMPL_FNS ) && defined( OSG_NOINLINE )
#ifdef __cplusplus
extern "C" {
#endif
OSG_FUNCTION_PREFIX void OGSleep( int is );
OSG_FUNCTION_PREFIX void OGUSleep( int ius );
OSG_FUNCTION_PREFIX double OGGetAbsoluteTime();
OSG_FUNCTION_PREFIX double OGGetFileTime( const char * file );
OSG_FUNCTION_PREFIX og_thread_t OGCreateThread( void * (routine)( void * ), void * parameter );
OSG_FUNCTION_PREFIX void * OGJoinThread( og_thread_t ot );
OSG_FUNCTION_PREFIX void OGCancelThread( og_thread_t ot );
OSG_FUNCTION_PREFIX og_mutex_t OGCreateMutex();
OSG_FUNCTION_PREFIX void OGLockMutex( og_mutex_t om );
OSG_FUNCTION_PREFIX void OGUnlockMutex( og_mutex_t om );
OSG_FUNCTION_PREFIX void OGDeleteMutex( og_mutex_t om );
OSG_FUNCTION_PREFIX og_sema_t OGCreateSema();
OSG_FUNCTION_PREFIX int OGGetSema( og_sema_t os );
OSG_FUNCTION_PREFIX void OGLockSema( og_sema_t os );
OSG_FUNCTION_PREFIX void OGUnlockSema( og_sema_t os );
OSG_FUNCTION_PREFIX void OGDeleteSema( og_sema_t os );

#ifdef __cplusplus
};
#endif

#else



#if defined( WIN32 ) || defined (WINDOWS) || defined( _WIN32)
#define USE_WINDOWS
#endif


#ifdef USE_WINDOWS

#include <windows.h>

OSG_FUNCTION_PREFIX void OGSleep( int is )
{
	Sleep( is*1000 );
}

OSG_FUNCTION_PREFIX void OGUSleep( int ius )
{
	Sleep( ius/1000 );
}

OSG_FUNCTION_PREFIX double OGGetAbsoluteTime()
{
	static LARGE_INTEGER lpf;
	LARGE_INTEGER li;

	if( !lpf.QuadPart )
	{
		QueryPerformanceFrequency( &lpf );
	}

	QueryPerformanceCounter( &li );
	return (double)li.QuadPart / (double)lpf.QuadPart;
}

OSG_FUNCTION_PREFIX double OGGetFileTime( const char * file )
{
	FILETIME ft;

	HANDLE h = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if( h==INVALID_HANDLE_VALUE )
		return -1;

	GetFileTime( h, 0, 0, &ft );

	CloseHandle( h );

	return ft.dwHighDateTime + ft.dwLowDateTime;
}

OSG_FUNCTION_PREFIX og_thread_t OGCreateThread( void * (routine)( void * ), void * parameter )
{
	return (og_thread_t)CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)routine, parameter, 0, 0 );
}

OSG_FUNCTION_PREFIX void * OGJoinThread( og_thread_t ot )
{
	WaitForSingleObject( ot, INFINITE );
	CloseHandle( ot );
	return 0;
}

OSG_FUNCTION_PREFIX void OGCancelThread( og_thread_t ot )
{
	CloseHandle( ot );	
}

OSG_FUNCTION_PREFIX og_mutex_t OGCreateMutex()
{
	return CreateMutex( 0, 0, 0 );
}

OSG_FUNCTION_PREFIX void OGLockMutex( og_mutex_t om )
{
	WaitForSingleObject(om, INFINITE);
}

OSG_FUNCTION_PREFIX void OGUnlockMutex( og_mutex_t om )
{
	ReleaseMutex(om);
}

OSG_FUNCTION_PREFIX void OGDeleteMutex( og_mutex_t om )
{
	CloseHandle( om );
}


OSG_FUNCTION_PREFIX og_sema_t OGCreateSema()
{
	HANDLE sem = CreateSemaphore( 0, 0, 32767, 0 );
	return (og_sema_t)sem;
}

OSG_FUNCTION_PREFIX int OGGetSema( og_sema_t os )
{
	typedef LONG NTSTATUS;
	HANDLE sem = (HANDLE)os;
	typedef NTSTATUS (NTAPI *_NtQuerySemaphore)(
		HANDLE SemaphoreHandle, 
		DWORD SemaphoreInformationClass, /* Would be SEMAPHORE_INFORMATION_CLASS */
		PVOID SemaphoreInformation,      /* but this is to much to dump here     */
		ULONG SemaphoreInformationLength, 
		PULONG ReturnLength OPTIONAL
	);

	typedef struct _SEMAPHORE_BASIC_INFORMATION {   
		ULONG CurrentCount; 
		ULONG MaximumCount;
	} SEMAPHORE_BASIC_INFORMATION;


	static _NtQuerySemaphore NtQuerySemaphore;
	SEMAPHORE_BASIC_INFORMATION BasicInfo;
	NTSTATUS Status;

	if( !NtQuerySemaphore )
	{	
	    NtQuerySemaphore = (_NtQuerySemaphore)GetProcAddress (GetModuleHandle ("ntdll.dll"), "NtQuerySemaphore");
		if( !NtQuerySemaphore )
		{
			return -1;
		}
	}

	
    Status = NtQuerySemaphore (sem, 0 /*SemaphoreBasicInformation*/, 
        &BasicInfo, sizeof (SEMAPHORE_BASIC_INFORMATION), NULL);

    if (Status == ERROR_SUCCESS)
    {       
        return BasicInfo.CurrentCount;
    }

	return -2;
}

OSG_FUNCTION_PREFIX void OGLockSema( og_sema_t os )
{
	WaitForSingleObject( (HANDLE)os, INFINITE );
}

OSG_FUNCTION_PREFIX void OGUnlockSema( og_sema_t os )
{
	ReleaseSemaphore( (HANDLE)os, 1, 0 );
}

OSG_FUNCTION_PREFIX void OGDeleteSema( og_sema_t os )
{
	CloseHandle( os );
}

#else

#define _GNU_SOURCE


#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <unistd.h>

OSG_FUNCTION_PREFIX void OGSleep( int is )
{
	sleep( is );
}

OSG_FUNCTION_PREFIX void OGUSleep( int ius )
{
	usleep( ius );
}

OSG_FUNCTION_PREFIX double OGGetAbsoluteTime()
{
	struct timeval tv;
	gettimeofday( &tv, 0 );
	return ((double)tv.tv_usec)/1000000. + (tv.tv_sec);
}

OSG_FUNCTION_PREFIX double OGGetFileTime( const char * file )
{
	struct stat buff; 

	int r = stat( file, &buff );

	if( r < 0 )
	{
		return -1;
	}

	return buff.st_mtime;
}

OSG_FUNCTION_PREFIX og_thread_t OGCreateThread( void * (routine)( void * ), void * parameter )
{
	pthread_t * ret = malloc( sizeof( pthread_t ) );
	int r = pthread_create( ret, 0, routine, parameter );
	if( r )
	{
		free( ret );
		return 0;
	}
	return (og_thread_t)ret;
}

OSG_FUNCTION_PREFIX void * OGJoinThread( og_thread_t ot )
{
	void * retval;
	if( !ot )
	{
		return 0;
	}
	pthread_join( *(pthread_t*)ot, &retval );
	free( ot );
	return retval;
}

OSG_FUNCTION_PREFIX void OGCancelThread( og_thread_t ot )
{
	if( !ot )
	{
		return;
	}
	pthread_cancel( *(pthread_t*)ot );
	free( ot );
}

OSG_FUNCTION_PREFIX og_mutex_t OGCreateMutex()
{
	pthread_mutexattr_t   mta;
	og_mutex_t r = malloc( sizeof( pthread_mutex_t ) );

	pthread_mutexattr_init(&mta);
	pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init( (pthread_mutex_t *)r, &mta );

	return r;
}

OSG_FUNCTION_PREFIX void OGLockMutex( og_mutex_t om )
{
	if( !om )
	{
		return;
	}
	pthread_mutex_lock( (pthread_mutex_t*)om );
}

OSG_FUNCTION_PREFIX void OGUnlockMutex( og_mutex_t om )
{
	if( !om )
	{
		return;
	}
	pthread_mutex_unlock( (pthread_mutex_t*)om );
}

OSG_FUNCTION_PREFIX void OGDeleteMutex( og_mutex_t om )
{
	if( !om )
	{
		return;
	}

	pthread_mutex_destroy( (pthread_mutex_t*)om );
	free( om );
}


OSG_FUNCTION_PREFIX og_sema_t OGCreateSema()
{
	sem_t * sem = malloc( sizeof( sem_t ) );
	sem_init( sem, 0, 0 );
	return (og_sema_t)sem;
}

OSG_FUNCTION_PREFIX int OGGetSema( og_sema_t os )
{
	int valp;
	sem_getvalue( os, &valp );
	return valp;
}

OSG_FUNCTION_PREFIX void OGLockSema( og_sema_t os )
{
	sem_wait( os );
}

OSG_FUNCTION_PREFIX void OGUnlockSema( og_sema_t os )
{
	sem_post( os );
}

OSG_FUNCTION_PREFIX void OGDeleteSema( og_sema_t os )
{
	sem_destroy( os );
	free(os);
}

#endif

#endif

#endif

