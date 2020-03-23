#include "geometry_object.h"
#include "common_hello.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "chew.h"
#include <GL/glu.h>
#include <openvr.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GeometryObject::GeometryObject() :
	m_bTaintIAFlags( 0 ),
	m_iTaintVertFlag( 0 ),
	m_unVertVAO( 0 ),
	m_unTexture( 0 ),
	m_dTextureFileTime( 0 ),
	m_iTextureWidth( 0 ),
	m_iTextureHeight( 0 ),
	m_sGeoName( "Unnamed" )
{
}

GeometryObject::GeometryObject( const std::string & sGeoBaseName ) : GeometryObject()
{
	m_sGeoName = sGeoBaseName;
}

GeometryObject::~GeometryObject()
{
	unsigned i;

	for ( i = 0; i < m_unIndexBuffer.size(); i++ )
	{
		if ( m_unIndexBuffer[i] )
		{
			glDeleteBuffers( 1, &m_unIndexBuffer[i] );
		}
	}

	if ( m_unVertVAO ) glDeleteVertexArrays( 1, &m_unVertVAO );

	for ( i = 0; i < m_glVertBuffers.size(); i++ )
	{
		if ( m_glVertBuffers[i] )
		{
			glDeleteBuffers( 1, &m_glVertBuffers[i] );
		}
	}

	if ( m_unTexture )
	{
		glDeleteTextures( 1, &m_unTexture );
	}
}

//Create from higher-level stuff
int GeometryObject::CreateFromRenderModel( const vr::RenderModel_t * vrModel )
{
	if ( !vrModel ) return -1;
	SetArrayProps( 0, 0, 8 );
	SetArrayPropsAdvanced( 0, 0, 3, 0 );
	SetArrayPropsAdvanced( 0, 1, 3, 3 );
	SetArrayPropsAdvanced( 0, 2, 2, 6 );
	std::vector<float> & verts = GetVertexArrayPtr( 0 );
	float * rvdata = (float*)vrModel->rVertexData;
	for ( unsigned i = 0; i < vrModel->unVertexCount * 8; i++ )
	{
		verts.push_back( rvdata[i] );
	}
	std::vector< unsigned > indices;
	for ( unsigned i = 0; i < vrModel->unTriangleCount * 3; i++ )
	{
		indices.push_back( vrModel->rIndexData[i] );
	}
	SetIndexArray( 0, indices, GL_TRIANGLES );
	Check();
	return 0;
}
int GeometryObject::ApplyTextureRM( const vr::RenderModel_TextureMap_t * vrDiffuseTexture )
{
	if ( !vrDiffuseTexture ) return -1;
	ApplyTexture( vrDiffuseTexture->rubTextureMapData, vrDiffuseTexture->unWidth, vrDiffuseTexture->unHeight );
	return 0;
}


void GeometryObject::ApplyTexture( const uint8_t * data, int width, int height, int channels )
{
	if( !m_unTexture ) glGenTextures( 1, &m_unTexture );
	glBindTexture( GL_TEXTURE_2D, m_unTexture );

	int channelmap[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };
	glTexImage2D( GL_TEXTURE_2D, 0, channelmap[channels], width, height,
		0, channelmap[channels], GL_UNSIGNED_BYTE, data );

	m_iTextureWidth = width;
	m_iTextureHeight = height;

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	//Best not to be default.
	//GLfloat fLargest;
	//glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &fLargest );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, fLargest );

	glBindTexture( GL_TEXTURE_2D, 0 );

}

void GeometryObject::Render( int index )
{
	if ( m_unTexture )
	{
		glActiveTextureCHEW( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_unTexture );
	}
	glBindVertexArray( m_unVertVAO );
	if ( (int)m_unIndexBuffer.size() <= index )
	{
		//Not an IBO, so it's a vertex-array draw.
		glDrawArrays( m_unRenderTypes[index], 0, (GLsizei)m_vertpos[index].size()/ m_vertstride[index] );
	}
	else
	{
		//Regular IBO Draw.
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer[index] );
		glDrawElements( m_unRenderTypes[index], (GLsizei)m_ia[index].size(), GL_UNSIGNED_INT, 0 );
	}
	glBindVertexArray( 0 );
	if ( m_unTexture )
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
}

void GeometryObject::SetRenderType( int index, unsigned int rendertype )
{
	if ( index >= (int)m_unRenderTypes.size() )
	{
		m_unRenderTypes.resize( index + 1 );
	}
	m_unRenderTypes[index] = rendertype;
}

void GeometryObject::LoadAsFile()
{
	//XXX TODO
}

