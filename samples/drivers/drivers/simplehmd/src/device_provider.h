//============ Copyright (c) Valve Corporation, All rights reserved. ============
#pragma once

#include <memory>

#include "hmd_device_driver.h"
#include "openvr_driver.h"

// make sure your class is publicly inheriting vr::IServerTrackedDeviceProvider!
class MyDeviceProvider : public vr::IServerTrackedDeviceProvider
{
public:
	vr::EVRInitError Init( vr::IVRDriverContext *pDriverContext ) override;
	const char *const *GetInterfaceVersions() override;

	void RunFrame() override;

	bool ShouldBlockStandbyMode() override;
	void EnterStandby() override;
	void LeaveStandby() override;

	void Cleanup() override;

private:
	std::unique_ptr<MyHMDControllerDeviceDriver> my_hmd_device_;
};