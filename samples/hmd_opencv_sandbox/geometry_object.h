#ifndef _GEOMETRY_OBJECT_H
#define _GEOMETRY_OBJECT_H

#include <string>
#include <vector>
#include "shared/Matrices.h"

namespace vr
{
	struct RenderModel_t;
	struct RenderModel_TextureMap_t;
}

class GeometryObject
{
public:
	GeometryObject();
	GeometryObject( const std::string & sGeoBaseName );
	~GeometryObject();

	//Setup functions
	void SetIndexArray( int index, std::vector<unsigned int> & intarray, unsigned int render_type );
	void SetArrayProps( int index, bool dynamic, int stride );
	void SetArrayPropsAdvanced( int index, int part, int elements, int offset );
	std::vector<unsigned int> & GetIndexArrayPtr( int index = 0 );
	std::vector<float> & GetVertexArrayPtr( int index = 0 );
	inline void TaintIndices( int index = 0 ) { m_bTaintIAFlags |= 1 << index; }
	inline void TaintVerts( int index = 0 ) { m_iTaintVertFlag |= 1 << index; }
	void SetRenderType( int index, unsigned int rendertype );

	//Create from higher-level stuff
	int CreateFromRenderModel( const vr::RenderModel_t * vrModel );
	int ApplyTextureRM( const vr::RenderModel_TextureMap_t * vrDiffuseTexture );
	void LoadAsFile(); //Not done.

	//Extra texture stuff
	void ApplyTexture( const uint8_t * data, int width, int height, int channels = 4 );
	void LoadTextureFile( const char * file ) { m_sTextureFile = file; m_dTextureFileTime = -1; }

	//Construction functions
	void TackVertex( int index, int attribute, float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0 );
	void TackIndex( int index, int idx );
	void TackCube( int index, Matrix4 xform ); //Assumes operating with a VA, no IBO, [Vertex Position, Texture Coord] Attributes.
	void MakeUnitSquare( int index ); //Square that goes from -1..1 in vertex and 0..1 in property 1 (texture coord)

	void Render( int index = 0 );
	void Check();

	inline std::string GetName() { return m_sGeoName; }
	inline void SetName( const std::string & name ) { m_sGeoName = name; }
	inline unsigned int GetVAO() { return m_unVertVAO; }
	inline unsigned int GetVBO( int index = 0 ) { return m_glVertBuffers[index]; }
	inline unsigned int GetIBO( int index = 0 ) { return m_unIndexBuffer[index]; }
	inline int GetTextureWidth() { return m_iTextureWidth; }
	inline int GetTextureHeight() { return m_iTextureHeight; }
private:
	std::string m_sGeoName;
	uint32_t m_iTaintVertFlag;
	uint32_t m_bTaintIAFlags;
	unsigned int m_unTexture;
	double       m_dTextureFileTime;
	int m_iTextureWidth, m_iTextureHeight;
	std::string  m_sTextureFile;

	std::vector< std::vector<float>	>		m_vertpos;
	std::vector< int >						m_vertstride;
	std::vector< std::vector< int > >		m_vertoffset;
	std::vector< std::vector< int > >		m_vertelements;
	std::vector< bool >						m_vertdynamic;

	std::vector< std::vector<unsigned int> > m_ia;

	std::vector< unsigned int > m_unIndexBuffer;
	std::vector< unsigned int > m_unRenderTypes;
	unsigned int m_unVertVAO;
	std::vector< unsigned int > m_glVertBuffers;

	void CheckVertexProps( int index );
};

#endif
