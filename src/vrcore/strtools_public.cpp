//========= Copyright Valve Corporation ============//
#include <vrcore/strtools_public.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <codecvt>
#include <iostream>
#include <functional>
#include <locale>
#include <codecvt>
#include <cstdarg>

#if !defined( VRCORE_NO_PLATFORM )
#include <vrcore/assert.h>
#else
#define AssertMsg( cond, ... )
#endif

#if defined( _WIN32 )
#include <windows.h>
#endif

#if defined( OSX ) || defined( LINUX )
//-----------------------------------------------------------------------------
// Purpose:  stricmp -> strcasecmp bridge
//-----------------------------------------------------------------------------
int stricmp( const char *pStr1, const char *pStr2 )
{
	return strcasecmp( pStr1, pStr2 );
}

//-----------------------------------------------------------------------------
// Purpose: strincmp -> strncasecmp bridge
//-----------------------------------------------------------------------------
int strnicmp( const char *pStr1, const char *pStr2, size_t unBufferLen )
{
	return strncasecmp( pStr1, pStr2, unBufferLen );
}
#endif

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool StringHasPrefix( const std::string & sString, const std::string & sPrefix )
{
	return 0 == strnicmp( sString.c_str(), sPrefix.c_str(), sPrefix.length() );
}

bool StringHasPrefixCaseSensitive( const std::string & sString, const std::string & sPrefix )
{
	return 0 == strncmp( sString.c_str(), sPrefix.c_str(), sPrefix.length() );
}


bool StringHasSuffix( const std::string &sString, const std::string &sSuffix )
{
	size_t cStrLen = sString.length();
	size_t cSuffixLen = sSuffix.length();

	if ( cSuffixLen > cStrLen )
		return false;

	std::string sStringSuffix = sString.substr( cStrLen - cSuffixLen, cSuffixLen );

	return 0 == stricmp( sStringSuffix.c_str(), sSuffix.c_str() );
}

bool StringHasSuffixCaseSensitive( const std::string &sString, const std::string &sSuffix )
{
	size_t cStrLen = sString.length();
	size_t cSuffixLen = sSuffix.length();

	if ( cSuffixLen > cStrLen )
		return false;

	std::string sStringSuffix = sString.substr( cStrLen - cSuffixLen, cSuffixLen );

	return 0 == strncmp( sStringSuffix.c_str(), sSuffix.c_str(),cSuffixLen );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
typedef std::codecvt_utf8< wchar_t > convert_type;

std::string UTF16to8(const wchar_t * in)
{
	static std::wstring_convert< convert_type, wchar_t > s_converter;  // construction of this can be expensive (or even serialized) depending on locale

	try
	{
		return s_converter.to_bytes( in );
	}
	catch ( ... )
	{
		return std::string();
	}
}

std::string UTF16to8( const std::wstring & in ) { return UTF16to8( in.c_str() ); }


std::wstring UTF8to16(const char * in)
{
	static std::wstring_convert< convert_type, wchar_t > s_converter;  // construction of this can be expensive (or even serialized) depending on locale

	try
	{
		return s_converter.from_bytes( in );
	}
	catch ( ... )
	{
		return std::wstring();
	}
}

std::wstring UTF8to16( const std::string & in ) { return UTF8to16( in.c_str() ); }

//-----------------------------------------------------------------------------
// Purpose: Format string to std::string converter
//-----------------------------------------------------------------------------
std::string Format( const char *pchFormat, ... )
{
	static constexpr size_t k_ulMaxStackString = 4096;

	va_list args;
	char pchBuffer[k_ulMaxStackString];

	va_start( args, pchFormat );
	int unSize = vsnprintf( pchBuffer, sizeof( pchBuffer ), pchFormat, args );
	va_end( args );

	// Something went fairly wrong
	if ( unSize < 0 )
	{
		AssertMsg( false, "Format string parse failure" );
		return "";
	}

	// Processing on the stack worked, success
	if ( unSize < k_ulMaxStackString )
	{
		return pchBuffer;
	}

	// If processing on the stack failed, fallback to a dynamic allocation
	std::vector< char > vecChar{};
	vecChar.resize( unSize + 1 );

	va_start( args, pchFormat );
	unSize = vsnprintf( vecChar.data(), vecChar.size(), pchFormat, args );
	va_end( args );

	// Double check, just in case
	if ( unSize < 0 )
	{
		AssertMsg( false, "Format string parse failure" );
		return "";
	}

	return vecChar.data();
}




#if defined( _WIN32 )
//-----------------------------------------------------------------------------
// Purpose: Convert LPSTR in the default CodePage to UTF8
//-----------------------------------------------------------------------------
std::string DefaultACPtoUTF8( const char *pszStr )
{
	if ( GetACP() == CP_UTF8 )
	{
		return pszStr;
	}
	else
	{
		std::vector<wchar_t> vecBuf( strlen( pszStr ) + 1 ); // should be guaranteed to be enough
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pszStr, -1, vecBuf.data(), (int) vecBuf.size() );
		return UTF16to8( vecBuf.data() );
	}
}
#endif

