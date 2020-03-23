#include "camera_app.h"
#include "common_hello.h"
#include "hmd_opencv_sandbox.h"
#include <iostream>


#ifndef GL_POINT_SPRITE_OES
#define GL_POINT_SPRITE_OES               0x8861
#endif

#define NUM_SAVED_SETTINGS (8)
CameraApp::settings_t g_savesettings[NUM_SAVED_SETTINGS];

CameraApp::CameraApp( CMainApplication * parentApp ) :
	m_ipxUpdatePlace( 0 )
	, m_frameno( 0 )
	, m_opencv_p( this )
	, m_maxframeno( 100000 )
	, m_ipxUpdatePlaceLast( 0 )
	, m_shdCharlesFloor( CONTENT_FOLDER"/charlesfloor" )
	, m_shdDumb( CONTENT_FOLDER"/dumb" )
	, m_shdPointCloud( CONTENT_FOLDER"/pointcloud" )
	, m_shdPointCloud2( CONTENT_FOLDER"/pointcloud2" )
	, m_shdWorld( CONTENT_FOLDER"/world" )
	, m_shdWorld2( CONTENT_FOLDER"/world2" )
	, m_shdWorld3( CONTENT_FOLDER"/world3" )
	, m_vCurrentColor( 1.0, 1.0, 1.0, 1.0 )
	, m_bSettingsChanged( false )
	, m_iDebugTextureW( 512 )
	, m_iDebugTextureH( 512 )
	, m_geoDepthMap( "DepthMap" )
	, m_geoCharlesFloor( "CharlesFloor" )
	, m_geoPointCloud( "PointCloud" )
	, m_uiPointCloudCount( 65536 * 2 )
	, m_geoJeremyFloor()
	, m_pDebugTextureData( 0 )
	, m_lastLoadedSettingsID( 0 )
{
	m_iTexture = 0;
	m_parent = parentApp;

	 //Alireza (notice different alg than F3)
	CameraApp::settings_t & savesettings_f1 = g_savesettings[0];
	savesettings_f1.iShowAnts = 5;
	savesettings_f1.iGeoMode = 0;
	savesettings_f1.fAntSize = 2.5;
	savesettings_f1.iAntEvery = -6;
	savesettings_f1.bMeshAsLines = 0;
	savesettings_f1.iFloorType = 0;
	savesettings_f1.bGreenMode = 0;
	savesettings_f1.fGeoAlpha = 1;
	savesettings_f1.iStereoAlg = 2;
	savesettings_f1.fAntMaxDist = 100;
	savesettings_f1.fImportanceOfDist = 25;
	savesettings_f1.iWhichAntShader = 1;

	CameraApp::settings_t & savesettings_f2 = g_savesettings[1];
	savesettings_f2.iShowAnts = 6;
	savesettings_f2.iGeoMode = 0;
	savesettings_f2.fAntSize = 1.5;
	savesettings_f2.iAntEvery = 2;
	savesettings_f2.bMeshAsLines = 0;
	savesettings_f2.iFloorType = 0;
	savesettings_f2.bGreenMode = 0;
	savesettings_f2.fGeoAlpha = 0.65f;
	savesettings_f2.iStereoAlg = 2;
	savesettings_f2.fAntMaxDist = 100;
	savesettings_f2.fImportanceOfDist = 10;
	savesettings_f2.iWhichAntShader = 0;

	CameraApp::settings_t & savesettings_f3 = g_savesettings[2];
	savesettings_f3.iShowAnts = 1;
	savesettings_f3.iGeoMode = 0;
	savesettings_f3.fAntSize = 4.9f;
	savesettings_f3.iAntEvery = 3;
	savesettings_f3.bMeshAsLines = 0;
	savesettings_f3.iFloorType = 0;
	savesettings_f3.bGreenMode = 0;
	savesettings_f3.fGeoAlpha = 0.1f;
	savesettings_f3.iStereoAlg = 0;
	
	//Eric Hope + Alireza (notice different alg)
	CameraApp::settings_t & savesettings_f4 = g_savesettings[3];
	savesettings_f4.iShowAnts = 7;
	savesettings_f4.iGeoMode = 0;
	savesettings_f4.fAntSize = 2.7f;
	savesettings_f4.iAntEvery = 4;
	savesettings_f4.bMeshAsLines = 1;
	savesettings_f4.iFloorType = 3;
	savesettings_f4.bGreenMode = 0;
	savesettings_f4.fGeoAlpha = 1;
	savesettings_f4.iStereoAlg = 2;
	savesettings_f4.fAntMaxDist = 100;
	savesettings_f4.fImportanceOfDist = 0;
	savesettings_f4.iWhichAntShader = 0;
	savesettings_f4.fSampleCoverage = 0;
	
	//Shiny Through
	CameraApp::settings_t & savesettings_f5 = g_savesettings[4];
	savesettings_f5.iShowAnts = 0;
	savesettings_f5.iGeoMode = 6;
	savesettings_f5.fAntSize = 0.9f;
	savesettings_f5.bMeshAsLines = 0;
	savesettings_f5.iFloorType = 0;
	savesettings_f5.bGreenMode = 0;
	savesettings_f5.fGeoAlpha = 0.65f;
	savesettings_f5.iStereoAlg = 2;

	//Passthrough
	CameraApp::settings_t & savesettings_f6 = g_savesettings[5];
	savesettings_f6.iShowAnts = 0;
	savesettings_f6.iGeoMode = 7;
	savesettings_f6.fAntSize = 1;
	savesettings_f6.iAntEvery = 4;
	savesettings_f6.bMeshAsLines = 0;
	savesettings_f6.iFloorType = 3;
	savesettings_f6.bGreenMode = 0;
	savesettings_f6.fGeoAlpha = 0.55f;
	savesettings_f6.iStereoAlg = 2;

	//Generic fire XXX THIS IS ACTUALLY UNUSED.
	CameraApp::settings_t & savesettings_f7 = g_savesettings[6];
	savesettings_f7.iShowAnts = 6;
	savesettings_f7.iGeoMode = 0;
	savesettings_f7.fAntSize = 1.5;
	savesettings_f7.iAntEvery = 3;
	savesettings_f7.bMeshAsLines = 0;
	savesettings_f7.iFloorType = 0;
	savesettings_f7.bGreenMode = 0;
	savesettings_f7.fGeoAlpha = 0.65f;
	savesettings_f7.iStereoAlg = 2;
	savesettings_f7.fAntMaxDist = 100;
	savesettings_f7.fImportanceOfDist = 15;
	savesettings_f7.iWhichAntShader = 0;

	//Passthrough 2.0
	CameraApp::settings_t & savesettings_f8 = g_savesettings[7];
	savesettings_f8.iShowAnts = 6;
	savesettings_f8.iGeoMode = 0;
	savesettings_f8.fAntSize = 2.5;
	savesettings_f8.iAntEvery = -6;
	savesettings_f8.bMeshAsLines = 0;
	savesettings_f8.iFloorType = 3;
	savesettings_f8.bGreenMode = 0;
	savesettings_f8.fGeoAlpha = 1;
	savesettings_f8.iStereoAlg = 2;
	savesettings_f8.fAntMaxDist = 1.5;
	savesettings_f8.fImportanceOfDist = 25;
	savesettings_f8.iWhichAntShader = 0;
	savesettings_f8.fSampleCoverage = 0.5;

	LoadSettings( 3 );
}

