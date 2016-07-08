//============ Copyright (c) Valve Corporation, All rights reserved. ============

#include <openvr_driver.h>
#include "driverlog.h"

#include <vector>
#include <thread>
#include <chrono>

#if defined( _WINDOWS )
#include <Windows.h>
#endif

using namespace vr;


#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )
#elif defined(GNUC) || defined(COMPILER_GCC)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C" 
#else
#error "Unsupported Platform."
#endif

inline HmdQuaternion_t HmdQuaternion_Init( double w, double x, double y, double z )
{
	HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}

inline void HmdMatrix_SetIdentity( HmdMatrix34_t *pMatrix )
{
	pMatrix->m[0][0] = 1.f;
	pMatrix->m[0][1] = 0.f;
	pMatrix->m[0][2] = 0.f;
	pMatrix->m[0][3] = 0.f;
	pMatrix->m[1][0] = 0.f;
	pMatrix->m[1][1] = 1.f;
	pMatrix->m[1][2] = 0.f;
	pMatrix->m[1][3] = 0.f;
	pMatrix->m[2][0] = 0.f;
	pMatrix->m[2][1] = 0.f;
	pMatrix->m[2][2] = 1.f;
	pMatrix->m[2][3] = 0.f;
}


// keys for use with the settings API
static const char * const k_pch_Sample_Section = "driver_sample";
static const char * const k_pch_Sample_EnableSampleDriver_Bool = "enable";
static const char * const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char * const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char * const k_pch_Sample_WindowX_Int32 = "windowX";
static const char * const k_pch_Sample_WindowY_Int32 = "windowY";
static const char * const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------

class CClientDriver_Sample : public IClientTrackedDeviceProvider
{
public:
	CClientDriver_Sample()
		: m_bEnableNullDriver( false )
		, m_bInit( false )
	{
		m_eDriverMode = ClientDriverMode_Normal;
		m_pWatchdogThread = nullptr;
	}

	virtual EVRInitError Init( vr::EClientDriverMode eDriverMode, vr::IDriverLog *pDriverLog, vr::IClientDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir ) ;
	virtual void Cleanup() ;
	virtual bool BIsHmdPresent( const char *pchUserDriverConfigDir ) ;
	virtual EVRInitError SetDisplayId( const char *pchDisplayId )  { return VRInitError_None; } // Null doesn't care
	virtual HiddenAreaMesh_t GetHiddenAreaMesh( EVREye eEye ) ;
	virtual uint32_t GetMCImage( uint32_t *pImgWidth, uint32_t *pImgHeight, uint32_t *pChannels, void *pDataBuffer, uint32_t unBufferLen )  { return 0; }

	void WatchdogWakeUp();
private:
	vr::IClientDriverHost *m_pClientDriverHost;

	bool m_bEnableNullDriver;
	bool m_bInit;
	vr::EClientDriverMode m_eDriverMode;
	std::thread *m_pWatchdogThread;
};

CClientDriver_Sample g_clientDriverNull;


bool g_bExiting = false;

void WatchdogThreadFunction(  )
{
	while ( !g_bExiting )
	{
#if defined( _WINDOWS )
		// on windows send the event when the Y key is pressed.
		if ( (0x01 & GetAsyncKeyState( 'Y' )) != 0 )
		{
			// Y key was pressed. 
			g_clientDriverNull.WatchdogWakeUp();
		}
		std::this_thread::sleep_for( std::chrono::microseconds( 500 ) );
#else
		// for the other platforms, just send one every five seconds
		std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
		g_clientDriverNull.WatchdogWakeUp();
#endif
	}
}

EVRInitError CClientDriver_Sample::Init( vr::EClientDriverMode eDriverMode, vr::IDriverLog *pDriverLog, vr::IClientDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir )
{
	m_pClientDriverHost = pDriverHost;
	InitDriverLog( pDriverLog );

	m_eDriverMode = eDriverMode;

	if ( !m_bInit )
	{
		if ( m_pClientDriverHost )
		{
			IVRSettings *pSettings = m_pClientDriverHost->GetSettings( vr::IVRSettings_Version );

			if ( !m_bEnableNullDriver && pSettings )
			{
				m_bEnableNullDriver = pSettings->GetBool( k_pch_Sample_Section, k_pch_Sample_EnableSampleDriver_Bool, false );
			}
		}
		m_bInit = true;
	}

	if ( eDriverMode == ClientDriverMode_Watchdog )
	{
		if ( !m_bEnableNullDriver )
		{
			return VRInitError_Init_LowPowerWatchdogNotSupported;
		}

		// Watchdog mode on Windows starts a thread that listens for the 'Y' key on the keyboard to 
		// be pressed. A real driver should wait for a system button event or something else from the 
		// the hardware that signals that the VR system should start up.
		g_bExiting = false;
		m_pWatchdogThread = new std::thread( WatchdogThreadFunction );
		if ( !m_pWatchdogThread )
		{
			DriverLog( "Unable to create watchdog thread\n");
			return VRInitError_Driver_Failed;
		}
	}

	return VRInitError_None;
}