// --------------------------------------------------------------------
// Purpose:
// --------------------------------------------------------------------
void strcpy_safe( char *pchBuffer, size_t unBufferSizeBytes, const char *pchSource )
{
	strncpy( pchBuffer, pchSource, unBufferSizeBytes - 1 );
	pchBuffer[unBufferSizeBytes - 1] = '\0';
}

// --------------------------------------------------------------------
// Purpose: converts a string to upper case
// --------------------------------------------------------------------
std::string StringToUpper( const std::string & sString )
{
	std::string sOut;
	sOut.reserve( sString.size() + 1 );
	for( std::string::const_iterator i = sString.begin(); i != sString.end(); i++ )
	{
		sOut.push_back( (char)toupper( *i ) );
	}

	return sOut;
}


// --------------------------------------------------------------------
// Purpose: converts a string to lower case
// --------------------------------------------------------------------
std::string StringToLower( const std::string & sString )
{
	std::string sOut;
	sOut.reserve( sString.size() + 1 );
	for( std::string::const_iterator i = sString.begin(); i != sString.end(); i++ )
	{
		sOut.push_back( (char)tolower( *i ) );
	}

	return sOut;
}


uint32_t ReturnStdString( const std::string & sValue, char *pchBuffer, uint32_t unBufferLen )
{
	uint32_t unLen = (uint32_t)sValue.length() + 1;
	if( !pchBuffer || !unBufferLen )
		return unLen;

	if( unBufferLen < unLen )
	{
		pchBuffer[0] = '\0';
	}
	else
	{
		memcpy( pchBuffer, sValue.c_str(), unLen );
	}

	return unLen;
}


/** Returns a std::string from a uint64_t */
std::string Uint64ToString( uint64_t ulValue )
{
	char buf[ 22 ];
#if defined( _WIN32 )
	sprintf_s( buf, "%llu", ulValue );
#else
    snprintf( buf, sizeof( buf ), "%llu", (long long unsigned int ) ulValue );
#endif
	return buf;
}


/** returns a uint64_t from a string */
uint64_t StringToUint64( const std::string & sValue )
{
	return strtoull( sValue.c_str(), NULL, 0 );
}

//-----------------------------------------------------------------------------
// Purpose: Helper for converting a numeric value to a hex digit, value should be 0-15.
//-----------------------------------------------------------------------------
char cIntToHexDigit( int nValue )
{
	//Assert( nValue >= 0 && nValue <= 15 );
	return "0123456789ABCDEF"[ nValue & 15 ];
}

//-----------------------------------------------------------------------------
// Purpose: Helper for converting a hex char value to numeric, return -1 if the char
//          is not a valid hex digit.
//-----------------------------------------------------------------------------
int iHexCharToInt( char cValue )
{
	int32_t iValue = cValue;
	if ( (uint32_t)( iValue - '0' ) < 10 )
		return iValue - '0';

	iValue |= 0x20;
	if ( (uint32_t)( iValue - 'a' ) < 6 )
		return iValue - 'a' + 10;

	return -1;
}


