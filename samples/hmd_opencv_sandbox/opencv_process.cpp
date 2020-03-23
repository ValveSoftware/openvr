#include "opencv_process.h"
#include "hmd_opencv_sandbox.h"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "common_hello.h"
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define NUM_DISP 96 //Max disparity.

inline double * CoPTr( const std::initializer_list<double>& d ) { return (double*)d.begin(); }
#define DO_FISHEYE 1

#define DO_PROFILE 1
#if DO_PROFILE
#define PROFILE( x ) { double Now = OGGetAbsoluteTime(); dprintf( 1, "\x1b[2K%27s: %.3fms\n", x, (Now-Start)*1000.0); Start = Now; }
#else
#define PROFILE( x )
#endif


Matrix4 Matrix4FromCVMatrix( cv::Mat matin )
{
	Matrix4 out;
	out.identity();
	for ( int y = 0; y < matin.rows; y++ )
	{
		for ( int x = 0; x < matin.cols; x++ )
		{
			out[x + y * 4] = (float)matin.at<double>( y, x );
		}
	}
	return out;
}

OpenCVProcess::OpenCVProcess( CameraApp * parent ) :
	  m_iFrameBufferLength( 0 )
	, m_pFrameBuffer( 0 )
	, m_pthread( 0 )
	, m_parent( parent )
	, m_iCurrentStereoAlgorithm( -1 )
	, m_bScreenshotNext( 0 )
	, m_iHasFrameForUpdate( 0 )
	, m_iDoneFrameOutput( 0 )
	, m_iProcFrames( 0 )
	, m_iFramesSinceFPS( 0 )
	, m_dTimeOfLastFPS( 0 ) 
	, m_bQuitThread( false )
{
	fNAN = nanf( "" );
}

OpenCVProcess::~OpenCVProcess()
{
	m_bQuitThread = true;
	if ( m_pthread )
	{
		m_pthread->join();
	}

	if ( m_iPBOids )
	{
		glDeleteBuffers( 2, m_iPBOids );
		glDeleteFramebuffers( 1, &m_iGLfrback );
		glDeleteTextures( 1, &m_iGLimback );
	}
}

