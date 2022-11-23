//========= Copyright Valve Corporation ============//
#include <vrcore/dirtools_public.h>
#include <vrcore/strtools_public.h>
#include <vrcore/pathtools_public.h>

#include <errno.h>
#include <string.h>

#ifdef _WIN32
#include "windows.h"
#else
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#if defined( OSX )
#include <sys/syslimits.h>
#endif

#define HMD_INVALID_HANDLE_VALUE ( ( HMDHANDLE )( LONG_PTR )-1 )

//-----------------------------------------------------------------------------
// Purpose: utility function to create dirs & subdirs
//-----------------------------------------------------------------------------
bool BCreateDirectoryRecursive( const char *pchPath )
{
	// Does it already exist?
	if ( Path_IsDirectory( pchPath ) )
		return true;

	// copy the path into something we can munge
	int len = (int)strlen( pchPath );
	char *path = (char *)malloc( len + 1 );
	strcpy( path, pchPath );

	// Walk backwards to first non-existing dir that we find
	char *s = path + len - 1;

	const char slash = Path_GetSlash();
	while ( s > path )
	{
		if ( *s == slash )
		{
			*s = '\0';
			bool bExists = Path_IsDirectory( path );
			*s = slash;

			if ( bExists )
			{
				++s;
				break;
			}
		}
		--s;
	}

	// and then move forwards from there

	while ( *s )
	{
		if ( *s == slash )
		{
			*s = '\0';
			BCreateDirectory( path );
			*s = slash;
		}
		s++;
	}

	bool bRetVal = BCreateDirectory( path );
	free( path );
	return bRetVal;
}


//-----------------------------------------------------------------------------
// Purpose: Creates the directory, returning true if it is created, or if it already existed
//-----------------------------------------------------------------------------
bool BCreateDirectory( const char *pchPath )
{
#ifdef WIN32
	std::wstring wPath = UTF8to16( pchPath );
	if ( ::CreateDirectoryW( wPath.c_str(), NULL ) )
		return true;

	if ( ::GetLastError() == ERROR_ALREADY_EXISTS )
		return true;

	return false;
#else
	int i = mkdir( pchPath, S_IRWXU | S_IRWXG | S_IRWXO );
	if ( i == 0 )
		return true;
	if ( errno == EEXIST )
		return true;

	return false;
#endif
}

#if !defined( VRCORE_NO_PLATFORM )

#include <vrcore/log.h>

