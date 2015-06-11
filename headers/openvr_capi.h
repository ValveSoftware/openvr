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

typedef uint32_t TrackedDeviceIndex_t;

typedef uint32_t VRNotificationId;

typedef struct VREvent_Controller_t
{
	unsigned int button;
} VREvent_Controller_t;
typedef struct VREvent_Mouse_t
{
	float x;
	float y;
	unsigned int button;
} VREvent_Mouse_t;
typedef struct VREvent_Process_t
{
	unsigned int pid;
	unsigned int oldPid;
} VREvent_Process_t;
typedef struct VREvent_Reserved_t
{
	unsigned long reserved0;
	unsigned long reserved1;
} VREvent_Reserved_t;
typedef union
{
	VREvent_Reserved_t reserved;
	VREvent_Controller_t controller;
	VREvent_Mouse_t mouse;
	VREvent_Process_t process;
} VREvent_Data_t;



unsigned int k_unTrackingStringSize = 32;
unsigned int k_unMaxTrackedDeviceCount = 16;
unsigned int k_unTrackedDeviceIndex_Hmd = 0;
unsigned int k_unMaxDriverDebugResponseSize = 32768;
unsigned int k_unTrackedDeviceIndexInvalid = 4294967295;
unsigned int k_unMaxPropertyStringSize = 32768;
unsigned int k_unControllerStateAxisCount = 5;
char * IVRSystem_Version = "IVRSystem_004";
char * IVRCameraAccess_Version = "IVRCameraAccess_001";
char * IVRChaperone_Version = "IVRChaperone_002";
char * IVRChaperoneSetup_Version = "IVRChaperoneSetup_001";
char * IVRCompositor_Version = "IVRCompositor_006";
unsigned long k_ulOverlayHandleInvalid = 0;
unsigned int k_unVROverlayMaxKeyLength = 128;
unsigned int k_unVROverlayMaxNameLength = 128;
char * IVROverlay_Version = "IVROverlay_001";
char * IVRRenderModels_Version = "IVRRenderModels_001";
char * IVRNotifications_Version = "IVRNotifications_001";
typedef enum Hmd_Eye
{
	Hmd_Eye_Eye_Left = 0,
	Hmd_Eye_Eye_Right = 1,
};
typedef enum GraphicsAPIConvention
{
	GraphicsAPIConvention_API_DirectX = 0,
	GraphicsAPIConvention_API_OpenGL = 1,
};
typedef enum HmdTrackingResult
{
	HmdTrackingResult_TrackingResult_Uninitialized = 1,
	HmdTrackingResult_TrackingResult_Calibrating_InProgress = 100,
	HmdTrackingResult_TrackingResult_Calibrating_OutOfRange = 101,
	HmdTrackingResult_TrackingResult_Running_OK = 200,
	HmdTrackingResult_TrackingResult_Running_OutOfRange = 201,
};
typedef enum TrackedDeviceClass
{
	TrackedDeviceClass_Invalid = 0,
	TrackedDeviceClass_HMD = 1,
	TrackedDeviceClass_Controller = 2,
	TrackedDeviceClass_TrackingReference = 4,
	TrackedDeviceClass_Other = 1000,
};
typedef enum TrackingUniverseOrigin
{
	TrackingUniverseOrigin_TrackingUniverseSeated = 0,
	TrackingUniverseOrigin_TrackingUniverseStanding = 1,
	TrackingUniverseOrigin_TrackingUniverseRawAndUncalibrated = 2,
};
typedef enum TrackedDeviceProperty
{
	TrackedDeviceProperty_Prop_TrackingSystemName_String = 1000,
	TrackedDeviceProperty_Prop_ModelNumber_String = 1001,
	TrackedDeviceProperty_Prop_SerialNumber_String = 1002,
	TrackedDeviceProperty_Prop_RenderModelName_String = 1003,
	TrackedDeviceProperty_Prop_WillDriftInYaw_Bool = 1004,
	TrackedDeviceProperty_Prop_ManufacturerName_String = 1005,
	TrackedDeviceProperty_Prop_TrackingFirmwareVersion_String = 1006,
	TrackedDeviceProperty_Prop_HardwareRevision_String = 1007,
	TrackedDeviceProperty_Prop_AllWirelessDongleDescriptions_String = 1008,
	TrackedDeviceProperty_Prop_ConnectedWirelessDongle_String = 1009,
	TrackedDeviceProperty_Prop_ReportsTimeSinceVSync_Bool = 2000,
	TrackedDeviceProperty_Prop_SecondsFromVsyncToPhotons_Float = 2001,
	TrackedDeviceProperty_Prop_DisplayFrequency_Float = 2002,
	TrackedDeviceProperty_Prop_UserIpdMeters_Float = 2003,
	TrackedDeviceProperty_Prop_CurrentUniverseId_Uint64 = 2004,
	TrackedDeviceProperty_Prop_PreviousUniverseId_Uint64 = 2005,
	TrackedDeviceProperty_Prop_DisplayFirmwareVersion_String = 2006,
	TrackedDeviceProperty_Prop_IsOnDesktop_Bool = 2007,
	TrackedDeviceProperty_Prop_AttachedDeviceId_String = 3000,
	TrackedDeviceProperty_Prop_SupportedButtons_Uint64 = 3001,
	TrackedDeviceProperty_Prop_Axis0Type_Int32 = 3002,
	TrackedDeviceProperty_Prop_Axis1Type_Int32 = 3003,
	TrackedDeviceProperty_Prop_Axis2Type_Int32 = 3004,
	TrackedDeviceProperty_Prop_Axis3Type_Int32 = 3005,
	TrackedDeviceProperty_Prop_Axis4Type_Int32 = 3006,
	TrackedDeviceProperty_Prop_FieldOfViewLeftDegrees_Float = 4000,
	TrackedDeviceProperty_Prop_FieldOfViewRightDegrees_Float = 4001,
	TrackedDeviceProperty_Prop_FieldOfViewTopDegrees_Float = 4002,
	TrackedDeviceProperty_Prop_FieldOfViewBottomDegrees_Float = 4003,
	TrackedDeviceProperty_Prop_TrackingRangeMinimumMeters_Float = 4004,
	TrackedDeviceProperty_Prop_TrackingRangeMaximumMeters_Float = 4005,
};
typedef enum TrackedPropertyError
{
	TrackedPropertyError_TrackedProp_Success = 0,
	TrackedPropertyError_TrackedProp_WrongDataType = 1,
	TrackedPropertyError_TrackedProp_WrongDeviceClass = 2,
	TrackedPropertyError_TrackedProp_BufferTooSmall = 3,
	TrackedPropertyError_TrackedProp_UnknownProperty = 4,
	TrackedPropertyError_TrackedProp_InvalidDevice = 5,
	TrackedPropertyError_TrackedProp_CouldNotContactServer = 6,
	TrackedPropertyError_TrackedProp_ValueNotProvidedByDevice = 7,
	TrackedPropertyError_TrackedProp_StringExceedsMaximumLength = 8,
};
typedef enum EVREventType
{
	EVREventType_VREvent_None = 0,
	EVREventType_VREvent_TrackedDeviceActivated = 100,
	EVREventType_VREvent_TrackedDeviceDeactivated = 101,
	EVREventType_VREvent_TrackedDeviceUpdated = 102,
	EVREventType_VREvent_ButtonPress = 200,
	EVREventType_VREvent_ButtonUnpress = 201,
	EVREventType_VREvent_ButtonTouch = 202,
	EVREventType_VREvent_ButtonUntouch = 203,
	EVREventType_VREvent_MouseMove = 300,
	EVREventType_VREvent_MouseButtonDown = 301,
	EVREventType_VREvent_MouseButtonUp = 302,
	EVREventType_VREvent_InputFocusCaptured = 400,
	EVREventType_VREvent_InputFocusReleased = 401,
	EVREventType_VREvent_SceneFocusLost = 402,
	EVREventType_VREvent_SceneFocusGained = 403,
	EVREventType_VREvent_OverlayShown = 500,
	EVREventType_VREvent_OverlayHidden = 501,
	EVREventType_VREvent_SystemOverlayActivated = 502,
	EVREventType_VREvent_SystemOverlayDeactivated = 503,
	EVREventType_VREvent_SystemOverlayThumbSelected = 504,
	EVREventType_VREvent_Notification_Dismissed = 600,
	EVREventType_VREvent_Notification_BeginInteraction = 601,
	EVREventType_VREvent_Notification_Scroll = 602,
	EVREventType_VREvent_Notification_ClickOn = 603,
	EVREventType_VREvent_Notification_ClickOff = 604,
};
typedef enum EVRButtonId
{
	EVRButtonId_k_EButton_System = 0,
	EVRButtonId_k_EButton_ApplicationMenu = 1,
	EVRButtonId_k_EButton_Grip = 2,
	EVRButtonId_k_EButton_Axis0 = 32,
	EVRButtonId_k_EButton_Axis1 = 33,
	EVRButtonId_k_EButton_Axis2 = 34,
	EVRButtonId_k_EButton_Axis3 = 35,
	EVRButtonId_k_EButton_Axis4 = 36,
	EVRButtonId_k_EButton_SteamVR_Touchpad = 32,
	EVRButtonId_k_EButton_SteamVR_Trigger = 33,
	EVRButtonId_k_EButton_Max = 64,
};
typedef enum EVRMouseButton
{
	EVRMouseButton_VRMouseButton_Left = 1,
	EVRMouseButton_VRMouseButton_Right = 2,
	EVRMouseButton_VRMouseButton_Middle = 4,
};
typedef enum EVRControllerAxisType
{
	EVRControllerAxisType_k_eControllerAxis_None = 0,
	EVRControllerAxisType_k_eControllerAxis_TrackPad = 1,
	EVRControllerAxisType_k_eControllerAxis_Joystick = 2,
	EVRControllerAxisType_k_eControllerAxis_Trigger = 3,
};
typedef enum EVRControllerEventOutputType
{
	EVRControllerEventOutputType_ControllerEventOutput_OSEvents = 0,
	EVRControllerEventOutputType_ControllerEventOutput_VREvents = 1,
};
typedef enum HmdError
{
	HmdError_None = 0,
	HmdError_Unknown = 1,
	HmdError_Init_InstallationNotFound = 100,
	HmdError_Init_InstallationCorrupt = 101,
	HmdError_Init_VRClientDLLNotFound = 102,
	HmdError_Init_FileNotFound = 103,
	HmdError_Init_FactoryNotFound = 104,
	HmdError_Init_InterfaceNotFound = 105,
	HmdError_Init_InvalidInterface = 106,
	HmdError_Init_UserConfigDirectoryInvalid = 107,
	HmdError_Init_HmdNotFound = 108,
	HmdError_Init_NotInitialized = 109,
	HmdError_Init_PathRegistryNotFound = 110,
	HmdError_Init_NoConfigPath = 111,
	HmdError_Init_NoLogPath = 112,
	HmdError_Init_PathRegistryNotWritable = 113,
	HmdError_Driver_Failed = 200,
	HmdError_Driver_Unknown = 201,
	HmdError_Driver_HmdUnknown = 202,
	HmdError_Driver_NotLoaded = 203,
	HmdError_Driver_RuntimeOutOfDate = 204,
	HmdError_Driver_HmdInUse = 205,
	HmdError_IPC_ServerInitFailed = 300,
	HmdError_IPC_ConnectFailed = 301,
	HmdError_IPC_SharedStateInitFailed = 302,
	HmdError_IPC_CompositorInitFailed = 303,
	HmdError_IPC_MutexInitFailed = 304,
	HmdError_VendorSpecific_UnableToConnectToOculusRuntime = 1000,
	HmdError_Steam_SteamInstallationNotFound = 2000,
};
typedef enum CameraImageResult
{
	CameraImageResult_OK = 0,
	CameraImageResult_Uninitalized = 1,
	CameraImageResult_NotReady = 2,
	CameraImageResult_SameFrame = 3,
};
typedef enum ChaperoneCalibrationState
{
	ChaperoneCalibrationState_OK = 1,
	ChaperoneCalibrationState_Warning = 100,
	ChaperoneCalibrationState_Warning_BaseStationMayHaveMoved = 101,
	ChaperoneCalibrationState_Warning_BaseStationRemoved = 102,
	ChaperoneCalibrationState_Warning_SeatedBoundsInvalid = 103,
	ChaperoneCalibrationState_Error = 200,
	ChaperoneCalibrationState_Error_BaseStationUninitalized = 201,
	ChaperoneCalibrationState_Error_BaseStationConflict = 202,
	ChaperoneCalibrationState_Error_SoftBoundsInvalid = 203,
	ChaperoneCalibrationState_Error_HardBoundsInvalid = 204,
};
typedef enum Compositor_DeviceType
{
	Compositor_DeviceType_None = 0,
	Compositor_DeviceType_D3D9 = 1,
	Compositor_DeviceType_D3D9Ex = 2,
	Compositor_DeviceType_D3D10 = 3,
	Compositor_DeviceType_D3D11 = 4,
	Compositor_DeviceType_OpenGL = 5,
};
typedef enum VRCompositorError
{
	VRCompositorError_None = 0,
	VRCompositorError_IncompatibleVersion = 100,
	VRCompositorError_DoNotHaveFocus = 101,
};
typedef enum VROverlayError
{
	VROverlayError_None = 0,
	VROverlayError_UnknownOverlay = 10,
	VROverlayError_InvalidHandle = 11,
	VROverlayError_PermissionDenied = 12,
	VROverlayError_OverlayLimitExceeded = 13,
	VROverlayError_WrongVisibilityType = 14,
	VROverlayError_KeyTooLong = 15,
	VROverlayError_NameTooLong = 16,
	VROverlayError_KeyInUse = 17,
	VROverlayError_WrongTransformType = 18,
	VROverlayError_InvalidTrackedDevice = 19,
};
typedef enum VROverlayInputMethod
{
	VROverlayInputMethod_None = 0,
	VROverlayInputMethod_Mouse = 1,
};
typedef enum VROverlayVisibility
{
	VROverlayVisibility_Manual = 0,
	VROverlayVisibility_SystemOverlay = 1,
};
typedef enum VROverlayTransformType
{
	VROverlayTransformType_VROverlayTransform_Absolute = 0,
	VROverlayTransformType_VROverlayTransform_TrackedDeviceRelative = 1,
	VROverlayTransformType_VROverlayTransform_SystemOverlay = 2,
};
typedef enum VROverlayFlags
{
	VROverlayFlags_None = 0,
	VROverlayFlags_Curved = 1,
	VROverlayFlags_RGSS4X = 2,
};
typedef struct HmdMatrix34_t
{
	float m[3][4]; //float[3][4]
} HmdMatrix34_t;
typedef struct HmdMatrix44_t
{
	float m[4][4]; //float[4][4]
} HmdMatrix44_t;
typedef struct HmdVector3_t
{
	float v[3]; //float[3]
} HmdVector3_t;
typedef struct HmdVector3d_t
{
	double v[3]; //double[3]
} HmdVector3d_t;
typedef struct HmdVector2_t
{
	float v[2]; //float[2]
} HmdVector2_t;
typedef struct HmdQuaternion_t
{
	double w;
	double x;
	double y;
	double z;
} HmdQuaternion_t;
typedef struct HmdQuad_t
{
	HmdVector3_t vCorners[4]; //HmdVector3_t[4]
} HmdQuad_t;
typedef struct DistortionCoordinates_t
{
	float rfRed[2]; //float[2]
	float rfGreen[2]; //float[2]
	float rfBlue[2]; //float[2]
} DistortionCoordinates_t;
typedef struct TrackedDevicePose_t
{
	HmdMatrix34_t mDeviceToAbsoluteTracking;
	HmdVector3_t vVelocity;
	HmdVector3_t vAngularVelocity;
	HmdTrackingResult eTrackingResult;
	bool bPoseIsValid;
	bool bDeviceIsConnected;
} TrackedDevicePose_t;
typedef struct RenderModel_Vertex_t
{
	HmdVector3_t vPosition;
	HmdVector3_t vNormal;
	float rfTextureCoord[2]; //float[2]
} RenderModel_Vertex_t;
typedef struct RenderModel_TextureMap_t
{
	char unWidth;
	char unHeight;
	uint8_t * rubTextureMapData; // const uint8_t *
} RenderModel_TextureMap_t;
typedef struct RenderModel_t
{
	unsigned long ulInternalHandle;
	struct RenderModel_Vertex_t * rVertexData; // const struct vr::RenderModel_Vertex_t *
	unsigned int unVertexCount;
	uint16_t * rIndexData; // const uint16_t *
	unsigned int unTriangleCount;
	RenderModel_TextureMap_t diffuseTexture;
} RenderModel_t;
typedef struct VRTextureBounds_t
{
	float uMin;
	float vMin;
	float uMax;
	float vMax;
} VRTextureBounds_t;
typedef struct HiddenAreaMesh_t
{
	struct HmdVector2_t * pVertexData; // const struct vr::HmdVector2_t *
	unsigned int unTriangleCount;
} HiddenAreaMesh_t;
typedef struct VRControllerAxis_t
{
	float x;
	float y;
} VRControllerAxis_t;
typedef struct VRControllerState_t
{
	unsigned int unPacketNum;
	unsigned long ulButtonPressed;
	unsigned long ulButtonTouched;
	VRControllerAxis_t rAxis[5]; //VRControllerAxis_t[5]
} VRControllerState_t;
typedef struct Compositor_OverlaySettings
{
	unsigned int size;
	bool curved;
	bool antialias;
	float scale;
	float distance;
	float alpha;
	float uOffset;
	float vOffset;
	float uScale;
	float vScale;
	float gridDivs;
	float gridWidth;
	float gridScale;
	HmdMatrix44_t transform;
} Compositor_OverlaySettings;
typedef struct CameraInfo_t
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	float fx;
	float cx;
	float fy;
	float cy;
} CameraInfo_t;
typedef struct CameraImage_t
{
	int frameID;
	HmdMatrix34_t pose;
	unsigned char * pBuffer; // unsigned char *
	unsigned int unBufferLen;
	CameraImageResult result;
} CameraImage_t;
typedef struct ChaperoneSoftBoundsInfo_t
{
	HmdQuad_t quadCorners;
} ChaperoneSoftBoundsInfo_t;
typedef struct ChaperoneSeatedBoundsInfo_t
{
	HmdVector3_t vSeatedHeadPosition;
	HmdVector3_t vDeskEdgePositions[2]; //HmdVector3_t[2]
} ChaperoneSeatedBoundsInfo_t;
typedef struct Compositor_FrameTiming
{
	unsigned int size;
	double frameStart;
	float frameVSync;
	unsigned int droppedFrames;
	unsigned int frameIndex;
	TrackedDevicePose_t pose;
} Compositor_FrameTiming;
typedef struct VROverlayIntersectionParams_t
{
	HmdVector3_t vSource;
	HmdVector3_t vDirection;
	TrackingUniverseOrigin eOrigin;
} VROverlayIntersectionParams_t;
typedef struct VROverlayIntersectionResults_t
{
	HmdVector3_t vPoint;
	HmdVector3_t vNormal;
	HmdVector2_t vUVs;
	float fDistance;
} VROverlayIntersectionResults_t;
typedef struct NotificationBitmap
{
	char * bytes; // char *
	int width;
	int height;
	int depth;
} NotificationBitmap;



