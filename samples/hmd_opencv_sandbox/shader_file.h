#pragma once

#include <string>
#include <sstream>


enum PresetUniformTypes
{
	UNIFORM_MODELVIEWPERSPECTIVE = 0,
	UNIFORM_MODELVIEW,
	UNIFORM_MODELVIEWINVERSE,
	UNIFORM_PERSPECTIVE,
	UNIFORM_PERSPECTIVEINVERSE,

	UNIFORM_EXP_COLOR = 7,

	UNIFORM_TS0 = 8,
	UNIFORM_TS1,
	UNIFORM_TS2,
	UNIFORM_TS3,
	UNIFORM_TS4,
	UNIFORM_TS5,
	UNIFORM_TS6,
	UNIFORM_TS7,

	UNIFORM_PRESET_MAX,
};


class ShaderFile
{
public:
	ShaderFile( std::string sShaderBaseName );
	~ShaderFile();
	int CheckShader( std::stringstream & errors ); //Returns 0 if no change, 1 if change but successful, negative if error.
	int GetUniformLocation( std::string m_sUniformName );
	int GetShaderProgramId() { return m_iShaderProgramId; }
	void Use();
private:
	std::string m_sShaderBaseName;
	double      m_dGeoLast;
	double      m_dFragLast;
	double      m_dVertLast;
	int         m_iShaderProgramId;
};