void CClientDriver_Sample::WatchdogWakeUp()
{
	if ( m_pClientDriverHost )
		m_pClientDriverHost->WatchdogWakeUp();
}


void CClientDriver_Sample::Cleanup() 
{
	g_bExiting = true;
	if ( m_pWatchdogThread )
	{
		m_pWatchdogThread->join();
		delete m_pWatchdogThread;
		m_pWatchdogThread = nullptr;
	}

	CleanupDriverLog();
}


bool CClientDriver_Sample::BIsHmdPresent( const char *pchUserDriverConfigDir ) 
{
	// optimistically return true. We'll have a chance to say whether there's actually an HMD later.
	// This should do something quick like look for a USB VID/PID combination that makes it likely
	// there's an HMD attached.
	return true;
}


// ------------------------------------------------------------------------------------------
// Purpose: Return a mesh that contains the hidden area for the current HMD
// ------------------------------------------------------------------------------------------
HiddenAreaMesh_t CClientDriver_Sample::GetHiddenAreaMesh( EVREye eEye )
{
	// Null doesn't do visible area meshes
	vr::HiddenAreaMesh_t mesh;
	mesh.pVertexData = NULL;
	mesh.unTriangleCount = 0;
	return mesh;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CSampleDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
{
public:
	CSampleDeviceDriver( vr::IServerDriverHost *pDriverHost )
		: m_pServerDriverHost( pDriverHost )
		, m_unObjectId( vr::k_unTrackedDeviceIndexInvalid )
	{
		IVRSettings *pSettings = m_pServerDriverHost ? m_pServerDriverHost->GetSettings( vr::IVRSettings_Version ) : NULL;

		if ( pSettings )
		{
			DriverLog( "Using settings values\n" );
			m_flIPD = pSettings->GetFloat( k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float, 0.063f );

			char buf[1024];
			pSettings->GetString( k_pch_Sample_Section, k_pch_Sample_SerialNumber_String, buf, sizeof(buf), "SAMPLE1234" );
			m_sSerialNumber = buf;

			pSettings->GetString( k_pch_Sample_Section, k_pch_Sample_ModelNumber_String, buf, sizeof(buf), "ED209" );
			m_sSerialNumber = buf;

			m_nWindowX = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowX_Int32, 0 );
			m_nWindowY = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowY_Int32, 0 );
			m_nWindowWidth = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowWidth_Int32, 1920 );
			m_nWindowHeight = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_WindowHeight_Int32, 1080 );
			m_nRenderWidth = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderWidth_Int32, 1344 );
			m_nRenderHeight = pSettings->GetInt32( k_pch_Sample_Section, k_pch_Sample_RenderHeight_Int32, 1512 );
			m_flSecondsFromVsyncToPhotons = pSettings->GetFloat( k_pch_Sample_Section, k_pch_Sample_SecondsFromVsyncToPhotons_Float, 0.0 );
			m_flDisplayFrequency = pSettings->GetFloat( k_pch_Sample_Section, k_pch_Sample_DisplayFrequency_Float, 0.0 );
		}
		else
		{

			DriverLog( "Could not get settings API. Using default values\n" );
			m_flIPD = 0.065f;

			m_sSerialNumber = "SAMPLE1234";
			m_sModelNumber = "ED209";

			m_nWindowX = 0;
			m_nWindowY = 0;
			m_nWindowWidth = 1920;
			m_nWindowHeight = 1080;
			m_nRenderWidth = 1344;
			m_nRenderHeight = 1512;
			m_flSecondsFromVsyncToPhotons = 0.0;
			m_flDisplayFrequency = 0.0;
		}

		DriverLog( "driver_null: Serial Number: %s\n", m_sSerialNumber.c_str() );
		DriverLog( "driver_null: Model Number: %s\n", m_sModelNumber.c_str() );
		DriverLog( "driver_null: Window: %d %d %d %d\n", m_nWindowX, m_nWindowY, m_nWindowWidth, m_nWindowHeight );
		DriverLog( "driver_null: Render Target: %d %d\n", m_nRenderWidth, m_nRenderHeight );
		DriverLog( "driver_null: Seconds from Vsync to Photons: %f\n", m_flSecondsFromVsyncToPhotons );
		DriverLog( "driver_null: Display Frequency: %f\n", m_flDisplayFrequency );
		DriverLog( "driver_null: IPD: %f\n", m_flIPD );
	}

	virtual ~CSampleDeviceDriver()
	{
		m_pServerDriverHost = NULL;
	}


	virtual EVRInitError Activate( uint32_t unObjectId ) 
	{
		m_unObjectId = unObjectId;
		return VRInitError_None;
	}

	virtual void Deactivate() 
	{
		m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	}

	void *GetComponent( const char *pchComponentNameAndVersion )
	{
		if ( !_stricmp( pchComponentNameAndVersion, vr::IVRDisplayComponent_Version ) )
		{
			return (vr::IVRDisplayComponent*)this;
		}

		// override this to add a component to a driver
		return NULL;
	}

	virtual void PowerOff() 
	{
	}

	/** debug request from a client */
	virtual void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) 
	{
		if( unResponseBufferSize >= 1 )
			pchResponseBuffer[0] = 0;
	}

	virtual void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		*pnX = m_nWindowX;
		*pnY = m_nWindowY;
		*pnWidth = m_nWindowWidth;
		*pnHeight = m_nWindowHeight;
	}

	virtual bool IsDisplayOnDesktop() 
	{
		return true;
	}

	virtual bool IsDisplayRealDisplay() 
	{
		return false;
	}

	virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		*pnWidth = m_nRenderWidth;
		*pnHeight = m_nRenderHeight;
	}

	virtual void GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) 
	{
		*pnY = 0;
		*pnWidth = m_nWindowWidth / 2;
		*pnHeight = m_nWindowHeight;
	
		if ( eEye == Eye_Left )
		{
			*pnX = 0;
		}
		else
		{
			*pnX = m_nWindowWidth / 2;
		}
	}

	virtual void GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) 
	{
		*pfLeft = -1.0;
		*pfRight = 1.0;
		*pfTop = -1.0;
		*pfBottom = 1.0;	
	}

	virtual DistortionCoordinates_t ComputeDistortion( EVREye eEye, float fU, float fV ) 
	{
		DistortionCoordinates_t coordinates;
		coordinates.rfBlue[0] = fU;
		coordinates.rfBlue[1] = fV;
		coordinates.rfGreen[0] = fU;
		coordinates.rfGreen[1] = fV;
		coordinates.rfRed[0] = fU;
		coordinates.rfRed[1] = fV;
		return coordinates;
	}

	virtual DriverPose_t GetPose() 
	{
		DriverPose_t pose = { 0 };
		pose.poseIsValid = true;
		pose.result = TrackingResult_Running_OK;
		pose.deviceIsConnected = true;

		pose.qWorldFromDriverRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
		pose.qDriverFromHeadRotation = HmdQuaternion_Init( 1, 0, 0, 0 );
		

		return pose;
	}
	
	virtual bool GetBoolTrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError ) 
	{
		*pError = vr::TrackedProp_Success;

		switch ( prop )
		{
		case vr::Prop_IsOnDesktop_Bool:
			// avoid "not fullscreen" warnings from vrmonitor
			return false;
		}

		*pError = vr::TrackedProp_ValueNotProvidedByDevice;
		return false;
	}

	virtual float GetFloatTrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError ) 
	{
		vr::ETrackedPropertyError error = vr::TrackedProp_ValueNotProvidedByDevice;
		float fRetVal = 0;
		switch ( prop )
		{
		case vr::Prop_UserIpdMeters_Float:
			fRetVal = m_flIPD;
			error = TrackedProp_Success;
			break;

		case vr::Prop_UserHeadToEyeDepthMeters_Float:
			error = vr::TrackedProp_Success;
			fRetVal = 0.f;
			break;

		case vr::Prop_DisplayFrequency_Float:
			error = vr::TrackedProp_Success;
			fRetVal = m_flDisplayFrequency;
			break;

		case vr::Prop_SecondsFromVsyncToPhotons_Float:
			error = vr::TrackedProp_Success;
			fRetVal = m_flSecondsFromVsyncToPhotons;
			break;

		}

		if ( pError )
		{
			*pError = error;
		}

		return fRetVal;
	}

	int32_t GetInt32TrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError )
	{
		int32_t nRetVal = 0;
		vr::ETrackedPropertyError error = vr::TrackedProp_UnknownProperty;

		switch ( prop )
		{
		case vr::Prop_DeviceClass_Int32:
			nRetVal = vr::TrackedDeviceClass_HMD;
			error = vr::TrackedProp_Success;
			break;
		}

		if ( pError )
		{
			*pError = error;
		}

		return nRetVal;
	}

	virtual uint64_t GetUint64TrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError ) 
	{
		*pError = vr::TrackedProp_Success;

		switch ( prop )
		{
		case vr::Prop_CurrentUniverseId_Uint64:
			// return a constant that's not 0 (invalid) or 1 (reserved for Oculus)
			return 2;
		}

		*pError = vr::TrackedProp_ValueNotProvidedByDevice;
		return 0;
	}

	vr::HmdMatrix34_t GetMatrix34TrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError ) 
	{
		*pError = vr::TrackedProp_ValueNotProvidedByDevice;
		HmdMatrix34_t matIdentity;
		HmdMatrix_SetIdentity( &matIdentity);
		return matIdentity;
	}

	virtual uint32_t GetStringTrackedDeviceProperty( vr::ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError *pError ) 
	{
		std::string sValue= GetStringTrackedDeviceProperty( prop, pError );
		if ( *pError == vr::TrackedProp_Success )
		{
			if ( sValue.size( ) + 1 > unBufferSize )
			{
				*pError = vr::TrackedProp_BufferTooSmall;
			}
			else
			{
				strcpy_s( pchValue, unBufferSize, sValue.c_str() );
			}
			return (uint32_t)sValue.size( ) + 1;
		}
		return 0;
	}

	std::string GetStringTrackedDeviceProperty( vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError *pError ) 
	{
		*pError = vr::TrackedProp_ValueNotProvidedByDevice;
		std::string sRetVal;

		switch ( prop )
		{
		case vr::Prop_ModelNumber_String:
			sRetVal = m_sModelNumber;
			*pError = vr::TrackedProp_Success;
			break;

		case vr::Prop_SerialNumber_String:
			sRetVal = m_sSerialNumber;
			*pError = vr::TrackedProp_Success;
			break;
		}

		return sRetVal;
	}


	void RunFrame()
	{
		// In a real driver, this should happen from some pose tracking thread.
		// The RunFrame interval is unspecified and can be very irregular if some other
		// driver blocks it for some periodic task.
		if ( m_unObjectId != vr::k_unTrackedDeviceIndexInvalid )
		{
			m_pServerDriverHost->TrackedDevicePoseUpdated( m_unObjectId, GetPose() );
		}
	}