void GeometryObject::TackVertex( int index, int attribute, float x, float y, float z, float w )
{
	int elements = m_vertelements[index][attribute];
	std::vector<float> & vf = m_vertpos[index];

	if ( elements > 0 ) vf.push_back( x );
	if ( elements > 1 ) vf.push_back( y );
	if ( elements > 2 ) vf.push_back( z );
	if ( elements > 3 ) vf.push_back( w );

	TaintVerts( index );
}

void GeometryObject::TackIndex( int index, int idx )
{
	if ( m_ia.size() <= (unsigned)index ) m_ia.resize( index + 1 );
	m_ia[index].push_back( idx );
	TaintIndices( index );
}

void GeometryObject::TackCube( int index, Matrix4 mat )
{
	//XXX This function is untested.
	Vector4 A = mat * Vector4( 0, 0, 0, 1 );
	Vector4 B = mat * Vector4( 1, 0, 0, 1 );
	Vector4 C = mat * Vector4( 1, 1, 0, 1 );
	Vector4 D = mat * Vector4( 0, 1, 0, 1 );
	Vector4 E = mat * Vector4( 0, 0, 1, 1 );
	Vector4 F = mat * Vector4( 1, 0, 1, 1 );
	Vector4 G = mat * Vector4( 1, 1, 1, 1 );
	Vector4 H = mat * Vector4( 0, 1, 1, 1 );

	float VertPosData[] = {
		E.x, E.y, E.z, F.x, F.y, F.z, G.x, G.y, G.z, G.x, G.y, G.z, H.x, H.y, H.z, E.x, E.y, E.z, //Front
		B.x, B.y, B.z, A.x, A.y, A.z, D.x, D.y, D.z, D.x, D.y, D.z, C.x, C.y, C.z, B.x, B.y, B.z, //Back
		H.x, H.y, H.z, G.x, G.y, G.z, C.x, C.y, C.z, C.x, C.y, C.z, D.x, D.y, D.z, H.x, H.y, H.z, //Top
		A.x, A.y, A.z, B.x, B.y, B.z, F.x, F.y, F.z, F.x, F.y, F.z, E.x, E.y, E.z, A.x, A.y, A.z, //Bottom
		A.x, A.y, A.z, E.x, E.y, E.z, H.x, H.y, H.z, H.x, H.y, H.z, D.x, D.y, D.z, A.x, A.y, A.z, //Left
		F.x, F.y, F.z, B.x, B.y, B.z, C.x, C.y, C.z, C.x, C.y, C.z, G.x, G.y, G.z, F.x, F.y, F.z, //Right
	};
	float VertTCDataRep[] = {
		0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
	};
	int i, j;
	for ( i = 0; i < 6; i++ ) 
	for ( j = 0; j < 6; j++ )
	{
		TackVertex( 0, 0, VertPosData[(j + i * 6) * 3 + 0], VertPosData[(j + i * 6) * 3 + 1], VertPosData[(j + i * 6) * 3 + 2] );
		TackVertex( 0, 1, VertTCDataRep[j * 2 + 0], VertTCDataRep[j * 2 + 1] );
	}
}

void GeometryObject::MakeUnitSquare( int index )
{
	SetArrayProps( 0, 0, 4 );
	SetArrayPropsAdvanced( 0, 0, 2, 0 );
	SetArrayPropsAdvanced( 0, 1, 2, 2 );
	SetRenderType( index, GL_TRIANGLES );
	m_bTaintIAFlags |= 1 << index;
	TackVertex( 0, 0, -1, 1 ); 	TackVertex( 0, 1, 0, 1 );
	TackVertex( 0, 0, 1, 1 ); 	TackVertex( 0, 1, 1, 1 );
	TackVertex( 0, 0, -1, -1 ); TackVertex( 0, 1, 0, 0 );
	TackVertex( 0, 0, 1, -1 ); 	TackVertex( 0, 1, 1, 0 );
	TackIndex( 0, 0 );
	TackIndex( 0, 1 );
	TackIndex( 0, 3 );
	TackIndex( 0, 0 );
	TackIndex( 0, 3 );
	TackIndex( 0, 2 );
}

void GeometryObject::SetIndexArray( int index, std::vector<unsigned int> & intarray, unsigned int render_type )
{
	if ( (int)m_ia.size() <= index ) m_ia.resize( index + 1 );
	m_ia[index] = intarray;
	SetRenderType( index, render_type );
	m_bTaintIAFlags |= 1<<index;
}

void GeometryObject::SetArrayProps( int index, bool dynamic, int stride )
{
	CheckVertexProps( index );
	m_vertstride[index] = stride;
	m_vertdynamic[index] = dynamic;
	if ( m_vertoffset[index].size() < 1 ) m_vertoffset[index].resize( 1 );
	if ( m_vertelements[index].size() < 1 ) m_vertelements[index].resize( 1 );
	m_vertoffset[index][0] = 0;
	m_vertelements[index][0] = stride;
	TaintVerts( index );
}

