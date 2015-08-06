//======= Copyright 1996-2014, Valve Corporation, All rights reserved. ========
//
// Purpose: This file contains C#/managed code bindings for the OpenVR interfaces
// This file is auto-generated, do not edit it.
//
//=============================================================================

using System;
using System.Runtime.InteropServices;
using Valve.VR;
using Valve.Interop;

namespace Valve.Interop
{

class VRNativeEntrypoints
{


	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetWindowBounds")]
	internal static extern void VR_IVRSystem_GetWindowBounds(IntPtr instancePtr, ref int pnX, ref int pnY, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetRecommendedRenderTargetSize")]
	internal static extern void VR_IVRSystem_GetRecommendedRenderTargetSize(IntPtr instancePtr, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetEyeOutputViewport")]
	internal static extern void VR_IVRSystem_GetEyeOutputViewport(IntPtr instancePtr, Hmd_Eye eEye, ref uint pnX, ref uint pnY, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetProjectionMatrix")]
	internal static extern HmdMatrix44_t VR_IVRSystem_GetProjectionMatrix(IntPtr instancePtr, Hmd_Eye eEye, float fNearZ, float fFarZ, GraphicsAPIConvention eProjType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetProjectionRaw")]
	internal static extern void VR_IVRSystem_GetProjectionRaw(IntPtr instancePtr, Hmd_Eye eEye, ref float pfLeft, ref float pfRight, ref float pfTop, ref float pfBottom);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ComputeDistortion")]
	internal static extern DistortionCoordinates_t VR_IVRSystem_ComputeDistortion(IntPtr instancePtr, Hmd_Eye eEye, float fU, float fV);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetEyeToHeadTransform")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetEyeToHeadTransform(IntPtr instancePtr, Hmd_Eye eEye);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetTimeSinceLastVsync")]
	internal static extern bool VR_IVRSystem_GetTimeSinceLastVsync(IntPtr instancePtr, ref float pfSecondsSinceLastVsync, ref ulong pulFrameCounter);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetD3D9AdapterIndex")]
	internal static extern int VR_IVRSystem_GetD3D9AdapterIndex(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetDXGIOutputInfo")]
	internal static extern void VR_IVRSystem_GetDXGIOutputInfo(IntPtr instancePtr, ref int pnAdapterIndex, ref int pnAdapterOutputIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_AttachToWindow")]
	internal static extern bool VR_IVRSystem_AttachToWindow(IntPtr instancePtr, IntPtr hWnd);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetDeviceToAbsoluteTrackingPose")]
	internal static extern void VR_IVRSystem_GetDeviceToAbsoluteTrackingPose(IntPtr instancePtr, TrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow,  [In, Out] TrackedDevicePose_t[] pTrackedDevicePoseArray, uint unTrackedDevicePoseArrayCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ResetSeatedZeroPose")]
	internal static extern void VR_IVRSystem_ResetSeatedZeroPose(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass")]
	internal static extern uint VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass(IntPtr instancePtr, TrackedDeviceClass eTrackedDeviceClass,  [In, Out] uint[] punTrackedDeviceIndexArray, uint unTrackedDeviceIndexArrayCount, uint unRelativeToTrackedDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetTrackedDeviceClass")]
	internal static extern TrackedDeviceClass VR_IVRSystem_GetTrackedDeviceClass(IntPtr instancePtr, uint unDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_IsTrackedDeviceConnected")]
	internal static extern bool VR_IVRSystem_IsTrackedDeviceConnected(IntPtr instancePtr, uint unDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetBoolTrackedDeviceProperty")]
	internal static extern bool VR_IVRSystem_GetBoolTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetFloatTrackedDeviceProperty")]
	internal static extern float VR_IVRSystem_GetFloatTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetInt32TrackedDeviceProperty")]
	internal static extern int VR_IVRSystem_GetInt32TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetUint64TrackedDeviceProperty")]
	internal static extern ulong VR_IVRSystem_GetUint64TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetMatrix34TrackedDeviceProperty")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetMatrix34TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetStringTrackedDeviceProperty")]
	internal static extern uint VR_IVRSystem_GetStringTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, TrackedDeviceProperty prop, System.Text.StringBuilder pchValue, uint unBufferSize, ref TrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetPropErrorNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetPropErrorNameFromEnum(IntPtr instancePtr, TrackedPropertyError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_PollNextEvent")]
	internal static extern bool VR_IVRSystem_PollNextEvent(IntPtr instancePtr, ref VREvent_t pEvent);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_PollNextEventWithPose")]
	internal static extern bool VR_IVRSystem_PollNextEventWithPose(IntPtr instancePtr, TrackingUniverseOrigin eOrigin, ref VREvent_t pEvent, ref TrackedDevicePose_t pTrackedDevicePose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetEventTypeNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetEventTypeNameFromEnum(IntPtr instancePtr, uint eType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetHiddenAreaMesh")]
	internal static extern HiddenAreaMesh_t VR_IVRSystem_GetHiddenAreaMesh(IntPtr instancePtr, Hmd_Eye eEye);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerState")]
	internal static extern bool VR_IVRSystem_GetControllerState(IntPtr instancePtr, uint unControllerDeviceIndex, ref VRControllerState_t pControllerState);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerStateWithPose")]
	internal static extern bool VR_IVRSystem_GetControllerStateWithPose(IntPtr instancePtr, TrackingUniverseOrigin eOrigin, uint unControllerDeviceIndex, ref VRControllerState_t pControllerState, ref TrackedDevicePose_t pTrackedDevicePose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_TriggerHapticPulse")]
	internal static extern void VR_IVRSystem_TriggerHapticPulse(IntPtr instancePtr, uint unControllerDeviceIndex, uint unAxisId, char usDurationMicroSec);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetButtonIdNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetButtonIdNameFromEnum(IntPtr instancePtr, uint eButtonId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerAxisTypeNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetControllerAxisTypeNameFromEnum(IntPtr instancePtr, uint eAxisType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_CaptureInputFocus")]
	internal static extern bool VR_IVRSystem_CaptureInputFocus(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ReleaseInputFocus")]
	internal static extern void VR_IVRSystem_ReleaseInputFocus(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_IsInputFocusCapturedByAnotherProcess")]
	internal static extern bool VR_IVRSystem_IsInputFocusCapturedByAnotherProcess(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_DriverDebugRequest")]
	internal static extern uint VR_IVRSystem_DriverDebugRequest(IntPtr instancePtr, uint unDeviceIndex, string pchRequest, string pchResponseBuffer, uint unResponseBufferSize);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetLastError")]
	internal static extern uint VR_IVRCompositor_GetLastError(IntPtr instancePtr, System.Text.StringBuilder pchBuffer, uint unBufferSize);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_SetVSync")]
	internal static extern void VR_IVRCompositor_SetVSync(IntPtr instancePtr, bool bVSync);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetVSync")]
	internal static extern bool VR_IVRCompositor_GetVSync(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_SetGamma")]
	internal static extern void VR_IVRCompositor_SetGamma(IntPtr instancePtr, float fGamma);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetGamma")]
	internal static extern float VR_IVRCompositor_GetGamma(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_WaitGetPoses")]
	internal static extern VRCompositorError VR_IVRCompositor_WaitGetPoses(IntPtr instancePtr,  [In, Out] TrackedDevicePose_t[] pRenderPoseArray, uint unRenderPoseArrayCount,  [In, Out] TrackedDevicePose_t[] pGamePoseArray, uint unGamePoseArrayCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_Submit")]
	internal static extern VRCompositorError VR_IVRCompositor_Submit(IntPtr instancePtr, Hmd_Eye eEye, GraphicsAPIConvention eTextureType, IntPtr pTexture, ref VRTextureBounds_t pBounds);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_ClearLastSubmittedFrame")]
	internal static extern void VR_IVRCompositor_ClearLastSubmittedFrame(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetFrameTiming")]
	internal static extern bool VR_IVRCompositor_GetFrameTiming(IntPtr instancePtr, ref Compositor_FrameTiming pTiming, uint unFramesAgo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_FadeToColor")]
	internal static extern void VR_IVRCompositor_FadeToColor(IntPtr instancePtr, float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_FadeGrid")]
	internal static extern void VR_IVRCompositor_FadeGrid(IntPtr instancePtr, float fSeconds, bool bFadeIn);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorBringToFront")]
	internal static extern void VR_IVRCompositor_CompositorBringToFront(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorGoToBack")]
	internal static extern void VR_IVRCompositor_CompositorGoToBack(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorQuit")]
	internal static extern void VR_IVRCompositor_CompositorQuit(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_IsFullscreen")]
	internal static extern bool VR_IVRCompositor_IsFullscreen(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_SetTrackingSpace")]
	internal static extern void VR_IVRCompositor_SetTrackingSpace(IntPtr instancePtr, TrackingUniverseOrigin eOrigin);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetTrackingSpace")]
	internal static extern TrackingUniverseOrigin VR_IVRCompositor_GetTrackingSpace(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetCurrentSceneFocusProcess")]
	internal static extern uint VR_IVRCompositor_GetCurrentSceneFocusProcess(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CanRenderScene")]
	internal static extern bool VR_IVRCompositor_CanRenderScene(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetCalibrationState")]
	internal static extern ChaperoneCalibrationState VR_IVRChaperone_GetCalibrationState(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetSoftBoundsInfo")]
	internal static extern bool VR_IVRChaperone_GetSoftBoundsInfo(IntPtr instancePtr, ref ChaperoneSoftBoundsInfo_t pInfo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetHardBoundsInfo")]
	internal static extern bool VR_IVRChaperone_GetHardBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, ref uint punQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetSeatedBoundsInfo")]
	internal static extern bool VR_IVRChaperone_GetSeatedBoundsInfo(IntPtr instancePtr, ref ChaperoneSeatedBoundsInfo_t pInfo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_ReloadInfo")]
	internal static extern void VR_IVRChaperone_ReloadInfo(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_FindOverlay")]
	internal static extern VROverlayError VR_IVROverlay_FindOverlay(IntPtr instancePtr, string pchOverlayKey, ref ulong pOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_CreateOverlay")]
	internal static extern VROverlayError VR_IVROverlay_CreateOverlay(IntPtr instancePtr, string pchOverlayKey, string pchOverlayFriendlyName, ref ulong pOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_DestroyOverlay")]
	internal static extern VROverlayError VR_IVROverlay_DestroyOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetHighQualityOverlay")]
	internal static extern VROverlayError VR_IVROverlay_SetHighQualityOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetHighQualityOverlay")]
	internal static extern ulong VR_IVROverlay_GetHighQualityOverlay(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayKey")]
	internal static extern uint VR_IVROverlay_GetOverlayKey(IntPtr instancePtr, ulong ulOverlayHandle, System.Text.StringBuilder pchValue, uint unBufferSize, ref VROverlayError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayName")]
	internal static extern uint VR_IVROverlay_GetOverlayName(IntPtr instancePtr, ulong ulOverlayHandle, System.Text.StringBuilder pchValue, uint unBufferSize, ref VROverlayError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayImageData")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayImageData(IntPtr instancePtr, ulong ulOverlayHandle, IntPtr pvBuffer, uint unBufferSize, ref uint punWidth, ref uint punHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayErrorNameFromEnum")]
	internal static extern IntPtr VR_IVROverlay_GetOverlayErrorNameFromEnum(IntPtr instancePtr, VROverlayError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayFlag")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayFlag(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayFlag")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayFlag(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayFlags eOverlayFlag, ref bool pbEnabled);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayColor")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayColor(IntPtr instancePtr, ulong ulOverlayHandle, float fRed, float fGreen, float fBlue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayColor")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayColor(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfRed, ref float pfGreen, ref float pfBlue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayAlpha")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayAlpha(IntPtr instancePtr, ulong ulOverlayHandle, float fAlpha);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayAlpha")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayAlpha(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfAlpha);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayGamma")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayGamma(IntPtr instancePtr, ulong ulOverlayHandle, float fGamma);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayGamma")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayGamma(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfGamma);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayWidthInMeters")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayWidthInMeters(IntPtr instancePtr, ulong ulOverlayHandle, float fWidthInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayWidthInMeters")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayWidthInMeters(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfWidthInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTextureBounds")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayTextureBounds(IntPtr instancePtr, ulong ulOverlayHandle, ref VRTextureBounds_t pOverlayTextureBounds);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTextureBounds")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayTextureBounds(IntPtr instancePtr, ulong ulOverlayHandle, ref VRTextureBounds_t pOverlayTextureBounds);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformType")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayTransformType(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayTransformType peTransformType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTransformAbsolute")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayTransformAbsolute(IntPtr instancePtr, ulong ulOverlayHandle, TrackingUniverseOrigin eTrackingOrigin, ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformAbsolute")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayTransformAbsolute(IntPtr instancePtr, ulong ulOverlayHandle, ref TrackingUniverseOrigin peTrackingOrigin, ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative(IntPtr instancePtr, ulong ulOverlayHandle, uint unTrackedDevice, ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative(IntPtr instancePtr, ulong ulOverlayHandle, ref uint punTrackedDevice, ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowOverlay")]
	internal static extern VROverlayError VR_IVROverlay_ShowOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_HideOverlay")]
	internal static extern VROverlayError VR_IVROverlay_HideOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsOverlayVisible")]
	internal static extern bool VR_IVROverlay_IsOverlayVisible(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_PollNextOverlayEvent")]
	internal static extern bool VR_IVROverlay_PollNextOverlayEvent(IntPtr instancePtr, ulong ulOverlayHandle, ref VREvent_t pEvent);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayInputMethod")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayInputMethod(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayInputMethod peInputMethod);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayInputMethod")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayInputMethod(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayInputMethod eInputMethod);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayMouseScale")]
	internal static extern VROverlayError VR_IVROverlay_GetOverlayMouseScale(IntPtr instancePtr, ulong ulOverlayHandle, ref HmdVector2_t pvecMouseScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayMouseScale")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayMouseScale(IntPtr instancePtr, ulong ulOverlayHandle, ref HmdVector2_t pvecMouseScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ComputeOverlayIntersection")]
	internal static extern bool VR_IVROverlay_ComputeOverlayIntersection(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayIntersectionParams_t pParams, ref VROverlayIntersectionResults_t pResults);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_HandleControllerOverlayInteractionAsMouse")]
	internal static extern bool VR_IVROverlay_HandleControllerOverlayInteractionAsMouse(IntPtr instancePtr, ulong ulOverlayHandle, uint unControllerDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTexture")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayTexture(IntPtr instancePtr, ulong ulOverlayHandle, GraphicsAPIConvention eTextureType, IntPtr pTexture);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ClearOverlayTexture")]
	internal static extern VROverlayError VR_IVROverlay_ClearOverlayTexture(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayRaw")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayRaw(IntPtr instancePtr, ulong ulOverlayHandle, IntPtr pvBuffer, uint unWidth, uint unHeight, uint unDepth);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayFromFile")]
	internal static extern VROverlayError VR_IVROverlay_SetOverlayFromFile(IntPtr instancePtr, ulong ulOverlayHandle, string pchFilePath);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_CreateDashboardOverlay")]
	internal static extern VROverlayError VR_IVROverlay_CreateDashboardOverlay(IntPtr instancePtr, string pchOverlayKey, string pchOverlayFriendlyName, ref ulong pMainHandle, ref ulong pThumbnailHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsDashboardVisible")]
	internal static extern bool VR_IVROverlay_IsDashboardVisible(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsActiveDashboardOverlay")]
	internal static extern bool VR_IVROverlay_IsActiveDashboardOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetDashboardOverlaySceneProcess")]
	internal static extern VROverlayError VR_IVROverlay_SetDashboardOverlaySceneProcess(IntPtr instancePtr, ulong ulOverlayHandle, uint unProcessId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetDashboardOverlaySceneProcess")]
	internal static extern VROverlayError VR_IVROverlay_GetDashboardOverlaySceneProcess(IntPtr instancePtr, ulong ulOverlayHandle, ref uint punProcessId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowDashboard")]
	internal static extern void VR_IVROverlay_ShowDashboard(IntPtr instancePtr, string pchOverlayToShow);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_LoadRenderModel")]
	internal static extern bool VR_IVRRenderModels_LoadRenderModel(IntPtr instancePtr, string pchRenderModelName, ref RenderModel_t pRenderModel);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_FreeRenderModel")]
	internal static extern void VR_IVRRenderModels_FreeRenderModel(IntPtr instancePtr, ref RenderModel_t pRenderModel);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetRenderModelName")]
	internal static extern uint VR_IVRRenderModels_GetRenderModelName(IntPtr instancePtr, uint unRenderModelIndex, System.Text.StringBuilder pchRenderModelName, uint unRenderModelNameLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetRenderModelCount")]
	internal static extern uint VR_IVRRenderModels_GetRenderModelCount(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverCount")]
	internal static extern uint VR_IVRControlPanel_GetDriverCount(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverId")]
	internal static extern uint VR_IVRControlPanel_GetDriverId(IntPtr instancePtr, uint unDriverIndex, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverDisplayCount")]
	internal static extern uint VR_IVRControlPanel_GetDriverDisplayCount(IntPtr instancePtr, string pchDriverId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverDisplayId")]
	internal static extern uint VR_IVRControlPanel_GetDriverDisplayId(IntPtr instancePtr, string pchDriverId, uint unDisplayIndex, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverDisplayModelNumber")]
	internal static extern uint VR_IVRControlPanel_GetDriverDisplayModelNumber(IntPtr instancePtr, string pchDriverId, string pchDisplayId, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetDriverDisplaySerialNumber")]
	internal static extern uint VR_IVRControlPanel_GetDriverDisplaySerialNumber(IntPtr instancePtr, string pchDriverId, string pchDisplayId, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_LoadSharedResource")]
	internal static extern uint VR_IVRControlPanel_LoadSharedResource(IntPtr instancePtr, string pchResourceName, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetIPD")]
	internal static extern float VR_IVRControlPanel_GetIPD(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_SetIPD")]
	internal static extern void VR_IVRControlPanel_SetIPD(IntPtr instancePtr, float fIPD);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_GetCurrentCompositorInterface")]
	internal static extern IntPtr VR_IVRControlPanel_GetCurrentCompositorInterface(IntPtr instancePtr, string pchInterfaceVersion);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_QuitProcess")]
	internal static extern bool VR_IVRControlPanel_QuitProcess(IntPtr instancePtr, uint pidProcessToQuit);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_StartVRProcess")]
	internal static extern uint VR_IVRControlPanel_StartVRProcess(IntPtr instancePtr, string pchExecutable, string pchArguments, uint unArgumentCount, string pchWorkingDirectory);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_SetMasterProcessToThis")]
	internal static extern void VR_IVRControlPanel_SetMasterProcessToThis(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRNotifications_GetErrorString")]
	internal static extern uint VR_IVRNotifications_GetErrorString(IntPtr instancePtr, NotificationError_t error, System.Text.StringBuilder pchBuffer, uint unBufferSize);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRNotifications_CreateNotification")]
	internal static extern NotificationError_t VR_IVRNotifications_CreateNotification(IntPtr instancePtr, ulong ulOverlayHandle, ulong ulUserValue, string strType, string strText, string strCategory, ref NotificationBitmap photo, ref uint notificationId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRNotifications_DismissNotification")]
	internal static extern NotificationError_t VR_IVRNotifications_DismissNotification(IntPtr instancePtr, uint notificationId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCameraAccess_GetCameraCount")]
	internal static extern uint VR_IVRCameraAccess_GetCameraCount(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCameraAccess_GetCameraId")]
	internal static extern uint VR_IVRCameraAccess_GetCameraId(IntPtr instancePtr, uint unCameraIndex, string pchBuffer, uint unBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCameraAccess_EnableCamera")]
	internal static extern bool VR_IVRCameraAccess_EnableCamera(IntPtr instancePtr, uint unCameraIndex, bool bEnabled);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCameraAccess_GetCameraInfo")]
	internal static extern bool VR_IVRCameraAccess_GetCameraInfo(IntPtr instancePtr, uint unCameraIndex, ref CameraInfo_t pCameraInfo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCameraAccess_GetCameraImage")]
	internal static extern bool VR_IVRCameraAccess_GetCameraImage(IntPtr instancePtr, uint unCameraIndex, ref CameraImage_t pCameraImage);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_CommitWorkingCopy")]
	internal static extern bool VR_IVRChaperoneSetup_CommitWorkingCopy(IntPtr instancePtr, string pchCalibrationName);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_RevertWorkingCopy")]
	internal static extern void VR_IVRChaperoneSetup_RevertWorkingCopy(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingSoftBoundsInfo")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingSoftBoundsInfo(IntPtr instancePtr, ref ChaperoneSoftBoundsInfo_t pInfo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingHardBoundsInfo")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingHardBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, ref uint punQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pmatStandingZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingSoftBoundsInfo")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingSoftBoundsInfo(IntPtr instancePtr, ref ChaperoneSoftBoundsInfo_t pInfo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingHardBoundsInfo")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingHardBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, uint unQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr instancePtr, IntPtr matSeatedZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(IntPtr instancePtr, IntPtr matStandingZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingTagPoses")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingTagPoses(IntPtr instancePtr,  [In, Out] HmdMatrix34_t[] pTagPosesBuffer, ref uint punTagPosesCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingTagPoseScales")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingTagPoseScales(IntPtr instancePtr,  [In, Out] float[] pflScaleBuffer, ref uint punTagPosesCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingTagPoseNameByIndex")]
	internal static extern uint VR_IVRChaperoneSetup_GetWorkingTagPoseNameByIndex(IntPtr instancePtr, uint nIndex, System.Text.StringBuilder pchBuffer, uint unBufferSize);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingTagPoseByName")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingTagPoseByName(IntPtr instancePtr, string pchTagName, ref HmdMatrix34_t pmatTagPose, ref float pflScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingTagPoseByName")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingTagPoseByName(IntPtr instancePtr, string pchTagName, IntPtr matSeatedZeroPoseToRawTrackingPose, float flScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_RemoveWorkingTagPoseByName")]
	internal static extern void VR_IVRChaperoneSetup_RemoveWorkingTagPoseByName(IntPtr instancePtr, string pchTagName);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_RemoveAllWorkingTagPoses")]
	internal static extern void VR_IVRChaperoneSetup_RemoveAllWorkingTagPoses(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_ReloadFromDisk")]
	internal static extern void VR_IVRChaperoneSetup_ReloadFromDisk(IntPtr instancePtr);

}

}

namespace Valve.VR
{

public abstract class IVRSystem
{
	public abstract IntPtr GetIntPtr();
	public abstract void GetWindowBounds(ref int pnX,ref int pnY,ref uint pnWidth,ref uint pnHeight);
	public abstract void GetRecommendedRenderTargetSize(ref uint pnWidth,ref uint pnHeight);
	public abstract void GetEyeOutputViewport(Hmd_Eye eEye,ref uint pnX,ref uint pnY,ref uint pnWidth,ref uint pnHeight);
	public abstract HmdMatrix44_t GetProjectionMatrix(Hmd_Eye eEye,float fNearZ,float fFarZ,GraphicsAPIConvention eProjType);
	public abstract void GetProjectionRaw(Hmd_Eye eEye,ref float pfLeft,ref float pfRight,ref float pfTop,ref float pfBottom);
	public abstract DistortionCoordinates_t ComputeDistortion(Hmd_Eye eEye,float fU,float fV);
	public abstract HmdMatrix34_t GetEyeToHeadTransform(Hmd_Eye eEye);
	public abstract bool GetTimeSinceLastVsync(ref float pfSecondsSinceLastVsync,ref ulong pulFrameCounter);
	public abstract int GetD3D9AdapterIndex();
	public abstract void GetDXGIOutputInfo(ref int pnAdapterIndex,ref int pnAdapterOutputIndex);
	public abstract bool AttachToWindow(IntPtr hWnd);
	public abstract void GetDeviceToAbsoluteTrackingPose(TrackingUniverseOrigin eOrigin,float fPredictedSecondsToPhotonsFromNow,TrackedDevicePose_t [] pTrackedDevicePoseArray);
	public abstract void ResetSeatedZeroPose();
	public abstract HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
	public abstract uint GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass eTrackedDeviceClass,uint [] punTrackedDeviceIndexArray,uint unRelativeToTrackedDeviceIndex);
	public abstract TrackedDeviceClass GetTrackedDeviceClass(uint unDeviceIndex);
	public abstract bool IsTrackedDeviceConnected(uint unDeviceIndex);
	public abstract bool GetBoolTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError);
	public abstract float GetFloatTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError);
	public abstract int GetInt32TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError);
	public abstract ulong GetUint64TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError);
	public abstract HmdMatrix34_t GetMatrix34TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError);
	public abstract uint GetStringTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,System.Text.StringBuilder pchValue,uint unBufferSize,ref TrackedPropertyError pError);
	public abstract string GetPropErrorNameFromEnum(TrackedPropertyError error);
	public abstract bool PollNextEvent(ref VREvent_t pEvent);
	public abstract bool PollNextEventWithPose(TrackingUniverseOrigin eOrigin,ref VREvent_t pEvent,ref TrackedDevicePose_t pTrackedDevicePose);
	public abstract string GetEventTypeNameFromEnum(uint eType);
	public abstract HiddenAreaMesh_t GetHiddenAreaMesh(Hmd_Eye eEye);
	public abstract bool GetControllerState(uint unControllerDeviceIndex,ref VRControllerState_t pControllerState);
	public abstract bool GetControllerStateWithPose(TrackingUniverseOrigin eOrigin,uint unControllerDeviceIndex,ref VRControllerState_t pControllerState,ref TrackedDevicePose_t pTrackedDevicePose);
	public abstract void TriggerHapticPulse(uint unControllerDeviceIndex,uint unAxisId,char usDurationMicroSec);
	public abstract string GetButtonIdNameFromEnum(uint eButtonId);
	public abstract string GetControllerAxisTypeNameFromEnum(uint eAxisType);
	public abstract bool CaptureInputFocus();
	public abstract void ReleaseInputFocus();
	public abstract bool IsInputFocusCapturedByAnotherProcess();
	public abstract uint DriverDebugRequest(uint unDeviceIndex,string pchRequest,string pchResponseBuffer,uint unResponseBufferSize);
}


public abstract class IVRCompositor
{
	public abstract IntPtr GetIntPtr();
	public abstract uint GetLastError(System.Text.StringBuilder pchBuffer,uint unBufferSize);
	public abstract void SetVSync(bool bVSync);
	public abstract bool GetVSync();
	public abstract void SetGamma(float fGamma);
	public abstract float GetGamma();
	public abstract VRCompositorError WaitGetPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray);
	public abstract VRCompositorError Submit(Hmd_Eye eEye,GraphicsAPIConvention eTextureType,IntPtr pTexture,ref VRTextureBounds_t pBounds);
	public abstract void ClearLastSubmittedFrame();
	public abstract bool GetFrameTiming(ref Compositor_FrameTiming pTiming,uint unFramesAgo);
	public abstract void FadeToColor(float fSeconds,float fRed,float fGreen,float fBlue,float fAlpha,bool bBackground);
	public abstract void FadeGrid(float fSeconds,bool bFadeIn);
	public abstract void CompositorBringToFront();
	public abstract void CompositorGoToBack();
	public abstract void CompositorQuit();
	public abstract bool IsFullscreen();
	public abstract void SetTrackingSpace(TrackingUniverseOrigin eOrigin);
	public abstract TrackingUniverseOrigin GetTrackingSpace();
	public abstract uint GetCurrentSceneFocusProcess();
	public abstract bool CanRenderScene();
}


public abstract class IVRChaperone
{
	public abstract IntPtr GetIntPtr();
	public abstract ChaperoneCalibrationState GetCalibrationState();
	public abstract bool GetSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo);
	public abstract bool GetHardBoundsInfo(out HmdQuad_t [] pQuadsBuffer);
	public abstract bool GetSeatedBoundsInfo(ref ChaperoneSeatedBoundsInfo_t pInfo);
	public abstract void ReloadInfo();
}


public abstract class IVROverlay
{
	public abstract IntPtr GetIntPtr();
	public abstract VROverlayError FindOverlay(string pchOverlayKey,ref ulong pOverlayHandle);
	public abstract VROverlayError CreateOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pOverlayHandle);
	public abstract VROverlayError DestroyOverlay(ulong ulOverlayHandle);
	public abstract VROverlayError SetHighQualityOverlay(ulong ulOverlayHandle);
	public abstract ulong GetHighQualityOverlay();
	public abstract uint GetOverlayKey(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref VROverlayError pError);
	public abstract uint GetOverlayName(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref VROverlayError pError);
	public abstract VROverlayError GetOverlayImageData(ulong ulOverlayHandle,IntPtr pvBuffer,uint unBufferSize,ref uint punWidth,ref uint punHeight);
	public abstract string GetOverlayErrorNameFromEnum(VROverlayError error);
	public abstract VROverlayError SetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,bool bEnabled);
	public abstract VROverlayError GetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,ref bool pbEnabled);
	public abstract VROverlayError SetOverlayColor(ulong ulOverlayHandle,float fRed,float fGreen,float fBlue);
	public abstract VROverlayError GetOverlayColor(ulong ulOverlayHandle,ref float pfRed,ref float pfGreen,ref float pfBlue);
	public abstract VROverlayError SetOverlayAlpha(ulong ulOverlayHandle,float fAlpha);
	public abstract VROverlayError GetOverlayAlpha(ulong ulOverlayHandle,ref float pfAlpha);
	public abstract VROverlayError SetOverlayGamma(ulong ulOverlayHandle,float fGamma);
	public abstract VROverlayError GetOverlayGamma(ulong ulOverlayHandle,ref float pfGamma);
	public abstract VROverlayError SetOverlayWidthInMeters(ulong ulOverlayHandle,float fWidthInMeters);
	public abstract VROverlayError GetOverlayWidthInMeters(ulong ulOverlayHandle,ref float pfWidthInMeters);
	public abstract VROverlayError SetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds);
	public abstract VROverlayError GetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds);
	public abstract VROverlayError GetOverlayTransformType(ulong ulOverlayHandle,ref VROverlayTransformType peTransformType);
	public abstract VROverlayError SetOverlayTransformAbsolute(ulong ulOverlayHandle,TrackingUniverseOrigin eTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	public abstract VROverlayError GetOverlayTransformAbsolute(ulong ulOverlayHandle,ref TrackingUniverseOrigin peTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	public abstract VROverlayError SetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,uint unTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	public abstract VROverlayError GetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,ref uint punTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	public abstract VROverlayError ShowOverlay(ulong ulOverlayHandle);
	public abstract VROverlayError HideOverlay(ulong ulOverlayHandle);
	public abstract bool IsOverlayVisible(ulong ulOverlayHandle);
	public abstract bool PollNextOverlayEvent(ulong ulOverlayHandle,ref VREvent_t pEvent);
	public abstract VROverlayError GetOverlayInputMethod(ulong ulOverlayHandle,ref VROverlayInputMethod peInputMethod);
	public abstract VROverlayError SetOverlayInputMethod(ulong ulOverlayHandle,VROverlayInputMethod eInputMethod);
	public abstract VROverlayError GetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale);
	public abstract VROverlayError SetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale);
	public abstract bool ComputeOverlayIntersection(ulong ulOverlayHandle,ref VROverlayIntersectionParams_t pParams,ref VROverlayIntersectionResults_t pResults);
	public abstract bool HandleControllerOverlayInteractionAsMouse(ulong ulOverlayHandle,uint unControllerDeviceIndex);
	public abstract VROverlayError SetOverlayTexture(ulong ulOverlayHandle,GraphicsAPIConvention eTextureType,IntPtr pTexture);
	public abstract VROverlayError ClearOverlayTexture(ulong ulOverlayHandle);
	public abstract VROverlayError SetOverlayRaw(ulong ulOverlayHandle,IntPtr pvBuffer,uint unWidth,uint unHeight,uint unDepth);
	public abstract VROverlayError SetOverlayFromFile(ulong ulOverlayHandle,string pchFilePath);
	public abstract VROverlayError CreateDashboardOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pMainHandle,ref ulong pThumbnailHandle);
	public abstract bool IsDashboardVisible();
	public abstract bool IsActiveDashboardOverlay(ulong ulOverlayHandle);
	public abstract VROverlayError SetDashboardOverlaySceneProcess(ulong ulOverlayHandle,uint unProcessId);
	public abstract VROverlayError GetDashboardOverlaySceneProcess(ulong ulOverlayHandle,ref uint punProcessId);
	public abstract void ShowDashboard(string pchOverlayToShow);
}


public abstract class IVRRenderModels
{
	public abstract IntPtr GetIntPtr();
	public abstract bool LoadRenderModel(string pchRenderModelName,ref RenderModel_t pRenderModel);
	public abstract void FreeRenderModel(ref RenderModel_t pRenderModel);
	public abstract uint GetRenderModelName(uint unRenderModelIndex,System.Text.StringBuilder pchRenderModelName,uint unRenderModelNameLen);
	public abstract uint GetRenderModelCount();
}


public abstract class IVRControlPanel
{
	public abstract IntPtr GetIntPtr();
	public abstract uint GetDriverCount();
	public abstract uint GetDriverId(uint unDriverIndex,string pchBuffer,uint unBufferLen);
	public abstract uint GetDriverDisplayCount(string pchDriverId);
	public abstract uint GetDriverDisplayId(string pchDriverId,uint unDisplayIndex,string pchBuffer,uint unBufferLen);
	public abstract uint GetDriverDisplayModelNumber(string pchDriverId,string pchDisplayId,string pchBuffer,uint unBufferLen);
	public abstract uint GetDriverDisplaySerialNumber(string pchDriverId,string pchDisplayId,string pchBuffer,uint unBufferLen);
	public abstract uint LoadSharedResource(string pchResourceName,string pchBuffer,uint unBufferLen);
	public abstract float GetIPD();
	public abstract void SetIPD(float fIPD);
	public abstract IVRCompositor GetCurrentCompositorInterface(string pchInterfaceVersion);
	public abstract bool QuitProcess(uint pidProcessToQuit);
	public abstract uint StartVRProcess(string pchExecutable,string pchArguments,uint unArgumentCount,string pchWorkingDirectory);
	public abstract void SetMasterProcessToThis();
}


public abstract class IVRNotifications
{
	public abstract IntPtr GetIntPtr();
	public abstract uint GetErrorString(NotificationError_t error,System.Text.StringBuilder pchBuffer,uint unBufferSize);
	public abstract NotificationError_t CreateNotification(ulong ulOverlayHandle,ulong ulUserValue,string strType,string strText,string strCategory,ref NotificationBitmap photo,ref uint notificationId);
	public abstract NotificationError_t DismissNotification(uint notificationId);
}


public abstract class IVRCameraAccess
{
	public abstract IntPtr GetIntPtr();
	public abstract uint GetCameraCount();
	public abstract uint GetCameraId(uint unCameraIndex,string pchBuffer,uint unBufferLen);
	public abstract bool EnableCamera(uint unCameraIndex,bool bEnabled);
	public abstract bool GetCameraInfo(uint unCameraIndex,ref CameraInfo_t pCameraInfo);
	public abstract bool GetCameraImage(uint unCameraIndex,ref CameraImage_t pCameraImage);
}


public abstract class IVRChaperoneSetup
{
	public abstract IntPtr GetIntPtr();
	public abstract bool CommitWorkingCopy(string pchCalibrationName);
	public abstract void RevertWorkingCopy();
	public abstract bool GetWorkingSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo);
	public abstract bool GetWorkingHardBoundsInfo(out HmdQuad_t [] pQuadsBuffer);
	public abstract bool GetWorkingSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
	public abstract bool GetWorkingStandingZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatStandingZeroPoseToRawTrackingPose);
	public abstract void SetWorkingSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo);
	public abstract void SetWorkingHardBoundsInfo(HmdQuad_t [] pQuadsBuffer);
	public abstract void SetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr matSeatedZeroPoseToRawTrackingPose);
	public abstract void SetWorkingStandingZeroPoseToRawTrackingPose(IntPtr matStandingZeroPoseToRawTrackingPose);
	public abstract bool GetWorkingTagPoses(out HmdMatrix34_t [] pTagPosesBuffer);
	public abstract bool GetWorkingTagPoseScales(out float [] pflScaleBuffer);
	public abstract uint GetWorkingTagPoseNameByIndex(uint nIndex,System.Text.StringBuilder pchBuffer,uint unBufferSize);
	public abstract bool GetWorkingTagPoseByName(string pchTagName,ref HmdMatrix34_t pmatTagPose,ref float pflScale);
	public abstract void SetWorkingTagPoseByName(string pchTagName,IntPtr matSeatedZeroPoseToRawTrackingPose,float flScale);
	public abstract void RemoveWorkingTagPoseByName(string pchTagName);
	public abstract void RemoveAllWorkingTagPoses();
	public abstract void ReloadFromDisk();
}


public class CVRSystem : IVRSystem
{
	public CVRSystem(IntPtr VRSystem)
	{
		m_pVRSystem = VRSystem;
	}
	IntPtr m_pVRSystem;

	public override IntPtr GetIntPtr() { return m_pVRSystem; }

	private void CheckIfUsable()
	{
		if (m_pVRSystem == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override void GetWindowBounds(ref int pnX,ref int pnY,ref uint pnWidth,ref uint pnHeight)
	{
		CheckIfUsable();
		pnX = 0;
		pnY = 0;
		pnWidth = 0;
		pnHeight = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetWindowBounds(m_pVRSystem,ref pnX,ref pnY,ref pnWidth,ref pnHeight);
	}
	public override void GetRecommendedRenderTargetSize(ref uint pnWidth,ref uint pnHeight)
	{
		CheckIfUsable();
		pnWidth = 0;
		pnHeight = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetRecommendedRenderTargetSize(m_pVRSystem,ref pnWidth,ref pnHeight);
	}
	public override void GetEyeOutputViewport(Hmd_Eye eEye,ref uint pnX,ref uint pnY,ref uint pnWidth,ref uint pnHeight)
	{
		CheckIfUsable();
		pnX = 0;
		pnY = 0;
		pnWidth = 0;
		pnHeight = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetEyeOutputViewport(m_pVRSystem,eEye,ref pnX,ref pnY,ref pnWidth,ref pnHeight);
	}
	public override HmdMatrix44_t GetProjectionMatrix(Hmd_Eye eEye,float fNearZ,float fFarZ,GraphicsAPIConvention eProjType)
	{
		CheckIfUsable();
		HmdMatrix44_t result = VRNativeEntrypoints.VR_IVRSystem_GetProjectionMatrix(m_pVRSystem,eEye,fNearZ,fFarZ,eProjType);
		return result;
	}
	public override void GetProjectionRaw(Hmd_Eye eEye,ref float pfLeft,ref float pfRight,ref float pfTop,ref float pfBottom)
	{
		CheckIfUsable();
		pfLeft = 0;
		pfRight = 0;
		pfTop = 0;
		pfBottom = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetProjectionRaw(m_pVRSystem,eEye,ref pfLeft,ref pfRight,ref pfTop,ref pfBottom);
	}
	public override DistortionCoordinates_t ComputeDistortion(Hmd_Eye eEye,float fU,float fV)
	{
		CheckIfUsable();
		DistortionCoordinates_t result = VRNativeEntrypoints.VR_IVRSystem_ComputeDistortion(m_pVRSystem,eEye,fU,fV);
		return result;
	}
	public override HmdMatrix34_t GetEyeToHeadTransform(Hmd_Eye eEye)
	{
		CheckIfUsable();
		HmdMatrix34_t result = VRNativeEntrypoints.VR_IVRSystem_GetEyeToHeadTransform(m_pVRSystem,eEye);
		return result;
	}
	public override bool GetTimeSinceLastVsync(ref float pfSecondsSinceLastVsync,ref ulong pulFrameCounter)
	{
		CheckIfUsable();
		pfSecondsSinceLastVsync = 0;
		pulFrameCounter = 0;
		bool result = VRNativeEntrypoints.VR_IVRSystem_GetTimeSinceLastVsync(m_pVRSystem,ref pfSecondsSinceLastVsync,ref pulFrameCounter);
		return result;
	}
	public override int GetD3D9AdapterIndex()
	{
		CheckIfUsable();
		int result = VRNativeEntrypoints.VR_IVRSystem_GetD3D9AdapterIndex(m_pVRSystem);
		return result;
	}
	public override void GetDXGIOutputInfo(ref int pnAdapterIndex,ref int pnAdapterOutputIndex)
	{
		CheckIfUsable();
		pnAdapterIndex = 0;
		pnAdapterOutputIndex = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetDXGIOutputInfo(m_pVRSystem,ref pnAdapterIndex,ref pnAdapterOutputIndex);
	}
	public override bool AttachToWindow(IntPtr hWnd)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_AttachToWindow(m_pVRSystem,hWnd);
		return result;
	}
	public override void GetDeviceToAbsoluteTrackingPose(TrackingUniverseOrigin eOrigin,float fPredictedSecondsToPhotonsFromNow,TrackedDevicePose_t [] pTrackedDevicePoseArray)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_GetDeviceToAbsoluteTrackingPose(m_pVRSystem,eOrigin,fPredictedSecondsToPhotonsFromNow,pTrackedDevicePoseArray,(uint) pTrackedDevicePoseArray.Length);
	}
	public override void ResetSeatedZeroPose()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_ResetSeatedZeroPose(m_pVRSystem);
	}
	public override HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose()
	{
		CheckIfUsable();
		HmdMatrix34_t result = VRNativeEntrypoints.VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(m_pVRSystem);
		return result;
	}
	public override uint GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass eTrackedDeviceClass,uint [] punTrackedDeviceIndexArray,uint unRelativeToTrackedDeviceIndex)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass(m_pVRSystem,eTrackedDeviceClass,punTrackedDeviceIndexArray,(uint) punTrackedDeviceIndexArray.Length,unRelativeToTrackedDeviceIndex);
		return result;
	}
	public override TrackedDeviceClass GetTrackedDeviceClass(uint unDeviceIndex)
	{
		CheckIfUsable();
		TrackedDeviceClass result = VRNativeEntrypoints.VR_IVRSystem_GetTrackedDeviceClass(m_pVRSystem,unDeviceIndex);
		return result;
	}
	public override bool IsTrackedDeviceConnected(uint unDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_IsTrackedDeviceConnected(m_pVRSystem,unDeviceIndex);
		return result;
	}
	public override bool GetBoolTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_GetBoolTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override float GetFloatTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRSystem_GetFloatTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override int GetInt32TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		int result = VRNativeEntrypoints.VR_IVRSystem_GetInt32TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override ulong GetUint64TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVRSystem_GetUint64TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override HmdMatrix34_t GetMatrix34TrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		HmdMatrix34_t result = VRNativeEntrypoints.VR_IVRSystem_GetMatrix34TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override uint GetStringTrackedDeviceProperty(uint unDeviceIndex,TrackedDeviceProperty prop,System.Text.StringBuilder pchValue,uint unBufferSize,ref TrackedPropertyError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRSystem_GetStringTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override string GetPropErrorNameFromEnum(TrackedPropertyError error)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetPropErrorNameFromEnum(m_pVRSystem,error);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override bool PollNextEvent(ref VREvent_t pEvent)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_PollNextEvent(m_pVRSystem,ref pEvent);
		return result;
	}
	public override bool PollNextEventWithPose(TrackingUniverseOrigin eOrigin,ref VREvent_t pEvent,ref TrackedDevicePose_t pTrackedDevicePose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_PollNextEventWithPose(m_pVRSystem,eOrigin,ref pEvent,ref pTrackedDevicePose);
		return result;
	}
	public override string GetEventTypeNameFromEnum(uint eType)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetEventTypeNameFromEnum(m_pVRSystem,eType);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override HiddenAreaMesh_t GetHiddenAreaMesh(Hmd_Eye eEye)
	{
		CheckIfUsable();
		HiddenAreaMesh_t result = VRNativeEntrypoints.VR_IVRSystem_GetHiddenAreaMesh(m_pVRSystem,eEye);
		return result;
	}
	public override bool GetControllerState(uint unControllerDeviceIndex,ref VRControllerState_t pControllerState)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_GetControllerState(m_pVRSystem,unControllerDeviceIndex,ref pControllerState);
		return result;
	}
	public override bool GetControllerStateWithPose(TrackingUniverseOrigin eOrigin,uint unControllerDeviceIndex,ref VRControllerState_t pControllerState,ref TrackedDevicePose_t pTrackedDevicePose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_GetControllerStateWithPose(m_pVRSystem,eOrigin,unControllerDeviceIndex,ref pControllerState,ref pTrackedDevicePose);
		return result;
	}
	public override void TriggerHapticPulse(uint unControllerDeviceIndex,uint unAxisId,char usDurationMicroSec)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_TriggerHapticPulse(m_pVRSystem,unControllerDeviceIndex,unAxisId,usDurationMicroSec);
	}
	public override string GetButtonIdNameFromEnum(uint eButtonId)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetButtonIdNameFromEnum(m_pVRSystem,eButtonId);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override string GetControllerAxisTypeNameFromEnum(uint eAxisType)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetControllerAxisTypeNameFromEnum(m_pVRSystem,eAxisType);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override bool CaptureInputFocus()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_CaptureInputFocus(m_pVRSystem);
		return result;
	}
	public override void ReleaseInputFocus()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_ReleaseInputFocus(m_pVRSystem);
	}
	public override bool IsInputFocusCapturedByAnotherProcess()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_IsInputFocusCapturedByAnotherProcess(m_pVRSystem);
		return result;
	}
	public override uint DriverDebugRequest(uint unDeviceIndex,string pchRequest,string pchResponseBuffer,uint unResponseBufferSize)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRSystem_DriverDebugRequest(m_pVRSystem,unDeviceIndex,pchRequest,pchResponseBuffer,unResponseBufferSize);
		return result;
	}
}


public class CVRCompositor : IVRCompositor
{
	public CVRCompositor(IntPtr VRCompositor)
	{
		m_pVRCompositor = VRCompositor;
	}
	IntPtr m_pVRCompositor;

	public override IntPtr GetIntPtr() { return m_pVRCompositor; }

	private void CheckIfUsable()
	{
		if (m_pVRCompositor == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override uint GetLastError(System.Text.StringBuilder pchBuffer,uint unBufferSize)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCompositor_GetLastError(m_pVRCompositor,pchBuffer,unBufferSize);
		return result;
	}
	public override void SetVSync(bool bVSync)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_SetVSync(m_pVRCompositor,bVSync);
	}
	public override bool GetVSync()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_GetVSync(m_pVRCompositor);
		return result;
	}
	public override void SetGamma(float fGamma)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_SetGamma(m_pVRCompositor,fGamma);
	}
	public override float GetGamma()
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRCompositor_GetGamma(m_pVRCompositor);
		return result;
	}
	public override VRCompositorError WaitGetPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray)
	{
		CheckIfUsable();
		VRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_WaitGetPoses(m_pVRCompositor,pRenderPoseArray,(uint) pRenderPoseArray.Length,pGamePoseArray,(uint) pGamePoseArray.Length);
		return result;
	}
	public override VRCompositorError Submit(Hmd_Eye eEye,GraphicsAPIConvention eTextureType,IntPtr pTexture,ref VRTextureBounds_t pBounds)
	{
		CheckIfUsable();
		VRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_Submit(m_pVRCompositor,eEye,eTextureType,pTexture,ref pBounds);
		return result;
	}
	public override void ClearLastSubmittedFrame()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_ClearLastSubmittedFrame(m_pVRCompositor);
	}
	public override bool GetFrameTiming(ref Compositor_FrameTiming pTiming,uint unFramesAgo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_GetFrameTiming(m_pVRCompositor,ref pTiming,unFramesAgo);
		return result;
	}
	public override void FadeToColor(float fSeconds,float fRed,float fGreen,float fBlue,float fAlpha,bool bBackground)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_FadeToColor(m_pVRCompositor,fSeconds,fRed,fGreen,fBlue,fAlpha,bBackground);
	}
	public override void FadeGrid(float fSeconds,bool bFadeIn)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_FadeGrid(m_pVRCompositor,fSeconds,bFadeIn);
	}
	public override void CompositorBringToFront()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_CompositorBringToFront(m_pVRCompositor);
	}
	public override void CompositorGoToBack()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_CompositorGoToBack(m_pVRCompositor);
	}
	public override void CompositorQuit()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_CompositorQuit(m_pVRCompositor);
	}
	public override bool IsFullscreen()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_IsFullscreen(m_pVRCompositor);
		return result;
	}
	public override void SetTrackingSpace(TrackingUniverseOrigin eOrigin)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_SetTrackingSpace(m_pVRCompositor,eOrigin);
	}
	public override TrackingUniverseOrigin GetTrackingSpace()
	{
		CheckIfUsable();
		TrackingUniverseOrigin result = VRNativeEntrypoints.VR_IVRCompositor_GetTrackingSpace(m_pVRCompositor);
		return result;
	}
	public override uint GetCurrentSceneFocusProcess()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCompositor_GetCurrentSceneFocusProcess(m_pVRCompositor);
		return result;
	}
	public override bool CanRenderScene()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_CanRenderScene(m_pVRCompositor);
		return result;
	}
}


public class CVRChaperone : IVRChaperone
{
	public CVRChaperone(IntPtr VRChaperone)
	{
		m_pVRChaperone = VRChaperone;
	}
	IntPtr m_pVRChaperone;

	public override IntPtr GetIntPtr() { return m_pVRChaperone; }

	private void CheckIfUsable()
	{
		if (m_pVRChaperone == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override ChaperoneCalibrationState GetCalibrationState()
	{
		CheckIfUsable();
		ChaperoneCalibrationState result = VRNativeEntrypoints.VR_IVRChaperone_GetCalibrationState(m_pVRChaperone);
		return result;
	}
	public override bool GetSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperone_GetSoftBoundsInfo(m_pVRChaperone,ref pInfo);
		return result;
	}
	public override bool GetHardBoundsInfo(out HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		uint punQuadsCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperone_GetHardBoundsInfo(m_pVRChaperone,null,ref punQuadsCount);
		pQuadsBuffer= new HmdQuad_t[punQuadsCount];
		result = VRNativeEntrypoints.VR_IVRChaperone_GetHardBoundsInfo(m_pVRChaperone,pQuadsBuffer,ref punQuadsCount);
		return result;
	}
	public override bool GetSeatedBoundsInfo(ref ChaperoneSeatedBoundsInfo_t pInfo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperone_GetSeatedBoundsInfo(m_pVRChaperone,ref pInfo);
		return result;
	}
	public override void ReloadInfo()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperone_ReloadInfo(m_pVRChaperone);
	}
}


public class CVROverlay : IVROverlay
{
	public CVROverlay(IntPtr VROverlay)
	{
		m_pVROverlay = VROverlay;
	}
	IntPtr m_pVROverlay;

	public override IntPtr GetIntPtr() { return m_pVROverlay; }

	private void CheckIfUsable()
	{
		if (m_pVROverlay == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override VROverlayError FindOverlay(string pchOverlayKey,ref ulong pOverlayHandle)
	{
		CheckIfUsable();
		pOverlayHandle = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_FindOverlay(m_pVROverlay,pchOverlayKey,ref pOverlayHandle);
		return result;
	}
	public override VROverlayError CreateOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pOverlayHandle)
	{
		CheckIfUsable();
		pOverlayHandle = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_CreateOverlay(m_pVROverlay,pchOverlayKey,pchOverlayFriendlyName,ref pOverlayHandle);
		return result;
	}
	public override VROverlayError DestroyOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_DestroyOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override VROverlayError SetHighQualityOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetHighQualityOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override ulong GetHighQualityOverlay()
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVROverlay_GetHighQualityOverlay(m_pVROverlay);
		return result;
	}
	public override uint GetOverlayKey(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref VROverlayError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayKey(m_pVROverlay,ulOverlayHandle,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override uint GetOverlayName(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref VROverlayError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayName(m_pVROverlay,ulOverlayHandle,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override VROverlayError GetOverlayImageData(ulong ulOverlayHandle,IntPtr pvBuffer,uint unBufferSize,ref uint punWidth,ref uint punHeight)
	{
		CheckIfUsable();
		punWidth = 0;
		punHeight = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayImageData(m_pVROverlay,ulOverlayHandle,pvBuffer,unBufferSize,ref punWidth,ref punHeight);
		return result;
	}
	public override string GetOverlayErrorNameFromEnum(VROverlayError error)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayErrorNameFromEnum(m_pVROverlay,error);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override VROverlayError SetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,bool bEnabled)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayFlag(m_pVROverlay,ulOverlayHandle,eOverlayFlag,bEnabled);
		return result;
	}
	public override VROverlayError GetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,ref bool pbEnabled)
	{
		CheckIfUsable();
		pbEnabled = false;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayFlag(m_pVROverlay,ulOverlayHandle,eOverlayFlag,ref pbEnabled);
		return result;
	}
	public override VROverlayError SetOverlayColor(ulong ulOverlayHandle,float fRed,float fGreen,float fBlue)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayColor(m_pVROverlay,ulOverlayHandle,fRed,fGreen,fBlue);
		return result;
	}
	public override VROverlayError GetOverlayColor(ulong ulOverlayHandle,ref float pfRed,ref float pfGreen,ref float pfBlue)
	{
		CheckIfUsable();
		pfRed = 0;
		pfGreen = 0;
		pfBlue = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayColor(m_pVROverlay,ulOverlayHandle,ref pfRed,ref pfGreen,ref pfBlue);
		return result;
	}
	public override VROverlayError SetOverlayAlpha(ulong ulOverlayHandle,float fAlpha)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayAlpha(m_pVROverlay,ulOverlayHandle,fAlpha);
		return result;
	}
	public override VROverlayError GetOverlayAlpha(ulong ulOverlayHandle,ref float pfAlpha)
	{
		CheckIfUsable();
		pfAlpha = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayAlpha(m_pVROverlay,ulOverlayHandle,ref pfAlpha);
		return result;
	}
	public override VROverlayError SetOverlayGamma(ulong ulOverlayHandle,float fGamma)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayGamma(m_pVROverlay,ulOverlayHandle,fGamma);
		return result;
	}
	public override VROverlayError GetOverlayGamma(ulong ulOverlayHandle,ref float pfGamma)
	{
		CheckIfUsable();
		pfGamma = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayGamma(m_pVROverlay,ulOverlayHandle,ref pfGamma);
		return result;
	}
	public override VROverlayError SetOverlayWidthInMeters(ulong ulOverlayHandle,float fWidthInMeters)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayWidthInMeters(m_pVROverlay,ulOverlayHandle,fWidthInMeters);
		return result;
	}
	public override VROverlayError GetOverlayWidthInMeters(ulong ulOverlayHandle,ref float pfWidthInMeters)
	{
		CheckIfUsable();
		pfWidthInMeters = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayWidthInMeters(m_pVROverlay,ulOverlayHandle,ref pfWidthInMeters);
		return result;
	}
	public override VROverlayError SetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTextureBounds(m_pVROverlay,ulOverlayHandle,ref pOverlayTextureBounds);
		return result;
	}
	public override VROverlayError GetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTextureBounds(m_pVROverlay,ulOverlayHandle,ref pOverlayTextureBounds);
		return result;
	}
	public override VROverlayError GetOverlayTransformType(ulong ulOverlayHandle,ref VROverlayTransformType peTransformType)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformType(m_pVROverlay,ulOverlayHandle,ref peTransformType);
		return result;
	}
	public override VROverlayError SetOverlayTransformAbsolute(ulong ulOverlayHandle,TrackingUniverseOrigin eTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTransformAbsolute(m_pVROverlay,ulOverlayHandle,eTrackingOrigin,ref pmatTrackingOriginToOverlayTransform);
		return result;
	}
	public override VROverlayError GetOverlayTransformAbsolute(ulong ulOverlayHandle,ref TrackingUniverseOrigin peTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformAbsolute(m_pVROverlay,ulOverlayHandle,ref peTrackingOrigin,ref pmatTrackingOriginToOverlayTransform);
		return result;
	}
	public override VROverlayError SetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,uint unTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative(m_pVROverlay,ulOverlayHandle,unTrackedDevice,ref pmatTrackedDeviceToOverlayTransform);
		return result;
	}
	public override VROverlayError GetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,ref uint punTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform)
	{
		CheckIfUsable();
		punTrackedDevice = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative(m_pVROverlay,ulOverlayHandle,ref punTrackedDevice,ref pmatTrackedDeviceToOverlayTransform);
		return result;
	}
	public override VROverlayError ShowOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ShowOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override VROverlayError HideOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_HideOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override bool IsOverlayVisible(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_IsOverlayVisible(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override bool PollNextOverlayEvent(ulong ulOverlayHandle,ref VREvent_t pEvent)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_PollNextOverlayEvent(m_pVROverlay,ulOverlayHandle,ref pEvent);
		return result;
	}
	public override VROverlayError GetOverlayInputMethod(ulong ulOverlayHandle,ref VROverlayInputMethod peInputMethod)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayInputMethod(m_pVROverlay,ulOverlayHandle,ref peInputMethod);
		return result;
	}
	public override VROverlayError SetOverlayInputMethod(ulong ulOverlayHandle,VROverlayInputMethod eInputMethod)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayInputMethod(m_pVROverlay,ulOverlayHandle,eInputMethod);
		return result;
	}
	public override VROverlayError GetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayMouseScale(m_pVROverlay,ulOverlayHandle,ref pvecMouseScale);
		return result;
	}
	public override VROverlayError SetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayMouseScale(m_pVROverlay,ulOverlayHandle,ref pvecMouseScale);
		return result;
	}
	public override bool ComputeOverlayIntersection(ulong ulOverlayHandle,ref VROverlayIntersectionParams_t pParams,ref VROverlayIntersectionResults_t pResults)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_ComputeOverlayIntersection(m_pVROverlay,ulOverlayHandle,ref pParams,ref pResults);
		return result;
	}
	public override bool HandleControllerOverlayInteractionAsMouse(ulong ulOverlayHandle,uint unControllerDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_HandleControllerOverlayInteractionAsMouse(m_pVROverlay,ulOverlayHandle,unControllerDeviceIndex);
		return result;
	}
	public override VROverlayError SetOverlayTexture(ulong ulOverlayHandle,GraphicsAPIConvention eTextureType,IntPtr pTexture)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTexture(m_pVROverlay,ulOverlayHandle,eTextureType,pTexture);
		return result;
	}
	public override VROverlayError ClearOverlayTexture(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ClearOverlayTexture(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override VROverlayError SetOverlayRaw(ulong ulOverlayHandle,IntPtr pvBuffer,uint unWidth,uint unHeight,uint unDepth)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayRaw(m_pVROverlay,ulOverlayHandle,pvBuffer,unWidth,unHeight,unDepth);
		return result;
	}
	public override VROverlayError SetOverlayFromFile(ulong ulOverlayHandle,string pchFilePath)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayFromFile(m_pVROverlay,ulOverlayHandle,pchFilePath);
		return result;
	}
	public override VROverlayError CreateDashboardOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pMainHandle,ref ulong pThumbnailHandle)
	{
		CheckIfUsable();
		pMainHandle = 0;
		pThumbnailHandle = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_CreateDashboardOverlay(m_pVROverlay,pchOverlayKey,pchOverlayFriendlyName,ref pMainHandle,ref pThumbnailHandle);
		return result;
	}
	public override bool IsDashboardVisible()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_IsDashboardVisible(m_pVROverlay);
		return result;
	}
	public override bool IsActiveDashboardOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_IsActiveDashboardOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override VROverlayError SetDashboardOverlaySceneProcess(ulong ulOverlayHandle,uint unProcessId)
	{
		CheckIfUsable();
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetDashboardOverlaySceneProcess(m_pVROverlay,ulOverlayHandle,unProcessId);
		return result;
	}
	public override VROverlayError GetDashboardOverlaySceneProcess(ulong ulOverlayHandle,ref uint punProcessId)
	{
		CheckIfUsable();
		punProcessId = 0;
		VROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetDashboardOverlaySceneProcess(m_pVROverlay,ulOverlayHandle,ref punProcessId);
		return result;
	}
	public override void ShowDashboard(string pchOverlayToShow)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVROverlay_ShowDashboard(m_pVROverlay,pchOverlayToShow);
	}
}