CameraApp::~CameraApp()
{
	if ( m_pDebugTextureData )
	{
		delete m_pDebugTextureData;
	}

}

static void PushV4( std::vector< float > & d, float v1, float v2, float v3, float v4, float f1, float f2, float f3, float f4 )
{
	d.push_back( v1 );
	d.push_back( v2 );
	d.push_back( v3 );
	d.push_back( v4 );

	d.push_back( f1 );
	d.push_back( f2 );
	d.push_back( f3 );
	d.push_back( f4 );
}

bool CameraApp::BInit()
{
	// Render floor lines
	for( int floorno = 0; floorno < 2; floorno++ )
	{
		m_geoJeremyFloor[floorno].SetArrayProps( 0, 0, 8 );
		m_geoJeremyFloor[floorno].SetArrayPropsAdvanced( 0, 0, 4, 0 );
		m_geoJeremyFloor[floorno].SetArrayPropsAdvanced( 0, 1, 4, 4 );

		std::vector< float > & jeremydata = m_geoJeremyFloor[floorno].GetVertexArrayPtr();

		const int kNumLines = floorno?201:21;
		float fRadius = (float)( floorno?80:4 );
		const float floorLineColor[] = { 0.3f, 0.3f, 0.3f };
		for ( int nLine = 0; nLine < kNumLines; ++nLine )
		{
			float val = (float)nLine / (float)(kNumLines - 1) * fRadius - 0.5f * fRadius;
			PushV4( jeremydata, -fRadius / 2.0f, 0, val, 1, floorLineColor[0], floorLineColor[1], floorLineColor[2], 1 );
			PushV4( jeremydata, fRadius / 2.0f, 0, val, 1, floorLineColor[0], floorLineColor[1], floorLineColor[2], 1 );
			PushV4( jeremydata, val, 0, -fRadius / 2.0f, 1, floorLineColor[0], floorLineColor[1], floorLineColor[2], 1 );
			PushV4( jeremydata, val, 0, fRadius / 2.0f, 1, floorLineColor[0], floorLineColor[1], floorLineColor[2], 1 );
		}

		m_geoJeremyFloor[floorno].SetRenderType( 0, GL_LINES );
		m_geoJeremyFloor[floorno].Check();
	}

	m_geoCharlesFloor.SetArrayProps( 0, 0, 3 );
	m_geoCharlesFloor.SetArrayProps( 1, 0, 2 );
	m_geoCharlesFloor.TackVertex( 0, 0, -600, 0, -600 ); m_geoCharlesFloor.TackVertex( 1, 0, 0, 0 ); m_geoCharlesFloor.TackIndex( 0, 0 );
	m_geoCharlesFloor.TackVertex( 0, 0,  600, 0, -600 ); m_geoCharlesFloor.TackVertex( 1, 0, 1, 0 ); m_geoCharlesFloor.TackIndex( 0, 1 );
	m_geoCharlesFloor.TackVertex( 0, 0, -600, 0,  600 ); m_geoCharlesFloor.TackVertex( 1, 0, 0, 1 ); m_geoCharlesFloor.TackIndex( 0, 2 );
	m_geoCharlesFloor.TackVertex( 0, 0, -600, 0,  600 ); m_geoCharlesFloor.TackVertex( 1, 0, 0, 1 ); m_geoCharlesFloor.TackIndex( 0, 3 );
	m_geoCharlesFloor.TackVertex( 0, 0,  600, 0, -600 ); m_geoCharlesFloor.TackVertex( 1, 0, 1, 0 ); m_geoCharlesFloor.TackIndex( 0, 4 );
	m_geoCharlesFloor.TackVertex( 0, 0,  600, 0,  600 ); m_geoCharlesFloor.TackVertex( 1, 0, 1, 1 ); m_geoCharlesFloor.TackIndex( 0, 5 );
	m_geoCharlesFloor.SetRenderType( 0, GL_TRIANGLES );
	m_geoCharlesFloor.Check();

	//Point cloud
	{
		m_geoPointCloud.SetArrayProps( 0, 1, 8 );
		m_geoPointCloud.SetArrayPropsAdvanced( 0, 0, 4, 0 ); //Position
		m_geoPointCloud.SetArrayPropsAdvanced( 0, 1, 4, 4 ); //Color
		m_geoPointCloud.SetRenderType( 0, GL_POINTS );
		std::vector<float> & pointclouddata = m_geoPointCloud.GetVertexArrayPtr();
		pointclouddata.resize( m_uiPointCloudCount * 8 );
		for ( int i = 0; i < (int)m_uiPointCloudCount; i++ )
		{
			pointclouddata[i * 8 + 0] = i * 0.01f;
			pointclouddata[i * 8 + 1] = i * 0.01f;
			pointclouddata[i * 8 + 2] = i * 0.01f;
			pointclouddata[i * 8 + 3] = 1;

			pointclouddata[i * 8 + 4] = 1.0;
			pointclouddata[i * 8 + 5] = 0.5;
			pointclouddata[i * 8 + 6] = 0.0;
			pointclouddata[i * 8 + 7] = 1.0;
		}
		m_geoPointCloud.Check();
	}

	glGenTextures( 1, &m_iTexture );

	////////////////////////////////////////////////////////////////////////////
	//For depth geometry
	{
		m_geoDepthMap.SetArrayProps( 0, 1, 4 );
		m_geoDepthMap.SetArrayProps( 1, 0, 2 );

		std::vector<float> & depth_vc = m_geoDepthMap.GetVertexArrayPtr( 0 );
		std::vector<float> & depth_tc = m_geoDepthMap.GetVertexArrayPtr( 1 );
		int uiDepthVertCount = DEPTHMAPX * DEPTHMAPY;
		depth_tc.resize( uiDepthVertCount * 2 );
		depth_vc.resize( uiDepthVertCount * 4 );
		for ( int y = 0; y < DEPTHMAPY; y++ )
			for ( int x = 0; x < DEPTHMAPX; x++ )
			{
				depth_tc[(x + y * DEPTHMAPX) * 2 + 0] = 1.0f * x / (DEPTHMAPX - 1);
				depth_tc[(x + y * DEPTHMAPX) * 2 + 1] = 1.0f * y / (DEPTHMAPY - 1);

				float cx = 10.0f * x / (DEPTHMAPX - 1) - 5;
				float cy = 10.0f * y / (DEPTHMAPY - 1) - 5;
				float sincR = sqrt( cx * cx + cy * cy );
				float sinc = sin( sincR ) / sincR;
				depth_vc[(x + y * DEPTHMAPX) * 4 + 0] = 2.0f * x / (DEPTHMAPY - 1) - 1.0f;
				depth_vc[(x + y * DEPTHMAPX) * 4 + 1] = 1 ? nanf( "" ) : sinc;
				depth_vc[(x + y * DEPTHMAPX) * 4 + 2] = 2.0f * y / (DEPTHMAPY - 1) - 1.0f;
				depth_vc[(x + y * DEPTHMAPX) * 4 + 3] = 1.0;
			}

		//From https://github.com/cnlohr/spreadgine/blob/master/src/spreadgine_util.c:216
		int dmxm1 = DEPTHMAPX - 1;
		int dmym1 = DEPTHMAPY - 1;

		//For GL_TRIANGLE_STRIP
		m_geoDepthMap.SetRenderType( 0, GL_TRIANGLE_STRIP );
		m_geoDepthMap.SetRenderType( 1, GL_LINE_STRIP );

		std::vector< unsigned int > & depth_ia = m_geoDepthMap.GetIndexArrayPtr( 0 );
		depth_ia.resize( DEPTHMAPX * dmym1 * 2 );
		//int uiDepthIndexCount = (unsigned int)depth_ia.size();
		for ( int y = 0; y < dmym1; y++ )
		{
			for ( int x = 0; x < DEPTHMAPX; x++ )
			{
				int sq = (x + y * dmxm1) * 2;
				depth_ia[sq + 0] = x + y * (DEPTHMAPX);
				depth_ia[sq + 1] = (x)+(y + 1) * (DEPTHMAPX);
			}
		}
		m_geoDepthMap.TaintIndices( 0 );


		std::vector< unsigned int > & depth_ia_lines = m_geoDepthMap.GetIndexArrayPtr( 1 );
		depth_ia_lines.resize( DEPTHMAPX * dmym1 * 2 );
		//int uiDepthIndexCountLines = (unsigned int)depth_ia_lines.size();

		for ( int y = 0; y < dmym1; y++ )
		{
			for ( int x = 0; x < DEPTHMAPX; x += 2 )
			{
				int sq = (x + y * dmxm1) * 2;
				depth_ia_lines[sq + 0] = x + y * (DEPTHMAPX);
				depth_ia_lines[sq + 1] = (x + 1) + (y) * (DEPTHMAPX);
				depth_ia_lines[sq + 2] = (x + 1) + (y + 1) * (DEPTHMAPX);
				depth_ia_lines[sq + 3] = (x + 2) + (y + 1) * (DEPTHMAPX);
			}
		}
		m_geoDepthMap.TaintIndices( 1 );
	}

	////////////////////////////////////////////////////////////////////////////

	m_pDebugTextureData = new uint32_t[m_iDebugTextureW*m_iDebugTextureH];
	glGenTextures( 1, &m_nDebugTexture );
	glBindTexture( GL_TEXTURE_2D, m_nDebugTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_iDebugTextureW, m_iDebugTextureH, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pDebugTextureData );
	glBindTexture( GL_TEXTURE_2D, 0 );

	m_opencv_p.OpenCVAppStart();

	return true;
}

