#ifndef _VRTERMINAL_H
#define _VRTERMINAL_H

#include "geometry_object.h"
#include "shader_file.h"
#include "vlinterm.h"

struct TermStructure;

class VRTerminal
{
public:
	VRTerminal();
	~VRTerminal();

	void Resize( int cols, int rows );
	void Prerender();
	void Render(  );
	inline void SetXForm( const Matrix4 & xform ) { m_xformTerm = xform; }
	inline void SetXFormPre( const Matrix4 & xform ) { m_xformTermPre = xform; }
	void Append( const std::string & appenddata );
private:
	Matrix4 m_xformTerm, m_xformTermPre;
	static GeometryObject m_geoTerm;
	static ShaderFile     m_shdTerm;
	static bool           m_bDidInit;
	struct TermStructure m_ts;
	unsigned int         m_unDataTexture;
	int lastCols;
	int lastRows;
	int font_w;
	int font_h;
};

#endif
