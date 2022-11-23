//========= Copyright Valve Corporation ============//
#pragma once

#include <stdint.h>
#include <string>


#if !defined(_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#endif


extern bool BCreateDirectoryRecursive( const char *pchPath );
extern bool BCreateDirectory( const char *pchPath );


#if !defined( VRCORE_NO_PLATFORM )

// for finddata_t
#include <vrcore/platform.h>

typedef void *HMDHANDLE;

// iterator class, initialize with the path & pattern you want to want files/dirs for.
//
// all string setters and accessors use UTF-8 encoding.
class CDirIterator
{
public:
	CDirIterator( const char *pchPath, const char *pchPattern );
	~CDirIterator();

	bool IsValid() const;

	// fetch the next file
	bool BNextFile();

	// name of the current file - file portion only, not full path
	std::string CurrentFileName();

	// size of the current file
	int64_t CurrentFileLength() const;

	// creation time of the current file
	int64_t CurrentFileCreateTime() const;

	// mod time of the current file
	int64_t CurrentFileWriteTime() const;

	// mode/type checks:

	// is the current file actually a directory?
	bool BCurrentIsDir() const;

	// is the current file hidden?
	bool BCurrentIsHidden() const;

	// is the current file read-only?
	bool BCurrentIsReadOnly() const;

	// is the current file a system file?
	bool BCurrentIsSystem() const;

	// is the current file's archive bit set?
	bool BCurrentIsMarkedForArchive() const;

private:
	void Init( const std::string &sPathAndPattern );
	bool BValidFilename();
	bool m_bNoFiles, m_bUsedFirstFile;

#if defined( _WIN32 )
	HMDHANDLE m_hFind;
	struct _WIN32_FIND_DATAW *m_pFindData;
	std::string m_sFilename;
#else
	int64_t m_hFind;
	struct _finddata_t *m_pFindData;
#endif
};

#endif // VRCORE_NO_PLATFORM