void CameraApp::EmitDot( float x, float y, float z, float w, float r, float g, float b, float a )
{
	m_ipxUpdatePlace++;
	if ( m_ipxUpdatePlace == m_uiPointCloudCount )m_ipxUpdatePlace = 0;
	int i = m_ipxUpdatePlace;

	std::vector<float> & pointclouddata = m_geoPointCloud.GetVertexArrayPtr();

	pointclouddata[i * 8 + 0] = x;
	pointclouddata[i * 8 + 1] = y;
	pointclouddata[i * 8 + 2] = z;
	pointclouddata[i * 8 + 3] = w;

	pointclouddata[i * 8 + 4] = r;
	pointclouddata[i * 8 + 5] = g;
	pointclouddata[i * 8 + 6] = b;
	pointclouddata[i * 8 + 7] = a;
}

void CameraApp::Shutdown()
{
}

void CameraApp::AdvanceSettings( int direction )
{
	CameraApp::LoadSettings( direction + m_lastLoadedSettingsID );
}

void CameraApp::LoadSettings( int demono )
{
	// callers like circular wrapping
	demono = ( demono % NUM_SAVED_SETTINGS + NUM_SAVED_SETTINGS ) % NUM_SAVED_SETTINGS;
	dprintf( 0, "Loading settings %d\n", demono );
	settings = g_savesettings[demono];
	m_lastLoadedSettingsID = demono;
}

