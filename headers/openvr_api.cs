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


	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetRecommendedRenderTargetSize")]
	internal static extern void VR_IVRSystem_GetRecommendedRenderTargetSize(IntPtr instancePtr, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetProjectionMatrix")]
	internal static extern HmdMatrix44_t VR_IVRSystem_GetProjectionMatrix(IntPtr instancePtr, EVREye eEye, float fNearZ, float fFarZ, EGraphicsAPIConvention eProjType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetProjectionRaw")]
	internal static extern void VR_IVRSystem_GetProjectionRaw(IntPtr instancePtr, EVREye eEye, ref float pfLeft, ref float pfRight, ref float pfTop, ref float pfBottom);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ComputeDistortion")]
	internal static extern DistortionCoordinates_t VR_IVRSystem_ComputeDistortion(IntPtr instancePtr, EVREye eEye, float fU, float fV);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetEyeToHeadTransform")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetEyeToHeadTransform(IntPtr instancePtr, EVREye eEye);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetTimeSinceLastVsync")]
	internal static extern bool VR_IVRSystem_GetTimeSinceLastVsync(IntPtr instancePtr, ref float pfSecondsSinceLastVsync, ref ulong pulFrameCounter);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetD3D9AdapterIndex")]
	internal static extern int VR_IVRSystem_GetD3D9AdapterIndex(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetDXGIOutputInfo")]
	internal static extern void VR_IVRSystem_GetDXGIOutputInfo(IntPtr instancePtr, ref int pnAdapterIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_IsDisplayOnDesktop")]
	internal static extern bool VR_IVRSystem_IsDisplayOnDesktop(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_SetDisplayVisibility")]
	internal static extern bool VR_IVRSystem_SetDisplayVisibility(IntPtr instancePtr, bool bIsVisibleOnDesktop);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetDeviceToAbsoluteTrackingPose")]
	internal static extern void VR_IVRSystem_GetDeviceToAbsoluteTrackingPose(IntPtr instancePtr, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow,  [In, Out] TrackedDevicePose_t[] pTrackedDevicePoseArray, uint unTrackedDevicePoseArrayCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ResetSeatedZeroPose")]
	internal static extern void VR_IVRSystem_ResetSeatedZeroPose(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetRawZeroPoseToStandingAbsoluteTrackingPose")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetRawZeroPoseToStandingAbsoluteTrackingPose(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass")]
	internal static extern uint VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass(IntPtr instancePtr, ETrackedDeviceClass eTrackedDeviceClass,  [In, Out] uint[] punTrackedDeviceIndexArray, uint unTrackedDeviceIndexArrayCount, uint unRelativeToTrackedDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetTrackedDeviceActivityLevel")]
	internal static extern EDeviceActivityLevel VR_IVRSystem_GetTrackedDeviceActivityLevel(IntPtr instancePtr, uint unDeviceId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ApplyTransform")]
	internal static extern void VR_IVRSystem_ApplyTransform(IntPtr instancePtr, ref TrackedDevicePose_t pOutputPose, ref TrackedDevicePose_t pTrackedDevicePose, ref HmdMatrix34_t pTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetTrackedDeviceClass")]
	internal static extern ETrackedDeviceClass VR_IVRSystem_GetTrackedDeviceClass(IntPtr instancePtr, uint unDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_IsTrackedDeviceConnected")]
	internal static extern bool VR_IVRSystem_IsTrackedDeviceConnected(IntPtr instancePtr, uint unDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetBoolTrackedDeviceProperty")]
	internal static extern bool VR_IVRSystem_GetBoolTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetFloatTrackedDeviceProperty")]
	internal static extern float VR_IVRSystem_GetFloatTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetInt32TrackedDeviceProperty")]
	internal static extern int VR_IVRSystem_GetInt32TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetUint64TrackedDeviceProperty")]
	internal static extern ulong VR_IVRSystem_GetUint64TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetMatrix34TrackedDeviceProperty")]
	internal static extern HmdMatrix34_t VR_IVRSystem_GetMatrix34TrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetStringTrackedDeviceProperty")]
	internal static extern uint VR_IVRSystem_GetStringTrackedDeviceProperty(IntPtr instancePtr, uint unDeviceIndex, ETrackedDeviceProperty prop, System.Text.StringBuilder pchValue, uint unBufferSize, ref ETrackedPropertyError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetPropErrorNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetPropErrorNameFromEnum(IntPtr instancePtr, ETrackedPropertyError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_PollNextEvent")]
	internal static extern bool VR_IVRSystem_PollNextEvent(IntPtr instancePtr, ref VREvent_t pEvent);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_PollNextEventWithPose")]
	internal static extern bool VR_IVRSystem_PollNextEventWithPose(IntPtr instancePtr, ETrackingUniverseOrigin eOrigin, ref VREvent_t pEvent, ref TrackedDevicePose_t pTrackedDevicePose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetEventTypeNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetEventTypeNameFromEnum(IntPtr instancePtr, EVREventType eType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetHiddenAreaMesh")]
	internal static extern HiddenAreaMesh_t VR_IVRSystem_GetHiddenAreaMesh(IntPtr instancePtr, EVREye eEye);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerState")]
	internal static extern bool VR_IVRSystem_GetControllerState(IntPtr instancePtr, uint unControllerDeviceIndex, ref VRControllerState_t pControllerState);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerStateWithPose")]
	internal static extern bool VR_IVRSystem_GetControllerStateWithPose(IntPtr instancePtr, ETrackingUniverseOrigin eOrigin, uint unControllerDeviceIndex, ref VRControllerState_t pControllerState, ref TrackedDevicePose_t pTrackedDevicePose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_TriggerHapticPulse")]
	internal static extern void VR_IVRSystem_TriggerHapticPulse(IntPtr instancePtr, uint unControllerDeviceIndex, uint unAxisId, char usDurationMicroSec);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetButtonIdNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetButtonIdNameFromEnum(IntPtr instancePtr, EVRButtonId eButtonId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_GetControllerAxisTypeNameFromEnum")]
	internal static extern IntPtr VR_IVRSystem_GetControllerAxisTypeNameFromEnum(IntPtr instancePtr, EVRControllerAxisType eAxisType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_CaptureInputFocus")]
	internal static extern bool VR_IVRSystem_CaptureInputFocus(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_ReleaseInputFocus")]
	internal static extern void VR_IVRSystem_ReleaseInputFocus(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_IsInputFocusCapturedByAnotherProcess")]
	internal static extern bool VR_IVRSystem_IsInputFocusCapturedByAnotherProcess(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_DriverDebugRequest")]
	internal static extern uint VR_IVRSystem_DriverDebugRequest(IntPtr instancePtr, uint unDeviceIndex, string pchRequest, string pchResponseBuffer, uint unResponseBufferSize);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_PerformFirmwareUpdate")]
	internal static extern EVRFirmwareError VR_IVRSystem_PerformFirmwareUpdate(IntPtr instancePtr, uint unDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_AcknowledgeQuit_Exiting")]
	internal static extern void VR_IVRSystem_AcknowledgeQuit_Exiting(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSystem_AcknowledgeQuit_UserPrompt")]
	internal static extern void VR_IVRSystem_AcknowledgeQuit_UserPrompt(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRExtendedDisplay_GetWindowBounds")]
	internal static extern void VR_IVRExtendedDisplay_GetWindowBounds(IntPtr instancePtr, ref int pnX, ref int pnY, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRExtendedDisplay_GetEyeOutputViewport")]
	internal static extern void VR_IVRExtendedDisplay_GetEyeOutputViewport(IntPtr instancePtr, EVREye eEye, ref uint pnX, ref uint pnY, ref uint pnWidth, ref uint pnHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRExtendedDisplay_GetDXGIOutputInfo")]
	internal static extern void VR_IVRExtendedDisplay_GetDXGIOutputInfo(IntPtr instancePtr, ref int pnAdapterIndex, ref int pnAdapterOutputIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_AddApplicationManifest")]
	internal static extern EVRApplicationError VR_IVRApplications_AddApplicationManifest(IntPtr instancePtr, string pchApplicationManifestFullPath, bool bTemporary);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_RemoveApplicationManifest")]
	internal static extern EVRApplicationError VR_IVRApplications_RemoveApplicationManifest(IntPtr instancePtr, string pchApplicationManifestFullPath);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_IsApplicationInstalled")]
	internal static extern bool VR_IVRApplications_IsApplicationInstalled(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationCount")]
	internal static extern uint VR_IVRApplications_GetApplicationCount(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationKeyByIndex")]
	internal static extern EVRApplicationError VR_IVRApplications_GetApplicationKeyByIndex(IntPtr instancePtr, uint unApplicationIndex, string pchAppKeyBuffer, uint unAppKeyBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationKeyByProcessId")]
	internal static extern EVRApplicationError VR_IVRApplications_GetApplicationKeyByProcessId(IntPtr instancePtr, uint unProcessId, string pchAppKeyBuffer, uint unAppKeyBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_LaunchApplication")]
	internal static extern EVRApplicationError VR_IVRApplications_LaunchApplication(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_LaunchDashboardOverlay")]
	internal static extern EVRApplicationError VR_IVRApplications_LaunchDashboardOverlay(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_IdentifyApplication")]
	internal static extern EVRApplicationError VR_IVRApplications_IdentifyApplication(IntPtr instancePtr, uint unProcessId, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationProcessId")]
	internal static extern uint VR_IVRApplications_GetApplicationProcessId(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationsErrorNameFromEnum")]
	internal static extern IntPtr VR_IVRApplications_GetApplicationsErrorNameFromEnum(IntPtr instancePtr, EVRApplicationError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationPropertyString")]
	internal static extern uint VR_IVRApplications_GetApplicationPropertyString(IntPtr instancePtr, string pchAppKey, EVRApplicationProperty eProperty, string pchPropertyValueBuffer, uint unPropertyValueBufferLen, ref EVRApplicationError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationPropertyBool")]
	internal static extern bool VR_IVRApplications_GetApplicationPropertyBool(IntPtr instancePtr, string pchAppKey, EVRApplicationProperty eProperty, ref EVRApplicationError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_SetApplicationAutoLaunch")]
	internal static extern EVRApplicationError VR_IVRApplications_SetApplicationAutoLaunch(IntPtr instancePtr, string pchAppKey, bool bAutoLaunch);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationAutoLaunch")]
	internal static extern bool VR_IVRApplications_GetApplicationAutoLaunch(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetStartingApplication")]
	internal static extern EVRApplicationError VR_IVRApplications_GetStartingApplication(IntPtr instancePtr, string pchAppKeyBuffer, uint unAppKeyBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetTransitionState")]
	internal static extern EVRApplicationTransitionState VR_IVRApplications_GetTransitionState(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_PerformApplicationPrelaunchCheck")]
	internal static extern EVRApplicationError VR_IVRApplications_PerformApplicationPrelaunchCheck(IntPtr instancePtr, string pchAppKey);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_GetApplicationsTransitionStateNameFromEnum")]
	internal static extern IntPtr VR_IVRApplications_GetApplicationsTransitionStateNameFromEnum(IntPtr instancePtr, EVRApplicationTransitionState state);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRApplications_IsQuitUserPromptRequested")]
	internal static extern bool VR_IVRApplications_IsQuitUserPromptRequested(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetCalibrationState")]
	internal static extern ChaperoneCalibrationState VR_IVRChaperone_GetCalibrationState(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetPlayAreaSize")]
	internal static extern bool VR_IVRChaperone_GetPlayAreaSize(IntPtr instancePtr, ref float pSizeX, ref float pSizeZ);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetPlayAreaRect")]
	internal static extern bool VR_IVRChaperone_GetPlayAreaRect(IntPtr instancePtr, ref HmdQuad_t rect);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_ReloadInfo")]
	internal static extern void VR_IVRChaperone_ReloadInfo(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_SetSceneColor")]
	internal static extern void VR_IVRChaperone_SetSceneColor(IntPtr instancePtr, HmdColor_t color);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_GetBoundsColor")]
	internal static extern void VR_IVRChaperone_GetBoundsColor(IntPtr instancePtr, ref HmdColor_t pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_AreBoundsVisible")]
	internal static extern bool VR_IVRChaperone_AreBoundsVisible(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperone_ForceBoundsVisible")]
	internal static extern void VR_IVRChaperone_ForceBoundsVisible(IntPtr instancePtr, bool bForce);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_CommitWorkingCopy")]
	internal static extern bool VR_IVRChaperoneSetup_CommitWorkingCopy(IntPtr instancePtr, EChaperoneConfigFile configFile);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_RevertWorkingCopy")]
	internal static extern void VR_IVRChaperoneSetup_RevertWorkingCopy(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingPlayAreaSize")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingPlayAreaSize(IntPtr instancePtr, ref float pSizeX, ref float pSizeZ);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingPlayAreaRect")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingPlayAreaRect(IntPtr instancePtr, ref HmdQuad_t rect);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingCollisionBoundsInfo")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingCollisionBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, ref uint punQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetLiveCollisionBoundsInfo")]
	internal static extern bool VR_IVRChaperoneSetup_GetLiveCollisionBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, ref uint punQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose")]
	internal static extern bool VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pmatStandingZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingPlayAreaSize")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingPlayAreaSize(IntPtr instancePtr, float sizeX, float sizeZ);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingCollisionBoundsInfo")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingCollisionBoundsInfo(IntPtr instancePtr,  [In, Out] HmdQuad_t[] pQuadsBuffer, uint unQuadsCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pMatSeatedZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose")]
	internal static extern void VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pMatStandingZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_ReloadFromDisk")]
	internal static extern void VR_IVRChaperoneSetup_ReloadFromDisk(IntPtr instancePtr, EChaperoneConfigFile configFile);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRChaperoneSetup_GetLiveSeatedZeroPoseToRawTrackingPose")]
	internal static extern bool VR_IVRChaperoneSetup_GetLiveSeatedZeroPoseToRawTrackingPose(IntPtr instancePtr, ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_SetTrackingSpace")]
	internal static extern void VR_IVRCompositor_SetTrackingSpace(IntPtr instancePtr, ETrackingUniverseOrigin eOrigin);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetTrackingSpace")]
	internal static extern ETrackingUniverseOrigin VR_IVRCompositor_GetTrackingSpace(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_WaitGetPoses")]
	internal static extern EVRCompositorError VR_IVRCompositor_WaitGetPoses(IntPtr instancePtr,  [In, Out] TrackedDevicePose_t[] pRenderPoseArray, uint unRenderPoseArrayCount,  [In, Out] TrackedDevicePose_t[] pGamePoseArray, uint unGamePoseArrayCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetLastPoses")]
	internal static extern EVRCompositorError VR_IVRCompositor_GetLastPoses(IntPtr instancePtr,  [In, Out] TrackedDevicePose_t[] pRenderPoseArray, uint unRenderPoseArrayCount,  [In, Out] TrackedDevicePose_t[] pGamePoseArray, uint unGamePoseArrayCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_Submit")]
	internal static extern EVRCompositorError VR_IVRCompositor_Submit(IntPtr instancePtr, EVREye eEye, ref Texture_t pTexture, ref VRTextureBounds_t pBounds, EVRSubmitFlags nSubmitFlags);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_ClearLastSubmittedFrame")]
	internal static extern void VR_IVRCompositor_ClearLastSubmittedFrame(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_PostPresentHandoff")]
	internal static extern void VR_IVRCompositor_PostPresentHandoff(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetFrameTiming")]
	internal static extern bool VR_IVRCompositor_GetFrameTiming(IntPtr instancePtr, ref Compositor_FrameTiming pTiming, uint unFramesAgo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetFrameTimeRemaining")]
	internal static extern float VR_IVRCompositor_GetFrameTimeRemaining(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_FadeToColor")]
	internal static extern void VR_IVRCompositor_FadeToColor(IntPtr instancePtr, float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_FadeGrid")]
	internal static extern void VR_IVRCompositor_FadeGrid(IntPtr instancePtr, float fSeconds, bool bFadeIn);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_SetSkyboxOverride")]
	internal static extern EVRCompositorError VR_IVRCompositor_SetSkyboxOverride(IntPtr instancePtr,  [In, Out] Texture_t[] pTextures, uint unTextureCount);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_ClearSkyboxOverride")]
	internal static extern void VR_IVRCompositor_ClearSkyboxOverride(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorBringToFront")]
	internal static extern void VR_IVRCompositor_CompositorBringToFront(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorGoToBack")]
	internal static extern void VR_IVRCompositor_CompositorGoToBack(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorQuit")]
	internal static extern void VR_IVRCompositor_CompositorQuit(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_IsFullscreen")]
	internal static extern bool VR_IVRCompositor_IsFullscreen(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetCurrentSceneFocusProcess")]
	internal static extern uint VR_IVRCompositor_GetCurrentSceneFocusProcess(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_GetLastFrameRenderer")]
	internal static extern uint VR_IVRCompositor_GetLastFrameRenderer(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CanRenderScene")]
	internal static extern bool VR_IVRCompositor_CanRenderScene(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_ShowMirrorWindow")]
	internal static extern void VR_IVRCompositor_ShowMirrorWindow(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_HideMirrorWindow")]
	internal static extern void VR_IVRCompositor_HideMirrorWindow(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_IsMirrorWindowVisible")]
	internal static extern bool VR_IVRCompositor_IsMirrorWindowVisible(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRCompositor_CompositorDumpImages")]
	internal static extern void VR_IVRCompositor_CompositorDumpImages(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_FindOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_FindOverlay(IntPtr instancePtr, string pchOverlayKey, ref ulong pOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_CreateOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_CreateOverlay(IntPtr instancePtr, string pchOverlayKey, string pchOverlayFriendlyName, ref ulong pOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_DestroyOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_DestroyOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetHighQualityOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_SetHighQualityOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetHighQualityOverlay")]
	internal static extern ulong VR_IVROverlay_GetHighQualityOverlay(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayKey")]
	internal static extern uint VR_IVROverlay_GetOverlayKey(IntPtr instancePtr, ulong ulOverlayHandle, System.Text.StringBuilder pchValue, uint unBufferSize, ref EVROverlayError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayName")]
	internal static extern uint VR_IVROverlay_GetOverlayName(IntPtr instancePtr, ulong ulOverlayHandle, System.Text.StringBuilder pchValue, uint unBufferSize, ref EVROverlayError pError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayImageData")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayImageData(IntPtr instancePtr, ulong ulOverlayHandle, IntPtr pvBuffer, uint unBufferSize, ref uint punWidth, ref uint punHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayErrorNameFromEnum")]
	internal static extern IntPtr VR_IVROverlay_GetOverlayErrorNameFromEnum(IntPtr instancePtr, EVROverlayError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayFlag")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayFlag(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayFlag")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayFlag(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayFlags eOverlayFlag, ref bool pbEnabled);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayColor")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayColor(IntPtr instancePtr, ulong ulOverlayHandle, float fRed, float fGreen, float fBlue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayColor")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayColor(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfRed, ref float pfGreen, ref float pfBlue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayAlpha")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayAlpha(IntPtr instancePtr, ulong ulOverlayHandle, float fAlpha);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayAlpha")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayAlpha(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfAlpha);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayWidthInMeters")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayWidthInMeters(IntPtr instancePtr, ulong ulOverlayHandle, float fWidthInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayWidthInMeters")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayWidthInMeters(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfWidthInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayAutoCurveDistanceRangeInMeters")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayAutoCurveDistanceRangeInMeters(IntPtr instancePtr, ulong ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayAutoCurveDistanceRangeInMeters")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayAutoCurveDistanceRangeInMeters(IntPtr instancePtr, ulong ulOverlayHandle, ref float pfMinDistanceInMeters, ref float pfMaxDistanceInMeters);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTextureColorSpace")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayTextureColorSpace(IntPtr instancePtr, ulong ulOverlayHandle, EColorSpace eTextureColorSpace);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTextureColorSpace")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayTextureColorSpace(IntPtr instancePtr, ulong ulOverlayHandle, ref EColorSpace peTextureColorSpace);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTextureBounds")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayTextureBounds(IntPtr instancePtr, ulong ulOverlayHandle, ref VRTextureBounds_t pOverlayTextureBounds);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTextureBounds")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayTextureBounds(IntPtr instancePtr, ulong ulOverlayHandle, ref VRTextureBounds_t pOverlayTextureBounds);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformType")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayTransformType(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayTransformType peTransformType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTransformAbsolute")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayTransformAbsolute(IntPtr instancePtr, ulong ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformAbsolute")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayTransformAbsolute(IntPtr instancePtr, ulong ulOverlayHandle, ref ETrackingUniverseOrigin peTrackingOrigin, ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative(IntPtr instancePtr, ulong ulOverlayHandle, uint unTrackedDevice, ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative(IntPtr instancePtr, ulong ulOverlayHandle, ref uint punTrackedDevice, ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_ShowOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_HideOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_HideOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsOverlayVisible")]
	internal static extern bool VR_IVROverlay_IsOverlayVisible(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_PollNextOverlayEvent")]
	internal static extern bool VR_IVROverlay_PollNextOverlayEvent(IntPtr instancePtr, ulong ulOverlayHandle, ref VREvent_t pEvent);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayInputMethod")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayInputMethod(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayInputMethod peInputMethod);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayInputMethod")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayInputMethod(IntPtr instancePtr, ulong ulOverlayHandle, VROverlayInputMethod eInputMethod);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetOverlayMouseScale")]
	internal static extern EVROverlayError VR_IVROverlay_GetOverlayMouseScale(IntPtr instancePtr, ulong ulOverlayHandle, ref HmdVector2_t pvecMouseScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayMouseScale")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayMouseScale(IntPtr instancePtr, ulong ulOverlayHandle, ref HmdVector2_t pvecMouseScale);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ComputeOverlayIntersection")]
	internal static extern bool VR_IVROverlay_ComputeOverlayIntersection(IntPtr instancePtr, ulong ulOverlayHandle, ref VROverlayIntersectionParams_t pParams, ref VROverlayIntersectionResults_t pResults);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_HandleControllerOverlayInteractionAsMouse")]
	internal static extern bool VR_IVROverlay_HandleControllerOverlayInteractionAsMouse(IntPtr instancePtr, ulong ulOverlayHandle, uint unControllerDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsHoverTargetOverlay")]
	internal static extern bool VR_IVROverlay_IsHoverTargetOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetGamepadFocusOverlay")]
	internal static extern ulong VR_IVROverlay_GetGamepadFocusOverlay(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetGamepadFocusOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_SetGamepadFocusOverlay(IntPtr instancePtr, ulong ulNewFocusOverlay);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayNeighbor")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayNeighbor(IntPtr instancePtr, EOverlayDirection eDirection, ulong ulFrom, ulong ulTo);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_MoveGamepadFocusToNeighbor")]
	internal static extern EVROverlayError VR_IVROverlay_MoveGamepadFocusToNeighbor(IntPtr instancePtr, EOverlayDirection eDirection, ulong ulFrom);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayTexture")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayTexture(IntPtr instancePtr, ulong ulOverlayHandle, ref Texture_t pTexture);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ClearOverlayTexture")]
	internal static extern EVROverlayError VR_IVROverlay_ClearOverlayTexture(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayRaw")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayRaw(IntPtr instancePtr, ulong ulOverlayHandle, IntPtr pvBuffer, uint unWidth, uint unHeight, uint unDepth);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetOverlayFromFile")]
	internal static extern EVROverlayError VR_IVROverlay_SetOverlayFromFile(IntPtr instancePtr, ulong ulOverlayHandle, string pchFilePath);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_CreateDashboardOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_CreateDashboardOverlay(IntPtr instancePtr, string pchOverlayKey, string pchOverlayFriendlyName, ref ulong pMainHandle, ref ulong pThumbnailHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsDashboardVisible")]
	internal static extern bool VR_IVROverlay_IsDashboardVisible(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_IsActiveDashboardOverlay")]
	internal static extern bool VR_IVROverlay_IsActiveDashboardOverlay(IntPtr instancePtr, ulong ulOverlayHandle);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_SetDashboardOverlaySceneProcess")]
	internal static extern EVROverlayError VR_IVROverlay_SetDashboardOverlaySceneProcess(IntPtr instancePtr, ulong ulOverlayHandle, uint unProcessId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetDashboardOverlaySceneProcess")]
	internal static extern EVROverlayError VR_IVROverlay_GetDashboardOverlaySceneProcess(IntPtr instancePtr, ulong ulOverlayHandle, ref uint punProcessId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowDashboard")]
	internal static extern void VR_IVROverlay_ShowDashboard(IntPtr instancePtr, string pchOverlayToShow);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowKeyboard")]
	internal static extern EVROverlayError VR_IVROverlay_ShowKeyboard(IntPtr instancePtr, int eInputMode, int eLineInputMode, string pchDescription, uint unCharMax, string pchExistingText, bool bUseMinimalMode, ulong uUserValue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_ShowKeyboardForOverlay")]
	internal static extern EVROverlayError VR_IVROverlay_ShowKeyboardForOverlay(IntPtr instancePtr, ulong ulOverlayHandle, int eInputMode, int eLineInputMode, string pchDescription, uint unCharMax, string pchExistingText, bool bUseMinimalMode, ulong uUserValue);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_GetKeyboardText")]
	internal static extern uint VR_IVROverlay_GetKeyboardText(IntPtr instancePtr, System.Text.StringBuilder pchText, uint cchText);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVROverlay_HideKeyboard")]
	internal static extern void VR_IVROverlay_HideKeyboard(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_LoadRenderModel")]
	internal static extern bool VR_IVRRenderModels_LoadRenderModel(IntPtr instancePtr, string pchRenderModelName, ref IntPtr ppRenderModel);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_FreeRenderModel")]
	internal static extern void VR_IVRRenderModels_FreeRenderModel(IntPtr instancePtr, ref RenderModel_t pRenderModel);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_LoadTexture")]
	internal static extern bool VR_IVRRenderModels_LoadTexture(IntPtr instancePtr, int textureId, ref IntPtr ppTexture);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_FreeTexture")]
	internal static extern void VR_IVRRenderModels_FreeTexture(IntPtr instancePtr, ref RenderModel_TextureMap_t pTexture);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetRenderModelName")]
	internal static extern uint VR_IVRRenderModels_GetRenderModelName(IntPtr instancePtr, uint unRenderModelIndex, System.Text.StringBuilder pchRenderModelName, uint unRenderModelNameLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetRenderModelCount")]
	internal static extern uint VR_IVRRenderModels_GetRenderModelCount(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetComponentCount")]
	internal static extern uint VR_IVRRenderModels_GetComponentCount(IntPtr instancePtr, string pchRenderModelName);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetComponentName")]
	internal static extern uint VR_IVRRenderModels_GetComponentName(IntPtr instancePtr, string pchRenderModelName, uint unComponentIndex, System.Text.StringBuilder pchComponentName, uint unComponentNameLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetComponentButtonMask")]
	internal static extern ulong VR_IVRRenderModels_GetComponentButtonMask(IntPtr instancePtr, string pchRenderModelName, string pchComponentName);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetComponentRenderModelName")]
	internal static extern uint VR_IVRRenderModels_GetComponentRenderModelName(IntPtr instancePtr, string pchRenderModelName, string pchComponentName, System.Text.StringBuilder pchComponentRenderModelName, uint unComponentRenderModelNameLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRRenderModels_GetComponentState")]
	internal static extern bool VR_IVRRenderModels_GetComponentState(IntPtr instancePtr, string pchRenderModelName, string pchComponentName, ref VRControllerState_t pControllerState, ref RenderModel_ComponentState_t pComponentState);
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
	internal static extern uint VR_IVRControlPanel_StartVRProcess(IntPtr instancePtr, string pchExecutable, ref string pchArguments, uint unArgumentCount, string pchWorkingDirectory);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_SetMasterProcessToThis")]
	internal static extern void VR_IVRControlPanel_SetMasterProcessToThis(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_StartAutolaunchOverlays")]
	internal static extern void VR_IVRControlPanel_StartAutolaunchOverlays(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_ForceQuitProcess")]
	internal static extern bool VR_IVRControlPanel_ForceQuitProcess(IntPtr instancePtr, uint pidProcessToQuit);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRControlPanel_AbortTransition")]
	internal static extern void VR_IVRControlPanel_AbortTransition(IntPtr instancePtr);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRNotifications_CreateNotification")]
	internal static extern EVRNotificationError VR_IVRNotifications_CreateNotification(IntPtr instancePtr, ulong ulOverlayHandle, ulong ulUserValue, EVRNotificationType type, string pchText, EVRNotificationStyle style, ref NotificationBitmap_t pImage, ref uint pNotificationId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRNotifications_RemoveNotification")]
	internal static extern EVRNotificationError VR_IVRNotifications_RemoveNotification(IntPtr instancePtr, uint notificationId);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_GetSettingsErrorNameFromEnum")]
	internal static extern IntPtr VR_IVRSettings_GetSettingsErrorNameFromEnum(IntPtr instancePtr, EVRSettingsError eError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_Sync")]
	internal static extern void VR_IVRSettings_Sync(IntPtr instancePtr, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_GetBool")]
	internal static extern bool VR_IVRSettings_GetBool(IntPtr instancePtr, string pchSection, string pchSettingsKey, bool bDefaultValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_SetBool")]
	internal static extern void VR_IVRSettings_SetBool(IntPtr instancePtr, string pchSection, string pchSettingsKey, bool bValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_GetInt32")]
	internal static extern int VR_IVRSettings_GetInt32(IntPtr instancePtr, string pchSection, string pchSettingsKey, int nDefaultValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_SetInt32")]
	internal static extern void VR_IVRSettings_SetInt32(IntPtr instancePtr, string pchSection, string pchSettingsKey, int nValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_GetFloat")]
	internal static extern float VR_IVRSettings_GetFloat(IntPtr instancePtr, string pchSection, string pchSettingsKey, float flDefaultValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_SetFloat")]
	internal static extern void VR_IVRSettings_SetFloat(IntPtr instancePtr, string pchSection, string pchSettingsKey, float flValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_GetString")]
	internal static extern void VR_IVRSettings_GetString(IntPtr instancePtr, string pchSection, string pchSettingsKey, string pchValue, uint unValueLen, string pchDefaultValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRSettings_SetString")]
	internal static extern void VR_IVRSettings_SetString(IntPtr instancePtr, string pchSection, string pchSettingsKey, string pchValue, ref EVRSettingsError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_HasCamera")]
	internal static extern bool VR_IVRTrackedCamera_HasCamera(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetCameraFirmwareDescription")]
	internal static extern bool VR_IVRTrackedCamera_GetCameraFirmwareDescription(IntPtr instancePtr, uint nDeviceIndex, string pBuffer, uint nBufferLen);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetCameraFrameDimensions")]
	internal static extern bool VR_IVRTrackedCamera_GetCameraFrameDimensions(IntPtr instancePtr, uint nDeviceIndex, ECameraVideoStreamFormat nVideoStreamFormat, ref uint pWidth, ref uint pHeight);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_SetCameraVideoStreamFormat")]
	internal static extern bool VR_IVRTrackedCamera_SetCameraVideoStreamFormat(IntPtr instancePtr, uint nDeviceIndex, ECameraVideoStreamFormat nVideoStreamFormat);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetCameraVideoStreamFormat")]
	internal static extern ECameraVideoStreamFormat VR_IVRTrackedCamera_GetCameraVideoStreamFormat(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_EnableCameraForStreaming")]
	internal static extern bool VR_IVRTrackedCamera_EnableCameraForStreaming(IntPtr instancePtr, uint nDeviceIndex, bool bEnable);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_StartVideoStream")]
	internal static extern bool VR_IVRTrackedCamera_StartVideoStream(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_StopVideoStream")]
	internal static extern bool VR_IVRTrackedCamera_StopVideoStream(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_IsVideoStreamActive")]
	internal static extern bool VR_IVRTrackedCamera_IsVideoStreamActive(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetVideoStreamElapsedTime")]
	internal static extern float VR_IVRTrackedCamera_GetVideoStreamElapsedTime(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetVideoStreamFrame")]
	internal static extern IntPtr VR_IVRTrackedCamera_GetVideoStreamFrame(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_ReleaseVideoStreamFrame")]
	internal static extern bool VR_IVRTrackedCamera_ReleaseVideoStreamFrame(IntPtr instancePtr, uint nDeviceIndex, ref CameraVideoStreamFrame_t pFrameImage);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_SetAutoExposure")]
	internal static extern bool VR_IVRTrackedCamera_SetAutoExposure(IntPtr instancePtr, uint nDeviceIndex, bool bEnable);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_SupportsPauseResume")]
	internal static extern bool VR_IVRTrackedCamera_SupportsPauseResume(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_PauseVideoStream")]
	internal static extern bool VR_IVRTrackedCamera_PauseVideoStream(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_ResumeVideoStream")]
	internal static extern bool VR_IVRTrackedCamera_ResumeVideoStream(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_IsVideoStreamPaused")]
	internal static extern bool VR_IVRTrackedCamera_IsVideoStreamPaused(IntPtr instancePtr, uint nDeviceIndex);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetCameraDistortion")]
	internal static extern bool VR_IVRTrackedCamera_GetCameraDistortion(IntPtr instancePtr, uint nDeviceIndex, float flInputU, float flInputV, ref float pflOutputU, ref float pflOutputV);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IVRTrackedCamera_GetCameraProjection")]
	internal static extern bool VR_IVRTrackedCamera_GetCameraProjection(IntPtr instancePtr, uint nDeviceIndex, float flWidthPixels, float flHeightPixels, float flZNear, float flZFar, ref HmdMatrix44_t pProjection);

}

}

namespace Valve.VR
{

public abstract class IVRSystem
{
	public abstract IntPtr GetIntPtr();
	public abstract void GetRecommendedRenderTargetSize(ref uint pnWidth,ref uint pnHeight);
	public abstract HmdMatrix44_t GetProjectionMatrix(EVREye eEye,float fNearZ,float fFarZ,EGraphicsAPIConvention eProjType);
	public abstract void GetProjectionRaw(EVREye eEye,ref float pfLeft,ref float pfRight,ref float pfTop,ref float pfBottom);
	public abstract DistortionCoordinates_t ComputeDistortion(EVREye eEye,float fU,float fV);
	public abstract HmdMatrix34_t GetEyeToHeadTransform(EVREye eEye);
	public abstract bool GetTimeSinceLastVsync(ref float pfSecondsSinceLastVsync,ref ulong pulFrameCounter);
	public abstract int GetD3D9AdapterIndex();
	public abstract void GetDXGIOutputInfo(ref int pnAdapterIndex);
	public abstract bool IsDisplayOnDesktop();
	public abstract bool SetDisplayVisibility(bool bIsVisibleOnDesktop);
	public abstract void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin,float fPredictedSecondsToPhotonsFromNow,TrackedDevicePose_t [] pTrackedDevicePoseArray);
	public abstract void ResetSeatedZeroPose();
	public abstract HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
	public abstract HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose();
	public abstract uint GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass,uint [] punTrackedDeviceIndexArray,uint unRelativeToTrackedDeviceIndex);
	public abstract EDeviceActivityLevel GetTrackedDeviceActivityLevel(uint unDeviceId);
	public abstract void ApplyTransform(ref TrackedDevicePose_t pOutputPose,ref TrackedDevicePose_t pTrackedDevicePose,ref HmdMatrix34_t pTransform);
	public abstract ETrackedDeviceClass GetTrackedDeviceClass(uint unDeviceIndex);
	public abstract bool IsTrackedDeviceConnected(uint unDeviceIndex);
	public abstract bool GetBoolTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError);
	public abstract float GetFloatTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError);
	public abstract int GetInt32TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError);
	public abstract ulong GetUint64TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError);
	public abstract HmdMatrix34_t GetMatrix34TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError);
	public abstract uint GetStringTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,System.Text.StringBuilder pchValue,uint unBufferSize,ref ETrackedPropertyError pError);
	public abstract string GetPropErrorNameFromEnum(ETrackedPropertyError error);
	public abstract bool PollNextEvent(ref VREvent_t pEvent);
	public abstract bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin,ref VREvent_t pEvent,ref TrackedDevicePose_t pTrackedDevicePose);
	public abstract string GetEventTypeNameFromEnum(EVREventType eType);
	public abstract HiddenAreaMesh_t GetHiddenAreaMesh(EVREye eEye);
	public abstract bool GetControllerState(uint unControllerDeviceIndex,ref VRControllerState_t pControllerState);
	public abstract bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin,uint unControllerDeviceIndex,ref VRControllerState_t pControllerState,ref TrackedDevicePose_t pTrackedDevicePose);
	public abstract void TriggerHapticPulse(uint unControllerDeviceIndex,uint unAxisId,char usDurationMicroSec);
	public abstract string GetButtonIdNameFromEnum(EVRButtonId eButtonId);
	public abstract string GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType);
	public abstract bool CaptureInputFocus();
	public abstract void ReleaseInputFocus();
	public abstract bool IsInputFocusCapturedByAnotherProcess();
	public abstract uint DriverDebugRequest(uint unDeviceIndex,string pchRequest,string pchResponseBuffer,uint unResponseBufferSize);
	public abstract EVRFirmwareError PerformFirmwareUpdate(uint unDeviceIndex);
	public abstract void AcknowledgeQuit_Exiting();
	public abstract void AcknowledgeQuit_UserPrompt();
}


public abstract class IVRExtendedDisplay
{
	public abstract IntPtr GetIntPtr();
	public abstract void GetWindowBounds(ref int pnX,ref int pnY,ref uint pnWidth,ref uint pnHeight);
	public abstract void GetEyeOutputViewport(EVREye eEye,ref uint pnX,ref uint pnY,ref uint pnWidth,ref uint pnHeight);
	public abstract void GetDXGIOutputInfo(ref int pnAdapterIndex,ref int pnAdapterOutputIndex);
}


public abstract class IVRApplications
{
	public abstract IntPtr GetIntPtr();
	public abstract EVRApplicationError AddApplicationManifest(string pchApplicationManifestFullPath,bool bTemporary);
	public abstract EVRApplicationError RemoveApplicationManifest(string pchApplicationManifestFullPath);
	public abstract bool IsApplicationInstalled(string pchAppKey);
	public abstract uint GetApplicationCount();
	public abstract EVRApplicationError GetApplicationKeyByIndex(uint unApplicationIndex,string pchAppKeyBuffer,uint unAppKeyBufferLen);
	public abstract EVRApplicationError GetApplicationKeyByProcessId(uint unProcessId,string pchAppKeyBuffer,uint unAppKeyBufferLen);
	public abstract EVRApplicationError LaunchApplication(string pchAppKey);
	public abstract EVRApplicationError LaunchDashboardOverlay(string pchAppKey);
	public abstract EVRApplicationError IdentifyApplication(uint unProcessId,string pchAppKey);
	public abstract uint GetApplicationProcessId(string pchAppKey);
	public abstract string GetApplicationsErrorNameFromEnum(EVRApplicationError error);
	public abstract uint GetApplicationPropertyString(string pchAppKey,EVRApplicationProperty eProperty,string pchPropertyValueBuffer,uint unPropertyValueBufferLen,ref EVRApplicationError peError);
	public abstract bool GetApplicationPropertyBool(string pchAppKey,EVRApplicationProperty eProperty,ref EVRApplicationError peError);
	public abstract EVRApplicationError SetApplicationAutoLaunch(string pchAppKey,bool bAutoLaunch);
	public abstract bool GetApplicationAutoLaunch(string pchAppKey);
	public abstract EVRApplicationError GetStartingApplication(string pchAppKeyBuffer,uint unAppKeyBufferLen);
	public abstract EVRApplicationTransitionState GetTransitionState();
	public abstract EVRApplicationError PerformApplicationPrelaunchCheck(string pchAppKey);
	public abstract string GetApplicationsTransitionStateNameFromEnum(EVRApplicationTransitionState state);
	public abstract bool IsQuitUserPromptRequested();
}


public abstract class IVRChaperone
{
	public abstract IntPtr GetIntPtr();
	public abstract ChaperoneCalibrationState GetCalibrationState();
	public abstract bool GetPlayAreaSize(ref float pSizeX,ref float pSizeZ);
	public abstract bool GetPlayAreaRect(ref HmdQuad_t rect);
	public abstract void ReloadInfo();
	public abstract void SetSceneColor(HmdColor_t color);
	public abstract void GetBoundsColor(ref HmdColor_t pOutputColorArray,int nNumOutputColors,float flCollisionBoundsFadeDistance);
	public abstract bool AreBoundsVisible();
	public abstract void ForceBoundsVisible(bool bForce);
}


public abstract class IVRChaperoneSetup
{
	public abstract IntPtr GetIntPtr();
	public abstract bool CommitWorkingCopy(EChaperoneConfigFile configFile);
	public abstract void RevertWorkingCopy();
	public abstract bool GetWorkingPlayAreaSize(ref float pSizeX,ref float pSizeZ);
	public abstract bool GetWorkingPlayAreaRect(ref HmdQuad_t rect);
	public abstract bool GetWorkingCollisionBoundsInfo(out HmdQuad_t [] pQuadsBuffer);
	public abstract bool GetLiveCollisionBoundsInfo(out HmdQuad_t [] pQuadsBuffer);
	public abstract bool GetWorkingSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
	public abstract bool GetWorkingStandingZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatStandingZeroPoseToRawTrackingPose);
	public abstract void SetWorkingPlayAreaSize(float sizeX,float sizeZ);
	public abstract void SetWorkingCollisionBoundsInfo(HmdQuad_t [] pQuadsBuffer);
	public abstract void SetWorkingSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pMatSeatedZeroPoseToRawTrackingPose);
	public abstract void SetWorkingStandingZeroPoseToRawTrackingPose(ref HmdMatrix34_t pMatStandingZeroPoseToRawTrackingPose);
	public abstract void ReloadFromDisk(EChaperoneConfigFile configFile);
	public abstract bool GetLiveSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose);
}


public abstract class IVRCompositor
{
	public abstract IntPtr GetIntPtr();
	public abstract void SetTrackingSpace(ETrackingUniverseOrigin eOrigin);
	public abstract ETrackingUniverseOrigin GetTrackingSpace();
	public abstract EVRCompositorError WaitGetPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray);
	public abstract EVRCompositorError GetLastPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray);
	public abstract EVRCompositorError Submit(EVREye eEye,ref Texture_t pTexture,ref VRTextureBounds_t pBounds,EVRSubmitFlags nSubmitFlags);
	public abstract void ClearLastSubmittedFrame();
	public abstract void PostPresentHandoff();
	public abstract bool GetFrameTiming(ref Compositor_FrameTiming pTiming,uint unFramesAgo);
	public abstract float GetFrameTimeRemaining();
	public abstract void FadeToColor(float fSeconds,float fRed,float fGreen,float fBlue,float fAlpha,bool bBackground);
	public abstract void FadeGrid(float fSeconds,bool bFadeIn);
	public abstract EVRCompositorError SetSkyboxOverride(Texture_t [] pTextures);
	public abstract void ClearSkyboxOverride();
	public abstract void CompositorBringToFront();
	public abstract void CompositorGoToBack();
	public abstract void CompositorQuit();
	public abstract bool IsFullscreen();
	public abstract uint GetCurrentSceneFocusProcess();
	public abstract uint GetLastFrameRenderer();
	public abstract bool CanRenderScene();
	public abstract void ShowMirrorWindow();
	public abstract void HideMirrorWindow();
	public abstract bool IsMirrorWindowVisible();
	public abstract void CompositorDumpImages();
}


public abstract class IVROverlay
{
	public abstract IntPtr GetIntPtr();
	public abstract EVROverlayError FindOverlay(string pchOverlayKey,ref ulong pOverlayHandle);
	public abstract EVROverlayError CreateOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pOverlayHandle);
	public abstract EVROverlayError DestroyOverlay(ulong ulOverlayHandle);
	public abstract EVROverlayError SetHighQualityOverlay(ulong ulOverlayHandle);
	public abstract ulong GetHighQualityOverlay();
	public abstract uint GetOverlayKey(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref EVROverlayError pError);
	public abstract uint GetOverlayName(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref EVROverlayError pError);
	public abstract EVROverlayError GetOverlayImageData(ulong ulOverlayHandle,IntPtr pvBuffer,uint unBufferSize,ref uint punWidth,ref uint punHeight);
	public abstract string GetOverlayErrorNameFromEnum(EVROverlayError error);
	public abstract EVROverlayError SetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,bool bEnabled);
	public abstract EVROverlayError GetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,ref bool pbEnabled);
	public abstract EVROverlayError SetOverlayColor(ulong ulOverlayHandle,float fRed,float fGreen,float fBlue);
	public abstract EVROverlayError GetOverlayColor(ulong ulOverlayHandle,ref float pfRed,ref float pfGreen,ref float pfBlue);
	public abstract EVROverlayError SetOverlayAlpha(ulong ulOverlayHandle,float fAlpha);
	public abstract EVROverlayError GetOverlayAlpha(ulong ulOverlayHandle,ref float pfAlpha);
	public abstract EVROverlayError SetOverlayWidthInMeters(ulong ulOverlayHandle,float fWidthInMeters);
	public abstract EVROverlayError GetOverlayWidthInMeters(ulong ulOverlayHandle,ref float pfWidthInMeters);
	public abstract EVROverlayError SetOverlayAutoCurveDistanceRangeInMeters(ulong ulOverlayHandle,float fMinDistanceInMeters,float fMaxDistanceInMeters);
	public abstract EVROverlayError GetOverlayAutoCurveDistanceRangeInMeters(ulong ulOverlayHandle,ref float pfMinDistanceInMeters,ref float pfMaxDistanceInMeters);
	public abstract EVROverlayError SetOverlayTextureColorSpace(ulong ulOverlayHandle,EColorSpace eTextureColorSpace);
	public abstract EVROverlayError GetOverlayTextureColorSpace(ulong ulOverlayHandle,ref EColorSpace peTextureColorSpace);
	public abstract EVROverlayError SetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds);
	public abstract EVROverlayError GetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds);
	public abstract EVROverlayError GetOverlayTransformType(ulong ulOverlayHandle,ref VROverlayTransformType peTransformType);
	public abstract EVROverlayError SetOverlayTransformAbsolute(ulong ulOverlayHandle,ETrackingUniverseOrigin eTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	public abstract EVROverlayError GetOverlayTransformAbsolute(ulong ulOverlayHandle,ref ETrackingUniverseOrigin peTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform);
	public abstract EVROverlayError SetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,uint unTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	public abstract EVROverlayError GetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,ref uint punTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform);
	public abstract EVROverlayError ShowOverlay(ulong ulOverlayHandle);
	public abstract EVROverlayError HideOverlay(ulong ulOverlayHandle);
	public abstract bool IsOverlayVisible(ulong ulOverlayHandle);
	public abstract bool PollNextOverlayEvent(ulong ulOverlayHandle,ref VREvent_t pEvent);
	public abstract EVROverlayError GetOverlayInputMethod(ulong ulOverlayHandle,ref VROverlayInputMethod peInputMethod);
	public abstract EVROverlayError SetOverlayInputMethod(ulong ulOverlayHandle,VROverlayInputMethod eInputMethod);
	public abstract EVROverlayError GetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale);
	public abstract EVROverlayError SetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale);
	public abstract bool ComputeOverlayIntersection(ulong ulOverlayHandle,ref VROverlayIntersectionParams_t pParams,ref VROverlayIntersectionResults_t pResults);
	public abstract bool HandleControllerOverlayInteractionAsMouse(ulong ulOverlayHandle,uint unControllerDeviceIndex);
	public abstract bool IsHoverTargetOverlay(ulong ulOverlayHandle);
	public abstract ulong GetGamepadFocusOverlay();
	public abstract EVROverlayError SetGamepadFocusOverlay(ulong ulNewFocusOverlay);
	public abstract EVROverlayError SetOverlayNeighbor(EOverlayDirection eDirection,ulong ulFrom,ulong ulTo);
	public abstract EVROverlayError MoveGamepadFocusToNeighbor(EOverlayDirection eDirection,ulong ulFrom);
	public abstract EVROverlayError SetOverlayTexture(ulong ulOverlayHandle,ref Texture_t pTexture);
	public abstract EVROverlayError ClearOverlayTexture(ulong ulOverlayHandle);
	public abstract EVROverlayError SetOverlayRaw(ulong ulOverlayHandle,IntPtr pvBuffer,uint unWidth,uint unHeight,uint unDepth);
	public abstract EVROverlayError SetOverlayFromFile(ulong ulOverlayHandle,string pchFilePath);
	public abstract EVROverlayError CreateDashboardOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pMainHandle,ref ulong pThumbnailHandle);
	public abstract bool IsDashboardVisible();
	public abstract bool IsActiveDashboardOverlay(ulong ulOverlayHandle);
	public abstract EVROverlayError SetDashboardOverlaySceneProcess(ulong ulOverlayHandle,uint unProcessId);
	public abstract EVROverlayError GetDashboardOverlaySceneProcess(ulong ulOverlayHandle,ref uint punProcessId);
	public abstract void ShowDashboard(string pchOverlayToShow);
	public abstract EVROverlayError ShowKeyboard(int eInputMode,int eLineInputMode,string pchDescription,uint unCharMax,string pchExistingText,bool bUseMinimalMode,ulong uUserValue);
	public abstract EVROverlayError ShowKeyboardForOverlay(ulong ulOverlayHandle,int eInputMode,int eLineInputMode,string pchDescription,uint unCharMax,string pchExistingText,bool bUseMinimalMode,ulong uUserValue);
	public abstract uint GetKeyboardText(System.Text.StringBuilder pchText,uint cchText);
	public abstract void HideKeyboard();
}


public abstract class IVRRenderModels
{
	public abstract IntPtr GetIntPtr();
	public abstract bool LoadRenderModel(string pchRenderModelName,ref IntPtr ppRenderModel);
	public abstract void FreeRenderModel(ref RenderModel_t pRenderModel);
	public abstract bool LoadTexture(int textureId,ref IntPtr ppTexture);
	public abstract void FreeTexture(ref RenderModel_TextureMap_t pTexture);
	public abstract uint GetRenderModelName(uint unRenderModelIndex,System.Text.StringBuilder pchRenderModelName,uint unRenderModelNameLen);
	public abstract uint GetRenderModelCount();
	public abstract uint GetComponentCount(string pchRenderModelName);
	public abstract uint GetComponentName(string pchRenderModelName,uint unComponentIndex,System.Text.StringBuilder pchComponentName,uint unComponentNameLen);
	public abstract ulong GetComponentButtonMask(string pchRenderModelName,string pchComponentName);
	public abstract uint GetComponentRenderModelName(string pchRenderModelName,string pchComponentName,System.Text.StringBuilder pchComponentRenderModelName,uint unComponentRenderModelNameLen);
	public abstract bool GetComponentState(string pchRenderModelName,string pchComponentName,ref VRControllerState_t pControllerState,ref RenderModel_ComponentState_t pComponentState);
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
	public abstract uint StartVRProcess(string pchExecutable,ref string pchArguments,uint unArgumentCount,string pchWorkingDirectory);
	public abstract void SetMasterProcessToThis();
	public abstract void StartAutolaunchOverlays();
	public abstract bool ForceQuitProcess(uint pidProcessToQuit);
	public abstract void AbortTransition();
}


public abstract class IVRNotifications
{
	public abstract IntPtr GetIntPtr();
	public abstract EVRNotificationError CreateNotification(ulong ulOverlayHandle,ulong ulUserValue,EVRNotificationType type,string pchText,EVRNotificationStyle style,ref NotificationBitmap_t pImage,ref uint pNotificationId);
	public abstract EVRNotificationError RemoveNotification(uint notificationId);
}


public abstract class IVRSettings
{
	public abstract IntPtr GetIntPtr();
	public abstract string GetSettingsErrorNameFromEnum(EVRSettingsError eError);
	public abstract void Sync(ref EVRSettingsError peError);
	public abstract bool GetBool(string pchSection,string pchSettingsKey,bool bDefaultValue,ref EVRSettingsError peError);
	public abstract void SetBool(string pchSection,string pchSettingsKey,bool bValue,ref EVRSettingsError peError);
	public abstract int GetInt32(string pchSection,string pchSettingsKey,int nDefaultValue,ref EVRSettingsError peError);
	public abstract void SetInt32(string pchSection,string pchSettingsKey,int nValue,ref EVRSettingsError peError);
	public abstract float GetFloat(string pchSection,string pchSettingsKey,float flDefaultValue,ref EVRSettingsError peError);
	public abstract void SetFloat(string pchSection,string pchSettingsKey,float flValue,ref EVRSettingsError peError);
	public abstract void GetString(string pchSection,string pchSettingsKey,string pchValue,uint unValueLen,string pchDefaultValue,ref EVRSettingsError peError);
	public abstract void SetString(string pchSection,string pchSettingsKey,string pchValue,ref EVRSettingsError peError);
}


public abstract class IVRTrackedCamera
{
	public abstract IntPtr GetIntPtr();
	public abstract bool HasCamera(uint nDeviceIndex);
	public abstract bool GetCameraFirmwareDescription(uint nDeviceIndex,string pBuffer,uint nBufferLen);
	public abstract bool GetCameraFrameDimensions(uint nDeviceIndex,ECameraVideoStreamFormat nVideoStreamFormat,ref uint pWidth,ref uint pHeight);
	public abstract bool SetCameraVideoStreamFormat(uint nDeviceIndex,ECameraVideoStreamFormat nVideoStreamFormat);
	public abstract ECameraVideoStreamFormat GetCameraVideoStreamFormat(uint nDeviceIndex);
	public abstract bool EnableCameraForStreaming(uint nDeviceIndex,bool bEnable);
	public abstract bool StartVideoStream(uint nDeviceIndex);
	public abstract bool StopVideoStream(uint nDeviceIndex);
	public abstract bool IsVideoStreamActive(uint nDeviceIndex);
	public abstract float GetVideoStreamElapsedTime(uint nDeviceIndex);
	public abstract CameraVideoStreamFrame_t GetVideoStreamFrame(uint nDeviceIndex);
	public abstract bool ReleaseVideoStreamFrame(uint nDeviceIndex,ref CameraVideoStreamFrame_t pFrameImage);
	public abstract bool SetAutoExposure(uint nDeviceIndex,bool bEnable);
	public abstract bool SupportsPauseResume(uint nDeviceIndex);
	public abstract bool PauseVideoStream(uint nDeviceIndex);
	public abstract bool ResumeVideoStream(uint nDeviceIndex);
	public abstract bool IsVideoStreamPaused(uint nDeviceIndex);
	public abstract bool GetCameraDistortion(uint nDeviceIndex,float flInputU,float flInputV,ref float pflOutputU,ref float pflOutputV);
	public abstract bool GetCameraProjection(uint nDeviceIndex,float flWidthPixels,float flHeightPixels,float flZNear,float flZFar,ref HmdMatrix44_t pProjection);
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
	public override void GetRecommendedRenderTargetSize(ref uint pnWidth,ref uint pnHeight)
	{
		CheckIfUsable();
		pnWidth = 0;
		pnHeight = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetRecommendedRenderTargetSize(m_pVRSystem,ref pnWidth,ref pnHeight);
	}
	public override HmdMatrix44_t GetProjectionMatrix(EVREye eEye,float fNearZ,float fFarZ,EGraphicsAPIConvention eProjType)
	{
		CheckIfUsable();
		HmdMatrix44_t result = VRNativeEntrypoints.VR_IVRSystem_GetProjectionMatrix(m_pVRSystem,eEye,fNearZ,fFarZ,eProjType);
		return result;
	}
	public override void GetProjectionRaw(EVREye eEye,ref float pfLeft,ref float pfRight,ref float pfTop,ref float pfBottom)
	{
		CheckIfUsable();
		pfLeft = 0;
		pfRight = 0;
		pfTop = 0;
		pfBottom = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetProjectionRaw(m_pVRSystem,eEye,ref pfLeft,ref pfRight,ref pfTop,ref pfBottom);
	}
	public override DistortionCoordinates_t ComputeDistortion(EVREye eEye,float fU,float fV)
	{
		CheckIfUsable();
		DistortionCoordinates_t result = VRNativeEntrypoints.VR_IVRSystem_ComputeDistortion(m_pVRSystem,eEye,fU,fV);
		return result;
	}
	public override HmdMatrix34_t GetEyeToHeadTransform(EVREye eEye)
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
	public override void GetDXGIOutputInfo(ref int pnAdapterIndex)
	{
		CheckIfUsable();
		pnAdapterIndex = 0;
		VRNativeEntrypoints.VR_IVRSystem_GetDXGIOutputInfo(m_pVRSystem,ref pnAdapterIndex);
	}
	public override bool IsDisplayOnDesktop()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_IsDisplayOnDesktop(m_pVRSystem);
		return result;
	}
	public override bool SetDisplayVisibility(bool bIsVisibleOnDesktop)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_SetDisplayVisibility(m_pVRSystem,bIsVisibleOnDesktop);
		return result;
	}
	public override void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin,float fPredictedSecondsToPhotonsFromNow,TrackedDevicePose_t [] pTrackedDevicePoseArray)
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
	public override HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose()
	{
		CheckIfUsable();
		HmdMatrix34_t result = VRNativeEntrypoints.VR_IVRSystem_GetRawZeroPoseToStandingAbsoluteTrackingPose(m_pVRSystem);
		return result;
	}
	public override uint GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass,uint [] punTrackedDeviceIndexArray,uint unRelativeToTrackedDeviceIndex)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass(m_pVRSystem,eTrackedDeviceClass,punTrackedDeviceIndexArray,(uint) punTrackedDeviceIndexArray.Length,unRelativeToTrackedDeviceIndex);
		return result;
	}
	public override EDeviceActivityLevel GetTrackedDeviceActivityLevel(uint unDeviceId)
	{
		CheckIfUsable();
		EDeviceActivityLevel result = VRNativeEntrypoints.VR_IVRSystem_GetTrackedDeviceActivityLevel(m_pVRSystem,unDeviceId);
		return result;
	}
	public override void ApplyTransform(ref TrackedDevicePose_t pOutputPose,ref TrackedDevicePose_t pTrackedDevicePose,ref HmdMatrix34_t pTransform)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_ApplyTransform(m_pVRSystem,ref pOutputPose,ref pTrackedDevicePose,ref pTransform);
	}
	public override ETrackedDeviceClass GetTrackedDeviceClass(uint unDeviceIndex)
	{
		CheckIfUsable();
		ETrackedDeviceClass result = VRNativeEntrypoints.VR_IVRSystem_GetTrackedDeviceClass(m_pVRSystem,unDeviceIndex);
		return result;
	}
	public override bool IsTrackedDeviceConnected(uint unDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_IsTrackedDeviceConnected(m_pVRSystem,unDeviceIndex);
		return result;
	}
	public override bool GetBoolTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_GetBoolTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override float GetFloatTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRSystem_GetFloatTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override int GetInt32TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		int result = VRNativeEntrypoints.VR_IVRSystem_GetInt32TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override ulong GetUint64TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVRSystem_GetUint64TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override HmdMatrix34_t GetMatrix34TrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		HmdMatrix34_t result = VRNativeEntrypoints.VR_IVRSystem_GetMatrix34TrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,ref pError);
		return result;
	}
	public override uint GetStringTrackedDeviceProperty(uint unDeviceIndex,ETrackedDeviceProperty prop,System.Text.StringBuilder pchValue,uint unBufferSize,ref ETrackedPropertyError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRSystem_GetStringTrackedDeviceProperty(m_pVRSystem,unDeviceIndex,prop,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override string GetPropErrorNameFromEnum(ETrackedPropertyError error)
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
	public override bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin,ref VREvent_t pEvent,ref TrackedDevicePose_t pTrackedDevicePose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSystem_PollNextEventWithPose(m_pVRSystem,eOrigin,ref pEvent,ref pTrackedDevicePose);
		return result;
	}
	public override string GetEventTypeNameFromEnum(EVREventType eType)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetEventTypeNameFromEnum(m_pVRSystem,eType);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override HiddenAreaMesh_t GetHiddenAreaMesh(EVREye eEye)
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
	public override bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin,uint unControllerDeviceIndex,ref VRControllerState_t pControllerState,ref TrackedDevicePose_t pTrackedDevicePose)
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
	public override string GetButtonIdNameFromEnum(EVRButtonId eButtonId)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSystem_GetButtonIdNameFromEnum(m_pVRSystem,eButtonId);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override string GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType)
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
	public override EVRFirmwareError PerformFirmwareUpdate(uint unDeviceIndex)
	{
		CheckIfUsable();
		EVRFirmwareError result = VRNativeEntrypoints.VR_IVRSystem_PerformFirmwareUpdate(m_pVRSystem,unDeviceIndex);
		return result;
	}
	public override void AcknowledgeQuit_Exiting()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_AcknowledgeQuit_Exiting(m_pVRSystem);
	}
	public override void AcknowledgeQuit_UserPrompt()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSystem_AcknowledgeQuit_UserPrompt(m_pVRSystem);
	}
}


public class CVRExtendedDisplay : IVRExtendedDisplay
{
	public CVRExtendedDisplay(IntPtr VRExtendedDisplay)
	{
		m_pVRExtendedDisplay = VRExtendedDisplay;
	}
	IntPtr m_pVRExtendedDisplay;

	public override IntPtr GetIntPtr() { return m_pVRExtendedDisplay; }

	private void CheckIfUsable()
	{
		if (m_pVRExtendedDisplay == IntPtr.Zero)
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
		VRNativeEntrypoints.VR_IVRExtendedDisplay_GetWindowBounds(m_pVRExtendedDisplay,ref pnX,ref pnY,ref pnWidth,ref pnHeight);
	}
	public override void GetEyeOutputViewport(EVREye eEye,ref uint pnX,ref uint pnY,ref uint pnWidth,ref uint pnHeight)
	{
		CheckIfUsable();
		pnX = 0;
		pnY = 0;
		pnWidth = 0;
		pnHeight = 0;
		VRNativeEntrypoints.VR_IVRExtendedDisplay_GetEyeOutputViewport(m_pVRExtendedDisplay,eEye,ref pnX,ref pnY,ref pnWidth,ref pnHeight);
	}
	public override void GetDXGIOutputInfo(ref int pnAdapterIndex,ref int pnAdapterOutputIndex)
	{
		CheckIfUsable();
		pnAdapterIndex = 0;
		pnAdapterOutputIndex = 0;
		VRNativeEntrypoints.VR_IVRExtendedDisplay_GetDXGIOutputInfo(m_pVRExtendedDisplay,ref pnAdapterIndex,ref pnAdapterOutputIndex);
	}
}


public class CVRApplications : IVRApplications
{
	public CVRApplications(IntPtr VRApplications)
	{
		m_pVRApplications = VRApplications;
	}
	IntPtr m_pVRApplications;

	public override IntPtr GetIntPtr() { return m_pVRApplications; }

	private void CheckIfUsable()
	{
		if (m_pVRApplications == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override EVRApplicationError AddApplicationManifest(string pchApplicationManifestFullPath,bool bTemporary)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_AddApplicationManifest(m_pVRApplications,pchApplicationManifestFullPath,bTemporary);
		return result;
	}
	public override EVRApplicationError RemoveApplicationManifest(string pchApplicationManifestFullPath)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_RemoveApplicationManifest(m_pVRApplications,pchApplicationManifestFullPath);
		return result;
	}
	public override bool IsApplicationInstalled(string pchAppKey)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRApplications_IsApplicationInstalled(m_pVRApplications,pchAppKey);
		return result;
	}
	public override uint GetApplicationCount()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationCount(m_pVRApplications);
		return result;
	}
	public override EVRApplicationError GetApplicationKeyByIndex(uint unApplicationIndex,string pchAppKeyBuffer,uint unAppKeyBufferLen)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationKeyByIndex(m_pVRApplications,unApplicationIndex,pchAppKeyBuffer,unAppKeyBufferLen);
		return result;
	}
	public override EVRApplicationError GetApplicationKeyByProcessId(uint unProcessId,string pchAppKeyBuffer,uint unAppKeyBufferLen)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationKeyByProcessId(m_pVRApplications,unProcessId,pchAppKeyBuffer,unAppKeyBufferLen);
		return result;
	}
	public override EVRApplicationError LaunchApplication(string pchAppKey)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_LaunchApplication(m_pVRApplications,pchAppKey);
		return result;
	}
	public override EVRApplicationError LaunchDashboardOverlay(string pchAppKey)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_LaunchDashboardOverlay(m_pVRApplications,pchAppKey);
		return result;
	}
	public override EVRApplicationError IdentifyApplication(uint unProcessId,string pchAppKey)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_IdentifyApplication(m_pVRApplications,unProcessId,pchAppKey);
		return result;
	}
	public override uint GetApplicationProcessId(string pchAppKey)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationProcessId(m_pVRApplications,pchAppKey);
		return result;
	}
	public override string GetApplicationsErrorNameFromEnum(EVRApplicationError error)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationsErrorNameFromEnum(m_pVRApplications,error);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override uint GetApplicationPropertyString(string pchAppKey,EVRApplicationProperty eProperty,string pchPropertyValueBuffer,uint unPropertyValueBufferLen,ref EVRApplicationError peError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationPropertyString(m_pVRApplications,pchAppKey,eProperty,pchPropertyValueBuffer,unPropertyValueBufferLen,ref peError);
		return result;
	}
	public override bool GetApplicationPropertyBool(string pchAppKey,EVRApplicationProperty eProperty,ref EVRApplicationError peError)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationPropertyBool(m_pVRApplications,pchAppKey,eProperty,ref peError);
		return result;
	}
	public override EVRApplicationError SetApplicationAutoLaunch(string pchAppKey,bool bAutoLaunch)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_SetApplicationAutoLaunch(m_pVRApplications,pchAppKey,bAutoLaunch);
		return result;
	}
	public override bool GetApplicationAutoLaunch(string pchAppKey)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationAutoLaunch(m_pVRApplications,pchAppKey);
		return result;
	}
	public override EVRApplicationError GetStartingApplication(string pchAppKeyBuffer,uint unAppKeyBufferLen)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_GetStartingApplication(m_pVRApplications,pchAppKeyBuffer,unAppKeyBufferLen);
		return result;
	}
	public override EVRApplicationTransitionState GetTransitionState()
	{
		CheckIfUsable();
		EVRApplicationTransitionState result = VRNativeEntrypoints.VR_IVRApplications_GetTransitionState(m_pVRApplications);
		return result;
	}
	public override EVRApplicationError PerformApplicationPrelaunchCheck(string pchAppKey)
	{
		CheckIfUsable();
		EVRApplicationError result = VRNativeEntrypoints.VR_IVRApplications_PerformApplicationPrelaunchCheck(m_pVRApplications,pchAppKey);
		return result;
	}
	public override string GetApplicationsTransitionStateNameFromEnum(EVRApplicationTransitionState state)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRApplications_GetApplicationsTransitionStateNameFromEnum(m_pVRApplications,state);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override bool IsQuitUserPromptRequested()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRApplications_IsQuitUserPromptRequested(m_pVRApplications);
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
	public override bool GetPlayAreaSize(ref float pSizeX,ref float pSizeZ)
	{
		CheckIfUsable();
		pSizeX = 0;
		pSizeZ = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperone_GetPlayAreaSize(m_pVRChaperone,ref pSizeX,ref pSizeZ);
		return result;
	}
	public override bool GetPlayAreaRect(ref HmdQuad_t rect)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperone_GetPlayAreaRect(m_pVRChaperone,ref rect);
		return result;
	}
	public override void ReloadInfo()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperone_ReloadInfo(m_pVRChaperone);
	}
	public override void SetSceneColor(HmdColor_t color)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperone_SetSceneColor(m_pVRChaperone,color);
	}
	public override void GetBoundsColor(ref HmdColor_t pOutputColorArray,int nNumOutputColors,float flCollisionBoundsFadeDistance)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperone_GetBoundsColor(m_pVRChaperone,ref pOutputColorArray,nNumOutputColors,flCollisionBoundsFadeDistance);
	}
	public override bool AreBoundsVisible()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperone_AreBoundsVisible(m_pVRChaperone);
		return result;
	}
	public override void ForceBoundsVisible(bool bForce)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperone_ForceBoundsVisible(m_pVRChaperone,bForce);
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
	public override bool CommitWorkingCopy(EChaperoneConfigFile configFile)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_CommitWorkingCopy(m_pVRChaperoneSetup,configFile);
		return result;
	}
	public override void RevertWorkingCopy()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_RevertWorkingCopy(m_pVRChaperoneSetup);
	}
	public override bool GetWorkingPlayAreaSize(ref float pSizeX,ref float pSizeZ)
	{
		CheckIfUsable();
		pSizeX = 0;
		pSizeZ = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingPlayAreaSize(m_pVRChaperoneSetup,ref pSizeX,ref pSizeZ);
		return result;
	}
	public override bool GetWorkingPlayAreaRect(ref HmdQuad_t rect)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingPlayAreaRect(m_pVRChaperoneSetup,ref rect);
		return result;
	}
	public override bool GetWorkingCollisionBoundsInfo(out HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		uint punQuadsCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingCollisionBoundsInfo(m_pVRChaperoneSetup,null,ref punQuadsCount);
		pQuadsBuffer= new HmdQuad_t[punQuadsCount];
		result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetWorkingCollisionBoundsInfo(m_pVRChaperoneSetup,pQuadsBuffer,ref punQuadsCount);
		return result;
	}
	public override bool GetLiveCollisionBoundsInfo(out HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		uint punQuadsCount = 0;
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetLiveCollisionBoundsInfo(m_pVRChaperoneSetup,null,ref punQuadsCount);
		pQuadsBuffer= new HmdQuad_t[punQuadsCount];
		result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetLiveCollisionBoundsInfo(m_pVRChaperoneSetup,pQuadsBuffer,ref punQuadsCount);
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
	public override void SetWorkingPlayAreaSize(float sizeX,float sizeZ)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingPlayAreaSize(m_pVRChaperoneSetup,sizeX,sizeZ);
	}
	public override void SetWorkingCollisionBoundsInfo(HmdQuad_t [] pQuadsBuffer)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingCollisionBoundsInfo(m_pVRChaperoneSetup,pQuadsBuffer,(uint) pQuadsBuffer.Length);
	}
	public override void SetWorkingSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pMatSeatedZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,ref pMatSeatedZeroPoseToRawTrackingPose);
	}
	public override void SetWorkingStandingZeroPoseToRawTrackingPose(ref HmdMatrix34_t pMatStandingZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,ref pMatStandingZeroPoseToRawTrackingPose);
	}
	public override void ReloadFromDisk(EChaperoneConfigFile configFile)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRChaperoneSetup_ReloadFromDisk(m_pVRChaperoneSetup,configFile);
	}
	public override bool GetLiveSeatedZeroPoseToRawTrackingPose(ref HmdMatrix34_t pmatSeatedZeroPoseToRawTrackingPose)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRChaperoneSetup_GetLiveSeatedZeroPoseToRawTrackingPose(m_pVRChaperoneSetup,ref pmatSeatedZeroPoseToRawTrackingPose);
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
	public override void SetTrackingSpace(ETrackingUniverseOrigin eOrigin)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_SetTrackingSpace(m_pVRCompositor,eOrigin);
	}
	public override ETrackingUniverseOrigin GetTrackingSpace()
	{
		CheckIfUsable();
		ETrackingUniverseOrigin result = VRNativeEntrypoints.VR_IVRCompositor_GetTrackingSpace(m_pVRCompositor);
		return result;
	}
	public override EVRCompositorError WaitGetPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray)
	{
		CheckIfUsable();
		EVRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_WaitGetPoses(m_pVRCompositor,pRenderPoseArray,(uint) pRenderPoseArray.Length,pGamePoseArray,(uint) pGamePoseArray.Length);
		return result;
	}
	public override EVRCompositorError GetLastPoses(TrackedDevicePose_t [] pRenderPoseArray,TrackedDevicePose_t [] pGamePoseArray)
	{
		CheckIfUsable();
		EVRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_GetLastPoses(m_pVRCompositor,pRenderPoseArray,(uint) pRenderPoseArray.Length,pGamePoseArray,(uint) pGamePoseArray.Length);
		return result;
	}
	public override EVRCompositorError Submit(EVREye eEye,ref Texture_t pTexture,ref VRTextureBounds_t pBounds,EVRSubmitFlags nSubmitFlags)
	{
		CheckIfUsable();
		EVRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_Submit(m_pVRCompositor,eEye,ref pTexture,ref pBounds,nSubmitFlags);
		return result;
	}
	public override void ClearLastSubmittedFrame()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_ClearLastSubmittedFrame(m_pVRCompositor);
	}
	public override void PostPresentHandoff()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_PostPresentHandoff(m_pVRCompositor);
	}
	public override bool GetFrameTiming(ref Compositor_FrameTiming pTiming,uint unFramesAgo)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_GetFrameTiming(m_pVRCompositor,ref pTiming,unFramesAgo);
		return result;
	}
	public override float GetFrameTimeRemaining()
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRCompositor_GetFrameTimeRemaining(m_pVRCompositor);
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
	public override EVRCompositorError SetSkyboxOverride(Texture_t [] pTextures)
	{
		CheckIfUsable();
		EVRCompositorError result = VRNativeEntrypoints.VR_IVRCompositor_SetSkyboxOverride(m_pVRCompositor,pTextures,(uint) pTextures.Length);
		return result;
	}
	public override void ClearSkyboxOverride()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_ClearSkyboxOverride(m_pVRCompositor);
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
	public override uint GetCurrentSceneFocusProcess()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCompositor_GetCurrentSceneFocusProcess(m_pVRCompositor);
		return result;
	}
	public override uint GetLastFrameRenderer()
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRCompositor_GetLastFrameRenderer(m_pVRCompositor);
		return result;
	}
	public override bool CanRenderScene()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_CanRenderScene(m_pVRCompositor);
		return result;
	}
	public override void ShowMirrorWindow()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_ShowMirrorWindow(m_pVRCompositor);
	}
	public override void HideMirrorWindow()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_HideMirrorWindow(m_pVRCompositor);
	}
	public override bool IsMirrorWindowVisible()
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRCompositor_IsMirrorWindowVisible(m_pVRCompositor);
		return result;
	}
	public override void CompositorDumpImages()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRCompositor_CompositorDumpImages(m_pVRCompositor);
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
	public override EVROverlayError FindOverlay(string pchOverlayKey,ref ulong pOverlayHandle)
	{
		CheckIfUsable();
		pOverlayHandle = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_FindOverlay(m_pVROverlay,pchOverlayKey,ref pOverlayHandle);
		return result;
	}
	public override EVROverlayError CreateOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pOverlayHandle)
	{
		CheckIfUsable();
		pOverlayHandle = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_CreateOverlay(m_pVROverlay,pchOverlayKey,pchOverlayFriendlyName,ref pOverlayHandle);
		return result;
	}
	public override EVROverlayError DestroyOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_DestroyOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override EVROverlayError SetHighQualityOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetHighQualityOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override ulong GetHighQualityOverlay()
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVROverlay_GetHighQualityOverlay(m_pVROverlay);
		return result;
	}
	public override uint GetOverlayKey(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref EVROverlayError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayKey(m_pVROverlay,ulOverlayHandle,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override uint GetOverlayName(ulong ulOverlayHandle,System.Text.StringBuilder pchValue,uint unBufferSize,ref EVROverlayError pError)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayName(m_pVROverlay,ulOverlayHandle,pchValue,unBufferSize,ref pError);
		return result;
	}
	public override EVROverlayError GetOverlayImageData(ulong ulOverlayHandle,IntPtr pvBuffer,uint unBufferSize,ref uint punWidth,ref uint punHeight)
	{
		CheckIfUsable();
		punWidth = 0;
		punHeight = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayImageData(m_pVROverlay,ulOverlayHandle,pvBuffer,unBufferSize,ref punWidth,ref punHeight);
		return result;
	}
	public override string GetOverlayErrorNameFromEnum(EVROverlayError error)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayErrorNameFromEnum(m_pVROverlay,error);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override EVROverlayError SetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,bool bEnabled)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayFlag(m_pVROverlay,ulOverlayHandle,eOverlayFlag,bEnabled);
		return result;
	}
	public override EVROverlayError GetOverlayFlag(ulong ulOverlayHandle,VROverlayFlags eOverlayFlag,ref bool pbEnabled)
	{
		CheckIfUsable();
		pbEnabled = false;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayFlag(m_pVROverlay,ulOverlayHandle,eOverlayFlag,ref pbEnabled);
		return result;
	}
	public override EVROverlayError SetOverlayColor(ulong ulOverlayHandle,float fRed,float fGreen,float fBlue)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayColor(m_pVROverlay,ulOverlayHandle,fRed,fGreen,fBlue);
		return result;
	}
	public override EVROverlayError GetOverlayColor(ulong ulOverlayHandle,ref float pfRed,ref float pfGreen,ref float pfBlue)
	{
		CheckIfUsable();
		pfRed = 0;
		pfGreen = 0;
		pfBlue = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayColor(m_pVROverlay,ulOverlayHandle,ref pfRed,ref pfGreen,ref pfBlue);
		return result;
	}
	public override EVROverlayError SetOverlayAlpha(ulong ulOverlayHandle,float fAlpha)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayAlpha(m_pVROverlay,ulOverlayHandle,fAlpha);
		return result;
	}
	public override EVROverlayError GetOverlayAlpha(ulong ulOverlayHandle,ref float pfAlpha)
	{
		CheckIfUsable();
		pfAlpha = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayAlpha(m_pVROverlay,ulOverlayHandle,ref pfAlpha);
		return result;
	}
	public override EVROverlayError SetOverlayWidthInMeters(ulong ulOverlayHandle,float fWidthInMeters)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayWidthInMeters(m_pVROverlay,ulOverlayHandle,fWidthInMeters);
		return result;
	}
	public override EVROverlayError GetOverlayWidthInMeters(ulong ulOverlayHandle,ref float pfWidthInMeters)
	{
		CheckIfUsable();
		pfWidthInMeters = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayWidthInMeters(m_pVROverlay,ulOverlayHandle,ref pfWidthInMeters);
		return result;
	}
	public override EVROverlayError SetOverlayAutoCurveDistanceRangeInMeters(ulong ulOverlayHandle,float fMinDistanceInMeters,float fMaxDistanceInMeters)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayAutoCurveDistanceRangeInMeters(m_pVROverlay,ulOverlayHandle,fMinDistanceInMeters,fMaxDistanceInMeters);
		return result;
	}
	public override EVROverlayError GetOverlayAutoCurveDistanceRangeInMeters(ulong ulOverlayHandle,ref float pfMinDistanceInMeters,ref float pfMaxDistanceInMeters)
	{
		CheckIfUsable();
		pfMinDistanceInMeters = 0;
		pfMaxDistanceInMeters = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayAutoCurveDistanceRangeInMeters(m_pVROverlay,ulOverlayHandle,ref pfMinDistanceInMeters,ref pfMaxDistanceInMeters);
		return result;
	}
	public override EVROverlayError SetOverlayTextureColorSpace(ulong ulOverlayHandle,EColorSpace eTextureColorSpace)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTextureColorSpace(m_pVROverlay,ulOverlayHandle,eTextureColorSpace);
		return result;
	}
	public override EVROverlayError GetOverlayTextureColorSpace(ulong ulOverlayHandle,ref EColorSpace peTextureColorSpace)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTextureColorSpace(m_pVROverlay,ulOverlayHandle,ref peTextureColorSpace);
		return result;
	}
	public override EVROverlayError SetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTextureBounds(m_pVROverlay,ulOverlayHandle,ref pOverlayTextureBounds);
		return result;
	}
	public override EVROverlayError GetOverlayTextureBounds(ulong ulOverlayHandle,ref VRTextureBounds_t pOverlayTextureBounds)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTextureBounds(m_pVROverlay,ulOverlayHandle,ref pOverlayTextureBounds);
		return result;
	}
	public override EVROverlayError GetOverlayTransformType(ulong ulOverlayHandle,ref VROverlayTransformType peTransformType)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformType(m_pVROverlay,ulOverlayHandle,ref peTransformType);
		return result;
	}
	public override EVROverlayError SetOverlayTransformAbsolute(ulong ulOverlayHandle,ETrackingUniverseOrigin eTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTransformAbsolute(m_pVROverlay,ulOverlayHandle,eTrackingOrigin,ref pmatTrackingOriginToOverlayTransform);
		return result;
	}
	public override EVROverlayError GetOverlayTransformAbsolute(ulong ulOverlayHandle,ref ETrackingUniverseOrigin peTrackingOrigin,ref HmdMatrix34_t pmatTrackingOriginToOverlayTransform)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformAbsolute(m_pVROverlay,ulOverlayHandle,ref peTrackingOrigin,ref pmatTrackingOriginToOverlayTransform);
		return result;
	}
	public override EVROverlayError SetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,uint unTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative(m_pVROverlay,ulOverlayHandle,unTrackedDevice,ref pmatTrackedDeviceToOverlayTransform);
		return result;
	}
	public override EVROverlayError GetOverlayTransformTrackedDeviceRelative(ulong ulOverlayHandle,ref uint punTrackedDevice,ref HmdMatrix34_t pmatTrackedDeviceToOverlayTransform)
	{
		CheckIfUsable();
		punTrackedDevice = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative(m_pVROverlay,ulOverlayHandle,ref punTrackedDevice,ref pmatTrackedDeviceToOverlayTransform);
		return result;
	}
	public override EVROverlayError ShowOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ShowOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override EVROverlayError HideOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_HideOverlay(m_pVROverlay,ulOverlayHandle);
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
	public override EVROverlayError GetOverlayInputMethod(ulong ulOverlayHandle,ref VROverlayInputMethod peInputMethod)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayInputMethod(m_pVROverlay,ulOverlayHandle,ref peInputMethod);
		return result;
	}
	public override EVROverlayError SetOverlayInputMethod(ulong ulOverlayHandle,VROverlayInputMethod eInputMethod)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayInputMethod(m_pVROverlay,ulOverlayHandle,eInputMethod);
		return result;
	}
	public override EVROverlayError GetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetOverlayMouseScale(m_pVROverlay,ulOverlayHandle,ref pvecMouseScale);
		return result;
	}
	public override EVROverlayError SetOverlayMouseScale(ulong ulOverlayHandle,ref HmdVector2_t pvecMouseScale)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayMouseScale(m_pVROverlay,ulOverlayHandle,ref pvecMouseScale);
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
	public override bool IsHoverTargetOverlay(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVROverlay_IsHoverTargetOverlay(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override ulong GetGamepadFocusOverlay()
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVROverlay_GetGamepadFocusOverlay(m_pVROverlay);
		return result;
	}
	public override EVROverlayError SetGamepadFocusOverlay(ulong ulNewFocusOverlay)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetGamepadFocusOverlay(m_pVROverlay,ulNewFocusOverlay);
		return result;
	}
	public override EVROverlayError SetOverlayNeighbor(EOverlayDirection eDirection,ulong ulFrom,ulong ulTo)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayNeighbor(m_pVROverlay,eDirection,ulFrom,ulTo);
		return result;
	}
	public override EVROverlayError MoveGamepadFocusToNeighbor(EOverlayDirection eDirection,ulong ulFrom)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_MoveGamepadFocusToNeighbor(m_pVROverlay,eDirection,ulFrom);
		return result;
	}
	public override EVROverlayError SetOverlayTexture(ulong ulOverlayHandle,ref Texture_t pTexture)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayTexture(m_pVROverlay,ulOverlayHandle,ref pTexture);
		return result;
	}
	public override EVROverlayError ClearOverlayTexture(ulong ulOverlayHandle)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ClearOverlayTexture(m_pVROverlay,ulOverlayHandle);
		return result;
	}
	public override EVROverlayError SetOverlayRaw(ulong ulOverlayHandle,IntPtr pvBuffer,uint unWidth,uint unHeight,uint unDepth)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayRaw(m_pVROverlay,ulOverlayHandle,pvBuffer,unWidth,unHeight,unDepth);
		return result;
	}
	public override EVROverlayError SetOverlayFromFile(ulong ulOverlayHandle,string pchFilePath)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetOverlayFromFile(m_pVROverlay,ulOverlayHandle,pchFilePath);
		return result;
	}
	public override EVROverlayError CreateDashboardOverlay(string pchOverlayKey,string pchOverlayFriendlyName,ref ulong pMainHandle,ref ulong pThumbnailHandle)
	{
		CheckIfUsable();
		pMainHandle = 0;
		pThumbnailHandle = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_CreateDashboardOverlay(m_pVROverlay,pchOverlayKey,pchOverlayFriendlyName,ref pMainHandle,ref pThumbnailHandle);
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
	public override EVROverlayError SetDashboardOverlaySceneProcess(ulong ulOverlayHandle,uint unProcessId)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_SetDashboardOverlaySceneProcess(m_pVROverlay,ulOverlayHandle,unProcessId);
		return result;
	}
	public override EVROverlayError GetDashboardOverlaySceneProcess(ulong ulOverlayHandle,ref uint punProcessId)
	{
		CheckIfUsable();
		punProcessId = 0;
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_GetDashboardOverlaySceneProcess(m_pVROverlay,ulOverlayHandle,ref punProcessId);
		return result;
	}
	public override void ShowDashboard(string pchOverlayToShow)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVROverlay_ShowDashboard(m_pVROverlay,pchOverlayToShow);
	}
	public override EVROverlayError ShowKeyboard(int eInputMode,int eLineInputMode,string pchDescription,uint unCharMax,string pchExistingText,bool bUseMinimalMode,ulong uUserValue)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ShowKeyboard(m_pVROverlay,eInputMode,eLineInputMode,pchDescription,unCharMax,pchExistingText,bUseMinimalMode,uUserValue);
		return result;
	}
	public override EVROverlayError ShowKeyboardForOverlay(ulong ulOverlayHandle,int eInputMode,int eLineInputMode,string pchDescription,uint unCharMax,string pchExistingText,bool bUseMinimalMode,ulong uUserValue)
	{
		CheckIfUsable();
		EVROverlayError result = VRNativeEntrypoints.VR_IVROverlay_ShowKeyboardForOverlay(m_pVROverlay,ulOverlayHandle,eInputMode,eLineInputMode,pchDescription,unCharMax,pchExistingText,bUseMinimalMode,uUserValue);
		return result;
	}
	public override uint GetKeyboardText(System.Text.StringBuilder pchText,uint cchText)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVROverlay_GetKeyboardText(m_pVROverlay,pchText,cchText);
		return result;
	}
	public override void HideKeyboard()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVROverlay_HideKeyboard(m_pVROverlay);
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
	public override bool LoadRenderModel(string pchRenderModelName,ref IntPtr ppRenderModel)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRRenderModels_LoadRenderModel(m_pVRRenderModels,pchRenderModelName,ref ppRenderModel);
		return result;
	}
	public override void FreeRenderModel(ref RenderModel_t pRenderModel)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRRenderModels_FreeRenderModel(m_pVRRenderModels,ref pRenderModel);
	}
	public override bool LoadTexture(int textureId,ref IntPtr ppTexture)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRRenderModels_LoadTexture(m_pVRRenderModels,textureId,ref ppTexture);
		return result;
	}
	public override void FreeTexture(ref RenderModel_TextureMap_t pTexture)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRRenderModels_FreeTexture(m_pVRRenderModels,ref pTexture);
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
	public override uint GetComponentCount(string pchRenderModelName)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRRenderModels_GetComponentCount(m_pVRRenderModels,pchRenderModelName);
		return result;
	}
	public override uint GetComponentName(string pchRenderModelName,uint unComponentIndex,System.Text.StringBuilder pchComponentName,uint unComponentNameLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRRenderModels_GetComponentName(m_pVRRenderModels,pchRenderModelName,unComponentIndex,pchComponentName,unComponentNameLen);
		return result;
	}
	public override ulong GetComponentButtonMask(string pchRenderModelName,string pchComponentName)
	{
		CheckIfUsable();
		ulong result = VRNativeEntrypoints.VR_IVRRenderModels_GetComponentButtonMask(m_pVRRenderModels,pchRenderModelName,pchComponentName);
		return result;
	}
	public override uint GetComponentRenderModelName(string pchRenderModelName,string pchComponentName,System.Text.StringBuilder pchComponentRenderModelName,uint unComponentRenderModelNameLen)
	{
		CheckIfUsable();
		uint result = VRNativeEntrypoints.VR_IVRRenderModels_GetComponentRenderModelName(m_pVRRenderModels,pchRenderModelName,pchComponentName,pchComponentRenderModelName,unComponentRenderModelNameLen);
		return result;
	}
	public override bool GetComponentState(string pchRenderModelName,string pchComponentName,ref VRControllerState_t pControllerState,ref RenderModel_ComponentState_t pComponentState)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRRenderModels_GetComponentState(m_pVRRenderModels,pchRenderModelName,pchComponentName,ref pControllerState,ref pComponentState);
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
	public override uint StartVRProcess(string pchExecutable,ref string pchArguments,uint unArgumentCount,string pchWorkingDirectory)
	{
		CheckIfUsable();
		pchArguments = "";
		uint result = VRNativeEntrypoints.VR_IVRControlPanel_StartVRProcess(m_pVRControlPanel,pchExecutable,ref pchArguments,unArgumentCount,pchWorkingDirectory);
		return result;
	}
	public override void SetMasterProcessToThis()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRControlPanel_SetMasterProcessToThis(m_pVRControlPanel);
	}
	public override void StartAutolaunchOverlays()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRControlPanel_StartAutolaunchOverlays(m_pVRControlPanel);
	}
	public override bool ForceQuitProcess(uint pidProcessToQuit)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRControlPanel_ForceQuitProcess(m_pVRControlPanel,pidProcessToQuit);
		return result;
	}
	public override void AbortTransition()
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRControlPanel_AbortTransition(m_pVRControlPanel);
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
	public override EVRNotificationError CreateNotification(ulong ulOverlayHandle,ulong ulUserValue,EVRNotificationType type,string pchText,EVRNotificationStyle style,ref NotificationBitmap_t pImage,ref uint pNotificationId)
	{
		CheckIfUsable();
		pNotificationId = 0;
		EVRNotificationError result = VRNativeEntrypoints.VR_IVRNotifications_CreateNotification(m_pVRNotifications,ulOverlayHandle,ulUserValue,type,pchText,style,ref pImage,ref pNotificationId);
		return result;
	}
	public override EVRNotificationError RemoveNotification(uint notificationId)
	{
		CheckIfUsable();
		EVRNotificationError result = VRNativeEntrypoints.VR_IVRNotifications_RemoveNotification(m_pVRNotifications,notificationId);
		return result;
	}
}


