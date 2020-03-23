#ifndef _HELLOCAMERA_OPENGL_MAIN_H
#define _HELLOCAMERA_OPENGL_MAIN_H

#include "chew.h"

#if defined( OSX )
#include <Foundation/Foundation.h>
#include <AppKit/AppKit.h>
#include <OpenGL/glu.h>
// Apple's version of glut.h #undef's APIENTRY, redefine it
#define APIENTRY
#else
#include <GL/glu.h>
#endif
#include <stdio.h>
#include <string>
#include <cstdlib>


#include <openvr.h>

#include <shared/lodepng.h>
#include <shared/Matrices.h>
#include <shared/pathtools.h>
#include "shader_file.h"
#include "common_hello.h"
#include "camera_app.h"
#include "vrterminal.h"


#include <SDL.h>
#include <SDL_opengl.h>


//-----------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
class CMainApplication
{
public:
	CMainApplication( int argc, char *argv[] );
	virtual ~CMainApplication();

	bool BInit();
	bool BInitGL();
	bool BInitCompositor();

	void Shutdown();

	void RunMainLoop();
	bool HandleInput();
	void ProcessVREvent( const vr::VREvent_t & event );
	void RenderFrame();

	bool SetupStereoRenderTargets();
	void SetupCompanionWindow();
	void SetupCameras();

	void RenderStereoTargets();
	void RenderCompanionWindow();
	void RenderScene( vr::Hmd_Eye nEye );

	Matrix4 GetHMDMatrixProjectionEye( vr::Hmd_Eye nEye );
	Matrix4 GetHMDMatrixPoseEye( vr::Hmd_Eye nEye );
	Matrix4 GetCurrentViewProjectionMatrix( vr::Hmd_Eye nEye );
	void UpdateHMDMatrixPose();

	GeometryObject *FindOrLoadRenderModel( const char *pchRenderModelName );


	//For the camapp

	Matrix4 m_mat4HMDPose;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4eyePosRight;

	Matrix4 m_mat4ProjectionCenter;
	Matrix4 m_mat4ProjectionLeft;
	Matrix4 m_mat4ProjectionRight;
	vr::IVRSystem *m_pIVRSystem;
	
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;
	ShaderFile m_shdCompanionWindowProgram;

	GeometryObject m_geoCompanion;

	CameraApp m_CameraApp;

	//Controller stuff.
	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		Matrix4 m_rmat4Pose;
		GeometryObject *m_pRenderModel = nullptr;
		std::string m_sRenderModelName;
		bool m_bShowController;
	};
	enum EHand
	{
		Left = 0,
		Right = 1,
	};
	ControllerInfo_t m_rHand[2];

	bool bQuit;
	VRTerminal m_trmProfile;
	VRTerminal m_trmErrors;

private:
	bool m_bDebugOpenGL;
	bool m_bVerbose;
	bool m_bPerf;
	bool m_bVblank;

	std::string m_strDriver;
	std::string m_strDisplay;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

private: // OpenGL bookkeeping
	SDL_Window *m_pCompanionWindow;
	SDL_GLContext m_pContext;

	Vector2 m_vAnalogValue;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	float m_fNearClip;
	float m_fFarClip;

	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	bool CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc );

	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	ShaderFile m_shdRenderModel;
	std::vector< GeometryObject * > m_vecRenderModels;

	vr::VRActionHandle_t m_actionAdvanceDemo = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionAnalongInput = vr::k_ulInvalidActionHandle;

	vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;
	vr::VROverlayHandle_t m_ulOverlayHandle;

};

extern CMainApplication * APP;

#endif