void CameraApp::KeyDown( int32_t c )
{
	if ( c == 's' )
	{
		m_opencv_p.m_bScreenshotNext = true;
		return;
	}

	bool bSettingsChanged = true;

	switch ( c )
	{
	case SDLK_a:		settings.iShowAnts++;			break;
	case SDLK_z:		settings.iWhichAntShader = (settings.iWhichAntShader + 1) % 2; break;
	case SDLK_r:		settings.iGeoMode++;				break;
	case SDLK_2:		settings.fAntSize += .1f;		break;
	case SDLK_w:		if ( settings.fAntSize > 0.1 ) settings.fAntSize -= .1f; break;
	case SDLK_3:		settings.iAntEvery ++;				break;
	case SDLK_e:		if ( settings.iAntEvery > -10 ) settings.iAntEvery --; break;
	case SDLK_6:       if ( settings.fGeoAlpha < 1.0f ) settings.fGeoAlpha += 0.05f; break;
	case SDLK_y:       if ( settings.fGeoAlpha > 0 ) settings.fGeoAlpha -= 0.05f; break;
	case SDLK_t:       settings.bMeshAsLines = !settings.bMeshAsLines; break;
	case SDLK_f:       settings.iFloorType = (settings.iFloorType+1)%4; break;
	case SDLK_g:       settings.bGreenMode = !settings.bGreenMode; break;
	case SDLK_d:       settings.iStereoAlg++; break;
	case SDLK_7:		if ( settings.fAntMaxDist < 30 ) settings.fAntMaxDist += .5; break;
	case SDLK_u:		if ( settings.fAntMaxDist >= 0.5 ) settings.fAntMaxDist -= .5; break;
	case SDLK_8:		if ( settings.fImportanceOfDist < 30 ) settings.fImportanceOfDist += .5f; break;
	case SDLK_i:		if ( settings.fImportanceOfDist >= 0.5f ) settings.fImportanceOfDist -= .5f; break;
	case SDLK_9:		if ( settings.fSampleCoverage < 1.0 ) settings.fSampleCoverage += 0.1f;  break;
	case SDLK_o:		if ( settings.fSampleCoverage > 0 ) settings.fSampleCoverage -= 0.1f; break;

	case SDLK_F1: LoadSettings( 0 ); break;
	case SDLK_F2: LoadSettings( 1 ); break;
	case SDLK_F3: LoadSettings( 2 ); break;
	case SDLK_F4: LoadSettings( 3 ); break;
	case SDLK_F5: LoadSettings( 4 ); break;
	case SDLK_F6: LoadSettings( 5 ); break;
	case SDLK_F7: LoadSettings( 6 ); break;
	case SDLK_F8: LoadSettings( 7 ); break;

	default: printf( "Unknown button pressed: %d\n", c );  bSettingsChanged = false; break;
	}
	if ( bSettingsChanged )
	{
		m_bSettingsChanged = true;
	}
}