public class CVRRenderModels : IVRRenderModels
{
	public CVRRenderModels(IntPtr VRRenderModels)
	{
		m_pVRRenderModels = VRRenderModels;
	}
	IntPtr m_pVRRenderModels;

	public override IntPtr GetIntPtr() { return m_pVRRenderModels; }

	private void CheckIfUsable()
	{
		if (m_pVRRenderModels == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override bool LoadRenderModel(string pchRenderModelName,ref RenderModel_t pRenderModel)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRRenderModels_LoadRenderModel(m_pVRRenderModels,pchRenderModelName,ref pRenderModel);
		return result;
	}
	public override void FreeRenderModel(ref RenderModel_t pRenderModel)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRRenderModels_FreeRenderModel(m_pVRRenderModels,ref pRenderModel);
	}
	public override uint GetRenderModelName(uint unRenderModelIndex,System.Text.StringBuilder pchRenderModelName,uint unRenderModelNameLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRRenderModels_GetRenderModelName(m_pVRRenderModels,unRenderModelIndex,pchRenderModelName,unRenderModelNameLen);
		return result;
	}
	public override uint GetRenderModelCount()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRRenderModels_GetRenderModelCount(m_pVRRenderModels);
		return result;
	}
}


public class CVRControlPanel : IVRControlPanel
{
	public CVRControlPanel(IntPtr VRControlPanel)
	{
		m_pVRControlPanel = VRControlPanel;
	}
	IntPtr m_pVRControlPanel;

