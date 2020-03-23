#include "vrterminal.h"
#include "chew.h"
#include "common_hello.h"
#include "vlinterm.h"

GeometryObject	VRTerminal::m_geoTerm( "GeoTerm" );
ShaderFile		VRTerminal::m_shdTerm( CONTENT_FOLDER"/terminal" );
bool			VRTerminal::m_bDidInit;

//Callbacks from vlinterm.
void HandleOSCCommand( struct TermStructure * ts, int parameter, const char * value )
{
}

void HandleBell( struct TermStructure * ts )
{
}


VRTerminal::VRTerminal()
{
	memset( &m_ts, 0, sizeof( m_ts ) );
	m_ts.screen_mutex = OGCreateMutex();
	ResetTerminal( &m_ts );
	Append( "Terminal is reset.\nOk.\n" );
	ResizeScreen( &m_ts, 40, 25 );
	m_xformTerm.identity();
	m_xformTermPre.identity();
}

VRTerminal::~VRTerminal()
{
	DestroyTerminal( &m_ts );
}

void VRTerminal::Resize( int cols, int rows )
{
	ResizeScreen( &m_ts, cols, rows );
}

void VRTerminal::Prerender()
{
	if ( !m_bDidInit )
	{
		m_geoTerm.MakeUnitSquare( 0 );
		m_geoTerm.LoadTextureFile( CONTENT_FOLDER"/codepage437.png" );
		m_geoTerm.Check();
		m_bDidInit = true;

		glGenTextures( 1, &m_unDataTexture );
	}
	if ( lastRows != m_ts.chary || lastCols != m_ts.charx )
	{
		lastRows = m_ts.chary;
		lastCols = m_ts.charx;
		glBindTexture( GL_TEXTURE_2D, m_unDataTexture );

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, lastCols, lastRows,
			0, GL_RGBA, GL_UNSIGNED_BYTE, m_ts.termbuffer );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	if ( m_ts.tainted )
	{
		m_ts.tainted = 0;
		glBindTexture( GL_TEXTURE_2D, m_unDataTexture );
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, lastCols, lastRows, GL_RGBA, GL_UNSIGNED_BYTE, m_ts.termbuffer );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	std::stringstream errors;
	if ( m_shdTerm.CheckShader( errors ) < 0 )
	{
		dprintf( 0, "Error compiling shader: %s\n", errors.str().c_str() );
		return;
	}
}

void VRTerminal::Render( )
{
	glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );

	m_shdTerm.Use();
	glUniformMatrix4fv( UNIFORM_MODELVIEWPERSPECTIVE, 1, GL_FALSE, gCurrentViewProjection.get() );
	glUniformMatrix4fv( UNIFORM_MODELVIEW, 1, GL_FALSE, m_xformTerm.get() );
	glUniformMatrix4fv( 21, 1, GL_FALSE, m_xformTermPre.get() );
	glUniform1i( UNIFORM_TS0, 0 );
	glUniform1i( UNIFORM_TS1, 1 );
	glUniform1i( UNIFORM_TS2, 2 );

	glUniform4f( 20, (float)m_geoTerm.GetTextureWidth(), (float)m_geoTerm.GetTextureHeight(),
		(float)m_ts.charx, (float)m_ts.chary );

	glEnable( GL_TEXTURE_2D );
	glActiveTextureCHEW( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D, m_unDataTexture );
	m_geoTerm.Render();
	glActiveTextureCHEW( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glActiveTextureCHEW( GL_TEXTURE0 + 0 );

	glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
}

void VRTerminal::Append( const std::string & appenddata )
{
	for ( const char & c : appenddata )
	{
		EmitChar( &m_ts, c );
	}
}