//-----------------------------------------------------------------------------
// Purpose: These define the set of characters to filter for components (which
//			need all the escaping we can muster) vs. paths (which don't want
//			/ and : escaped so we don't break less compliant URL handling code.
//-----------------------------------------------------------------------------
static bool CharNeedsEscape_Component( const char c )
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9')
		&& c != '-' && c != '_' && c != '.');
}
static bool CharNeedsEscape_FullPath( const char c )
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9')
		&& c != '-' && c != '_' && c != '.' && c != '/' && c != ':' );
}


//-----------------------------------------------------------------------------
// Purpose: Internal implementation of encode, works in the strict RFC manner, or
//          with spaces turned to + like HTML form encoding.
//-----------------------------------------------------------------------------
void V_URLEncodeInternal( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen, 
	bool bUsePlusForSpace, std::function< bool(const char)> fnNeedsEscape )
{
	//AssertMsg( nDestLen > 3*nSourceLen, "Target buffer for V_URLEncode should be 3x source length, plus one for terminating null\n" );
	
	int iDestPos = 0;
	for ( int i=0; i < nSourceLen; ++i )
	{
		// worst case we need 3 additional chars
		if( (iDestPos+3) > nDestLen  )
		{
			pchDest[0] = '\0';
//			AssertMsg( false, "Target buffer too short\n" );
			return;
		}

		// We allow only a-z, A-Z, 0-9, period, underscore, and hyphen to pass through unescaped.
		// These are the characters allowed by both the original RFC 1738 and the latest RFC 3986.
		// Current specs also allow '~', but that is forbidden under original RFC 1738.
		if ( fnNeedsEscape( pchSource[i] ) )
		{
			if ( bUsePlusForSpace && pchSource[i] == ' ' )
			{
				pchDest[iDestPos++] = '+';
			}
			else
			{
				pchDest[iDestPos++] = '%';
				uint8_t iValue = pchSource[i];
				if ( iValue == 0 )
				{
					pchDest[iDestPos++] = '0';
					pchDest[iDestPos++] = '0';
				}
				else
				{
					char cHexDigit1 = cIntToHexDigit( iValue % 16 );
					iValue /= 16;
					char cHexDigit2 = cIntToHexDigit( iValue );
					pchDest[iDestPos++] = cHexDigit2;
					pchDest[iDestPos++] = cHexDigit1;
				}
			}
		}
		else
		{
			pchDest[iDestPos++] = pchSource[i];
		}
	}

	if( (iDestPos+1) > nDestLen )
	{
		pchDest[0] = '\0';
		//AssertMsg( false, "Target buffer too short to terminate\n" );
		return;
	}

	// Null terminate
	pchDest[iDestPos++] = 0;
}