	public override IntPtr GetIntPtr() { return m_pVRControlPanel; }

	private void CheckIfUsable()
	{
		if (m_pVRControlPanel == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override uint GetDriverCount()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverCount(m_pVRControlPanel);
		return result;
	}
	public override uint GetDriverId(uint unDriverIndex,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverId(m_pVRControlPanel,unDriverIndex,pchBuffer,unBufferLen);
		return result;
	}
	public override uint GetDriverDisplayCount(string pchDriverId)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverDisplayCount(m_pVRControlPanel,pchDriverId);
		return result;
	}
	public override uint GetDriverDisplayId(string pchDriverId,uint unDisplayIndex,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverDisplayId(m_pVRControlPanel,pchDriverId,unDisplayIndex,pchBuffer,unBufferLen);
		return result;
	}
	public override uint GetDriverDisplayModelNumber(string pchDriverId,string pchDisplayId,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverDisplayModelNumber(m_pVRControlPanel,pchDriverId,pchDisplayId,pchBuffer,unBufferLen);
		return result;
	}
	public override uint GetDriverDisplaySerialNumber(string pchDriverId,string pchDisplayId,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_GetDriverDisplaySerialNumber(m_pVRControlPanel,pchDriverId,pchDisplayId,pchBuffer,unBufferLen);
		return result;
	}
	public override uint LoadSharedResource(string pchResourceName,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_LoadSharedResource(m_pVRControlPanel,pchResourceName,pchBuffer,unBufferLen);
		return result;
	}
	public override float GetIPD()
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRControlPanel_GetIPD(m_pVRControlPanel);
		return result;
	}
	public override void SetIPD(float fIPD)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRControlPanel_SetIPD(m_pVRControlPanel,fIPD);
	}
	public override IVRCompositor GetCurrentCompositorInterface(string pchInterfaceVersion)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRControlPanel_GetCurrentCompositorInterface(m_pVRControlPanel,pchInterfaceVersion);
		return (IVRCompositor) Marshal.PtrToStructure(result, typeof(IVRCompositor));
	}
	public override bool QuitProcess(uint pidProcessToQuit)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRControlPanel_QuitProcess(m_pVRControlPanel,pidProcessToQuit);
		return result;
	}
	public override uint StartVRProcess(string pchExecutable,string pchArguments,uint unArgumentCount,string pchWorkingDirectory)
	{
		CheckIfUsable();
		pchArguments = "";
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_StartVRProcess(m_pVRControlPanel,pchExecutable,pchArguments,unArgumentCount,pchWorkingDirectory);
		return result;
	}
	public override void SetMasterProcessToThis()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRControlPanel_SetMasterProcessToThis(m_pVRControlPanel);
	}
}