private:
	vr::IServerDriverHost *m_pServerDriverHost;
	uint32_t m_unObjectId;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CServerDriver_Sample: public IServerTrackedDeviceProvider
{
public:
	CServerDriver_Sample()
		: m_pNullHmdLatest( NULL )
		, m_bEnableNullDriver( false )
	{
	}

	virtual EVRInitError Init( IDriverLog *pDriverLog, 	vr::IServerDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir ) ;
	virtual void Cleanup() ;
	virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual uint32_t GetTrackedDeviceCount();
	virtual ITrackedDeviceServerDriver *GetTrackedDeviceDriver( uint32_t unWhich ) ;
	virtual ITrackedDeviceServerDriver* FindTrackedDeviceDriver( const char *pchId ) ;
	virtual void RunFrame() ;
	virtual bool ShouldBlockStandbyMode()  { return false; }
	virtual void EnterStandby()  {}
	virtual void LeaveStandby()  {}

private:
	CSampleDeviceDriver *m_pNullHmdLatest;
	
	bool m_bEnableNullDriver;
};

CServerDriver_Sample g_serverDriverNull;


EVRInitError CServerDriver_Sample::Init( IDriverLog *pDriverLog, vr::IServerDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir ) 
{
	InitDriverLog( pDriverLog );

	IVRSettings *pSettings = pDriverHost ? pDriverHost->GetSettings( vr::IVRSettings_Version ) : NULL;

	m_bEnableNullDriver = pSettings && pSettings->GetBool( k_pch_Sample_Section, k_pch_Sample_EnableSampleDriver_Bool, false );

	if ( !m_bEnableNullDriver )
		return VRInitError_Init_HmdNotFound;

	m_pNullHmdLatest = new CSampleDeviceDriver( pDriverHost);
	return VRInitError_None;
}

void CServerDriver_Sample::Cleanup() 
{
	CleanupDriverLog();
	delete m_pNullHmdLatest;
	m_pNullHmdLatest = NULL;
}


uint32_t CServerDriver_Sample::GetTrackedDeviceCount()
{
	if ( m_bEnableNullDriver )
		return 1;

	return 0;
}


ITrackedDeviceServerDriver *CServerDriver_Sample::GetTrackedDeviceDriver( uint32_t unWhich )
{
	return m_pNullHmdLatest;
}


ITrackedDeviceServerDriver* CServerDriver_Sample::FindTrackedDeviceDriver( const char *pchId )
{
	return m_pNullHmdLatest;
}


void CServerDriver_Sample::RunFrame()
{
	if ( m_pNullHmdLatest )
	{
		m_pNullHmdLatest->RunFrame();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory( const char *pInterfaceName, int *pReturnCode )
{
	if( 0 == strcmp( IServerTrackedDeviceProvider_Version, pInterfaceName ) )
	{
		return &g_serverDriverNull;
	}
	if( 0 == strcmp( IClientTrackedDeviceProvider_Version, pInterfaceName ) )
	{
		return &g_clientDriverNull;
	}

	if( pReturnCode )
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}