bool OpenCVProcess::OpenCVAppStart()
{
#define MOGRIFY_X 4
#define MOGRIFY_Y 4
#define IGNORE_EDGE_DATA_PIXELS 4 

#define LAGFRAMES 4
#define DENOISE_PASSES 2

	vr::EVRTrackedCameraError ce = vr::VRTrackedCamera()->AcquireVideoStreamingService( vr::k_unTrackedDeviceIndex_Hmd, &m_pCamera );
	if ( ce )
	{
		dprintf( 0, "Error getting video streaming service. Exiting. Error: %d\n", ce );
		return false;
	}

	vr::ETrackedPropertyError err;
	m_parent->m_parent->m_pIVRSystem->GetArrayTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_CameraToHeadTransforms_Matrix34_Array, vr::k_unHmdMatrix34PropertyTag, (void *)m_headFromCamera, sizeof( vr::HmdMatrix34_t ) * 2, &err );
	if ( err != vr::TrackedProp_Success )
	{
		dprintf( 0, "ERROR:  Could not get camera to head transforms.\n" );
		return false;
	}

	for ( int nEye = vr::Eye_Left; nEye <= vr::Eye_Right; nEye++ )
	{
		vr::HmdVector2_t focalLength;
		vr::HmdVector2_t center;
		//vr::EVRDistortionFunctionType eDistortionType;
		//double fTempDistCoeffs[vr::k_unMaxDistortionFunctionParameters];

		vr::EVRTrackedCameraError vrTrackedCameraError = vr::VRTrackedCamera()->GetCameraIntrinsics( vr::k_unTrackedDeviceIndex_Hmd, nEye, vr::VRTrackedCameraFrameType_Undistorted, &focalLength, &center );
		if ( vrTrackedCameraError != vr::VRTrackedCameraError_None )
			dprintf( 0, "error on GetCameraIntrinsics: %d\n", vrTrackedCameraError );

		uint32_t nUndistortedWidth, nUndistortedHeight;
		vrTrackedCameraError = vr::VRTrackedCamera()->GetCameraFrameSize( vr::k_unTrackedDeviceIndex_Hmd, vr::VRTrackedCameraFrameType_Undistorted, &nUndistortedWidth, &nUndistortedHeight, nullptr );

		dprintf( 0, "undisorted frame size:  %d  %d\n", nUndistortedWidth, nUndistortedHeight );

		// Currently can't get intrinsics (focal length and center) with "Distorted" type.  If I get them with "Undistorted" type,
		// and the undistorted size is different from the distorted size, the center will need to be corrected to account for the
		// difference.  Something like the below...
		//
		//center.v[ 0 ] -= ( ( nUndistortedWidth - m_nCameraFrameWidth ) / 2 );
		//center.v[ 1 ] -= ( ( nUndistortedHeight - m_nCameraFrameHeight ) / 2 );
		//

		m_cameraIntrinsics[nEye].fx = focalLength.v[0];	//414, 416
		m_cameraIntrinsics[nEye].cx = center.v[0]; // center.v[0];		//479, 486
		m_cameraIntrinsics[nEye].fy = focalLength.v[1]; //414, 416
		m_cameraIntrinsics[nEye].cy = center.v[1]; // center.v[1];		//502, 500
	}

	//Create Rectification Maps

	double fx, cx, fy, cy;
	double tmp[3][3] = { 0 };
	tmp[2][2] = 1.0;

	fx = m_cameraIntrinsics[0].fx;
	cx = m_cameraIntrinsics[0].cx;
	fy = m_cameraIntrinsics[0].fy;
	cy = m_cameraIntrinsics[0].cy;

	tmp[0][0] = fx;
	tmp[0][2] = cx;
	tmp[1][1] = fy;
	tmp[1][2] = cy;

	//Get coefficients... This is problematic.  So, we use "undistorted" imagery from the camera.
	double distortion_coefficients[vr::k_unMaxDistortionFunctionParameters * 2] = { 0 };
	//double distortion_coefficients_from_steamvr[vr::k_unMaxDistortionFunctionParameters * 2];
	if ( DO_FISHEYE )
	{
		m_parent->m_parent->m_pIVRSystem->GetArrayTrackedDeviceProperty( vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_CameraDistortionCoefficients_Float_Array, vr::k_unFloatPropertyTag,
			(void*)distortion_coefficients, vr::k_unMaxDistortionFunctionParameters * 2 * sizeof( double ), 0 );
	}


	cv::Mat K1 = cv::Mat( cv::Size( 3, 3 ), CV_64F, &(tmp[0][0]) ).clone();
	cv::Mat D1( DO_FISHEYE ? 4 : 8, 1, CV_64F, &distortion_coefficients[vr::k_unMaxDistortionFunctionParameters * 0] );

	fx = m_cameraIntrinsics[1].fx;
	cx = m_cameraIntrinsics[1].cx;
	fy = m_cameraIntrinsics[1].fy;
	cy = m_cameraIntrinsics[1].cy;
	/// ROW COL
	tmp[0][0] = fx;
	tmp[0][2] = cx;
	tmp[1][1] = fy;
	tmp[1][2] = cy;

	cv::Mat K2 = cv::Mat( cv::Size( 3, 3 ), CV_64F, &(tmp[0][0]) ).clone();
	cv::Mat D2( DO_FISHEYE ? 4 : 8, 1, CV_64F, &distortion_coefficients[vr::k_unMaxDistortionFunctionParameters * 1] );


	Matrix4 headFromLeftCamera_steamvr = ConvertSteamVRMatrixToMatrix4( m_headFromCamera[0] );
	Matrix4 headFromRightCamera_steamvr = ConvertSteamVRMatrixToMatrix4( m_headFromCamera[1] );
	Matrix4 RightCamerafromHead_steamvr = headFromRightCamera_steamvr.invert();
	Matrix4 rotate180AroundX;
	rotate180AroundX.identity();
	rotate180AroundX.rotateX( 180 );
	Matrix4 rightCameraFromLeftCamera_steamvr = RightCamerafromHead_steamvr * headFromLeftCamera_steamvr;
	Matrix4	rightCameraFromLeftCamera_opencv = rotate180AroundX * rightCameraFromLeftCamera_steamvr * rotate180AroundX;

	Vector4 RightEyeFromLeftEye = (rightCameraFromLeftCamera_steamvr * Vector4( 0, 0, 0, 1 ));
	m_centerFromLeftEye = RightEyeFromLeftEye / 2;
	m_CameraDistanceMeters = ( RightEyeFromLeftEye * Vector4( 1, 1, 1, 0 ) ).length();
	m_centerFromLeftEye.w = 0;

	double posetrans[3] = { rightCameraFromLeftCamera_opencv[12], rightCameraFromLeftCamera_opencv[13], rightCameraFromLeftCamera_opencv[14] };

	rightCameraFromLeftCamera_opencv.transpose();
	double posemat[9] = {
		rightCameraFromLeftCamera_opencv[0], rightCameraFromLeftCamera_opencv[1], rightCameraFromLeftCamera_opencv[2],
		rightCameraFromLeftCamera_opencv[4], rightCameraFromLeftCamera_opencv[5], rightCameraFromLeftCamera_opencv[6],
		rightCameraFromLeftCamera_opencv[8], rightCameraFromLeftCamera_opencv[9], rightCameraFromLeftCamera_opencv[10] };

	cv::Mat R( cv::Size( 3, 3 ), CV_64F, posemat /* Boy I hope the major is right */ );
	cv::Mat T( 3, 1, CV_64F, posetrans );
	cv::Mat R1, R2, P1, P2;

	if ( DO_FISHEYE )
	{
		cv::fisheye::stereoRectify( K1, D1, K2, D2, cv::Size( 960, 960 ), R, T, R1, R2, P1, P2, m_cvQ, cv::CALIB_ZERO_DISPARITY, cv::Size( 960, 960 ), 0.0, 0.7 );
		cv::fisheye::initUndistortRectifyMap( K1, D1, R1, P1, cv::Size( 960, 960 ), CV_16SC2, m_leftMap1, m_leftMap2 );
		cv::fisheye::initUndistortRectifyMap( K2, D2, R2, P2, cv::Size( 960, 960 ), CV_16SC2, m_rightMap1, m_rightMap2 );
	}
	else
	{
		cv::stereoRectify( K1, D1, K2, D2, cv::Size( 960, 960 ), R, T, R1, R2, P1, P2, m_cvQ, cv::CALIB_ZERO_DISPARITY );
		cv::initUndistortRectifyMap( K1, D1, R1, P1, cv::Size( 960, 960 ), CV_16SC2, m_leftMap1, m_leftMap2 );
		cv::initUndistortRectifyMap( K2, D2, R2, P2, cv::Size( 960, 960 ), CV_16SC2, m_rightMap1, m_rightMap2 );
	}

	m_R1 = Matrix4FromCVMatrix( R1 );
	m_R1inv = m_R1;
	m_R1inv = m_R1inv.invert();
	m_Q = Matrix4FromCVMatrix( m_cvQ );


	uint32_t width, height;
	vr::VRTextureBounds_t vtb;
	dprintf( 0, "Get Cam: %lld\n", m_pCamera );
	ce = vr::VRTrackedCamera()->GetVideoStreamTextureSize( vr::k_unTrackedDeviceIndex_Hmd, DO_FISHEYE ? vr::VRTrackedCameraFrameType_Distorted : vr::VRTrackedCameraFrameType_Undistorted, &vtb, &width, &height );
	if ( ce )
	{
		dprintf( 0, "Error getting frame size (%d)\n", ce );
		return false;
	}
	m_iFrameBufferLength = width * height * 4;
	m_iFBSideWidth = width / 2;
	m_iFBSideHeight = height;

	m_iFBAlgoWidth = m_iFBSideWidth / MOGRIFY_X;
	m_iFBAlgoHeight = m_iFBSideHeight / MOGRIFY_Y;

	m_pDisparity = (uint16_t*)malloc( m_iFBAlgoWidth * m_iFBAlgoHeight * 2 );

	for ( int side = 0; side < 2; side++ )
	{
		m_pFBSides[side] = (uint8_t*)malloc( (m_iFBAlgoWidth+ NUM_DISP)* m_iFBAlgoHeight * 1 );
		m_pFBSidesColor[side] = (uint32_t*)malloc( m_iFBAlgoWidth * m_iFBAlgoHeight * 4 );
	}

	m_pColorOut = (uint32_t*) calloc( m_iFBAlgoWidth * m_iFBAlgoHeight, sizeof( uint32_t ) );
	m_pColorOut2 = (uint32_t*) calloc( m_iFBSideWidth * m_iFBSideHeight, sizeof( uint32_t ) );

	glBindTexture( GL_TEXTURE_2D, m_parent->m_iTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );  //Always set the base and max mipmap levels of a texture.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_iFBSideWidth, m_iFBSideHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pColorOut2 );
	glBindTexture( GL_TEXTURE_2D, 0 );


	m_valids.resize( m_iFBAlgoHeight  * m_iFBAlgoWidth );
	m_valids.assign( m_valids.size(), 1 );
	m_depths.resize( m_iFBAlgoHeight  * m_iFBAlgoWidth );


	m_pthread = new std::thread( &OpenCVProcess::Thread, this );


	//Readback Buffers

	//Create PBOs to allow streaming of data.
	glGenBuffers( 2, m_iPBOids );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, m_iPBOids[0] );
	glBufferData( GL_PIXEL_PACK_BUFFER, 960 * 960 * 2 * 4, 0, GL_STREAM_READ );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, m_iPBOids[1] );
	glBufferData( GL_PIXEL_PACK_BUFFER, 960 * 960 * 2 * 4, 0, GL_STREAM_READ );
	glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

	//Create target framebuffer and texture to stream camera data into.
	glGenFramebuffers( 1, &m_iGLfrback );
	glGenTextures( 1, &m_iGLimback );
	glBindTexture( GL_TEXTURE_2D, m_iGLimback );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_iFBSideWidth, m_iFBSideHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );

	//Set up what matrices we can to prevent dynamic memory allocation.
	mdisparity_expanded = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth + NUM_DISP, CV_16S );

	return true;
}

