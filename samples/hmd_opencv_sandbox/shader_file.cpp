#include "shader_file.h"
#include "common_hello.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "chew.h"
#include <GL/glu.h>

ShaderFile::ShaderFile( std::string sShaderBaseName )
	: m_sShaderBaseName( sShaderBaseName )
	, m_dFragLast( 0 )
	, m_dGeoLast( 0 )
	, m_dVertLast( 0 )
	, m_iShaderProgramId( 0 )
{
}

ShaderFile::~ShaderFile()
{
	glDeleteProgram( m_iShaderProgramId );
}

static GLuint CompileShaderPart( GLuint shader_type, std::string & shadername, std::string & shaderdata, std::stringstream & errors )
{
	GLuint nShader = glCreateShader( shader_type );
	const char * compstr = shaderdata.c_str();
	glShaderSource( nShader, 1, &compstr, NULL );
	glCompileShader( nShader );
	GLint vShaderCompiled = GL_FALSE;

	glGetShaderiv( nShader, GL_COMPILE_STATUS, &vShaderCompiled );
	if ( vShaderCompiled != GL_TRUE )
	{
		errors << shadername << ": Unable to compile shader: " << nShader << std::endl;
		int retval;
		glGetShaderiv( nShader, GL_INFO_LOG_LENGTH, &retval );
		if ( retval > 1 ) {
			char * log = (char*)malloc( retval );
			glGetShaderInfoLog( nShader, retval, NULL, log );
			errors << log << std::endl;;
			free( log );
		}

		glDeleteShader( nShader );
		return 0;
	}
	return nShader;
}

int ShaderFile::CheckShader( std::stringstream & errors )
{
	std::string sGeoName = m_sShaderBaseName + ".geo";
	std::string sFragName = m_sShaderBaseName + ".frag";
	std::string sVertName = m_sShaderBaseName + ".vert";

	if ( FileTimeCached( sGeoName.c_str() ) != m_dGeoLast ||
		FileTimeCached( sFragName.c_str() ) != m_dFragLast ||
		FileTimeCached( sVertName.c_str() ) != m_dVertLast )
	{
		std::string GeoData = FileToString( sGeoName.c_str() );
		std::string FragData = FileToString( sFragName.c_str() );
		std::string VertData = FileToString( sVertName.c_str() );

		if ( !FragData.length() || !VertData.length() )
		{
			errors << "Error: Cannot open frag + vertex shader for " << m_sShaderBaseName;
			return -2;
		}

		m_dGeoLast = FileTimeCached( sGeoName.c_str() );
		m_dFragLast = FileTimeCached( sFragName.c_str() );
		m_dVertLast = FileTimeCached( sVertName.c_str() );

		GLuint nGeoShader = CompileShaderPart( GL_GEOMETRY_SHADER, sGeoName, GeoData, errors );
		GLuint nFragShader = CompileShaderPart( GL_FRAGMENT_SHADER, sFragName, FragData, errors );
		GLuint nVertShader = CompileShaderPart( GL_VERTEX_SHADER, sVertName, VertData, errors );

		bool compfail = false;
		if ( GeoData.length() )
		{
			if ( !nGeoShader )
				compfail = true;
		}
		if ( !nVertShader || !nFragShader )
		{
			compfail = true;
		}

		GLuint unProgramID = 0;

		if ( !compfail )
		{
			unProgramID = glCreateProgram();
			if ( GeoData.length() )
			{
				glAttachShader( unProgramID, nGeoShader );
			}
			glAttachShader( unProgramID, nFragShader );
			glAttachShader( unProgramID, nVertShader );
			glLinkProgram( unProgramID );

			GLint programSuccess = GL_TRUE;
			glGetProgramiv( unProgramID, GL_LINK_STATUS, &programSuccess );
			if ( programSuccess != GL_TRUE )
			{
				errors << m_sShaderBaseName << ": Compilation failed." << std::endl;
				int retval;
				glGetShaderiv( unProgramID, GL_INFO_LOG_LENGTH, &retval );
				if ( retval > 1 ) {
					char * log = (char*)malloc( retval );
					glGetProgramInfoLog( unProgramID, retval, NULL, log );
					errors << log << std::endl;;
					free( log );
				}
				glDeleteProgram( unProgramID );
				compfail = true;
			}
		}
		else
		{
			errors << "Compilation failed." << std::endl;
		}

		if ( unProgramID )
		{
			if ( m_iShaderProgramId )
			{
				glDeleteProgram( m_iShaderProgramId );
			}
			m_iShaderProgramId = unProgramID;
		}

		glDeleteShader( nGeoShader );
		glDeleteShader( nFragShader );
		glDeleteShader( nVertShader );

		return compfail?-1:1;
	}
	else
	{
		return 0;
	}
}


int ShaderFile::GetUniformLocation( std::string m_sUniformName )
{
	return glGetUniformLocation( m_iShaderProgramId, m_sUniformName.c_str() );
}

void ShaderFile::Use()
{
	glUseProgram( m_iShaderProgramId );
}
