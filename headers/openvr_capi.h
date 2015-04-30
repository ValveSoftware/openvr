//====== Copyright (c) 1996-2014, Valve Corporation, All rights reserved. =======
//
// Purpose: Header for flatted SteamAPI. Use this for binding to other languages.
// This file is auto-generated, do not edit it.
//
//=============================================================================

#ifndef __OPENVR_API_FLAT_H__
#define __OPENVR_API_FLAT_H__
#ifdef _WIN32
#pragma once
#endif

// OPENVR API export macro
#if defined( _WIN32 ) && !defined( _X360 )
	#if defined( OPENVR_API_EXPORTS )
	#define S_API extern "C" __declspec( dllexport ) 
	#elif defined( OPENVR_API_NODLL )
	#define S_API extern "C"
	#else
	#define S_API extern "C" __declspec( dllimport ) 
	#endif // OPENVR_API_EXPORTS
#elif defined( GNUC )
	#if defined( OPENVR_API_EXPORTS )
	#define S_API extern "C" __attribute__ ((visibility("default"))) 
	#else
	#define S_API extern "C" 
	#endif // OPENVR_API_EXPORTS
#else // !WIN32
	#if defined( OPENVR_API_EXPORTS )
	#define S_API extern "C"  
	#else
	#define S_API extern "C" 
	#endif // OPENVR_API_EXPORTS
#endif


#include <stdint.h>
#include "openvr.h"
#include "ivrsystem.h"
#include "ivrcameraaccess.h"
#include "ivrchaperone.h"
#include "ivrchaperonesetup.h"
#include "ivrcompositor.h"