void OpenCVProcess::Thread()
{
	while ( !m_bQuitThread )
	{
		if ( m_iHasFrameForUpdate == 2 )
		{
			if( m_pFrameBuffer )
				OpenCVAppUpdate();
			m_iHasFrameForUpdate = 0;
		}
		OGUSleep( 1000 );
	}
}

void OpenCVProcess::Prerender()
{
	if ( m_iDoneFrameOutput )
	{
		m_iProcFrames++;
		m_iFramesSinceFPS++;

		double Start = OGGetAbsoluteTime();
		if ( Start >= m_dTimeOfLastFPS + 1 )
		{
			if ( Start - m_dTimeOfLastFPS < 4 )
				m_dTimeOfLastFPS++;
			else
				m_dTimeOfLastFPS = Start;
			m_iFPS = m_iFramesSinceFPS;
			m_iFramesSinceFPS = 0;			
		}

		glBindTexture( GL_TEXTURE_2D, m_parent->m_iTexture );
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_iFBSideWidth, m_iFBSideHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pColorOut2 );	//If you want to debug m_pColorOut, you can select that here.
		glBindTexture( GL_TEXTURE_2D, 0 );
		PROFILE( "[GL] Updating output texture" )
		m_parent->m_geoDepthMap.TaintVerts( 0 );
		m_parent->m_geoDepthMap.Check();
		PROFILE( "[GL] Updating output verts" )
		m_iDoneFrameOutput = 0;
	}

	if ( m_iHasFrameForUpdate == 1 )
	{
		double Start = OGGetAbsoluteTime();
		glBindBuffer( GL_PIXEL_PACK_BUFFER, m_iPBOids[0] );
		m_pFrameBuffer = (GLubyte*)glMapBuffer( GL_PIXEL_PACK_BUFFER, GL_READ_ONLY );
		glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
		m_iHasFrameForUpdate = 2;
		PROFILE( "[GL] Readback" )
	}

	if ( m_iHasFrameForUpdate == 0 )
	{
		double Start = OGGetAbsoluteTime();

#if DO_PROFILE
		dprintf( 1, "\x1b[1;1f" );
		dprintf( 1, "\x1b[2K\x1b[34mFrames: %5d; %3d FPS\x1b[0m\n", m_iProcFrames, m_iFPS );
		dprintf( 1, "\x1b[32mGreen FG Test\x1b[0m\n" );
		dprintf( 1, "\x1b[31mRed FG Test\x1b[0m\n" );
		dprintf( 1, "\x1b[0m" );
#endif

		//This uses OpenGL to read back the pixels.  It seems to be MUCH faster than the DX alternative inside SteamVR.
		vr::EVRTrackedCameraError ce = vr::VRTrackedCamera()->GetVideoStreamTextureGL( m_pCamera, DO_FISHEYE ? vr::VRTrackedCameraFrameType_Distorted : vr::VRTrackedCameraFrameType_Undistorted, &m_iGLimback, &m_lastFrameHeader, sizeof( m_lastFrameHeader ) );
		m_lastFrameHeaderMatrix = ConvertSteamVRMatrixToMatrix4( m_lastFrameHeader.trackedDevicePose.mDeviceToAbsoluteTracking );

		PROFILE( "[GL] GetVideoStreamTexture" )
		glFinish();
		PROFILE( "[GL] Flush" )

		glBindFramebuffer( GL_FRAMEBUFFER, m_iGLfrback );
		glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_iGLimback, 0 );
		glBindBuffer( GL_PIXEL_PACK_BUFFER, m_iPBOids[0] );
		if ( m_pFrameBuffer ) glUnmapBuffer( GL_PIXEL_PACK_BUFFER );
		glReadPixels( 0, 0, m_lastFrameHeader.nWidth, m_lastFrameHeader.nHeight, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
		glBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		PROFILE( "[GL] PBO read is setup." )

		if ( ce )
		{
			dprintf( 0, "Error getting frame (%d)\n", ce );
		}
		else
		{
			m_iHasFrameForUpdate = 1;
		}
	}


}

