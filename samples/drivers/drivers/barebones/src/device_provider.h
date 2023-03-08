//============ Copyright (c) Valve Corporation, All rights reserved. ============
#pragma once
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
};