void CameraApp::UseShader( ShaderFile & shd, vr::Hmd_Eye nEye )
{
	shd.Use();
	glUniformMatrix4fv( UNIFORM_MODELVIEWPERSPECTIVE, 1, GL_FALSE, gCurrentViewProjection.get() );

	glUniform4f( UNIFORM_EXP_COLOR, m_vCurrentColor.x, m_vCurrentColor.y, m_vCurrentColor.z, m_vCurrentColor.w );

	Matrix4 ModelView = ((nEye == vr::Eye_Left) ? m_parent->m_mat4eyePosLeft : m_parent->m_mat4eyePosRight) * m_parent->m_mat4HMDPose;
	glUniformMatrix4fv( UNIFORM_MODELVIEW, 1, GL_FALSE, ModelView.get() );
	ModelView.invert();
	glUniformMatrix4fv( UNIFORM_MODELVIEWINVERSE, 1, GL_FALSE, ModelView.get() );

	Matrix4 Projection = (nEye == vr::Eye_Left) ? m_parent->m_mat4ProjectionLeft : m_parent->m_mat4ProjectionRight;
	glUniformMatrix4fv( UNIFORM_PERSPECTIVE, 1, GL_FALSE, Projection.get() );
	Projection.invert();
	glUniformMatrix4fv( UNIFORM_PERSPECTIVEINVERSE, 1, GL_FALSE, Projection.get() );
}

