#pragma once


#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/core/affine.hpp"
#include "opencv2/calib3d.hpp"
#include "shared/Matrices.h"
#include <thread>
#include <openvr.h>

class CameraApp;

class OpenCVProcess
{
public:
	OpenCVProcess( CameraApp * parent );
	~OpenCVProcess();
	bool OpenCVAppStart();
	void OpenCVAppUpdate();
	void Thread();
	void Prerender();
	void TakeScreenshot();

	void ConvertToGray( cv::InputArray src, cv::OutputArray dst );
	void BlurDepths();
	Vector4 TransformToWorldSpace( float x, float y, int disp );
	Vector4 TransformToLocalSpace( float x, float y, int disp );

	vr::TrackedCameraHandle_t m_pCamera;

	struct { float fx, cx, fy, cy; } m_cameraIntrinsics[2];
	vr::HmdMatrix34_t  m_headFromCamera[2];
	Vector4 m_centerFromLeftEye;
	cv::Mat m_leftMap1, m_leftMap2, m_rightMap1, m_rightMap2;
	Matrix4 m_R1, m_R1inv, m_Q, m_Qinv;
	cv::Ptr< cv::StereoSGBM > m_stereo;

	CameraApp * m_parent;

	uint8_t * m_pFrameBuffer;
	uint8_t * m_pFBSides[2];
	uint32_t * m_pFBSidesColor[2];
	uint16_t * m_pDisparity;
	uint32_t * m_pColorOut;
	uint32_t * m_pColorOut2;
	uint32_t  m_iFBSideWidth;
	uint32_t  m_iFBSideHeight;
	std::vector< float > m_valids;
	std::vector< float > m_depths;

	uint32_t  m_iFBAlgoWidth;
	uint32_t  m_iFBAlgoWidthExp;
	uint32_t  m_iFBAlgoHeight;
	uint32_t  m_iProcFrames;
	uint32_t  m_iFramesSinceFPS, m_iFPS;
	double    m_dTimeOfLastFPS;

	uint32_t  m_iFrameBufferLength;

	int m_iHasFrameForUpdate;
	int m_iDoneFrameOutput;

	vr::CameraVideoStreamFrameHeader_t m_lastFrameHeader;
	Matrix4 m_lastFrameHeaderMatrix;
	float m_CameraDistanceMeters;
	cv::Mat m_cvQ;
	std::thread * m_pthread;
	bool m_bQuitThread;
	float fNAN;

	bool m_bScreenshotNext;
	int m_iCurrentStereoAlgorithm;
	unsigned int m_iPBOids[2];
	unsigned int m_iGLfrback;
	unsigned int m_iGLimback;

	//Matrices used in the stereo computation.
	cv::Mat origStereoPair;
	cv::Mat origLeft;
	cv::Mat origRight;
	cv::Mat rectLeft;
	cv::Mat rectRight;
	cv::Mat resizedLeftGray;
	cv::Mat resizedRightGray;
	cv::Mat resizedLeft;
	cv::Mat resizedRight;
	cv::Mat mdisparity;
	cv::Mat mdisparity_expanded;
};