S_API void IVRSystem_GetWindowBounds(intptr_t instancePtr, int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void IVRSystem_GetRecommendedRenderTargetSize(intptr_t instancePtr, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void IVRSystem_GetEyeOutputViewport(intptr_t instancePtr, Hmd_Eye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API struct HmdMatrix44_t IVRSystem_GetProjectionMatrix(intptr_t instancePtr, Hmd_Eye eEye, float fNearZ, float fFarZ, GraphicsAPIConvention eProjType);
S_API void IVRSystem_GetProjectionRaw(intptr_t instancePtr, Hmd_Eye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom);
S_API struct DistortionCoordinates_t IVRSystem_ComputeDistortion(intptr_t instancePtr, Hmd_Eye eEye, float fU, float fV);
S_API struct HmdMatrix34_t IVRSystem_GetEyeToHeadTransform(intptr_t instancePtr, Hmd_Eye eEye);
S_API bool IVRSystem_GetTimeSinceLastVsync(intptr_t instancePtr, float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter);
S_API int32_t IVRSystem_GetD3D9AdapterIndex(intptr_t instancePtr);
S_API void IVRSystem_GetDXGIOutputInfo(intptr_t instancePtr, int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex);
S_API bool IVRSystem_AttachToWindow(intptr_t instancePtr, void * hWnd);
S_API void IVRSystem_GetDeviceToAbsoluteTrackingPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount);
S_API void IVRSystem_ResetSeatedZeroPose(intptr_t instancePtr);
S_API struct HmdMatrix34_t IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(intptr_t instancePtr);
S_API TrackedDeviceClass IVRSystem_GetTrackedDeviceClass(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex);
S_API bool IVRSystem_IsTrackedDeviceConnected(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex);
S_API bool IVRSystem_GetBoolTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API float IVRSystem_GetFloatTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API int32_t IVRSystem_GetInt32TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API uint64_t IVRSystem_GetUint64TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API struct HmdMatrix34_t IVRSystem_GetMatrix34TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API uint32_t IVRSystem_GetStringTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, TrackedPropertyError * pError);
S_API char * IVRSystem_GetPropErrorNameFromEnum(intptr_t instancePtr, TrackedPropertyError error);
S_API bool IVRSystem_PollNextEvent(intptr_t instancePtr, struct VREvent_t * pEvent);
S_API bool IVRSystem_PollNextEventWithPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, VREvent_t * pEvent, TrackedDevicePose_t * pTrackedDevicePose);
S_API char * IVRSystem_GetEventTypeNameFromEnum(intptr_t instancePtr, EVREventType eType);
S_API struct HiddenAreaMesh_t IVRSystem_GetHiddenAreaMesh(intptr_t instancePtr, Hmd_Eye eEye);
S_API bool IVRSystem_GetControllerState(intptr_t instancePtr, TrackedDeviceIndex_t unControllerDeviceIndex, VRControllerState_t * pControllerState);
S_API bool IVRSystem_GetControllerStateWithPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, TrackedDeviceIndex_t unControllerDeviceIndex, VRControllerState_t * pControllerState, struct TrackedDevicePose_t * pTrackedDevicePose);
S_API void IVRSystem_TriggerHapticPulse(intptr_t instancePtr, TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec);
S_API char * IVRSystem_GetButtonIdNameFromEnum(intptr_t instancePtr, EVRButtonId eButtonId);
S_API char * IVRSystem_GetControllerAxisTypeNameFromEnum(intptr_t instancePtr, EVRControllerAxisType eAxisType);
S_API bool IVRSystem_CaptureInputFocus(intptr_t instancePtr);
S_API void IVRSystem_ReleaseInputFocus(intptr_t instancePtr);
S_API bool IVRSystem_IsInputFocusCapturedByAnotherProcess(intptr_t instancePtr);
S_API uint32_t IVRSystem_DriverDebugRequest(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize);
S_API uint32_t IVRCameraAccess_GetCameraCount(intptr_t instancePtr);
S_API uint32_t IVRCameraAccess_GetCameraId(intptr_t instancePtr, uint32_t unCameraIndex, char * pchBuffer, uint32_t unBufferLen);
S_API bool IVRCameraAccess_EnableCamera(intptr_t instancePtr, uint32_t unCameraIndex, bool bEnabled);
S_API bool IVRCameraAccess_GetCameraInfo(intptr_t instancePtr, uint32_t unCameraIndex, struct CameraInfo_t * pCameraInfo);
S_API bool IVRCameraAccess_GetCameraImage(intptr_t instancePtr, uint32_t unCameraIndex, struct CameraImage_t * pCameraImage);
S_API ChaperoneCalibrationState IVRChaperone_GetCalibrationState(intptr_t instancePtr);
S_API bool IVRChaperone_GetSoftBoundsInfo(intptr_t instancePtr, struct ChaperoneSoftBoundsInfo_t * pInfo);
S_API bool IVRChaperone_GetHardBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool IVRChaperone_GetSeatedBoundsInfo(intptr_t instancePtr, struct ChaperoneSeatedBoundsInfo_t * pInfo);
S_API bool IVRChaperoneSetup_CommitWorkingCopy(intptr_t instancePtr, const char * pchCalibrationName);
S_API void IVRChaperoneSetup_RevertWorkingCopy(intptr_t instancePtr);
S_API bool IVRChaperoneSetup_GetWorkingSoftBoundsInfo(intptr_t instancePtr, struct ChaperoneSoftBoundsInfo_t * pInfo);
S_API bool IVRChaperoneSetup_GetWorkingHardBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, struct HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose);
S_API bool IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, struct HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose);
S_API void IVRChaperoneSetup_SetWorkingSoftBoundsInfo(intptr_t instancePtr, const struct ChaperoneSoftBoundsInfo_t * pInfo);
S_API void IVRChaperoneSetup_SetWorkingHardBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount);
S_API void IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct HmdMatrix34_t & matSeatedZeroPoseToRawTrackingPose);
S_API void IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct HmdMatrix34_t & matStandingZeroPoseToRawTrackingPose);
S_API bool IVRChaperoneSetup_GetWorkingTagPoses(intptr_t instancePtr, struct HmdMatrix34_t * pTagPosesBuffer, uint32_t * punTagPosesCount);
S_API bool IVRChaperoneSetup_GetWorkingTagPoseScales(intptr_t instancePtr, float * pflScaleBuffer, uint32_t * punTagPosesCount);
S_API uint32_t IVRChaperoneSetup_GetWorkingTagPoseNameByIndex(intptr_t instancePtr, uint32_t nIndex, char * pchBuffer, uint32_t unBufferSize);
S_API bool IVRChaperoneSetup_GetWorkingTagPoseByName(intptr_t instancePtr, const char * pchTagName, struct HmdMatrix34_t * pmatTagPose, float * pflScale);
S_API void IVRChaperoneSetup_SetWorkingTagPoseByName(intptr_t instancePtr, const char * pchTagName, const struct HmdMatrix34_t & matSeatedZeroPoseToRawTrackingPose, float flScale);
S_API void IVRChaperoneSetup_RemoveWorkingTagPoseByName(intptr_t instancePtr, const char * pchTagName);
S_API void IVRChaperoneSetup_RemoveAllWorkingTagPoses(intptr_t instancePtr);
S_API void IVRChaperoneSetup_ReloadFromDisk(intptr_t instancePtr);
S_API uint32_t IVRCompositor_GetLastError(intptr_t instancePtr, char * pchBuffer, uint32_t unBufferSize);
S_API void IVRCompositor_SetVSync(intptr_t instancePtr, bool bVSync);
S_API bool IVRCompositor_GetVSync(intptr_t instancePtr);
S_API void IVRCompositor_SetGamma(intptr_t instancePtr, float fGamma);
S_API float IVRCompositor_GetGamma(intptr_t instancePtr);
S_API void IVRCompositor_SetGraphicsDevice(intptr_t instancePtr, Compositor_DeviceType eType, void * pDevice);
S_API VRCompositorError IVRCompositor_WaitGetPoses(intptr_t instancePtr, struct TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount);
S_API VRCompositorError IVRCompositor_Submit(intptr_t instancePtr, Hmd_Eye eEye, void * pTexture, struct VRTextureBounds_t * pBounds);
S_API void IVRCompositor_ClearLastSubmittedFrame(intptr_t instancePtr);
S_API bool IVRCompositor_GetFrameTiming(intptr_t instancePtr, struct Compositor_FrameTiming * pTiming, uint32_t unFramesAgo);
S_API void IVRCompositor_FadeToColor(intptr_t instancePtr, float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground);
S_API void IVRCompositor_FadeGrid(intptr_t instancePtr, float fSeconds, bool bFadeIn);
S_API void IVRCompositor_CompositorBringToFront(intptr_t instancePtr);
S_API void IVRCompositor_CompositorGoToBack(intptr_t instancePtr);
S_API void IVRCompositor_CompositorQuit(intptr_t instancePtr);
S_API bool IVRCompositor_IsFullscreen(intptr_t instancePtr);
S_API void IVRCompositor_SetTrackingSpace(intptr_t instancePtr, TrackingUniverseOrigin eOrigin);
S_API TrackingUniverseOrigin IVRCompositor_GetTrackingSpace(intptr_t instancePtr);
S_API uint32_t IVRCompositor_GetCurrentSceneFocusProcess(intptr_t instancePtr);
S_API bool IVRCompositor_CanRenderScene(intptr_t instancePtr);
S_API VROverlayError IVROverlay_FindOverlay(intptr_t instancePtr, const char * pchOverlayKey, VROverlayHandle_t * pOverlayHandle);
S_API VROverlayError IVROverlay_CreateOverlay(intptr_t instancePtr, const char * pchOverlayKey, const char * pchOverlayFriendlyName, VROverlayHandle_t * pOverlayHandle);
S_API VROverlayError IVROverlay_DestroyOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError IVROverlay_SetHighQualityOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayHandle_t IVROverlay_GetHighQualityOverlay(intptr_t instancePtr);
S_API char * IVROverlay_GetOverlayErrorNameFromEnum(intptr_t instancePtr, VROverlayError error);
S_API VROverlayError IVROverlay_SetOverlayFlag(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled);
S_API VROverlayError IVROverlay_GetOverlayFlag(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool * pbEnabled);
S_API VROverlayError IVROverlay_SetOverlayAlpha(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fAlpha);
S_API VROverlayError IVROverlay_GetOverlayAlpha(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfAlpha);
S_API VROverlayError IVROverlay_SetOverlayGamma(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fGamma);
S_API VROverlayError IVROverlay_GetOverlayGamma(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfGamma);
S_API VROverlayError IVROverlay_SetOverlayWidthInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fWidthInMeters);
S_API VROverlayError IVROverlay_GetOverlayWidthInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters);
S_API VROverlayError IVROverlay_SetOverlayTextureBounds(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct VRTextureBounds_t * pOverlayTextureBounds);
S_API VROverlayError IVROverlay_GetOverlayTextureBounds(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct VRTextureBounds_t * pOverlayTextureBounds);
S_API VROverlayError IVROverlay_GetOverlayTransformType(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayTransformType * peTransformType);
S_API VROverlayError IVROverlay_SetOverlayTransformAbsolute(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackingUniverseOrigin eTrackingOrigin, const struct HmdMatrix34_t * pmatTrackingOriginToOverlayTransform);
S_API VROverlayError IVROverlay_GetOverlayTransformAbsolute(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackingUniverseOrigin * peTrackingOrigin, struct HmdMatrix34_t * pmatTrackingOriginToOverlayTransform);
S_API VROverlayError IVROverlay_SetOverlayTransformTrackedDeviceRelative(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const struct HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform);
S_API VROverlayError IVROverlay_GetOverlayTransformTrackedDeviceRelative(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t * punTrackedDevice, struct HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform);
S_API VROverlayError IVROverlay_GetOverlayVisibility(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayVisibility * peOverlayVisibility);
S_API VROverlayError IVROverlay_SetOverlayVisibility(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayVisibility eOverlayVisibility);
S_API VROverlayError IVROverlay_ShowOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError IVROverlay_HideOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API bool IVROverlay_IsOverlayVisible(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API bool IVROverlay_PollNextOverlayEvent(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct VREvent_t * pEvent);
S_API VROverlayError IVROverlay_GetOverlayInputMethod(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod * peInputMethod);
S_API VROverlayError IVROverlay_SetOverlayInputMethod(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod);
S_API VROverlayError IVROverlay_GetOverlayMouseScale(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct HmdVector2_t * pvecMouseScale);
S_API VROverlayError IVROverlay_SetOverlayMouseScale(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct HmdVector2_t * pvecMouseScale);
S_API bool IVROverlay_ComputeOverlayIntersection(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct VROverlayIntersectionParams_t * pParams, struct VROverlayIntersectionResults_t * pResults);
S_API bool IVROverlay_HandleControllerOverlayInteractionAsMouse(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unControllerDeviceIndex);
S_API VROverlayError IVROverlay_SetOverlayTexture(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, void * pTexture);
S_API VROverlayError IVROverlay_SetOverlayRaw(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth);
S_API VROverlayError IVROverlay_SetOverlayFromFile(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const char * pchFilePath);
S_API bool IVROverlay_IsSystemOverlayVisible(intptr_t instancePtr);
S_API bool IVROverlay_IsActiveSystemOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError IVROverlay_SetSystemOverlaySceneProcess(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId);
S_API VROverlayError IVROverlay_GetSystemOverlaySceneProcess(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId);
S_API bool IVRRenderModels_LoadRenderModel(intptr_t instancePtr, const char * pchRenderModelName, struct RenderModel_t * pRenderModel);
S_API void IVRRenderModels_FreeRenderModel(intptr_t instancePtr, struct RenderModel_t * pRenderModel);
S_API uint32_t IVRRenderModels_GetRenderModelName(intptr_t instancePtr, uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen);
S_API uint32_t IVRRenderModels_GetRenderModelCount(intptr_t instancePtr);
S_API uint32_t IVRNotifications_GetLastError(intptr_t instancePtr, char * pchBuffer, uint32_t unBufferSize);
S_API bool IVRNotifications_NotificationStart(intptr_t instancePtr, char * type, struct NotificationBitmap texture, VRNotificationId * notificationId);
S_API bool IVRNotifications_UpdateTexture(intptr_t instancePtr, VRNotificationId notificationId, struct NotificationBitmap texture);
S_API bool IVRNotifications_UpdateBitmap(intptr_t instancePtr, VRNotificationId notificationId, struct NotificationBitmap texture);
S_API bool IVRNotifications_GetPointerLocation(intptr_t instancePtr, VRNotificationId notificationId, bool * pointerActive, int32_t * pointerX, int32_t * pointerY);
S_API bool IVRNotifications_DismissNotification(intptr_t instancePtr, VRNotificationId notificationId);
#endif // __OPENVR_API_FLAT_H__