public class CVRNotifications : IVRNotifications
{
	public CVRNotifications(IntPtr VRNotifications)
	{
		m_pVRNotifications = VRNotifications;
	}
	IntPtr m_pVRNotifications;

	public override IntPtr GetIntPtr() { return m_pVRNotifications; }

	private void CheckIfUsable()
	{
		if (m_pVRNotifications == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override uint GetErrorString(NotificationError_t error,System.Text.StringBuilder pchBuffer,uint unBufferSize)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRNotifications_GetErrorString(m_pVRNotifications,error,pchBuffer,unBufferSize);
		return result;
	}
	public override NotificationError_t CreateNotification(ulong ulOverlayHandle,ulong ulUserValue,string strType,string strText,string strCategory,ref NotificationBitmap photo,ref uint notificationId)
	{
		CheckIfUsable();
		notificationId = 0;
		NotificationError_t result = VRNativeEntrypoints.VR_IVRNotifications_CreateNotification(m_pVRNotifications,ulOverlayHandle,ulUserValue,strType,strText,strCategory,ref photo,ref notificationId);
		return result;
	}
	public override NotificationError_t DismissNotification(uint notificationId)
	{
		CheckIfUsable();
		NotificationError_t result = VRNativeEntrypoints.VR_IVRNotifications_DismissNotification(m_pVRNotifications,notificationId);
		return result;
	}
}


public class CVRCameraAccess : IVRCameraAccess
{
	public CVRCameraAccess(IntPtr VRCameraAccess)
	{
		m_pVRCameraAccess = VRCameraAccess;
	}
	IntPtr m_pVRCameraAccess;