void OpenCVProcess::ConvertToGray( cv::InputArray src, cv::OutputArray dst )
{
	//You can do this the OpenCV way, but my alternative transform seems more successful.
	if ( 0 )
	{
		cv::cvtColor( src, dst, cv::COLOR_BGR2GRAY );
	}
	else
	{
		cv::Mat msrc( src.getMat() );
		cv::Mat mdst( dst.getMat() );
		int wd = mdst.cols;
		int w = msrc.cols;
		int h = msrc.rows;
		uint32_t * indata = (uint32_t*)msrc.data;
		uint8_t * outdata;
		int x, y;
		for ( y = 0; y < h; y++ )
		{
			outdata = ((uint8_t*)mdst.data) + y * wd + NUM_DISP;
			for ( x = 0; x < w; x++ )
			{
				uint32_t inx = *(indata++);
				int r = (inx >> 0) & 0xff;
				int g = (inx >> 8) & 0xff;
				int b = (inx >> 16) & 0xff;
				//*(outdata++) = inx >> (chan * 8);
				*(outdata++) = (uint8_t)((r + g + b) / 3);
			}
		}
	}
}

Vector4 OpenCVProcess::TransformToLocalSpace( float x, float y, int disp )
{
	float fDisp = ( float ) disp / 16.f; //  16-bit fixed-point disparity map (where each disparity value has 4 fractional bits)
	float lz = m_Q[11] * m_CameraDistanceMeters / ( fDisp * MOGRIFY_X );
	float ly = -(y * MOGRIFY_Y + m_Q[7]) / m_Q[11];
	float lx = (x * MOGRIFY_X + m_Q[3]) / m_Q[11];
	lx *= lz;
	ly *= lz;
	lz *= -1;
	return m_R1inv * Vector4( lx, ly, lz, 1.0 );
}

