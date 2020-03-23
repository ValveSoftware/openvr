#ifndef _CAMERA_APP_H
#define _CAMERA_APP_H

#include <openvr.h>
#include "chew.h"

#include <shared/lodepng.h>
#include <shared/Matrices.h>
#include <shared/pathtools.h>
#include "geometry_object.h"

#include "opencv_process.h"
#include "shader_file.h"

class CMainApplication;

#define DEPTHMAPX 240
#define DEPTHMAPY 240

class CameraApp
{
public:
	CameraApp( CMainApplication * parentApp );
	~CameraApp();

	bool BInit();
	void Shutdown();
	void RenderScene( vr::Hmd_Eye );
	void CompanionRender();

	//For accessibility to the OpenCV app.
	GLuint m_iTexture; //background texture
	CMainApplication * m_parent;
	int m_frameno;
	int m_maxframeno;

	void EmitDot( float x, float y, float z, float w, float r, float g, float b, float a );
	void Prerender();
	void KeyDown( int32_t c );

	void AdvanceSettings( int direction );
	void LoadSettings( int demono );

	void UseShader( ShaderFile & shd, vr::Hmd_Eye nEye );

	/* Configurable items */
	struct settings_t
	{
		int iShowAnts = 0;
		int iGeoMode = 1;
		float fAntSize = 1.0;
		int iAntEvery = 5;
		bool bMeshAsLines = false;
		int iFloorType = 0;
		bool bGreenMode = 0;
		float fGeoAlpha = 1.0;
		int iStereoAlg = 0;
		float fAntMaxDist = 100.0;
		float fImportanceOfDist = 0;
		int iWhichAntShader = 0;
		float fSampleCoverage = 0;

		void Print();
	} settings;

	bool m_bSettingsChanged;
	Vector4 m_vCurrentColor;

	GLuint m_nDebugTexture;
	uint32_t * m_pDebugTextureData;
	int m_iDebugTextureW;
	int m_iDebugTextureH;


	GeometryObject m_geoDepthMap;
private:

	ShaderFile m_shdCharlesFloor;
	ShaderFile m_shdDumb;

	GeometryObject m_geoCharlesFloor;
	GeometryObject m_geoJeremyFloor[2];

	//For point cloud.
	ShaderFile m_shdPointCloud;
	ShaderFile m_shdPointCloud2;

	GeometryObject m_geoPointCloud;
	int m_uiPointCloudCount;
	int m_ipxUpdatePlace;
	int m_ipxUpdatePlaceLast;

	//For depth geometry.
	ShaderFile m_shdWorld, m_shdWorld2, m_shdWorld3;

	int m_lastLoadedSettingsID;
	OpenCVProcess  m_opencv_p;
};

#endif