void GeometryObject::CheckVertexProps( int index )
{
	if ( (int)m_vertpos.size() <= index )		m_vertpos.resize( index + 1 );
	if ( (int)m_vertstride.size() <= index )	m_vertstride.resize( index + 1 );
	if ( (int)m_vertdynamic.size() <= index )	m_vertdynamic.resize( index + 1 );
	if ( (int)m_vertoffset.size() <= index )	m_vertoffset.resize( index + 1 );
	if ( (int)m_vertelements.size() <= index )	m_vertelements.resize( index + 1 );
}

void GeometryObject::SetArrayPropsAdvanced( int index, int part, int elements, int offset )
{
	CheckVertexProps( index );
	if ( (int)m_vertoffset[index].size() <= part )		m_vertoffset[index].resize( part+1 );
	if ( (int)m_vertelements[index].size() <= part )	m_vertelements[index].resize( part+1 );
	m_vertoffset[index][part] = offset;
	m_vertelements[index][part] = elements;
	TaintVerts( index );
}


std::vector<unsigned int> & GeometryObject::GetIndexArrayPtr( int index )
{
	if ( index >= (int)m_ia.size() )
	{
		m_ia.resize( index + 1 );
	}
	return m_ia[index];
}
std::vector<float> & GeometryObject::GetVertexArrayPtr( int index )
{
	return m_vertpos[index];
}


void GeometryObject::Check()
{
	if ( m_sTextureFile.length() && FileTimeCached( m_sTextureFile.c_str() ) != m_dTextureFileTime )
	{
		//TODO: Load asychronously.
		if ( !m_unTexture ) glGenTextures( 1, &m_unTexture );
		m_dTextureFileTime = FileTimeCached( m_sTextureFile.c_str() );

		int width, height, channels;
		uint8_t * data = stbi_load( m_sTextureFile.c_str(), &width, &height, &channels, 0 );
		if ( data )
		{
			ApplyTexture( data, width, height, channels );
		}
		free( data );
	}

	if ( !m_iTaintVertFlag && !m_bTaintIAFlags ) return;

	if ( !m_unVertVAO )
	{
		glGenVertexArrays( 1, &m_unVertVAO );
	}


	glBindVertexArray( m_unVertVAO );


	if ( m_bTaintIAFlags )
	{
		unsigned i;

		for ( i = 0; i < m_ia.size(); i++ )
		{
			if ( m_bTaintIAFlags & (1 << i) )
			{
				if ( m_ia[i].size() )
				{
					if ( m_unIndexBuffer.size() <= i )
					{
						m_unIndexBuffer.resize( i + 1 );
					}
					if ( !m_unIndexBuffer[i] )
					{
						glGenBuffers( 1, &m_unIndexBuffer[i] );
					}
					glGenBuffers( 1, &m_unIndexBuffer[i] );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_unIndexBuffer[i] );
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_ia[i].size() * sizeof( unsigned int ), &m_ia[i][0], GL_STATIC_DRAW );
				}
			}
			m_bTaintIAFlags &= ~(1 << i);
		}

	}
	if ( m_iTaintVertFlag )
	{
		//Tricky: Make sure all bit flags outside of our valid buffer range are set to 0.

		for ( unsigned i = 0; i < (unsigned)m_vertpos.size(); i++ )
		{
			m_iTaintVertFlag &= ~(1 << i);
			if ( m_glVertBuffers.size() <= i )
			{
				m_glVertBuffers.resize( i + 1 );
			}
			if ( !m_glVertBuffers[i] )
			{
				glGenBuffers( 1, &m_glVertBuffers[i] );
			}
			if ( i >= m_vertpos.size() )
			{
				//Invalid array.
				continue;
			}

			int stride = (i < m_vertstride.size()) ? m_vertstride[i] : 4;
			int dynamic = (i < m_vertdynamic.size()) ? m_vertdynamic[i] : false;

			glBindBuffer( GL_ARRAY_BUFFER, m_glVertBuffers[i] );
			glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * m_vertpos[i].size(), &m_vertpos[i][0], dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
			if ( m_vertoffset[i].size() == 1 )
			{
				glEnableVertexAttribArray( i );
				glVertexAttribPointer( i, stride, GL_FLOAT, GL_FALSE, stride * sizeof( float ), 0 );
			}
			else
			{
				int j;
				for ( j = 0; j < (int)m_vertoffset[i].size(); j++ )
				{
					glEnableVertexAttribArray( j );
					int size = m_vertelements[i][j];
					int istride = stride * sizeof( float );
					void * ioffset = (void*)(m_vertoffset[i][j] * sizeof( float ));
					glVertexAttribPointer( j, size, GL_FLOAT, GL_FALSE, istride, ioffset );
				}
			}
		}
	}
	glBindVertexArray( 0 );


}