Vector4 OpenCVProcess::TransformToWorldSpace( float x, float y, int disp )
{
	Vector4 local = TransformToLocalSpace( x, y, disp );

	Matrix4 mlefteye = m_lastFrameHeaderMatrix;
	Vector4 placerelview( local );
	Vector4 Worldview = mlefteye * (placerelview);
	return Worldview;
}


void OpenCVProcess::BlurDepths()
{
	//This does an actual blurring function. 
#define RIGHT_EDGE_NO_TRUST 24

	std::vector< float > validsback( m_iFBAlgoHeight  * m_iFBAlgoWidth );
	std::vector< float > depthsback( m_iFBAlgoHeight  * m_iFBAlgoWidth );

	//Initialize the data for this frame
	for ( unsigned y = 0; y < m_iFBAlgoWidth; y++ )
	{
		for ( unsigned x = 0; x < m_iFBAlgoHeight; x++ )
		{
			int idx = y * m_iFBAlgoWidth + x;
			uint16_t pxi = m_pDisparity[idx];
			if ( pxi == 0 || pxi >= m_iFBAlgoWidth * 16 )
			{
				//If we don't know the depth, then we just discard it.  We could handle that here.  Additionally,
				//if we wanted, we could emit fake dots where we believe the floor to be.
				//Right now, we do nothing.
			}
			else
			{
				m_valids[idx] = 1.0;
				m_depths[idx] = pxi;
			}

			//Never trust the right side of the screen.
			if ( x >= m_iFBAlgoWidth - RIGHT_EDGE_NO_TRUST )
			{
				m_valids[idx] = 0;
				m_depths[idx] = 0;
			}
		}
	}
	for ( int iter = 0; iter < 10; iter++ )
	{
		for ( unsigned y = 4; y < m_iFBAlgoWidth - 4; y++ )
		{
			for ( unsigned x = 4; x < m_iFBAlgoHeight - 4; x++ )
			{
				int idx = y * m_iFBAlgoWidth + x;
				float tval = 0;
				float tdepths = 0;
				for ( int ly = -1; ly <= 1; ly++ )
					for ( int lx = -1; lx <= 1; lx++ )
					{
						int idxx = (y+ly) * m_iFBAlgoWidth + x+lx;
						tval += m_valids[idxx];
						tdepths += m_depths[idxx];
					}
				validsback[idx] = tval / 9;
				depthsback[idx] = tdepths / 9;
			}
		}
		memcpy( &m_valids[0], &validsback[0], sizeof( float ) * validsback.size() );
		memcpy( &m_depths[0], &depthsback[0], sizeof( float ) * depthsback.size() );

	}

	for ( unsigned y = 0; y < m_iFBAlgoHeight; y++ )
	{
		for ( unsigned x = 0; x < m_iFBAlgoWidth; x++ )
		{
			int idx = y * m_iFBAlgoWidth + x;
			if ( x < 1 || x >= m_iFBAlgoWidth - 1 )
			{
				//Must throw out edges.
				m_pDisparity[idx] = 0xfff0;
				continue;
			}
			uint16_t pxi = m_pDisparity[idx];
			if ( pxi == 0 || pxi >= m_iFBAlgoWidth * 16 )
			{
				if ( m_valids[idx] < .00005 )
				{
					m_valids[idx] = 0;
					m_depths[idx] = 0;
					m_pDisparity[idx] = 0xfff0;
				}
				else
				{
					m_pDisparity[idx] = (uint16_t)(m_depths[idx] / m_valids[idx]);
				}
			}
			m_valids[idx] *= .9f;
			m_depths[idx] *= .9f;
		}
	}
}