S_API void VR_IVRSystem_GetWindowBounds(intptr_t instancePtr, int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void VR_IVRSystem_GetRecommendedRenderTargetSize(intptr_t instancePtr, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void VR_IVRSystem_GetEyeOutputViewport(intptr_t instancePtr, vr::Hmd_Eye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API struct vr::HmdMatrix44_t VR_IVRSystem_GetProjectionMatrix(intptr_t instancePtr, vr::Hmd_Eye eEye, float fNearZ, float fFarZ, vr::GraphicsAPIConvention eProjType);
S_API void VR_IVRSystem_GetProjectionRaw(intptr_t instancePtr, vr::Hmd_Eye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom);
S_API struct vr::DistortionCoordinates_t VR_IVRSystem_ComputeDistortion(intptr_t instancePtr, vr::Hmd_Eye eEye, float fU, float fV);
S_API struct vr::HmdMatrix34_t VR_IVRSystem_GetEyeToHeadTransform(intptr_t instancePtr, vr::Hmd_Eye eEye);
S_API bool VR_IVRSystem_GetTimeSinceLastVsync(intptr_t instancePtr, float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter);
S_API int32_t VR_IVRSystem_GetD3D9AdapterIndex(intptr_t instancePtr);
S_API void VR_IVRSystem_GetDXGIOutputInfo(intptr_t instancePtr, int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex);
S_API bool VR_IVRSystem_AttachToWindow(intptr_t instancePtr, void * hWnd);
S_API void VR_IVRSystem_GetDeviceToAbsoluteTrackingPose(intptr_t instancePtr, vr::TrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct vr::TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount);
S_API void VR_IVRSystem_ResetSeatedZeroPose(intptr_t instancePtr);
S_API struct vr::HmdMatrix34_t VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(intptr_t instancePtr);
S_API bool VR_IVRSystem_LoadRenderModel(intptr_t instancePtr, const char * pchRenderModelName, struct vr::RenderModel_t * pRenderModel);
S_API void VR_IVRSystem_FreeRenderModel(intptr_t instancePtr, struct vr::RenderModel_t * pRenderModel);
S_API vr::TrackedDeviceClass VR_IVRSystem_GetTrackedDeviceClass(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex);
S_API bool VR_IVRSystem_IsTrackedDeviceConnected(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex);
S_API bool VR_IVRSystem_GetBoolTrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * pError);
S_API float VR_IVRSystem_GetFloatTrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * pError);
S_API int32_t VR_IVRSystem_GetInt32TrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * pError);
S_API uint64_t VR_IVRSystem_GetUint64TrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * pError);
S_API struct vr::HmdMatrix34_t VR_IVRSystem_GetMatrix34TrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * pError);
S_API uint32_t VR_IVRSystem_GetStringTrackedDeviceProperty(intptr_t instancePtr, vr::TrackedDeviceIndex_t unDeviceIndex, vr::TrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::TrackedPropertyError * pError);
S_API const char * VR_IVRSystem_GetPropErrorNameFromEnum(intptr_t instancePtr, vr::TrackedPropertyError error);
S_API bool VR_IVRSystem_PollNextEvent(intptr_t instancePtr, struct vr::VREvent_t * pEvent);
S_API bool VR_IVRSystem_PollNextEventWithPose(intptr_t instancePtr, vr::TrackingUniverseOrigin eOrigin, vr::VREvent_t * pEvent, vr::TrackedDevicePose_t * pTrackedDevicePose);
S_API const char * VR_IVRSystem_GetEventTypeNameFromEnum(intptr_t instancePtr, vr::EVREventType eType);
S_API struct vr::HiddenAreaMesh_t VR_IVRSystem_GetHiddenAreaMesh(intptr_t instancePtr, vr::Hmd_Eye eEye);
S_API bool VR_IVRSystem_GetControllerState(intptr_t instancePtr, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState);
S_API bool VR_IVRSystem_GetControllerStateWithPose(intptr_t instancePtr, vr::TrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t * pControllerState, struct vr::TrackedDevicePose_t * pTrackedDevicePose);
S_API void VR_IVRSystem_TriggerHapticPulse(intptr_t instancePtr, vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec);
S_API const char * VR_IVRSystem_GetButtonIdNameFromEnum(intptr_t instancePtr, vr::EVRButtonId eButtonId);
S_API const char * VR_IVRSystem_GetControllerAxisTypeNameFromEnum(intptr_t instancePtr, vr::EVRControllerAxisType eAxisType);
S_API bool VR_IVRSystem_HandleControllerOverlayInteractionAsMouse(intptr_t instancePtr, const vr::Compositor_OverlaySettings & overlaySettings, vr::HmdVector2_t vecWindowClientPositionOnScreen, vr::HmdVector2_t vecWindowClientSize, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::EVRControllerEventOutputType eOutputType);
S_API bool VR_IVRSystem_CaptureInputFocus(intptr_t instancePtr);
S_API void VR_IVRSystem_ReleaseInputFocus(intptr_t instancePtr);
S_API bool VR_IVRSystem_IsInputFocusCapturedByAnotherProcess(intptr_t instancePtr);
S_API uint32_t VR_IVRCameraAccess_GetCameraCount(intptr_t instancePtr);
S_API uint32_t VR_IVRCameraAccess_GetCameraId(intptr_t instancePtr, uint32_t unCameraIndex, char * pchBuffer, uint32_t unBufferLen);
S_API bool VR_IVRCameraAccess_EnableCamera(intptr_t instancePtr, uint32_t unCameraIndex, bool bEnabled);
S_API bool VR_IVRCameraAccess_GetCameraInfo(intptr_t instancePtr, uint32_t unCameraIndex, struct vr::CameraInfo_t * pCameraInfo);
S_API bool VR_IVRCameraAccess_GetCameraImage(intptr_t instancePtr, uint32_t unCameraIndex, struct vr::CameraImage_t * pCameraImage);
S_API vr::ChaperoneCalibrationState VR_IVRChaperone_GetCalibrationState(intptr_t instancePtr);
S_API bool VR_IVRChaperone_GetSoftBoundsInfo(intptr_t instancePtr, struct vr::ChaperoneSoftBoundsInfo_t * pInfo);
S_API bool VR_IVRChaperone_GetHardBoundsInfo(intptr_t instancePtr, struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool VR_IVRChaperone_GetSeatedBoundsInfo(intptr_t instancePtr, struct vr::ChaperoneSeatedBoundsInfo_t * pInfo);
S_API bool VR_IVRChaperoneSetup_CommitWorkingCopy(intptr_t instancePtr, const char * pchCalibrationName);
S_API void VR_IVRChaperoneSetup_RevertWorkingCopy(intptr_t instancePtr);
S_API bool VR_IVRChaperoneSetup_GetWorkingSoftBoundsInfo(intptr_t instancePtr, struct vr::ChaperoneSoftBoundsInfo_t * pInfo);
S_API bool VR_IVRChaperoneSetup_GetWorkingHardBoundsInfo(intptr_t instancePtr, struct vr::HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, struct vr::HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose);
S_API bool VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, struct vr::HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose);
S_API void VR_IVRChaperoneSetup_SetWorkingSoftBoundsInfo(intptr_t instancePtr, const struct vr::ChaperoneSoftBoundsInfo_t * pInfo);
S_API void VR_IVRChaperoneSetup_SetWorkingHardBoundsInfo(intptr_t instancePtr, struct vr::HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount);
S_API void VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct vr::HmdMatrix34_t & matSeatedZeroPoseToRawTrackingPose);
S_API void VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct vr::HmdMatrix34_t & matStandingZeroPoseToRawTrackingPose);
S_API uint32_t VR_IVRCompositor_GetLastError(intptr_t instancePtr, char * pchBuffer, uint32_t unBufferSize);
S_API void VR_IVRCompositor_SetVSync(intptr_t instancePtr, bool bVSync);
S_API bool VR_IVRCompositor_GetVSync(intptr_t instancePtr);
S_API void VR_IVRCompositor_SetGamma(intptr_t instancePtr, float fGamma);
S_API float VR_IVRCompositor_GetGamma(intptr_t instancePtr);
S_API void VR_IVRCompositor_SetGraphicsDevice(intptr_t instancePtr, vr::Compositor_DeviceType eType, void * pDevice);
S_API void VR_IVRCompositor_WaitGetPoses(intptr_t instancePtr, struct vr::TrackedDevicePose_t * pPoseArray, uint32_t unPoseArrayCount);
S_API void VR_IVRCompositor_Submit(intptr_t instancePtr, vr::Hmd_Eye eEye, void * pTexture, struct vr::Compositor_TextureBounds * pBounds);
S_API void VR_IVRCompositor_ClearLastSubmittedFrame(intptr_t instancePtr);
S_API void VR_IVRCompositor_GetOverlayDefaults(intptr_t instancePtr, struct vr::Compositor_OverlaySettings * pSettings);
S_API void VR_IVRCompositor_SetOverlay(intptr_t instancePtr, void * pTexture, struct vr::Compositor_OverlaySettings * pSettings);
S_API void VR_IVRCompositor_SetOverlayRaw(intptr_t instancePtr, void * buffer, uint32_t width, uint32_t height, uint32_t depth, struct vr::Compositor_OverlaySettings * pSettings);
S_API void VR_IVRCompositor_SetOverlayFromFile(intptr_t instancePtr, const char * pchFilePath, struct vr::Compositor_OverlaySettings * pSettings);
S_API void VR_IVRCompositor_ClearOverlay(intptr_t instancePtr);
S_API bool VR_IVRCompositor_GetFrameTiming(intptr_t instancePtr, struct vr::Compositor_FrameTiming * pTiming, uint32_t unFramesAgo);
S_API void VR_IVRCompositor_FadeToColor(intptr_t instancePtr, float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground);
S_API void VR_IVRCompositor_FadeGrid(intptr_t instancePtr, float fSeconds, bool bFadeIn);
S_API void VR_IVRCompositor_CompositorBringToFront(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorGoToBack(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorQuit(intptr_t instancePtr);
S_API bool VR_IVRCompositor_IsFullscreen(intptr_t instancePtr);
S_API bool VR_IVRCompositor_ComputeOverlayIntersection(intptr_t instancePtr, const struct vr::Compositor_OverlaySettings * pSettings, float fAspectRatio, vr::TrackingUniverseOrigin eOrigin, vr::HmdVector3_t vSource, vr::HmdVector3_t vDirection, vr::HmdVector2_t * pvecIntersectionUV, vr::HmdVector3_t * pvecIntersectionTrackingSpace);
S_API void VR_IVRCompositor_SetTrackingSpace(intptr_t instancePtr, vr::TrackingUniverseOrigin eOrigin);
S_API vr::TrackingUniverseOrigin VR_IVRCompositor_GetTrackingSpace(intptr_t instancePtr);
#endif // __OPENVR_API_FLAT_H__