	public override IntPtr GetIntPtr() { return m_pVRCameraAccess; }

	private void CheckIfUsable()
	{
		if (m_pVRCameraAccess == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override uint GetCameraCount()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCameraAccess_GetCameraCount(m_pVRCameraAccess);
		return result;
	}
	public override uint GetCameraId(uint unCameraIndex,string pchBuffer,uint unBufferLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCameraAccess_GetCameraId(m_pVRCameraAccess,unCameraIndex,pchBuffer,unBufferLen);
		return result;
	}
	public override bool EnableCamera(uint unCameraIndex,bool bEnabled)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCameraAccess_EnableCamera(m_pVRCameraAccess,unCameraIndex,bEnabled);
		return result;
	}
	public override bool GetCameraInfo(uint unCameraIndex,ref CameraInfo_t pCameraInfo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCameraAccess_GetCameraInfo(m_pVRCameraAccess,unCameraIndex,ref pCameraInfo);
		return result;
	}
	public override bool GetCameraImage(uint unCameraIndex,ref CameraImage_t pCameraImage)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCameraAccess_GetCameraImage(m_pVRCameraAccess,unCameraIndex,ref pCameraImage);
		return result;
	}
}


public class CVRChaperoneSetup : IVRChaperoneSetup
{
	public CVRChaperoneSetup(IntPtr VRChaperoneSetup)
	{
		m_pVRChaperoneSetup = VRChaperoneSetup;
	}
	IntPtr m_pVRChaperoneSetup;