//-----------------------------------------------------------------------------
// Purpose: Internal implementation of decode, works in the strict RFC manner, or
//          with spaces turned to + like HTML form encoding.
//
//			Returns the amount of space used in the output buffer.
//-----------------------------------------------------------------------------
size_t V_URLDecodeInternal( char *pchDecodeDest, int nDecodeDestLen, const char *pchEncodedSource, int nEncodedSourceLen, bool bUsePlusForSpace )
{
	if ( nDecodeDestLen < nEncodedSourceLen )
	{
		//AssertMsg( false, "V_URLDecode needs a dest buffer at least as large as the source" );
		return 0;
	}

	int iDestPos = 0;
	for( int i=0; i < nEncodedSourceLen; ++i )
	{
		if ( bUsePlusForSpace && pchEncodedSource[i] == '+' )
		{
			pchDecodeDest[ iDestPos++ ] = ' ';
		}
		else if ( pchEncodedSource[i] == '%' )
		{
			// Percent signifies an encoded value, look ahead for the hex code, convert to numeric, and use that

			// First make sure we have 2 more chars
			if ( i < nEncodedSourceLen - 2 )
			{
				char cHexDigit1 = pchEncodedSource[i+1];
				char cHexDigit2 = pchEncodedSource[i+2];

				// Turn the chars into a hex value, if they are not valid, then we'll
				// just place the % and the following two chars direct into the string,
				// even though this really shouldn't happen, who knows what bad clients
				// may do with encoding.
				bool bValid = false;
				int iValue = iHexCharToInt( cHexDigit1 );
				if ( iValue != -1 )
				{
					iValue *= 16;
					int iValue2 = iHexCharToInt( cHexDigit2 );
					if ( iValue2 != -1 )
					{
						iValue += iValue2;
						pchDecodeDest[ iDestPos++ ] = (char)iValue;
						bValid = true;
					}
				}

				if ( !bValid )
				{
					pchDecodeDest[ iDestPos++ ] = '%';
					pchDecodeDest[ iDestPos++ ] = cHexDigit1;
					pchDecodeDest[ iDestPos++ ] = cHexDigit2;
				}
			}

			// Skip ahead
			i += 2;
		}
		else
		{
			pchDecodeDest[ iDestPos++ ] = pchEncodedSource[i];
		}
	}

	// We may not have extra room to NULL terminate, since this can be used on raw data, but if we do
	// go ahead and do it as this can avoid bugs.
	if ( iDestPos < nDecodeDestLen )
	{
		pchDecodeDest[iDestPos] = 0;
	}

	return (size_t)iDestPos;
}

//-----------------------------------------------------------------------------
// Purpose: Encodes a string (or binary data) from URL encoding format, see rfc1738 section 2.2.  
//          This version of the call isn't a strict RFC implementation, but uses + for space as is
//          the standard in HTML form encoding, despite it not being part of the RFC.
//
//          Dest buffer should be at least as large as source buffer to guarantee room for decode.
//-----------------------------------------------------------------------------
void V_URLEncode( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen )
{
	return V_URLEncodeInternal( pchDest, nDestLen, pchSource, nSourceLen, true, CharNeedsEscape_Component );
}


void V_URLEncodeNoPlusForSpace( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen )
{
	return V_URLEncodeInternal( pchDest, nDestLen, pchSource, nSourceLen, false, CharNeedsEscape_Component );
}

void V_URLEncodeFullPath( char *pchDest, int nDestLen, const char *pchSource, int nSourceLen )
{
	return V_URLEncodeInternal( pchDest, nDestLen, pchSource, nSourceLen, false, CharNeedsEscape_FullPath );
}

//-----------------------------------------------------------------------------
// Purpose: Decodes a string (or binary data) from URL encoding format, see rfc1738 section 2.2.  
//          This version of the call isn't a strict RFC implementation, but uses + for space as is
//          the standard in HTML form encoding, despite it not being part of the RFC.
//
//          Dest buffer should be at least as large as source buffer to guarantee room for decode.
//			Dest buffer being the same as the source buffer (decode in-place) is explicitly allowed.
//-----------------------------------------------------------------------------
size_t V_URLDecode( char *pchDecodeDest, int nDecodeDestLen, const char *pchEncodedSource, int nEncodedSourceLen )
{
	return V_URLDecodeInternal( pchDecodeDest, nDecodeDestLen, pchEncodedSource, nEncodedSourceLen, true );
}

size_t V_URLDecodeNoPlusForSpace( char *pchDecodeDest, int nDecodeDestLen, const char *pchEncodedSource, int nEncodedSourceLen )
{
	return V_URLDecodeInternal( pchDecodeDest, nDecodeDestLen, pchEncodedSource, nEncodedSourceLen, false );
}