void CameraApp::Prerender()
{
	//Update any dots and update the render thread.
	m_opencv_p.Prerender();

	//Update our points with any that were written into the points array.
	//Tricky: Only update dots which have changd.
	{
		int to_update = m_ipxUpdatePlace - m_ipxUpdatePlaceLast;
		int vbo = m_geoPointCloud.GetVBO();
		std::vector<float> & pointclouddata = m_geoPointCloud.GetVertexArrayPtr();
		if ( to_update < 0 )
		{
			//Tricky: to_update has flipped.
			glBindBuffer( GL_ARRAY_BUFFER, vbo );
			glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)(sizeof( float ) * 8 * m_ipxUpdatePlaceLast), sizeof( float ) * (m_uiPointCloudCount - m_ipxUpdatePlaceLast) * 8, &pointclouddata[m_ipxUpdatePlaceLast * 8] );
			glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( float ) * (m_ipxUpdatePlace), &pointclouddata[0] );
		}
		else if ( to_update > 0 )
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbo );
			glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)(sizeof( float ) * 8 * m_ipxUpdatePlaceLast), sizeof( float ) * 8 * to_update, &pointclouddata[m_ipxUpdatePlaceLast * 8] );
		}
		m_ipxUpdatePlaceLast = m_ipxUpdatePlace;
	}

	std::stringstream err;
	if ( m_shdDumb.CheckShader( err ) < 0 ||
		m_shdPointCloud.CheckShader( err ) < 0 ||
		m_shdPointCloud2.CheckShader( err ) < 0 ||
		m_shdCharlesFloor.CheckShader( err ) < 0 ||
		m_shdWorld.CheckShader( err ) < 0 ||
		m_shdWorld3.CheckShader( err ) < 0 ||
		m_shdWorld2.CheckShader( err ) < 0
		)
	{
		dprintf( 0, "Shader Comp Error: %s\n", err.str().c_str() );
	}
}