public class CVRSettings : IVRSettings
{
	public CVRSettings(IntPtr VRSettings)
	{
		m_pVRSettings = VRSettings;
	}
	IntPtr m_pVRSettings;

	public override IntPtr GetIntPtr() { return m_pVRSettings; }

	private void CheckIfUsable()
	{
		if (m_pVRSettings == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override string GetSettingsErrorNameFromEnum(EVRSettingsError eError)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRSettings_GetSettingsErrorNameFromEnum(m_pVRSettings,eError);
		return (string) Marshal.PtrToStructure(result, typeof(string));
	}
	public override void Sync(ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_Sync(m_pVRSettings,ref peError);
	}
	public override bool GetBool(string pchSection,string pchSettingsKey,bool bDefaultValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRSettings_GetBool(m_pVRSettings,pchSection,pchSettingsKey,bDefaultValue,ref peError);
		return result;
	}
	public override void SetBool(string pchSection,string pchSettingsKey,bool bValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_SetBool(m_pVRSettings,pchSection,pchSettingsKey,bValue,ref peError);
	}
	public override int GetInt32(string pchSection,string pchSettingsKey,int nDefaultValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		int result = VRNativeEntrypoints.VR_IVRSettings_GetInt32(m_pVRSettings,pchSection,pchSettingsKey,nDefaultValue,ref peError);
		return result;
	}
	public override void SetInt32(string pchSection,string pchSettingsKey,int nValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_SetInt32(m_pVRSettings,pchSection,pchSettingsKey,nValue,ref peError);
	}
	public override float GetFloat(string pchSection,string pchSettingsKey,float flDefaultValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRSettings_GetFloat(m_pVRSettings,pchSection,pchSettingsKey,flDefaultValue,ref peError);
		return result;
	}
	public override void SetFloat(string pchSection,string pchSettingsKey,float flValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_SetFloat(m_pVRSettings,pchSection,pchSettingsKey,flValue,ref peError);
	}
	public override void GetString(string pchSection,string pchSettingsKey,string pchValue,uint unValueLen,string pchDefaultValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_GetString(m_pVRSettings,pchSection,pchSettingsKey,pchValue,unValueLen,pchDefaultValue,ref peError);
	}
	public override void SetString(string pchSection,string pchSettingsKey,string pchValue,ref EVRSettingsError peError)
	{
		CheckIfUsable();
		VRNativeEntrypoints.VR_IVRSettings_SetString(m_pVRSettings,pchSection,pchSettingsKey,pchValue,ref peError);
	}
}


public class CVRTrackedCamera : IVRTrackedCamera
{
	public CVRTrackedCamera(IntPtr VRTrackedCamera)
	{
		m_pVRTrackedCamera = VRTrackedCamera;
	}
	IntPtr m_pVRTrackedCamera;

