//============ Copyright (c) Valve Corporation, All rights reserved. ============
#pragma once

#include <array>
#include <string>

#include "openvr_driver.h"
#include <atomic>
#include <thread>

enum MyComponent
{
	MyComponent_system_touch,
	MyComponent_system_click,

	MyComponent_MAX
};

struct MyHMDDisplayDriverConfiguration
{
	int32_t window_x;
	int32_t window_y;

	int32_t window_width;
	int32_t window_height;

	int32_t render_width;
	int32_t render_height;
};

class MyHMDDisplayComponent : public vr::IVRDisplayComponent
{
public:
	explicit MyHMDDisplayComponent( const MyHMDDisplayDriverConfiguration &config );

	// ----- Functions to override vr::IVRDisplayComponent -----
	bool IsDisplayOnDesktop() override;
	bool IsDisplayRealDisplay() override;
	void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) override;
	void GetEyeOutputViewport( vr::EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override;
	void GetProjectionRaw( vr::EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) override;
	vr::DistortionCoordinates_t ComputeDistortion( vr::EVREye eEye, float fU, float fV ) override;
	void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override;

private:
	MyHMDDisplayDriverConfiguration config_;
};

//-----------------------------------------------------------------------------
// Purpose: Represents a single tracked device in the system.
// What this device actually is (controller, hmd) depends on what the
// IServerTrackedDeviceProvider calls to TrackedDeviceAdded and the
// properties within Activate() of the ITrackedDeviceServerDriver class.
//-----------------------------------------------------------------------------
class MyHMDControllerDeviceDriver : public vr::ITrackedDeviceServerDriver
{
public:
	MyHMDControllerDeviceDriver();
	vr::EVRInitError Activate( uint32_t unObjectId ) override;
	void EnterStandby() override;
	void *GetComponent( const char *pchComponentNameAndVersion ) override;
	void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) override;
	vr::DriverPose_t GetPose() override;
	void Deactivate() override;

	// ----- Functions we declare ourselves below -----
	const std::string &MyGetSerialNumber();
	void MyRunFrame();
	void MyProcessEvent( const vr::VREvent_t &vrevent );
	void MyPoseUpdateThread();

private:
	std::unique_ptr< MyHMDDisplayComponent > my_display_component_;

	std::string my_hmd_model_number_;
	std::string my_hmd_serial_number_;

	std::array< vr::VRInputComponentHandle_t, MyComponent_MAX > my_input_handles_{};
	std::atomic< int > frame_number_;
	std::atomic< bool > is_active_;
	std::atomic< uint32_t > device_index_;

	std::thread my_pose_update_thread_;
};