//-----------------------------------------------------------------------------
void V_StripExtension( std::string &in )
{
	// Find the last dot. If it's followed by a dot or a slash, then it's part of a 
	// directory specifier like ../../somedir/./blah.
	std::string::size_type test = in.rfind( '.' );
	if ( test != std::string::npos )
	{
		// This handles things like ".\blah" or "c:\my@email.com\abc\def\geh"
		// Which would otherwise wind up with "" and "c:\my@email", respectively.
		if ( in.rfind( '\\' ) < test && in.rfind( '/' ) < test )
		{
			in.resize( test );
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Tokenizes a string into a vector of strings
//-----------------------------------------------------------------------------
std::vector<std::string> TokenizeString( const std::string & sString, char cToken )
{
	std::vector<std::string> vecStrings;
	std::istringstream stream( sString );
	std::string s;
	while ( std::getline( stream, s, cToken ) )
	{
		vecStrings.push_back( s );
	}

	if ( !sString.empty() && sString.back() == cToken )
	{
		vecStrings.push_back( "" );
	}

	return vecStrings;
}

//-----------------------------------------------------------------------------
// Purpose: Repairs a should-be-UTF-8 string to a for-sure-is-UTF-8 string, plus return boolean if we subbed in '?' somewhere
//-----------------------------------------------------------------------------
bool RepairUTF8( const char *pbegin, const char *pend, std::string & sOutputUtf8 )
{
	typedef std::codecvt_utf8<char32_t> facet_type;
	facet_type myfacet;

	std::mbstate_t mystate = std::mbstate_t();

	sOutputUtf8.clear();
	sOutputUtf8.reserve( pend - pbegin );
	bool bSqueakyClean = true;

	const char *pmid = pbegin;
	while ( pmid != pend )
	{
		bool bHasError = false;
		bool bHasValidData = false;

		char32_t out = 0xdeadbeef, *pout;
		pbegin = pmid;
		switch ( myfacet.in( mystate, pbegin, pend, pmid, &out, &out + 1, pout ) )
		{
		case facet_type::ok:
			bHasValidData = true;
			break;

		case facet_type::noconv:
			// unexpected! always converting type
			bSqueakyClean = false;
			break;

		case facet_type::partial:
			bHasError = pbegin == pmid;
			if ( bHasError )
			{
				bSqueakyClean = false;
			}
			else
			{
				bHasValidData = true;
			}
			break;

		case facet_type::error:
			bHasError = true;
			bSqueakyClean = false;
			break;
		}

		if ( bHasValidData )
		{
			// could convert back, but no need
			for ( const char *p = pbegin; p != pmid; ++p )
			{
				sOutputUtf8 += *p;
			}
		}

		if ( bHasError )
		{
			sOutputUtf8 += '?';
		}

		if ( pmid == pbegin )
		{
			pmid++;
		}
	}

	return bSqueakyClean;
}

//-----------------------------------------------------------------------------
// Purpose: Repairs a should-be-UTF-8 string to a for-sure-is-UTF-8 string, plus return boolean if we subbed in '?' somewhere
//-----------------------------------------------------------------------------
bool RepairUTF8( const std::string & sInputUtf8, std::string & sOutputUtf8 )
{
	return RepairUTF8( sInputUtf8.data(), sInputUtf8.data() + sInputUtf8.size(), sOutputUtf8 );
}


//-----------------------------------------------------------------------------
// Purpose: Trims trailing CR, LF, Tab, and Space characters
//-----------------------------------------------------------------------------
std::string TrimTrailingWhitespace( const std::string& in )
{
	// trim string at the point where a non-whitespace character occurs -- often at the end of the string
	size_t endpos = in.find_last_not_of( " \t\n\r" );
	if ( std::string::npos != endpos )
	{
		return in.substr( 0, endpos + 1 );
	}
	else
	{
		return in;
	}
}

/** Returns an IP address as a string */
std::string IpToString( uint32_t unIpH )
{
	uint8_t *ip = ( uint8_t * )&unIpH;
	return Format( "%d.%d.%d.%d", ip[ 3 ], ip[ 2 ], ip[ 1 ], ip[ 0 ] );
}

/** Returns an IP address and port as a string */
std::string IpAndPortToString( uint32_t unIpH, uint16_t usPortH )
{
	uint8_t *ip = ( uint8_t * )&unIpH;
	return Format( "%d.%d.%d.%d:%u", ip[ 3 ], ip[ 2 ], ip[ 1 ], ip[ 0 ], usPortH );
}