void OpenCVProcess::OpenCVAppUpdate()
{
	double Start = OGGetAbsoluteTime();

	if ( m_iCurrentStereoAlgorithm != m_parent->settings.iStereoAlg )
	{
		if ( m_parent->settings.iStereoAlg >= 3 ) m_parent->settings.iStereoAlg = 0;
		m_iCurrentStereoAlgorithm = m_parent->settings.iStereoAlg;

		if ( m_iCurrentStereoAlgorithm == 0 )
		{
			m_stereo = cv::StereoSGBM::create( 0, NUM_DISP, 7,
				0, 0, 0,
				4, 55,
				25, 4,
				cv::StereoSGBM::MODE_SGBM );
		}
		else if ( m_iCurrentStereoAlgorithm == 1 )
		{
			m_stereo = cv::StereoSGBM::create( 0, NUM_DISP, 2,
				0, 0, 0,
				4, 35,
				10, 3,
				cv::StereoSGBM::MODE_SGBM );
		}
		else if ( m_iCurrentStereoAlgorithm == 2 )
		{
			m_stereo = cv::StereoSGBM::create( 0, NUM_DISP, 15,
				0, 0, 0,
				4, 5,
				200, 1,
				cv::StereoSGBM::MODE_SGBM );
		}
	}

	origStereoPair = cv::Mat( m_iFBSideHeight, m_iFBSideWidth * 2, CV_8UC4, m_pFrameBuffer );
	origLeft = origStereoPair( cv::Rect( 0, 0, 960, 960 ) );
	origRight = origStereoPair( cv::Rect( 960, 0, 960, 960 ) );

	cv::remap( origLeft, rectLeft, m_leftMap1, m_leftMap2, CV_INTER_LINEAR, cv::BORDER_CONSTANT );
	cv::remap( origRight, rectRight, m_rightMap1, m_rightMap2, CV_INTER_LINEAR, cv::BORDER_CONSTANT );

	resizedLeft = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth, CV_8UC4, m_pFBSidesColor[0] );
	resizedRight = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth, CV_8UC4, m_pFBSidesColor[1] );
	cv::resize( rectLeft, resizedLeft, cv::Size( m_iFBAlgoWidth, m_iFBAlgoHeight ) );
	cv::resize( rectRight, resizedRight, cv::Size( m_iFBAlgoWidth, m_iFBAlgoHeight ) );

	resizedLeftGray = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth + NUM_DISP, CV_8U, m_pFBSides[0] );
	resizedRightGray = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth + NUM_DISP, CV_8U, m_pFBSides[1] );
	mdisparity = cv::Mat( m_iFBAlgoHeight, m_iFBAlgoWidth, CV_16S, m_pDisparity );

	ConvertToGray( resizedLeft, resizedLeftGray );
	ConvertToGray( resizedRight, resizedRightGray );
	PROFILE( "[OP] Setup" )

	{
		m_stereo->compute( resizedLeftGray, resizedRightGray, mdisparity_expanded );
		uint32_t x, y;
		int wd = mdisparity.cols;
		int w = mdisparity_expanded.cols;
		//int h = mdisparity_expanded.rows;

		for ( y = 0; y < m_iFBAlgoWidth; y++ )
		{
			uint16_t * indata = ((uint16_t*)mdisparity_expanded.data) + y * w + NUM_DISP;
			uint16_t * outdata = ((uint16_t*)mdisparity.data) + y * wd;
			for ( x = 0; x < m_iFBAlgoHeight; x++ )
			{
				*(outdata++) = *(indata++);
			}
		}
	}

	if ( m_bScreenshotNext )
	{
		TakeScreenshot();
		m_bScreenshotNext = false;
	}

	PROFILE( "[OP] Stereo Computation")

	static int rframe;
	//For frame decimation
	//rframe++;	if ( rframe == 10 ) rframe = 0;

	if ( rframe == 0 )
	{
		int x, y;
		for ( y = 0; y < (int)m_iFBSideHeight; y++ )
		{
			uint32_t * pdsp = &((uint32_t*)rectLeft.data)[y*m_iFBSideWidth];
			uint32_t * outlines = &m_pColorOut2[y*m_iFBSideWidth];
			for ( x = 0; x < (int)m_iFBSideWidth; x++ )
			{
				outlines[x] = pdsp[x];// ((*(uint32_t*)(&pxdl[x * 4 + 0])) & 0xff) | ((*(uint32_t*)(&pxdr[x * 4 + 0])) & 0xff00);
			}
		}
	}
	PROFILE( "[OP] Outlines update" )
	//Potentially emit dots.
	if ( 1 )
	{
		unsigned x, y;
		for ( y = 0; y < m_iFBAlgoHeight; y++ )
		{
			uint16_t * pxin = &m_pDisparity[y*m_iFBAlgoWidth];
			for ( x = IGNORE_EDGE_DATA_PIXELS; x < m_iFBAlgoWidth - IGNORE_EDGE_DATA_PIXELS; x++ )
			{
				uint32_t pxc = pxin[x];

				//Color
				uint32_t pxo = m_pFBSidesColor[0][(x)+y * m_iFBAlgoWidth];
				int pxr = ((pxo >> 0) & 0xff);
				int pxg = ((pxo >> 8) & 0xff);
				int pxb = ((pxo >> 16) & 0xff);

				if ( pxc < 0xfff0 )
				{
					float frx = x + (rand() % 1000) / 1000.0f;
					float fry = y + (rand() % 1000) / 1000.0f;
					Vector4 Worldspace = TransformToWorldSpace( frx, fry, pxc );

					if ( 1 ) //&& Worldspace.y >= 0 && Worldspace.y < 1.5 )
					{
						//Create debug map (this appears to the left of the window)
						int dx = (int) ( -Worldspace.x * 50.0 + m_parent->m_iDebugTextureW/2 );
						int dy = (int) ( Worldspace.z * 50.0 + m_parent->m_iDebugTextureH/2 );
						if ( dx >= 0 && dy >= 0 && dx < m_parent->m_iDebugTextureW && dy < m_parent->m_iDebugTextureH )
						{
							m_parent->m_pDebugTextureData[dx + dy * m_parent->m_iDebugTextureH] = pxo | 0xff;
						}
					}
					 
					int emitevery = (int)(m_parent->settings.iAntEvery + m_parent->settings.fImportanceOfDist * 200 / pxc );
					if ( emitevery < 1 ) emitevery = 1;
					if ( 0 == (rand() % emitevery) )
						m_parent->EmitDot( Worldspace.x, Worldspace.y, Worldspace.z, 1,
							pxr / 255.0f, pxg / 255.0f, pxb / 255.0f, (float)((m_parent->m_frameno % m_parent->m_maxframeno) + rand()*10.0 / RAND_MAX) );
				}



			}
		}
	}

	PROFILE( "[OP] Emit Dots")
	if ( 1 )
	{
		BlurDepths(  );
	}
	PROFILE( "[OP] Blur" )

	std::vector< float > & depth_vc = m_parent->m_geoDepthMap.GetVertexArrayPtr( 0 );
	if ( rframe == 0 && 1 ) //Process Output
	{
		uint32_t x, y;
		for ( y = 0; y < m_iFBAlgoHeight; y++ )
		{
			uint16_t * pxin = &m_pDisparity[y*m_iFBAlgoWidth];
			uint32_t * pxout = &m_pColorOut[y*m_iFBAlgoWidth];
			for ( x = IGNORE_EDGE_DATA_PIXELS; x < m_iFBAlgoWidth - IGNORE_EDGE_DATA_PIXELS; x++ )
			{
				uint32_t pxo = pxin[x];
				int idx = y * m_iFBAlgoWidth + x;

				if ( pxo >= 0xfff0 )
				{
					pxo = 0x202020;// (x > m_iFBAlgoWidth / 2) ? 0 : 129;
					depth_vc[idx * 4 + 0] = fNAN;
					depth_vc[idx * 4 + 1] = fNAN;
					depth_vc[idx * 4 + 2] = fNAN;
					depth_vc[idx * 4 + 3] = 0;
				}
				else
				{
					//depths[x + y * m_iFBAlgoWidth] = pxin[x];
					if ( 1 )
					{
						//Update depth geometry.
						Vector4 Worldspace = TransformToWorldSpace( (float)x, (float)y, pxin[x] );
						depth_vc[idx * 4 + 0] = Worldspace.x;
						depth_vc[idx * 4 + 1] = Worldspace.y;
						depth_vc[idx * 4 + 2] = Worldspace.z;
						depth_vc[idx * 4 + 3] = m_valids[idx];
					}

					//OPTIONAL: Write the color buffer out.
					if ( 1 )
					{
						pxout[x] = pxin[x];
					}
				}
			}
		}
	}
	m_iDoneFrameOutput = 1;

	PROFILE( "[OP] Process" )


}