	public override IntPtr GetIntPtr() { return m_pVRChaperoneSetup; }

	private void CheckIfUsable()
	{
		if (m_pVRChaperoneSetup == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override bool CommitWorkingCopy(string pchCalibrationName)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_CommitWorkingCopy(m_pVRChaperoneSetup,pchCalibrationName);
		return result;
	}
	public override void RevertWorkingCopy()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_RevertWorkingCopy(m_pVRChaperoneSetup);
	}
	public override bool GetWorkingSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingSoftBoundsInfo(m_pVRChaperoneSetup,ref pInfo);
		return result;
	}
	public override bool GetWorkingHardBoundsInfo(out HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		uint punQuadsCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingHardBoundsInfo(m_pVRChaperoneSetup,null,ref punQuadsCount);
		pQuadsBuffer= new HmdQuad_t[punQuadsCount];
		result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingHardBoundsInfo(m_pVRChaperoneSetup,pQuadsBuffer,ref punQuadsCount);
		return result;
	}
	public override bool GetWorkingSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,ref pmatSeatedZeroPoseToRawTrackingPose);
		return result;
	}
	public override bool GetWorkingStandingZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatStandingZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,ref pmatStandingZeroPoseToRawTrackingPose);
		return result;
	}
	public override void SetWorkingSoftBoundsInfo(ref ChaperoneSoftBoundsInfo_t pInfo)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingSoftBoundsInfo(m_pVRChaperoneSetup,ref pInfo);
	}
	public override void SetWorkingHardBoundsInfo(HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingHardBoundsInfo(m_pVRChaperoneSetup,pQuadsBuffer,(uint) pQuadsBuffer.Length);
	}
	public override void SetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr matSeatedZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,matSeatedZeroPoseToRawTrackingPose);
	}
	public override void SetWorkingStandingZeroPoseToRawTrackingPose(IntPtr matStandingZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,matStandingZeroPoseToRawTrackingPose);
	}
	public override bool GetWorkingTagPoses(out HmdMatrix34_t [] pTagPosesBuffer)
	{
		CheckIfUsable();
		uint punTagPosesCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoses(m_pVRChaperoneSetup,null,ref punTagPosesCount);
		pTagPosesBuffer= new HmdMatrix34_t[punTagPosesCount];
		result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoses(m_pVRChaperoneSetup,pTagPosesBuffer,ref punTagPosesCount);
		return result;
	}
	public override bool GetWorkingTagPoseScales(out float [] pflScaleBuffer)
	{
		CheckIfUsable();
		uint punTagPosesCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoseScales(m_pVRChaperoneSetup,null,ref punTagPosesCount);
		pflScaleBuffer= new float[punTagPosesCount];
		result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoseScales(m_pVRChaperoneSetup,pflScaleBuffer,ref punTagPosesCount);
		return result;
	}
	public override uint GetWorkingTagPoseNameByIndex(uint nIndex,System.Text.StringBuilder pchBuffer,uint unBufferSize)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoseNameByIndex(m_pVRChaperoneSetup,nIndex,pchBuffer,unBufferSize);
		return result;
	}
	public override bool GetWorkingTagPoseByName(string pchTagName,ref HmdMatrix34_t pmatTagPose,ref float pflScale)
	{
		CheckIfUsable();
		pflScale = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingTagPoseByName(m_pVRChaperoneSetup,pchTagName,ref pmatTagPose,ref pflScale);
		return result;
	}
	public override void SetWorkingTagPoseByName(string pchTagName,IntPtr matSeatedZeroPoseToRawTrackingPose,float flScale)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingTagPoseByName(m_pVRChaperoneSetup,pchTagName,matSeatedZeroPoseToRawTrackingPose,flScale);
	}
	public override void RemoveWorkingTagPoseByName(string pchTagName)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_RemoveWorkingTagPoseByName(m_pVRChaperoneSetup,pchTagName);
	}
	public override void RemoveAllWorkingTagPoses()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_RemoveAllWorkingTagPoses(m_pVRChaperoneSetup);
	}
	public override void ReloadFromDisk()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_ReloadFromDisk(m_pVRChaperoneSetup);
	}
}