void CameraApp::RenderScene( vr::Hmd_Eye nEye )
{
	m_frameno++;

	if ( settings.iFloorType == 0 )
	{
		//Render Floor
		m_vCurrentColor = Vector4( settings.bGreenMode ? 0.0f : 1.0f, 1.0f, settings.bGreenMode ? 0.0f : 1.0f, 1.0f );
		UseShader( m_shdCharlesFloor, nEye );
		m_geoCharlesFloor.Render();
	}
	if( settings.iFloorType  == 1 || settings.iFloorType  == 2 )
	{
		//Jeremy floor
		glDepthMask( false );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		m_vCurrentColor = Vector4( settings.bGreenMode ? 0.0f : 1.0f, 1.0f, settings.bGreenMode ? 0.0f : 1.0f, 1.0f );
		UseShader( m_shdDumb, nEye );

		glLineWidth( 1 );
		m_geoJeremyFloor[settings.iFloorType-1].Render();
		glDepthMask( true );

	}


	glBindTexture( GL_TEXTURE_2D, m_iTexture );
	{
		if ( settings.iGeoMode > 7 ) settings.iGeoMode = 0;
		if ( settings.iGeoMode != 0 )
		{
			//Render depth geometry.
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			m_vCurrentColor = Vector4( settings.bGreenMode ? 0.0f : 1.0f, 1.0f, settings.bGreenMode ? 0.0f : 1.0f, settings.fGeoAlpha );
			UseShader( m_shdWorld, nEye );

			glLineWidth( 1.0 );

			switch ( settings.iGeoMode )
			{
			case 1:
				//Jeremy Mode
				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 2.0f, -.2f, 0.06f /* 3rd parameter determines if we clip the floor */ );
				m_geoDepthMap.Render( settings.bMeshAsLines ? 1 : 0 );
				break;
			case 2:
				//Show everything.
				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 20.0f, -.01f, -100.0f );
				m_geoDepthMap.Render( settings.bMeshAsLines ? 1 : 0 );
				break;
			case 3:
			case 4:
				//Crazy mix

				if ( settings.iGeoMode == 4 )
				{
					m_vCurrentColor = Vector4( 0.0, 1.0, 0.0, settings.fGeoAlpha  );
					UseShader( m_shdWorld, nEye );
				}

				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 20.0f, -.01f, -100.0f );
				m_geoDepthMap.Render( 1 );

				m_vCurrentColor = Vector4( 1.0, 1.0, 1.0, settings.fGeoAlpha );
				UseShader( m_shdWorld, nEye );

				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 1.8f, -.3f, -100.0 );
				m_geoDepthMap.Render( settings.bMeshAsLines ? 1 : 0 );
				break;
			case 5:
				//Show minimal things
				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 100.0f, -99.f, -100.0f );
				//glDrawElements( settings.bMeshAsLines ? GL_LINE_STRIP : GL_TRIANGLE_STRIP, m_uiDepthIndexCount, GL_UNSIGNED_INT, 0 );
				glPointSize( 2 );
				m_geoDepthMap.Render( 1 );
				break;
			case 6:
				//Show everything.
				UseShader( m_shdWorld2, nEye );
				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 20.0f, -.01f, -100.0f );
				m_geoDepthMap.Render( settings.bMeshAsLines ? 1 : 0 );
				break;

			case 7:
				//composite.
				UseShader( m_shdWorld3, nEye );
				glUniform3f( m_shdWorld.GetUniformLocation( "tolforconf" ), 20.0f, -.01f, -100.0f );
				m_geoDepthMap.Render( settings.bMeshAsLines ? 1 : 0 );
				break;

			}

			m_vCurrentColor = Vector4( 1., 1., 1., 1. );


			glDisable( GL_BLEND );
		}
	}

	if ( settings.iShowAnts == 8 ) settings.iShowAnts = 0;

	if ( settings.iShowAnts )
	{
		ShaderFile & sf = (settings.iWhichAntShader) ? m_shdPointCloud2 : m_shdPointCloud;
		glEnable( GL_POINT_SPRITE_OES );
		UseShader( sf, nEye );

		if ( settings.fSampleCoverage > 0.05 )
		{
			glEnable( GL_SAMPLE_COVERAGE );
			glSampleCoverageCHEW( settings.fSampleCoverage, GL_TRUE );
		}

		glEnable( GL_BLEND );
		if ( ( settings.iShowAnts % 4 ) == 1 )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glUniform4f( sf.GetUniformLocation( "particleprops" ), 0.01f, 1.0f, 0.001f, 0.0 );
		}
		else if ( ( settings.iShowAnts % 4 ) == 2 )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glDepthMask( false );
			glUniform4f( sf.GetUniformLocation( "particleprops" ), 0.005f, 0.3f, 0.001f, 0.0 );
		}
		else if ( ( settings.iShowAnts % 4 ) == 3 )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glUniform4f( sf.GetUniformLocation( "particleprops" ), 0.02f, 1.0f, 0.0f, 1.0 );
		}
		glEnable( GL_PROGRAM_POINT_SIZE );

		//	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		//	glEnable( GL_BLEND );
		//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glUniform1f( sf.GetUniformLocation("max_frameno"), (float)m_maxframeno );
		glUniform1f( sf.GetUniformLocation("frameno"), (float)(m_frameno % m_maxframeno) );

		glUniform4f( sf.GetUniformLocation( "antprops" ), settings.fAntSize, (float)settings.iShowAnts, settings.fAntMaxDist, 0.0 );

		m_geoPointCloud.Render();

		glDepthMask( true );
		glDisable( GL_SAMPLE_COVERAGE );
	}
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	if ( m_bSettingsChanged )
	{
		settings.Print();
		m_bSettingsChanged = false;
	}

}