void OpenCVProcess::TakeScreenshot( )
{
	struct tm timeinfo;
	time_t rawtime;
	time( &rawtime );
	localtime_s( &timeinfo, &rawtime );
	char timebuffer[128];
	std::strftime( timebuffer, sizeof( timebuffer ), "%Y%m%d %H%M%S", &timeinfo );
	std::string nowstr = timebuffer;

	//Make the alpha channel of the RGB maps solid.
	int sidepix = m_iFBSideWidth * m_iFBSideHeight;
	for ( int i = 0; i < sidepix; i++ )
	{
		((uint32_t*)origStereoPair.data)[i * 2 + 1] |= 0xff000000;
		((uint32_t*)origStereoPair.data)[i * 2 + 0] |= 0xff000000;
		((uint32_t*)rectLeft.data)[i] |= 0xff000000;
		((uint32_t*)rectRight.data)[i] |= 0xff000000;
	}

	stbi_write_png( (nowstr + "_Orig_RGB0.png").c_str(), m_iFBSideWidth, m_iFBSideHeight, 4, origLeft.data, m_iFBSideWidth * 8 );
	stbi_write_png( (nowstr + "_Orig_RGB1.png").c_str(), m_iFBSideWidth, m_iFBSideHeight, 4, origRight.data, m_iFBSideWidth * 8 );
	stbi_write_png( (nowstr + "_RGB0.png").c_str(), m_iFBSideWidth, m_iFBSideHeight, 4, rectLeft.data, m_iFBSideWidth * 4 );
	stbi_write_png( (nowstr + "_RGB1.png").c_str(), m_iFBSideWidth, m_iFBSideHeight, 4, rectRight.data, m_iFBSideWidth * 4 );
	stbi_write_png( (nowstr + "_Gray0.png").c_str(), m_iFBAlgoWidth, m_iFBAlgoHeight, 1, resizedLeftGray.data, m_iFBAlgoWidth );
	stbi_write_png( (nowstr + "_Gray1.png").c_str(), m_iFBAlgoWidth, m_iFBAlgoHeight, 1, resizedRightGray.data, m_iFBAlgoWidth );

	int pxl = m_iFBAlgoWidth * m_iFBAlgoHeight;
	uint8_t * disp_px = new uint8_t[pxl];
	for ( int i = 0; i < pxl; i++ )
	{
		disp_px[i] = (uint8_t)(((uint16_t*)(mdisparity.data))[i] / 16);
	}
	stbi_write_png( (nowstr + "_Disp.png").c_str(), m_iFBAlgoWidth, m_iFBAlgoHeight, 1, disp_px, m_iFBAlgoWidth );
	delete[] disp_px;
}

