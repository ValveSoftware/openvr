#pragma once

#include <array>
#include "openvr_driver.h"

enum InputHandles {
	kInputHandle_A_click,
	kInputHandle_A_touch,
	kInputHandle_trigger_value,
	kInputHandle_trigger_click,
	kInputHandle_joystick_x,
	kInputHandle_joystick_y,
	kInputHandle_joystick_click,
	kInputHandle_haptic,
	kInputHandle_COUNT
};

class ControllerDevice : public vr::ITrackedDeviceServerDriver {
public:
	ControllerDevice(vr::ETrackedControllerRole role);

	// Inherited via ITrackedDeviceServerDriver
	virtual vr::EVRInitError Activate(uint32_t unObjectId) override;
	virtual void Deactivate() override;
	virtual void EnterStandby() override;
	virtual void* GetComponent(const char* pchComponentNameAndVersion) override;
	virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
	virtual vr::DriverPose_t GetPose() override;

	void RunFrame();
	void HandleEvent(const vr::VREvent_t& vrevent);

private:
	std::array<vr::VRInputComponentHandle_t, kInputHandle_COUNT> input_handles_;

	vr::ETrackedControllerRole role_;
	vr::TrackedDeviceIndex_t device_id_;
};