void CameraApp::CompanionRender()
{
	m_parent->m_shdCompanionWindowProgram.Use();
	glViewport( m_parent->m_nCompanionWindowWidth / 2, 0, m_parent->m_nCompanionWindowWidth / 2, m_parent->m_nCompanionWindowHeight );
	// render left eye (first half of index array )
	glBindTexture( GL_TEXTURE_2D, m_nDebugTexture );
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_iDebugTextureW, m_iDebugTextureH, GL_RGBA, GL_UNSIGNED_BYTE, m_pDebugTextureData );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	m_parent->m_geoCompanion.Render();
}

void CameraApp::settings_t::Print()
{
	dprintf( 0, "CameraApp::settings_t savesettings;\n"
		"savesettings.iShowAnts = %d\n"
		"savesettings.iGeoMode = %d\n"
		"savesettings.fAntSize = %.3f\n"
		"savesettings.iAntEvery = %d\n"
		"savesettings.bMeshAsLines = %d\n"
		"savesettings.iFloorType = %d\n"
		"savesettings.bGreenMode = %d\n"
		"savesettings.fGeoAlpha = %.3f\n"
		"savesettings.iStereoAlg = %d\n"
		"savesettings.fAntMaxDist = %.3f\n"
		"savesettings.fImportanceOfDist = %.3f\n"
		"savesettings.iWhichAntShader = %d\n"
		"savesettings.fSampleCoverage = %.3f\n\n"
		,
		iShowAnts,
		iGeoMode,
		fAntSize,
		iAntEvery,
		bMeshAsLines,
		iFloorType,
		bGreenMode,
		fGeoAlpha,
		iStereoAlg,
		fAntMaxDist,
		fImportanceOfDist,
		iWhichAntShader,
		fSampleCoverage );
}