public class OpenVRInterop
{
	[DllImportAttribute("openvr_api", EntryPoint = "VR_Init")]
	internal static extern IntPtr Init(ref HmdError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_Shutdown")]
	internal static extern void Shutdown();
	[DllImportAttribute("openvr_api", EntryPoint = "VR_GetGenericInterface")]
	internal static extern IntPtr GetGenericInterface([In, MarshalAs(UnmanagedType.LPStr)] string pchInterfaceVersion, ref HmdError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IsHmdPresent")]
	internal static extern bool IsHmdPresent();
	[DllImportAttribute("openvr_api", EntryPoint = "VR_GetStringForHmdError")]
	internal static extern IntPtr GetStringForHmdError(HmdError error);
}


public enum Hmd_Eye
{
	Eye_Left = 0,
	Eye_Right = 1,
}
public enum GraphicsAPIConvention
{
	API_DirectX = 0,
	API_OpenGL = 1,
}
public enum HmdTrackingResult
{
	TrackingResult_Uninitialized = 1,
	TrackingResult_Calibrating_InProgress = 100,
	TrackingResult_Calibrating_OutOfRange = 101,
	TrackingResult_Running_OK = 200,
	TrackingResult_Running_OutOfRange = 201,
}
public enum TrackedDeviceClass
{
	Invalid = 0,
	HMD = 1,
	Controller = 2,
	TrackingReference = 4,
	Other = 1000,
}
public enum TrackingUniverseOrigin
{
	TrackingUniverseSeated = 0,
	TrackingUniverseStanding = 1,
	TrackingUniverseRawAndUncalibrated = 2,
}
public enum TrackedDeviceProperty
{
	Prop_TrackingSystemName_String = 1000,
	Prop_ModelNumber_String = 1001,
	Prop_SerialNumber_String = 1002,
	Prop_RenderModelName_String = 1003,
	Prop_WillDriftInYaw_Bool = 1004,
	Prop_ManufacturerName_String = 1005,
	Prop_TrackingFirmwareVersion_String = 1006,
	Prop_HardwareRevision_String = 1007,
	Prop_AllWirelessDongleDescriptions_String = 1008,
	Prop_ConnectedWirelessDongle_String = 1009,
	Prop_DeviceIsWireless_Bool = 1010,
	Prop_DeviceIsCharging_Bool = 1011,
	Prop_DeviceBatteryPercentage_Float = 1012,
	Prop_StatusDisplayTransform_Matrix34 = 1013,
	Prop_ReportsTimeSinceVSync_Bool = 2000,
	Prop_SecondsFromVsyncToPhotons_Float = 2001,
	Prop_DisplayFrequency_Float = 2002,
	Prop_UserIpdMeters_Float = 2003,
	Prop_CurrentUniverseId_Uint64 = 2004,
	Prop_PreviousUniverseId_Uint64 = 2005,
	Prop_DisplayFirmwareVersion_String = 2006,
	Prop_IsOnDesktop_Bool = 2007,
	Prop_AttachedDeviceId_String = 3000,
	Prop_SupportedButtons_Uint64 = 3001,
	Prop_Axis0Type_Int32 = 3002,
	Prop_Axis1Type_Int32 = 3003,
	Prop_Axis2Type_Int32 = 3004,
	Prop_Axis3Type_Int32 = 3005,
	Prop_Axis4Type_Int32 = 3006,
	Prop_FieldOfViewLeftDegrees_Float = 4000,
	Prop_FieldOfViewRightDegrees_Float = 4001,
	Prop_FieldOfViewTopDegrees_Float = 4002,
	Prop_FieldOfViewBottomDegrees_Float = 4003,
	Prop_TrackingRangeMinimumMeters_Float = 4004,
	Prop_TrackingRangeMaximumMeters_Float = 4005,
}
public enum TrackedPropertyError
{
	TrackedProp_Success = 0,
	TrackedProp_WrongDataType = 1,
	TrackedProp_WrongDeviceClass = 2,
	TrackedProp_BufferTooSmall = 3,
	TrackedProp_UnknownProperty = 4,
	TrackedProp_InvalidDevice = 5,
	TrackedProp_CouldNotContactServer = 6,
	TrackedProp_ValueNotProvidedByDevice = 7,
	TrackedProp_StringExceedsMaximumLength = 8,
}
public enum VRStatusState_t
{
	State_OK = 0,
	State_Error = 1,
	State_Warning = 2,
	State_Undefined = 3,
	State_NotSet = 4,
}
public enum EVREventType
{
	VREvent_None = 0,
	VREvent_TrackedDeviceActivated = 100,
	VREvent_TrackedDeviceDeactivated = 101,
	VREvent_TrackedDeviceUpdated = 102,
	VREvent_ButtonPress = 200,
	VREvent_ButtonUnpress = 201,
	VREvent_ButtonTouch = 202,
	VREvent_ButtonUntouch = 203,
	VREvent_MouseMove = 300,
	VREvent_MouseButtonDown = 301,
	VREvent_MouseButtonUp = 302,
	VREvent_InputFocusCaptured = 400,
	VREvent_InputFocusReleased = 401,
	VREvent_SceneFocusLost = 402,
	VREvent_SceneFocusGained = 403,
	VREvent_OverlayShown = 500,
	VREvent_OverlayHidden = 501,
	VREvent_DashboardActivated = 502,
	VREvent_DashboardDeactivated = 503,
	VREvent_DashboardThumbSelected = 504,
	VREvent_DashboardRequested = 505,
	VREvent_ResetDashboard = 506,
	VREvent_RenderToast = 507,
	VREvent_ImageLoaded = 508,
	VREvent_Notification_Show = 600,
	VREvent_Notification_Dismissed = 601,
	VREvent_Notification_BeginInteraction = 602,
	VREvent_Quit = 700,
	VREvent_ProcessQuit = 701,
	VREvent_ChaperoneDataHasChanged = 800,
	VREvent_ChaperoneUniverseHasChanged = 801,
	VREvent_StatusUpdate = 900,
}
public enum EVRButtonId
{
	k_EButton_System = 0,
	k_EButton_ApplicationMenu = 1,
	k_EButton_Grip = 2,
	k_EButton_Axis0 = 32,
	k_EButton_Axis1 = 33,
	k_EButton_Axis2 = 34,
	k_EButton_Axis3 = 35,
	k_EButton_Axis4 = 36,
	k_EButton_SteamVR_Touchpad = 32,
	k_EButton_SteamVR_Trigger = 33,
	k_EButton_Max = 64,
}
public enum EVRMouseButton
{
	VRMouseButton_Left = 1,
	VRMouseButton_Right = 2,
	VRMouseButton_Middle = 4,
}
public enum EVRControllerAxisType
{
	k_eControllerAxis_None = 0,
	k_eControllerAxis_TrackPad = 1,
	k_eControllerAxis_Joystick = 2,
	k_eControllerAxis_Trigger = 3,
}
public enum EVRControllerEventOutputType
{
	ControllerEventOutput_OSEvents = 0,
	ControllerEventOutput_VREvents = 1,
}
public enum VROverlayError
{
	None = 0,
	UnknownOverlay = 10,
	InvalidHandle = 11,
	PermissionDenied = 12,
	OverlayLimitExceeded = 13,
	WrongVisibilityType = 14,
	KeyTooLong = 15,
	NameTooLong = 16,
	KeyInUse = 17,
	WrongTransformType = 18,
	InvalidTrackedDevice = 19,
	InvalidParameter = 20,
	ThumbnailCantBeDestroyed = 21,
	ArrayTooSmall = 22,
	RequestFailed = 23,
	InvalidTexture = 24,
	UnableToLoadFile = 25,
}
public enum HmdError
{
	None = 0,
	Unknown = 1,
	Init_InstallationNotFound = 100,
	Init_InstallationCorrupt = 101,
	Init_VRClientDLLNotFound = 102,
	Init_FileNotFound = 103,
	Init_FactoryNotFound = 104,
	Init_InterfaceNotFound = 105,
	Init_InvalidInterface = 106,
	Init_UserConfigDirectoryInvalid = 107,
	Init_HmdNotFound = 108,
	Init_NotInitialized = 109,
	Init_PathRegistryNotFound = 110,
	Init_NoConfigPath = 111,
	Init_NoLogPath = 112,
	Init_PathRegistryNotWritable = 113,
	Init_AppInfoInitFailed = 114,
	Driver_Failed = 200,
	Driver_Unknown = 201,
	Driver_HmdUnknown = 202,
	Driver_NotLoaded = 203,
	Driver_RuntimeOutOfDate = 204,
	Driver_HmdInUse = 205,
	Driver_NotCalibrated = 206,
	Driver_CalibrationInvalid = 207,
	Driver_HmdDisplayNotFound = 208,
	IPC_ServerInitFailed = 300,
	IPC_ConnectFailed = 301,
	IPC_SharedStateInitFailed = 302,
	IPC_CompositorInitFailed = 303,
	IPC_MutexInitFailed = 304,
	VendorSpecific_UnableToConnectToOculusRuntime = 1000,
	Steam_SteamInstallationNotFound = 2000,
}
public enum VRCompositorError
{
	None = 0,
	IncompatibleVersion = 100,
	DoNotHaveFocus = 101,
	InvalidTexture = 102,
}
public enum ChaperoneCalibrationState
{
	OK = 1,
	Warning = 100,
	Warning_BaseStationMayHaveMoved = 101,
	Warning_BaseStationRemoved = 102,
	Warning_SeatedBoundsInvalid = 103,
	Error = 200,
	Error_BaseStationUninitalized = 201,
	Error_BaseStationConflict = 202,
	Error_SoftBoundsInvalid = 203,
	Error_HardBoundsInvalid = 204,
}
public enum VROverlayInputMethod
{
	None = 0,
	Mouse = 1,
}
public enum VROverlayTransformType
{
	VROverlayTransform_Absolute = 0,
	VROverlayTransform_TrackedDeviceRelative = 1,
	VROverlayTransform_SystemOverlay = 2,
}
public enum VROverlayFlags
{
	None = 0,
	Curved = 1,
	RGSS4X = 2,
	NoDashboardTab = 3,
}
public enum NotificationError_t
{
	k_ENotificationError_OK = 0,
	k_ENotificationError_Fail = 1,
	k_eNotificationError_InvalidParam = 2,
}
public enum CameraImageResult
{
	OK = 0,
	Uninitalized = 1,
	NotReady = 2,
	SameFrame = 3,
}

[StructLayout(LayoutKind.Explicit)] public struct VREvent_Data_t
{
	[FieldOffset(0)] public VREvent_Reserved_t reserved;
	[FieldOffset(0)] public VREvent_Controller_t controller;
	[FieldOffset(0)] public VREvent_Mouse_t mouse;
}

[StructLayout(LayoutKind.Sequential)] public struct HmdMatrix34_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 12, ArraySubType = UnmanagedType.R4)]
	public float[] m; //float[3][4]
}
[StructLayout(LayoutKind.Sequential)] public struct HmdMatrix44_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.R4)]
	public float[] m; //float[4][4]
}
[StructLayout(LayoutKind.Sequential)] public struct HmdVector3_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3, ArraySubType = UnmanagedType.R4)]
	public float[] v; //float[3]
}
[StructLayout(LayoutKind.Sequential)] public struct HmdVector3d_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 3, ArraySubType = UnmanagedType.R8)]
	public double[] v; //double[3]
}
[StructLayout(LayoutKind.Sequential)] public struct HmdVector2_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
	public float[] v; //float[2]
}
[StructLayout(LayoutKind.Sequential)] public struct HmdQuaternion_t
{
	public double w;
	public double x;
	public double y;
	public double z;
}
[StructLayout(LayoutKind.Sequential)] public struct HmdQuad_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4, ArraySubType = UnmanagedType.Struct)]
	public HmdVector3_t[] vCorners; //HmdVector3_t[4]
}
[StructLayout(LayoutKind.Sequential)] public struct DistortionCoordinates_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
	public float[] rfRed; //float[2]
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
	public float[] rfGreen; //float[2]
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
	public float[] rfBlue; //float[2]
}
[StructLayout(LayoutKind.Sequential)] public struct TrackedDevicePose_t
{
	public HmdMatrix34_t mDeviceToAbsoluteTracking;
	public HmdVector3_t vVelocity;
	public HmdVector3_t vAngularVelocity;
	public HmdTrackingResult eTrackingResult;
	[MarshalAs(UnmanagedType.I1)]
	public bool bPoseIsValid;
	[MarshalAs(UnmanagedType.I1)]
	public bool bDeviceIsConnected;
}
[StructLayout(LayoutKind.Sequential)] public struct RenderModel_Vertex_t
{
	public HmdVector3_t vPosition;
	public HmdVector3_t vNormal;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.R4)]
	public float[] rfTextureCoord; //float[2]
}
[StructLayout(LayoutKind.Sequential)] public struct RenderModel_TextureMap_t
{
	public char unWidth;
	public char unHeight;
	public IntPtr rubTextureMapData; // const uint8_t *
}
[StructLayout(LayoutKind.Sequential)] public struct RenderModel_t
{
	public ulong ulInternalHandle;
	public IntPtr rVertexData; // const struct vr::RenderModel_Vertex_t *
	public uint unVertexCount;
	public IntPtr rIndexData; // const uint16_t *
	public uint unTriangleCount;
	public RenderModel_TextureMap_t diffuseTexture;
}
[StructLayout(LayoutKind.Sequential)] public struct VRTextureBounds_t
{
	public float uMin;
	public float vMin;
	public float uMax;
	public float vMax;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Controller_t
{
	public uint button;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Mouse_t
{
	public float x;
	public float y;
	public uint button;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Notification_t
{
	public ulong ulUserValue;
	public uint notificationId;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Process_t
{
	public uint pid;
	public uint oldPid;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Overlay_t
{
	public ulong overlayHandle;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Status_t
{
	public VRStatusState_t statusState;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Reserved_t
{
	public ulong reserved0;
	public ulong reserved1;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_t
{
	public uint eventType;
	public uint trackedDeviceIndex;
	public VREvent_Data_t data;
	public float eventAgeSeconds;
}
[StructLayout(LayoutKind.Sequential)] public struct HiddenAreaMesh_t
{
	public IntPtr pVertexData; // const struct vr::HmdVector2_t *
	public uint unTriangleCount;
}
[StructLayout(LayoutKind.Sequential)] public struct VRControllerAxis_t
{
	public float x;
	public float y;
}
[StructLayout(LayoutKind.Sequential)] public struct VRControllerState_t
{
	public uint unPacketNum;
	public ulong ulButtonPressed;
	public ulong ulButtonTouched;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 5, ArraySubType = UnmanagedType.Struct)]
	public VRControllerAxis_t[] rAxis; //VRControllerAxis_t[5]
}
[StructLayout(LayoutKind.Sequential)] public struct Compositor_OverlaySettings
{
	public uint size;
	[MarshalAs(UnmanagedType.I1)]
	public bool curved;
	[MarshalAs(UnmanagedType.I1)]
	public bool antialias;
	public float scale;
	public float distance;
	public float alpha;
	public float uOffset;
	public float vOffset;
	public float uScale;
	public float vScale;
	public float gridDivs;
	public float gridWidth;
	public float gridScale;
	public HmdMatrix44_t transform;
}
[StructLayout(LayoutKind.Sequential)] public struct Compositor_FrameTiming
{
	public uint size;
	public double frameStart;
	public float frameVSync;
	public uint droppedFrames;
	public uint frameIndex;
	public TrackedDevicePose_t pose;
	public float prediction;
	public float m_flFrameIntervalMs;
	public float m_flSceneRenderCpuMs;
	public float m_flSceneRenderGpuMs;
	public float m_flCompositorRenderCpuMs;
	public float m_flCompositorRenderGpuMs;
	public float m_flPresentCallCpuMs;
	public float m_flRunningStartMs;
}
[StructLayout(LayoutKind.Sequential)] public struct ChaperoneSoftBoundsInfo_t
{
	public HmdQuad_t quadCorners;
}
[StructLayout(LayoutKind.Sequential)] public struct ChaperoneSeatedBoundsInfo_t
{
	public HmdVector3_t vSeatedHeadPosition;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.Struct)]
	public HmdVector3_t[] vDeskEdgePositions; //HmdVector3_t[2]
}
[StructLayout(LayoutKind.Sequential)] public struct VROverlayIntersectionParams_t
{
	public HmdVector3_t vSource;
	public HmdVector3_t vDirection;
	public TrackingUniverseOrigin eOrigin;
}
[StructLayout(LayoutKind.Sequential)] public struct VROverlayIntersectionResults_t
{
	public HmdVector3_t vPoint;
	public HmdVector3_t vNormal;
	public HmdVector2_t vUVs;
	public float fDistance;
}
[StructLayout(LayoutKind.Sequential)] public struct NotificationBitmap
{
	public IntPtr bytes; // char *
	public int width;
	public int height;
	public int depth;
}
[StructLayout(LayoutKind.Sequential)] public struct NotificationItem
{
	public uint notificationId;
}
[StructLayout(LayoutKind.Sequential)] public struct CameraInfo_t
{
	public uint width;
	public uint height;
	public uint depth;
	public float fx;
	public float cx;
	public float fy;
	public float cy;
}
[StructLayout(LayoutKind.Sequential)] public struct CameraImage_t
{
	public int frameID;
	public HmdMatrix34_t pose;
	public IntPtr pBuffer; // unsigned char *
	public uint unBufferLen;
	public CameraImageResult result;
}

public class OpenVR
{
	public static IntPtr Init(ref HmdError peError)
	{
		return OpenVRInterop.Init(ref peError);
	}

	public static void Shutdown()
	{
		OpenVRInterop.Shutdown();
	}

	public static IntPtr GetGenericInterface(string pchInterfaceVersion, ref HmdError peError)
	{
		return OpenVRInterop.GetGenericInterface(pchInterfaceVersion, ref peError);
	}

	public static bool IsHmdPresent()
	{
		return OpenVRInterop.IsHmdPresent();
	}

	public static string GetStringForHmdError(HmdError error)
	{
		return Marshal.PtrToStringAnsi(OpenVRInterop.GetStringForHmdError(error));
	}

	public const uint k_unTrackingStringSize = 32;
	public const uint k_unMaxTrackedDeviceCount = 16;
	public const uint k_unTrackedDeviceIndex_Hmd = 0;
	public const uint k_unMaxDriverDebugResponseSize = 32768;
	public const uint k_unTrackedDeviceIndexInvalid = 4294967295;
	public const uint k_unMaxPropertyStringSize = 32768;
	public const uint k_unControllerStateAxisCount = 5;
	public const ulong k_ulOverlayHandleInvalid = 0;
	public const string IVRSystem_Version = "IVRSystem_005";
	public const string IVRCompositor_Version = "IVRCompositor_007";
	public const string IVRChaperone_Version = "IVRChaperone_002";
	public const uint k_unVROverlayMaxKeyLength = 128;
	public const uint k_unVROverlayMaxNameLength = 128;
	public const uint k_unMaxOverlayCount = 32;
	public const string IVROverlay_Version = "IVROverlay_003";
	public const string IVRRenderModels_Version = "IVRRenderModels_001";
	public const string IVRControlPanel_Version = "IVRControlPanel_001";
	public const uint k_unNotificationTypeMaxSize = 16;
	public const uint k_unNotificationTextMaxSize = 128;
	public const uint k_unNotificationCatagoryMaxSize = 32;
	public const string IVRNotifications_Version = "IVRNotifications_001";
	public const string IVRCameraAccess_Version = "IVRCameraAccess_001";
	public const string IVRChaperoneSetup_Version = "IVRChaperoneSetup_001";
}



}