	public override IntPtr GetIntPtr() { return m_pVRTrackedCamera; }

	private void CheckIfUsable()
	{
		if (m_pVRTrackedCamera == IntPtr.Zero)
		{
			throw new Exception("Steam Pointer not configured");
		}
	}
	public override bool HasCamera(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_HasCamera(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool GetCameraFirmwareDescription(uint nDeviceIndex,string pBuffer,uint nBufferLen)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetCameraFirmwareDescription(m_pVRTrackedCamera,nDeviceIndex,pBuffer,nBufferLen);
		return result;
	}
	public override bool GetCameraFrameDimensions(uint nDeviceIndex,ECameraVideoStreamFormat nVideoStreamFormat,ref uint pWidth,ref uint pHeight)
	{
		CheckIfUsable();
		pWidth = 0;
		pHeight = 0;
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetCameraFrameDimensions(m_pVRTrackedCamera,nDeviceIndex,nVideoStreamFormat,ref pWidth,ref pHeight);
		return result;
	}
	public override bool SetCameraVideoStreamFormat(uint nDeviceIndex,ECameraVideoStreamFormat nVideoStreamFormat)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_SetCameraVideoStreamFormat(m_pVRTrackedCamera,nDeviceIndex,nVideoStreamFormat);
		return result;
	}
	public override ECameraVideoStreamFormat GetCameraVideoStreamFormat(uint nDeviceIndex)
	{
		CheckIfUsable();
		ECameraVideoStreamFormat result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetCameraVideoStreamFormat(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool EnableCameraForStreaming(uint nDeviceIndex,bool bEnable)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_EnableCameraForStreaming(m_pVRTrackedCamera,nDeviceIndex,bEnable);
		return result;
	}
	public override bool StartVideoStream(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_StartVideoStream(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool StopVideoStream(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_StopVideoStream(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool IsVideoStreamActive(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_IsVideoStreamActive(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override float GetVideoStreamElapsedTime(uint nDeviceIndex)
	{
		CheckIfUsable();
		float result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetVideoStreamElapsedTime(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override CameraVideoStreamFrame_t GetVideoStreamFrame(uint nDeviceIndex)
	{
		CheckIfUsable();
		IntPtr result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetVideoStreamFrame(m_pVRTrackedCamera,nDeviceIndex);
		return (CameraVideoStreamFrame_t) Marshal.PtrToStructure(result, typeof(CameraVideoStreamFrame_t));
	}
	public override bool ReleaseVideoStreamFrame(uint nDeviceIndex,ref CameraVideoStreamFrame_t pFrameImage)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_ReleaseVideoStreamFrame(m_pVRTrackedCamera,nDeviceIndex,ref pFrameImage);
		return result;
	}
	public override bool SetAutoExposure(uint nDeviceIndex,bool bEnable)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_SetAutoExposure(m_pVRTrackedCamera,nDeviceIndex,bEnable);
		return result;
	}
	public override bool SupportsPauseResume(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_SupportsPauseResume(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool PauseVideoStream(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_PauseVideoStream(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool ResumeVideoStream(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_ResumeVideoStream(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool IsVideoStreamPaused(uint nDeviceIndex)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_IsVideoStreamPaused(m_pVRTrackedCamera,nDeviceIndex);
		return result;
	}
	public override bool GetCameraDistortion(uint nDeviceIndex,float flInputU,float flInputV,ref float pflOutputU,ref float pflOutputV)
	{
		CheckIfUsable();
		pflOutputU = 0;
		pflOutputV = 0;
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetCameraDistortion(m_pVRTrackedCamera,nDeviceIndex,flInputU,flInputV,ref pflOutputU,ref pflOutputV);
		return result;
	}
	public override bool GetCameraProjection(uint nDeviceIndex,float flWidthPixels,float flHeightPixels,float flZNear,float flZFar,ref HmdMatrix44_t pProjection)
	{
		CheckIfUsable();
		bool result = VRNativeEntrypoints.VR_IVRTrackedCamera_GetCameraProjection(m_pVRTrackedCamera,nDeviceIndex,flWidthPixels,flHeightPixels,flZNear,flZFar,ref pProjection);
		return result;
	}
}


public class OpenVRInterop
{
	[DllImportAttribute("openvr_api", EntryPoint = "VR_Init")]
	internal static extern IntPtr Init(ref EVRInitError peError, EVRApplicationType eApplicationType);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_Shutdown")]
	internal static extern void Shutdown();
	[DllImportAttribute("openvr_api", EntryPoint = "VR_GetGenericInterface")]
	internal static extern IntPtr GetGenericInterface([In, MarshalAs(UnmanagedType.LPStr)] string pchInterfaceVersion, ref EVRInitError peError);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IsHmdPresent")]
	internal static extern bool IsHmdPresent();
	[DllImportAttribute("openvr_api", EntryPoint = "VR_GetStringForHmdError")]
	internal static extern IntPtr GetStringForHmdError(EVRInitError error);
	[DllImportAttribute("openvr_api", EntryPoint = "VR_IsRuntimeInstalled")]
	internal static extern bool VR_IsRuntimeInstalled();
}


public enum EVREye
{
	Eye_Left = 0,
	Eye_Right = 1,
}
public enum EGraphicsAPIConvention
{
	API_DirectX = 0,
	API_OpenGL = 1,
}
public enum EColorSpace
{
	Auto = 0,
	Gamma = 1,
	Linear = 2,
}
public enum ETrackingResult
{
	Uninitialized = 1,
	Calibrating_InProgress = 100,
	Calibrating_OutOfRange = 101,
	Running_OK = 200,
	Running_OutOfRange = 201,
}
public enum ETrackedDeviceClass
{
	Invalid = 0,
	HMD = 1,
	Controller = 2,
	TrackingReference = 4,
	Other = 1000,
}
public enum ETrackingUniverseOrigin
{
	TrackingUniverseSeated = 0,
	TrackingUniverseStanding = 1,
	TrackingUniverseRawAndUncalibrated = 2,
}
public enum ETrackedDeviceProperty
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
	Prop_Firmware_UpdateAvailable_Bool = 1014,
	Prop_Firmware_ManualUpdate_Bool = 1015,
	Prop_Firmware_ManualUpdateURL_String = 1016,
	Prop_HardwareRevision_Uint64 = 1017,
	Prop_FirmwareVersion_Uint64 = 1018,
	Prop_FPGAVersion_Uint64 = 1019,
	Prop_VRCVersion_Uint64 = 1020,
	Prop_RadioVersion_Uint64 = 1021,
	Prop_DongleVersion_Uint64 = 1022,
	Prop_BlockServerShutdown_Bool = 1023,
	Prop_CanUnifyCoordinateSystemWithHmd_Bool = 1024,
	Prop_ContainsProximitySensor_Bool = 1025,
	Prop_DeviceProvidesBatteryStatus_Bool = 1026,
	Prop_ReportsTimeSinceVSync_Bool = 2000,
	Prop_SecondsFromVsyncToPhotons_Float = 2001,
	Prop_DisplayFrequency_Float = 2002,
	Prop_UserIpdMeters_Float = 2003,
	Prop_CurrentUniverseId_Uint64 = 2004,
	Prop_PreviousUniverseId_Uint64 = 2005,
	Prop_DisplayFirmwareVersion_String = 2006,
	Prop_IsOnDesktop_Bool = 2007,
	Prop_DisplayMCType_Int32 = 2008,
	Prop_DisplayMCOffset_Float = 2009,
	Prop_DisplayMCScale_Float = 2010,
	Prop_EdidVendorID_Int32 = 2011,
	Prop_DisplayMCImageLeft_String = 2012,
	Prop_DisplayMCImageRight_String = 2013,
	Prop_DisplayGCBlackClamp_Float = 2014,
	Prop_EdidProductID_Int32 = 2015,
	Prop_CameraToHeadTransform_Matrix34 = 2016,
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
	Prop_VendorSpecific_Reserved_Start = 10000,
	Prop_VendorSpecific_Reserved_End = 10999,
}
public enum ETrackedPropertyError
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
	TrackedProp_NotYetAvailable = 9,
}
public enum EVRSubmitFlags
{
	Submit_Default = 0,
	Submit_LensDistortionAlreadyApplied = 1,
	Submit_GlRenderBuffer = 2,
}
public enum EVRState
{
	Undefined = -1,
	Off = 0,
	Searching = 1,
	Searching_Alert = 2,
	Ready = 3,
	Ready_Alert = 4,
	NotReady = 5,
}
public enum EVREventType
{
	VREvent_None = 0,
	VREvent_TrackedDeviceActivated = 100,
	VREvent_TrackedDeviceDeactivated = 101,
	VREvent_TrackedDeviceUpdated = 102,
	VREvent_TrackedDeviceUserInteractionStarted = 103,
	VREvent_TrackedDeviceUserInteractionEnded = 104,
	VREvent_IpdChanged = 105,
	VREvent_ButtonPress = 200,
	VREvent_ButtonUnpress = 201,
	VREvent_ButtonTouch = 202,
	VREvent_ButtonUntouch = 203,
	VREvent_MouseMove = 300,
	VREvent_MouseButtonDown = 301,
	VREvent_MouseButtonUp = 302,
	VREvent_FocusEnter = 303,
	VREvent_FocusLeave = 304,
	VREvent_InputFocusCaptured = 400,
	VREvent_InputFocusReleased = 401,
	VREvent_SceneFocusLost = 402,
	VREvent_SceneFocusGained = 403,
	VREvent_SceneApplicationChanged = 404,
	VREvent_SceneFocusChanged = 405,
	VREvent_OverlayShown = 500,
	VREvent_OverlayHidden = 501,
	VREvent_DashboardActivated = 502,
	VREvent_DashboardDeactivated = 503,
	VREvent_DashboardThumbSelected = 504,
	VREvent_DashboardRequested = 505,
	VREvent_ResetDashboard = 506,
	VREvent_RenderToast = 507,
	VREvent_ImageLoaded = 508,
	VREvent_ShowKeyboard = 509,
	VREvent_HideKeyboard = 510,
	VREvent_OverlayGamepadFocusGained = 511,
	VREvent_OverlayGamepadFocusLost = 512,
	VREvent_Notification_Shown = 600,
	VREvent_Notification_Hidden = 601,
	VREvent_Notification_BeginInteraction = 602,
	VREvent_Notification_Destroyed = 603,
	VREvent_Quit = 700,
	VREvent_ProcessQuit = 701,
	VREvent_QuitAborted_UserPrompt = 702,
	VREvent_QuitAcknowledged = 703,
	VREvent_ChaperoneDataHasChanged = 800,
	VREvent_ChaperoneUniverseHasChanged = 801,
	VREvent_ChaperoneTempDataHasChanged = 802,
	VREvent_ChaperoneSettingsHaveChanged = 803,
	VREvent_StatusUpdate = 900,
	VREvent_MCImageUpdated = 1000,
	VREvent_FirmwareUpdateStarted = 1100,
	VREvent_FirmwareUpdateFinished = 1101,
	VREvent_KeyboardClosed = 1200,
	VREvent_KeyboardCharInput = 1201,
	VREvent_ApplicationTransitionStarted = 1300,
	VREvent_ApplicationTransitionAborted = 1301,
	VREvent_ApplicationTransitionNewAppStarted = 1302,
	VREvent_Compositor_MirrorWindowShown = 1400,
	VREvent_Compositor_MirrorWindowHidden = 1401,
	VREvent_TrackedCamera_StartVideoStream = 1500,
	VREvent_TrackedCamera_StopVideoStream = 1501,
	VREvent_TrackedCamera_PauseVideoStream = 1502,
	VREvent_TrackedCamera_ResumeVideoStream = 1503,
	VREvent_VendorSpecific_Reserved_Start = 10000,
	VREvent_VendorSpecific_Reserved_End = 19999,
}
public enum EDeviceActivityLevel
{
	k_EDeviceActivityLevel_Unknown = -1,
	k_EDeviceActivityLevel_Idle = 0,
	k_EDeviceActivityLevel_UserInteraction = 1,
	k_EDeviceActivityLevel_UserInteraction_Timeout = 2,
}
public enum EVRButtonId
{
	k_EButton_System = 0,
	k_EButton_ApplicationMenu = 1,
	k_EButton_Grip = 2,
	k_EButton_DPad_Left = 3,
	k_EButton_DPad_Up = 4,
	k_EButton_DPad_Right = 5,
	k_EButton_DPad_Down = 6,
	k_EButton_A = 7,
	k_EButton_Axis0 = 32,
	k_EButton_Axis1 = 33,
	k_EButton_Axis2 = 34,
	k_EButton_Axis3 = 35,
	k_EButton_Axis4 = 36,
	k_EButton_SteamVR_Touchpad = 32,
	k_EButton_SteamVR_Trigger = 33,
	k_EButton_Dashboard_Back = 2,
	k_EButton_Max = 64,
}
public enum EVRMouseButton
{
	Left = 1,
	Right = 2,
	Middle = 4,
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
public enum ECollisionBoundsStyle
{
	COLLISION_BOUNDS_STYLE_BEGINNER = 0,
	COLLISION_BOUNDS_STYLE_INTERMEDIATE = 1,
	COLLISION_BOUNDS_STYLE_SQUARES = 2,
	COLLISION_BOUNDS_STYLE_ADVANCED = 3,
	COLLISION_BOUNDS_STYLE_NONE = 4,
	COLLISION_BOUNDS_STYLE_COUNT = 5,
}
public enum EVROverlayError
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
	VROVerlayError_KeyboardAlreadyInUse = 26,
	NoNeighbor = 27,
}
public enum EVRApplicationType
{
	VRApplication_Other = 0,
	VRApplication_Scene = 1,
	VRApplication_Overlay = 2,
	VRApplication_Background = 3,
}
public enum EVRFirmwareError
{
	None = 0,
	Success = 1,
	Fail = 2,
}
public enum EVRNotificationError
{
	OK = 0,
	InvalidNotificationId = 100,
}
public enum EVRInitError
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
	Init_Retry = 115,
	Init_InitCanceledByUser = 116,
	Init_AnotherAppLaunching = 117,
	Init_SettingsInitFailed = 118,
	Init_ShuttingDown = 119,
	Init_TooManyObjects = 120,
	Init_NoServerForBackgroundApp = 121,
	Init_NotSupportedWithCompositor = 122,
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
	IPC_Failed = 305,
	VendorSpecific_UnableToConnectToOculusRuntime = 1000,
	VendorSpecific_HmdFound_But = 1100,
	VendorSpecific_HmdFound_CantOpenDevice = 1101,
	VendorSpecific_HmdFound_UnableToRequestConfigStart = 1102,
	VendorSpecific_HmdFound_NoStoredConfig = 1103,
	VendorSpecific_HmdFound_ConfigTooBig = 1104,
	VendorSpecific_HmdFound_ConfigTooSmall = 1105,
	VendorSpecific_HmdFound_UnableToInitZLib = 1106,
	VendorSpecific_HmdFound_CantReadFirmwareVersion = 1107,
	VendorSpecific_HmdFound_UnableToSendUserDataStart = 1108,
	VendorSpecific_HmdFound_UnableToGetUserDataStart = 1109,
	VendorSpecific_HmdFound_UnableToGetUserDataNext = 1110,
	VendorSpecific_HmdFound_UserDataAddressRange = 1111,
	VendorSpecific_HmdFound_UserDataError = 1112,
	Steam_SteamInstallationNotFound = 2000,
}
public enum EVRApplicationError
{
	None = 0,
	AppKeyAlreadyExists = 100,
	NoManifest = 101,
	NoApplication = 102,
	InvalidIndex = 103,
	UnknownApplication = 104,
	IPCFailed = 105,
	ApplicationAlreadyRunning = 106,
	InvalidManifest = 107,
	InvalidApplication = 108,
	LaunchFailed = 109,
	ApplicationAlreadyStarting = 110,
	LaunchInProgress = 111,
	OldApplicationQuitting = 112,
	TransitionAborted = 113,
	BufferTooSmall = 200,
	PropertyNotSet = 201,
	UnknownProperty = 202,
}
public enum EVRApplicationProperty
{
	Name_String = 0,
	LaunchType_String = 11,
	WorkingDirectory_String = 12,
	BinaryPath_String = 13,
	Arguments_String = 14,
	URL_String = 15,
	Description_String = 50,
	NewsURL_String = 51,
	ImagePath_String = 52,
	Source_String = 53,
	IsDashboardOverlay_Bool = 60,
}
public enum EVRApplicationTransitionState
{
	VRApplicationTransition_None = 0,
	VRApplicationTransition_OldAppQuitSent = 10,
	VRApplicationTransition_WaitingForExternalLaunch = 11,
	VRApplicationTransition_NewAppLaunched = 20,
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
	Error_PlayAreaInvalid = 203,
	Error_CollisionBoundsInvalid = 204,
}
public enum EChaperoneConfigFile
{
	Live = 1,
	Temp = 2,
}
public enum EVRCompositorError
{
	None = 0,
	IncompatibleVersion = 100,
	DoNotHaveFocus = 101,
	InvalidTexture = 102,
	IsNotSceneApplication = 103,
	TextureIsOnWrongDevice = 104,
	TextureUsesUnsupportedFormat = 105,
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
	AcceptsGamepadEvents = 4,
	ShowGamepadFocus = 5,
}
public enum EGamepadTextInputMode
{
	k_EGamepadTextInputModeNormal = 0,
	k_EGamepadTextInputModePassword = 1,
}
public enum EGamepadTextInputLineMode
{
	k_EGamepadTextInputLineModeSingleLine = 0,
	k_EGamepadTextInputLineModeMultipleLines = 1,
}
public enum EOverlayDirection
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3,
	Count = 4,
}
public enum EVRComponentProperty
{
	IsStatic = 1,
	IsVisible = 2,
	IsTouched = 4,
	IsPressed = 8,
}
public enum EVRNotificationType
{
	Transient = 0,
	Persistent = 1,
}
public enum EVRNotificationStyle
{
	None = 0,
	Application = 100,
	Contact_Disabled = 200,
	Contact_Enabled = 201,
	Contact_Active = 202,
}
public enum EVRSettingsError
{
	None = 0,
	IPCFailed = 1,
	WriteFailed = 2,
	ReadFailed = 3,
}
public enum ECameraVideoStreamFormat
{
	CVS_FORMAT_UNKNOWN = 0,
	CVS_FORMAT_RAW10 = 1,
	CVS_FORMAT_NV12 = 2,
	CVS_FORMAT_RGB24 = 3,
	CVS_MAX_FORMATS = 4,
}

[StructLayout(LayoutKind.Explicit)] public struct VREvent_Data_t
{
	[FieldOffset(0)] public VREvent_Reserved_t reserved;
	[FieldOffset(0)] public VREvent_Controller_t controller;
	[FieldOffset(0)] public VREvent_Mouse_t mouse;
	[FieldOffset(0)] public VREvent_Process_t process;
	[FieldOffset(0)] public VREvent_Notification_t notification;
	[FieldOffset(0)] public VREvent_Overlay_t overlay;
	[FieldOffset(0)] public VREvent_Status_t status;
	[FieldOffset(0)] public VREvent_Keyboard_t keyboard;
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
[StructLayout(LayoutKind.Sequential)] public struct HmdVector4_t
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4, ArraySubType = UnmanagedType.R4)]
	public float[] v; //float[4]
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
[StructLayout(LayoutKind.Sequential)] public struct HmdColor_t
{
	public float r;
	public float g;
	public float b;
	public float a;
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
[StructLayout(LayoutKind.Sequential)] public struct Texture_t
{
	public IntPtr handle; // void *
	public EGraphicsAPIConvention eType;
	public EColorSpace eColorSpace;
}
[StructLayout(LayoutKind.Sequential)] public struct TrackedDevicePose_t
{
	public HmdMatrix34_t mDeviceToAbsoluteTracking;
	public HmdVector3_t vVelocity;
	public HmdVector3_t vAngularVelocity;
	public ETrackingResult eTrackingResult;
	[MarshalAs(UnmanagedType.I1)]
	public bool bPoseIsValid;
	[MarshalAs(UnmanagedType.I1)]
	public bool bDeviceIsConnected;
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
	public EVRButtonId button;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Mouse_t
{
	public float x;
	public float y;
	public EVRMouseButton button;
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
	[MarshalAs(UnmanagedType.I1)]
	public bool bForced;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Overlay_t
{
	public ulong overlayHandle;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Status_t
{
	public EVRState statusState;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Keyboard_t
{
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 8)]
	public string cNewInput; //char[8]
	public ulong uUserValue;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Ipd_t
{
	public float ipdMeters;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Chaperone_t
{
	public ulong m_nPreviousUniverse;
	public ulong m_nCurrentUniverse;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_Reserved_t
{
	public ulong reserved0;
	public ulong reserved1;
}
[StructLayout(LayoutKind.Sequential)] public struct VREvent_t
{
	public EVREventType eventType;
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
	public float m_flHandoffStartMs;
	public float m_flHandoffEndMs;
}
[StructLayout(LayoutKind.Sequential)] public struct VROverlayIntersectionParams_t
{
	public HmdVector3_t vSource;
	public HmdVector3_t vDirection;
	public ETrackingUniverseOrigin eOrigin;
}
[StructLayout(LayoutKind.Sequential)] public struct VROverlayIntersectionResults_t
{
	public HmdVector3_t vPoint;
	public HmdVector3_t vNormal;
	public HmdVector2_t vUVs;
	public float fDistance;
}
[StructLayout(LayoutKind.Sequential)] public struct RenderModel_ComponentState_t
{
	public HmdMatrix34_t mTrackingToComponentRenderModel;
	public HmdMatrix34_t mTrackingToComponentLocal;
	public uint uProperties;
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
	public IntPtr rVertexData; // const struct vr::RenderModel_Vertex_t *
	public uint unVertexCount;
	public IntPtr rIndexData; // const uint16_t *
	public uint unTriangleCount;
	public int diffuseTextureId;
}
[StructLayout(LayoutKind.Sequential)] public struct NotificationBitmap_t
{
	public IntPtr bytes; // void *
	public int width;
	public int height;
	public int depth;
}
[StructLayout(LayoutKind.Sequential)] public struct CameraVideoStreamFrame_t
{
	public ECameraVideoStreamFormat m_nStreamFormat;
	public uint m_nWidth;
	public uint m_nHeight;
	public uint m_nFrameSequence;
	public uint m_nTimeStamp;
	public uint m_nBufferIndex;
	public uint m_nBufferCount;
	public uint m_nImageDataSize;
	public double m_flFrameElapsedTime;
	public double m_flFrameCaptureTime;
	[MarshalAs(UnmanagedType.I1)]
	public bool m_bPoseIsValid;
	public HmdMatrix34_t m_matDeviceToAbsoluteTracking;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4, ArraySubType = UnmanagedType.R4)]
	public float[] m_Pad; //float[4]
	public IntPtr m_pImageData; // void *
}

public class OpenVR
{
	public static IntPtr Init(ref EVRInitError peError, EVRApplicationType eApplicationType = EVRApplicationType.VRApplication_Scene)
	{
		return OpenVRInterop.Init(ref peError, eApplicationType);
	}

	public static void Shutdown()
	{
		OpenVRInterop.Shutdown();
	}

	public static IntPtr GetGenericInterface(string pchInterfaceVersion, ref EVRInitError peError)
	{
		return OpenVRInterop.GetGenericInterface(pchInterfaceVersion, ref peError);
	}

	public static bool IsHmdPresent()
	{
		return OpenVRInterop.IsHmdPresent();
	}

	public static string GetStringForHmdError(EVRInitError error)
	{
		return Marshal.PtrToStringAnsi(OpenVRInterop.GetStringForHmdError(error));
	}

	public const uint k_unTrackingStringSize = 32;
	public const uint k_unMaxDriverDebugResponseSize = 32768;
	public const uint k_unTrackedDeviceIndex_Hmd = 0;
	public const uint k_unMaxTrackedDeviceCount = 16;
	public const uint k_unTrackedDeviceIndexInvalid = 4294967295;
	public const uint k_unMaxPropertyStringSize = 32768;
	public const uint k_unControllerStateAxisCount = 5;
	public const ulong k_ulOverlayHandleInvalid = 0;
	public const string IVRSystem_Version = "IVRSystem_009";
	public const string IVRExtendedDisplay_Version = "IVRExtendedDisplay_001";
	public const uint k_unMaxApplicationKeyLength = 128;
	public const string IVRApplications_Version = "IVRApplications_002";
	public const string IVRChaperone_Version = "IVRChaperone_003";
	public const string IVRChaperoneSetup_Version = "IVRChaperoneSetup_004";
	public const string IVRCompositor_Version = "IVRCompositor_009";
	public const uint k_unVROverlayMaxKeyLength = 128;
	public const uint k_unVROverlayMaxNameLength = 128;
	public const uint k_unMaxOverlayCount = 32;
	public const string IVROverlay_Version = "IVROverlay_007";
	public const string k_pch_Controller_Component_GDC2015 = "gdc2015";
	public const string k_pch_Controller_Component_Base = "base";
	public const string k_pch_Controller_Component_Tip = "tip";
	public const string k_pch_Controller_Component_HandGrip = "handgrip";
	public const string IVRRenderModels_Version = "IVRRenderModels_002";
	public const string IVRControlPanel_Version = "IVRControlPanel_001";
	public const uint k_unNotificationTextMaxSize = 256;
	public const string IVRNotifications_Version = "IVRNotifications_002";
	public const uint k_unMaxSettingsKeyLength = 128;
	public const string k_pch_SteamVR_Section = "steamvr";
	public const string k_pch_SteamVR_RequireHmd_String = "requireHmd";
	public const string k_pch_SteamVR_ForcedDriverKey_String = "forcedDriver";
	public const string k_pch_SteamVR_ForcedHmdKey_String = "forcedHmd";
	public const string k_pch_SteamVR_DisplayDebug_Bool = "displayDebug";
	public const string k_pch_SteamVR_EnableDistortion_Bool = "enableDistortion";
	public const string k_pch_SteamVR_DisplayDebugX_Int32 = "displayDebugX";
	public const string k_pch_SteamVR_DisplayDebugY_Int32 = "displayDebugY";
	public const string k_pch_SteamVR_SendSystemButtonToAllApps_Bool = "sendSystemButtonToAllApps";
	public const string k_pch_SteamVR_LogLevel_Int32 = "loglevel";
	public const string k_pch_SteamVR_IPD_Float = "ipd";
	public const string k_pch_SteamVR_Background_String = "background";
	public const string k_pch_SteamVR_ActivateMultipleDrivers_Bool = "activateMultipleDrivers";
	public const string k_pch_Lighthouse_Section = "driver_lighthouse";
	public const string k_pch_Lighthouse_DisableIMU_Bool = "disableimu";
	public const string k_pch_Lighthouse_UseDisambiguation_String = "usedisambiguation";
	public const string k_pch_Lighthouse_DisambiguationDebug_Int32 = "disambiguationdebug";
	public const string k_pch_Lighthouse_PrimaryBasestation_Int32 = "primarybasestation";
	public const string k_pch_Lighthouse_LighthouseName_String = "lighthousename";
	public const string k_pch_Lighthouse_MaxIncidenceAngleDegrees_Float = "maxincidenceangledegrees";
	public const string k_pch_Lighthouse_UseLighthouseDirect_Bool = "uselighthousedirect";
	public const string k_pch_Lighthouse_DBHistory_Bool = "dbhistory";
	public const string k_pch_Lighthouse_OriginOffsetX_Float = "originoffsetx";
	public const string k_pch_Lighthouse_OriginOffsetY_Float = "originoffsety";
	public const string k_pch_Lighthouse_OriginOffsetZ_Float = "originoffsetz";
	public const string k_pch_Lighthouse_HeadingOffset_Float = "headingoffset";
	public const string k_pch_Null_Section = "driver_null";
	public const string k_pch_Null_EnableNullDriver_Bool = "enable";
	public const string k_pch_Null_Id_String = "id";
	public const string k_pch_Null_SerialNumber_String = "serialNumber";
	public const string k_pch_Null_ModelNumber_String = "modelNumber";
	public const string k_pch_Null_WindowX_Int32 = "windowX";
	public const string k_pch_Null_WindowY_Int32 = "windowY";
	public const string k_pch_Null_WindowWidth_Int32 = "windowWidth";
	public const string k_pch_Null_WindowHeight_Int32 = "windowHeight";
	public const string k_pch_Null_RenderWidth_Int32 = "renderWidth";
	public const string k_pch_Null_RenderHeight_Int32 = "renderHeight";
	public const string k_pch_Null_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
	public const string k_pch_Null_DisplayFrequency_Float = "displayFrequency";
	public const string k_pch_Notifications_Section = "notifications";
	public const string k_pch_Notifications_DoNotDisturb_Bool = "DoNotDisturb";
	public const string k_pch_Perf_Section = "perfcheck";
	public const string k_pch_Perf_HeuristicActive_Bool = "heuristicActive";
	public const string k_pch_Perf_NotifyInHMD_Bool = "warnInHMD";
	public const string k_pch_Perf_NotifyOnlyOnce_Bool = "warnOnlyOnce";
	public const string k_pch_Perf_AllowTimingStore_Bool = "allowTimingStore";
	public const string k_pch_Perf_SaveTimingsOnExit_Bool = "saveTimingsOnExit";
	public const string IVRSettings_Version = "IVRSettings_001";
	public const string IVRTrackedCamera_Version = "IVRTrackedCamera_001";
}



public class Unity
{
	public const int k_nRenderEventID_WaitGetPoses = 201510020;
	public const int k_nRenderEventID_SubmitL = 201510021;
	public const int k_nRenderEventID_SubmitR = 201510022;
	public const int k_nRenderEventID_Flush = 201510023;
	public const int k_nRenderEventID_PostPresentHandoff = 201510024;

	[DllImport("openvr_api", EntryPoint="UnityHooks_GetRenderEventFunc")]
	public static extern IntPtr GetRenderEventFunc();

	[DllImport("openvr_api", EntryPoint="UnityHooks_SetSubmitParams")]
	public static extern void SetSubmitParams(VRTextureBounds_t boundsL, VRTextureBounds_t boundsR, EVRSubmitFlags nSubmitFlags);

	[DllImport("openvr_api", EntryPoint="UnityHooks_SetColorSpace")]
	public static extern void SetColorSpace(EColorSpace eColorSpace);

	[DllImport("openvr_api", EntryPoint="UnityHooks_EventWriteString")]
	public static extern void EventWriteString([In, MarshalAs(UnmanagedType.LPWStr)] string sEvent);
}


}