//-----------------------------------------------------------------------------
CDirIterator::CDirIterator( const char *pchPath, const char *pchPattern )
{
	m_pFindData = NULL;

	// put in the path
	if ( pchPath )
	{
		std::string sPathAndPattern = Path_Join( pchPath, pchPattern );

		Init( sPathAndPattern.c_str() );
	}
	else
	{
		m_bNoFiles = true;
		m_bUsedFirstFile = true;

#if defined( _WIN32 )
		m_hFind = HMD_INVALID_HANDLE_VALUE;
		m_pFindData = new WIN32_FIND_DATAW;
#else
		m_hFind = -1;
		m_pFindData = new _finddata_t;
#endif
		memset( m_pFindData, 0, sizeof( *m_pFindData ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Initialize iteration structure
//-----------------------------------------------------------------------------
void CDirIterator::Init( const std::string &sPathAndPattern )
{
#if defined( _WIN32 )
	m_pFindData = new WIN32_FIND_DATAW;
	memset( m_pFindData, 0, sizeof( *m_pFindData ) );

	std::wstring sWPathAndPattern = UTF8to16( sPathAndPattern.c_str() );

	m_hFind = FindFirstFileW( sWPathAndPattern.c_str(), m_pFindData );
	bool bSuccess = ( m_hFind != HMD_INVALID_HANDLE_VALUE );
	// Conversion should never fail with valid filenames...
	if ( bSuccess )
	{
		m_sFilename = UTF16to8( m_pFindData->cFileName );
	}
#else
	m_pFindData = new _finddata_t;
	memset( m_pFindData, 0, sizeof( *m_pFindData ) );

	m_hFind = _findfirst( sPathAndPattern.c_str(), m_pFindData );
	bool bSuccess = ( m_hFind != -1 );
#endif

	if ( !bSuccess )
	{
		m_bNoFiles = true;
		m_bUsedFirstFile = true;
	}
	else
	{
		m_bNoFiles = false;
		// if we're pointing at . or .., set it as used
		// so we'll look for the next item when BNextFile() is called
		m_bUsedFirstFile = !BValidFilename();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CDirIterator::~CDirIterator()
{
#if defined( _WIN32 )
	if ( m_hFind != HMD_INVALID_HANDLE_VALUE )
	{
		FindClose( m_hFind );
	}
	delete m_pFindData;
#else
	if ( m_hFind != -1 )
	{
		_findclose( m_hFind );
	}
	if ( m_pFindData )
	{
		for ( int i = 0; i < m_pFindData->numNames; i++ )
		{
			// scandir allocates with malloc, so free with free
			free( m_pFindData->namelist[ i ] );
		}
		free( m_pFindData->namelist );
		delete m_pFindData;
	}
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Check for successful construction
//-----------------------------------------------------------------------------
bool CDirIterator::IsValid() const
{
#if defined( _WIN32 )
	return m_hFind != HMD_INVALID_HANDLE_VALUE;
#else
	return m_hFind != -1;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Filter out . and ..
//-----------------------------------------------------------------------------
bool CDirIterator::BValidFilename()
{
#if defined( _WIN32 )
	const char *pch = m_sFilename.c_str();
#else
	const char *pch = m_pFindData->name;
#endif

	if ( ( pch[ 0 ] == '.' && pch[ 1 ] == 0 ) || ( pch[ 0 ] == '.' && pch[ 1 ] == '.' && pch[ 2 ] == 0 ) )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: returns true if there is a file to read
//-----------------------------------------------------------------------------
bool CDirIterator::BNextFile()
{
	if ( m_bNoFiles )
		return false;

	// use the first result
	if ( !m_bUsedFirstFile )
	{
		m_bUsedFirstFile = true;
		return true;
	}

	// find the next item
	for ( ;; )
	{
#if defined( _WIN32 )
		bool bFound = ( FindNextFileW( m_hFind, m_pFindData ) != FALSE );
		// Conversion should never fail with valid filenames...
		if ( bFound )
		{
			m_sFilename = UTF16to8( m_pFindData->cFileName );
		}
#else
		bool bFound = ( _findnext( m_hFind, m_pFindData ) == 0 );
#endif

		if ( !bFound )
		{
			// done
			m_bNoFiles = true;
			return false;
		}

		// skip over the '.' and '..' paths
		if ( !BValidFilename() )
			continue;

		break;
	}

	// have one more file
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: returns name (filename portion only) of the current file.
// Name is emitted in UTF-8 encoding.
// NOTE: This method returns a pointer into a static buffer, either a member
// or the buffer inside the _finddata_t.
//-----------------------------------------------------------------------------
std::string CDirIterator::CurrentFileName()
{
#if defined( _WIN32 )
	return m_sFilename;
#else
	return m_pFindData->name;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns size of the file
//-----------------------------------------------------------------------------
int64_t CDirIterator::CurrentFileLength() const
{
#if defined( _WIN32 )
	LARGE_INTEGER li = { { m_pFindData->nFileSizeLow, ( LONG )m_pFindData->nFileSizeHigh } };
	return li.QuadPart;
#else
	return ( int64_t )m_pFindData->size;
#endif
}

#if defined( _WIN32 )
//-----------------------------------------------------------------------------
// Purpose: utility for converting a regular time to a system filetime
//-----------------------------------------------------------------------------
FILETIME UnixTimeToFileTime( int64_t t )
{
	FILETIME retf;
	long long int ll;
	ll = ( ( long long )t * 10000000LL );
	ll += 116444736000000000LL;
	retf.dwHighDateTime = ll >> 32;
	retf.dwLowDateTime = ll & 0x00000000FFFFFFFF;
	return retf;
}


//-----------------------------------------------------------------------------
// Purpose: utility for converting a system filetime to a regular time
//-----------------------------------------------------------------------------
int64_t FileTimeToUnixTime( FILETIME filetime )
{
	long long int t = filetime.dwHighDateTime;
	t <<= 32;
	t += ( unsigned long )filetime.dwLowDateTime;
	t -= 116444736000000000LL;
	return t / 10000000;
}
#endif // _WIN32

//-----------------------------------------------------------------------------
// Purpose: returns last write time of the file
//-----------------------------------------------------------------------------
int64_t CDirIterator::CurrentFileWriteTime() const
{
#if defined( _WIN32 )
	return FileTimeToUnixTime( m_pFindData->ftLastWriteTime );
#else
	return m_pFindData->time_write;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns the creation time of the file
//-----------------------------------------------------------------------------
int64_t CDirIterator::CurrentFileCreateTime() const
{
#if defined( _WIN32 )
	return FileTimeToUnixTime( m_pFindData->ftCreationTime );
#else
	return m_pFindData->time_create;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns whether current item under examination is a directory
//-----------------------------------------------------------------------------
bool CDirIterator::BCurrentIsDir() const
{
#if defined( _WIN32 )
	return ( m_pFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
	return ( m_pFindData->attrib & _A_SUBDIR ? true : false );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns whether current item under examination is a hidden file
//-----------------------------------------------------------------------------
bool CDirIterator::BCurrentIsHidden() const
{
#if defined( _WIN32 )
	return ( m_pFindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) != 0;
#else
	return ( m_pFindData->attrib & _A_HIDDEN ? true : false );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns whether current item under examination is read-only
//-----------------------------------------------------------------------------
bool CDirIterator::BCurrentIsReadOnly() const
{
#if defined( _WIN32 )
	return ( m_pFindData->dwFileAttributes & FILE_ATTRIBUTE_READONLY ) != 0;
#else
	return ( m_pFindData->attrib & _A_RDONLY ? true : false );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns whether current item under examination is marked as a system file
//-----------------------------------------------------------------------------
bool CDirIterator::BCurrentIsSystem() const
{
#if defined( _WIN32 )
	return ( m_pFindData->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) != 0;
#else
	return ( m_pFindData->attrib & _A_SYSTEM ? true : false );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns whether current item under examination is marked for archiving
//-----------------------------------------------------------------------------
bool CDirIterator::BCurrentIsMarkedForArchive() const
{
#if defined( _WIN32 )
	return ( m_pFindData->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ) != 0;
#else
	return ( m_pFindData->attrib & _A_ARCH ? true : false );
#endif
}


//-----------------------------------------------------------------------------
//
// A huge pile of stuff from fileio.cpp in steam/tier1
//
//-----------------------------------------------------------------------------

#if !defined( _WIN32 )
// findfirst/findnext implementation from filesystem/linux_support.[h|cpp]

static char selectBuf[ PATH_MAX ];

#if defined( OSX ) && !defined( __MAC_10_8 )
static int FileSelect( direntBig_t *ent )
#elif defined( LINUX ) || defined( OSX )
static int FileSelect( const direntBig_t *ent )
#else
#error
#endif
{
	const char *mask = selectBuf;
	const char *name = ent->d_name;

	return FileSelect( name, mask );
}

static int FileSelect( const char *name, const char *mask )
{
	// printf("Test:%s %s\n",mask,name);

	if ( !strcmp( name, "." ) || !strcmp( name, ".." ) )
		return 0;

	if ( !strcmp( mask, "*.*" ) || !strcmp( mask, "*" ) )
		return 1;

	while ( *mask && *name )
	{
		if ( *mask == '*' )
		{
			mask++;		  // move to the next char in the mask
			if ( !*mask ) // if this is the end of the mask its a match
			{
				return 1;
			}
			while ( *name && toupper( *name ) != toupper( *mask ) )
			{ // while the two don't meet up again
				name++;
			}
			if ( !*name )
			{ // end of the name
				break;
			}
		}
		else if ( *mask != '?' )
		{
			if ( toupper( *mask ) != toupper( *name ) )
			{ // mismatched!
				return 0;
			}
			else
			{
				mask++;
				name++;
				if ( !*mask && !*name )
				{ // if its at the end of the buffer
					return 1;
				}
			}
		}
		else /* mask is "?", we don't care*/
		{
			mask++;
			name++;
		}
	}

	return ( !*mask && !*name ); // both of the strings are at the end
}

int FillDataStruct( _finddata_t *dat )
{
	statBig_t fileStat;

	if ( dat->curName >= dat->numNames )
		return -1;

	strncpy( dat->name, dat->namelist[ dat->curName ]->d_name, sizeof( dat->name ) );
	char szFullPath[ MAX_PATH ];
	int nWriteSize = snprintf( szFullPath, sizeof( szFullPath ), "%s%c%s", dat->dirBase, Path_GetSlash(), dat->name );
	if ( nWriteSize >= sizeof( szFullPath ) )
	{
		Log( LogError, "File path truncated\n" );
	}
	if ( statBig( szFullPath, &fileStat ) == 0 )
	{
		dat->attrib = fileStat.st_mode;
		dat->size = fileStat.st_size;
		dat->time_write = fileStat.st_mtime;
		dat->time_create = fileStat.st_ctime;
	}
	else
	{
		dat->attrib = 0;
		dat->size = 0;
		dat->time_write = 0;
		dat->time_create = 0;
	}
	free( dat->namelist[ dat->curName ] );
	dat->namelist[ dat->curName ] = NULL;
	dat->curName++;
	return 1;
}

int _findfirst( const char *fileName, _finddata_t *dat )
{
	char nameStore[ MAX_PATH ];
	char *dir = NULL;
	int n, iret = -1;

	strncpy( nameStore, fileName, sizeof( nameStore ) );

	if ( strrchr( nameStore, '/' ) )
	{
		dir = nameStore;
		while ( strrchr( dir, '/' ) )
		{
			statBig_t dirChk;

			// zero this with the dir name
			dir = strrchr( nameStore, '/' );
			*dir = '\0';
			if ( dir == nameStore )
			{
				strcpy( nameStore, "/" ); // C++11 would prefer no `dir = "/";`
			}
			else
			{
				dir = nameStore;
			}

			if ( statBig( dir, &dirChk ) == 0 && S_ISDIR( dirChk.st_mode ) )
			{
				break;
			}
		}
	}
	else
	{
		// couldn't find a dir separator...
		return -1;
	}

	if ( strlen( dir ) > 0 )
	{
		if ( strlen( dir ) == 1 )
			strncpy( selectBuf, fileName + 1, sizeof( selectBuf ) );
		else
			strncpy( selectBuf, fileName + strlen( dir ) + 1, sizeof( selectBuf ) );

		n = scandirBig( dir, &dat->namelist, FileSelect, alphasortBig );
		if ( n < 0 )
		{
			// silently return, nothing interesting
		}
		else
		{
			dat->curName = 0;
			dat->numNames = n; // n is the number of matches
			strncpy( dat->dirBase, dir, sizeof( dat->dirBase ) );
			iret = FillDataStruct( dat );
			if ( iret < 0 )
			{
				free( dat->namelist );
				dat->namelist = NULL;
				dat->curName = 0;
				dat->numNames = 0;
			}
		}
	}

	//  printf("Returning: %i \n",iret);
	return iret;
}

int _findnext( int64_t handle, _finddata_t *dat )
{
	if ( dat->curName >= dat->numNames )
	{
		free( dat->namelist );
		dat->namelist = NULL;
		dat->curName = 0;
		dat->numNames = 0;
		return -1; // no matches left
	}

	FillDataStruct( dat );
	return 0;
}

bool _findclose( int64_t handle )
{
	return true;
}

#endif // !defined( _WIN32 )

#endif // VRCORE_NO_PLATFORM