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

typedef uint64_t VROverlayHandle_t;

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
unsigned int k_unMaxDriverDebugResponseSize = 32768;
unsigned int k_unTrackedDeviceIndex_Hmd = 0;
unsigned int k_unMaxTrackedDeviceCount = 16;
unsigned int k_unTrackedDeviceIndexInvalid = 4294967295;
unsigned int k_unMaxPropertyStringSize = 32768;
unsigned int k_unControllerStateAxisCount = 5;
unsigned long k_ulOverlayHandleInvalid = 0;
char * IVRSystem_Version = "IVRSystem_007";
unsigned int k_unMaxApplicationKeyLength = 128;
char * IVRApplications_Version = "IVRApplications_001";
char * IVRChaperone_Version = "IVRChaperone_003";
char * IVRCompositor_Version = "IVRCompositor_008";
unsigned int k_unVROverlayMaxKeyLength = 128;
unsigned int k_unVROverlayMaxNameLength = 128;
unsigned int k_unMaxOverlayCount = 32;
char * IVROverlay_Version = "IVROverlay_006";
char * k_pch_Controller_Component_GDC2015 = "gdc2015";
char * k_pch_Controller_Component_Base = "base";
char * k_pch_Controller_Component_Tip = "tip";
char * k_pch_Controller_Component_HandGrip = "handgrip";
char * IVRRenderModels_Version = "IVRRenderModels_001";
char * IVRControlPanel_Version = "IVRControlPanel_001";
unsigned int k_unNotificationTypeMaxSize = 16;
unsigned int k_unNotificationTextMaxSize = 128;
unsigned int k_unNotificationCatagoryMaxSize = 32;
char * IVRNotifications_Version = "IVRNotifications_001";
unsigned int k_unMaxSettingsKeyLength = 128;
char * k_pch_SteamVR_Section = "steamvr";
char * k_pch_SteamVR_RequireHmd_String = "requireHmd";
char * k_pch_SteamVR_ForcedDriverKey_String = "forcedDriver";
char * k_pch_SteamVR_ForcedHmdKey_String = "forcedHmd";
char * k_pch_SteamVR_DisplayDebug_Bool = "displayDebug";
char * k_pch_SteamVR_EnableDistortion_Bool = "enableDistortion";
char * k_pch_SteamVR_DisplayDebugX_Int32 = "displayDebugX";
char * k_pch_SteamVR_DisplayDebugY_Int32 = "displayDebugY";
char * k_pch_SteamVR_SendSystemButtonToAllApps_Bool = "sendSystemButtonToAllApps";
char * k_pch_SteamVR_LogLevel_Int32 = "loglevel";
char * k_pch_SteamVR_IPD_Float = "ipd";
char * k_pch_SteamVR_Background_String = "background";
char * k_pch_Lighthouse_Section = "driver_lighthouse";
char * k_pch_Lighthouse_DisableIMU_Bool = "disableimu";
char * k_pch_Lighthouse_UseDisambiguation_String = "usedisambiguation";
char * k_pch_Lighthouse_DisambiguationDebug_Int32 = "disambiguationdebug";
char * k_pch_Lighthouse_PrimaryBasestation_Int32 = "primarybasestation";
char * k_pch_Lighthouse_LighthouseName_String = "lighthousename";
char * k_pch_Lighthouse_MaxIncidenceAngleDegrees_Float = "maxincidenceangledegrees";
char * k_pch_Lighthouse_UseLighthouseDirect_Bool = "uselighthousedirect";
char * k_pch_Lighthouse_DBHistory_Bool = "dbhistory";
char * k_pch_Lighthouse_OriginOffsetX_Float = "originoffsetx";
char * k_pch_Lighthouse_OriginOffsetY_Float = "originoffsety";
char * k_pch_Lighthouse_OriginOffsetZ_Float = "originoffsetz";
char * k_pch_Lighthouse_HeadingOffset_Float = "headingoffset";
char * k_pch_Null_Section = "driver_null";
char * k_pch_Null_EnableNullDriver_Bool = "enable";
char * k_pch_Null_Id_String = "id";
char * k_pch_Null_SerialNumber_String = "serialNumber";
char * k_pch_Null_ModelNumber_String = "modelNumber";
char * k_pch_Null_WindowX_Int32 = "windowX";
char * k_pch_Null_WindowY_Int32 = "windowY";
char * k_pch_Null_WindowWidth_Int32 = "windowWidth";
char * k_pch_Null_WindowHeight_Int32 = "windowHeight";
char * k_pch_Null_RenderWidth_Int32 = "renderWidth";
char * k_pch_Null_RenderHeight_Int32 = "renderHeight";
char * k_pch_Null_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
char * k_pch_Null_DisplayFrequency_Float = "displayFrequency";
char * k_pch_Notifications_Section = "notifications";
char * k_pch_Notifications_DoNotDisturb_Bool = "DoNotDisturb";
char * k_pch_Perf_Section = "perfcheck";
char * k_pch_Perf_HeuristicActive_Bool = "heuristicActive";
char * k_pch_Perf_NotifyInHMD_Bool = "notifyInHMD";
char * k_pch_Perf_NotifyOnlyOnce_Bool = "notifyOnlyOnce";
char * k_pch_Perf_AllowTimingStore_Bool = "allowTimingStore";
char * k_pch_Perf_SaveTimingsOnExit_Bool = "saveTimingsOnExit";
char * IVRSettings_Version = "IVRSettings_001";
char * IVRTrackedCamera_Version = "IVRTrackedCamera_001";
char * IVRChaperoneSetup_Version = "IVRChaperoneSetup_003";
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
	TrackedDeviceProperty_Prop_DeviceIsWireless_Bool = 1010,
	TrackedDeviceProperty_Prop_DeviceIsCharging_Bool = 1011,
	TrackedDeviceProperty_Prop_DeviceBatteryPercentage_Float = 1012,
	TrackedDeviceProperty_Prop_StatusDisplayTransform_Matrix34 = 1013,
	TrackedDeviceProperty_Prop_Firmware_UpdateAvailable_Bool = 1014,
	TrackedDeviceProperty_Prop_Firmware_ManualUpdate_Bool = 1015,
	TrackedDeviceProperty_Prop_Firmware_ManualUpdateURL_String = 1016,
	TrackedDeviceProperty_Prop_HardwareRevision_Uint64 = 1017,
	TrackedDeviceProperty_Prop_FirmwareVersion_Uint64 = 1018,
	TrackedDeviceProperty_Prop_FPGAVersion_Uint64 = 1019,
	TrackedDeviceProperty_Prop_VRCVersion_Uint64 = 1020,
	TrackedDeviceProperty_Prop_RadioVersion_Uint64 = 1021,
	TrackedDeviceProperty_Prop_DongleVersion_Uint64 = 1022,
	TrackedDeviceProperty_Prop_BlockServerShutdown_Bool = 1023,
	TrackedDeviceProperty_Prop_CanUnifyCoordinateSystemWithHmd_Bool = 1024,
	TrackedDeviceProperty_Prop_ReportsTimeSinceVSync_Bool = 2000,
	TrackedDeviceProperty_Prop_SecondsFromVsyncToPhotons_Float = 2001,
	TrackedDeviceProperty_Prop_DisplayFrequency_Float = 2002,
	TrackedDeviceProperty_Prop_UserIpdMeters_Float = 2003,
	TrackedDeviceProperty_Prop_CurrentUniverseId_Uint64 = 2004,
	TrackedDeviceProperty_Prop_PreviousUniverseId_Uint64 = 2005,
	TrackedDeviceProperty_Prop_DisplayFirmwareVersion_String = 2006,
	TrackedDeviceProperty_Prop_IsOnDesktop_Bool = 2007,
	TrackedDeviceProperty_Prop_DisplayMCType_Int32 = 2008,
	TrackedDeviceProperty_Prop_DisplayMCOffset_Float = 2009,
	TrackedDeviceProperty_Prop_DisplayMCScale_Float = 2010,
	TrackedDeviceProperty_Prop_VendorID_Int32 = 2011,
	TrackedDeviceProperty_Prop_DisplayMCImageLeft_String = 2012,
	TrackedDeviceProperty_Prop_DisplayMCImageRight_String = 2013,
	TrackedDeviceProperty_Prop_DisplayGCBlackClamp_Float = 2014,
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
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsFX_Float = 5000,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsFY_Float = 5001,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsCX_Float = 5002,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsCY_Float = 5003,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsK1_Float = 5004,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsK2_Float = 5005,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsP1_Float = 5006,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsP2_Float = 5007,
	TrackedDeviceProperty_Prop_TrackedCamera_IntrinsicsK3_Float = 5008,
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
typedef enum VRSubmitFlags_t
{
	VRSubmitFlags_t_Submit_Default = 0,
	VRSubmitFlags_t_Submit_LensDistortionAlreadyApplied = 1,
	VRSubmitFlags_t_Submit_GlRenderBuffer = 2,
};
typedef enum VRState_t
{
	VRState_t_VRState_Undefined = -1,
	VRState_t_VRState_Off = 0,
	VRState_t_VRState_Searching = 1,
	VRState_t_VRState_Searching_Alert = 2,
	VRState_t_VRState_Ready = 3,
	VRState_t_VRState_Ready_Alert = 4,
	VRState_t_VRState_NotReady = 5,
};
typedef enum EVREventType
{
	EVREventType_VREvent_None = 0,
	EVREventType_VREvent_TrackedDeviceActivated = 100,
	EVREventType_VREvent_TrackedDeviceDeactivated = 101,
	EVREventType_VREvent_TrackedDeviceUpdated = 102,
	EVREventType_VREvent_TrackedDeviceUserInteractionStarted = 103,
	EVREventType_VREvent_TrackedDeviceUserInteractionEnded = 104,
	EVREventType_VREvent_ButtonPress = 200,
	EVREventType_VREvent_ButtonUnpress = 201,
	EVREventType_VREvent_ButtonTouch = 202,
	EVREventType_VREvent_ButtonUntouch = 203,
	EVREventType_VREvent_MouseMove = 300,
	EVREventType_VREvent_MouseButtonDown = 301,
	EVREventType_VREvent_MouseButtonUp = 302,
	EVREventType_VREvent_FocusEnter = 303,
	EVREventType_VREvent_FocusLeave = 304,
	EVREventType_VREvent_InputFocusCaptured = 400,
	EVREventType_VREvent_InputFocusReleased = 401,
	EVREventType_VREvent_SceneFocusLost = 402,
	EVREventType_VREvent_SceneFocusGained = 403,
	EVREventType_VREvent_SceneApplicationChanged = 404,
	EVREventType_VREvent_SceneFocusChanged = 405,
	EVREventType_VREvent_OverlayShown = 500,
	EVREventType_VREvent_OverlayHidden = 501,
	EVREventType_VREvent_DashboardActivated = 502,
	EVREventType_VREvent_DashboardDeactivated = 503,
	EVREventType_VREvent_DashboardThumbSelected = 504,
	EVREventType_VREvent_DashboardRequested = 505,
	EVREventType_VREvent_ResetDashboard = 506,
	EVREventType_VREvent_RenderToast = 507,
	EVREventType_VREvent_ImageLoaded = 508,
	EVREventType_VREvent_ShowKeyboard = 509,
	EVREventType_VREvent_HideKeyboard = 510,
	EVREventType_VREvent_OverlayGamepadFocusGained = 511,
	EVREventType_VREvent_OverlayGamepadFocusLost = 512,
	EVREventType_VREvent_Notification_Show = 600,
	EVREventType_VREvent_Notification_Dismissed = 601,
	EVREventType_VREvent_Notification_BeginInteraction = 602,
	EVREventType_VREvent_Quit = 700,
	EVREventType_VREvent_ProcessQuit = 701,
	EVREventType_VREvent_ChaperoneDataHasChanged = 800,
	EVREventType_VREvent_ChaperoneUniverseHasChanged = 801,
	EVREventType_VREvent_ChaperoneTempDataHasChanged = 802,
	EVREventType_VREvent_StatusUpdate = 900,
	EVREventType_VREvent_MCImageUpdated = 1000,
	EVREventType_VREvent_FirmwareUpdateStarted = 1100,
	EVREventType_VREvent_FirmwareUpdateFinished = 1101,
	EVREventType_VREvent_KeyboardClosed = 1200,
	EVREventType_VREvent_KeyboardCharInput = 1201,
	EVREventType_VREvent_ApplicationTransitionStarted = 1300,
	EVREventType_VREvent_ApplicationTransitionAborted = 1301,
	EVREventType_VREvent_ApplicationTransitionNewAppStarted = 1302,
};
typedef enum EDeviceActivityLevel
{
	EDeviceActivityLevel_k_EDeviceActivityLevel_Unknown = -1,
	EDeviceActivityLevel_k_EDeviceActivityLevel_Idle = 0,
	EDeviceActivityLevel_k_EDeviceActivityLevel_UserInteraction = 1,
	EDeviceActivityLevel_k_EDeviceActivityLevel_UserInteraction_Timeout = 2,
};
typedef enum EVRButtonId
{
	EVRButtonId_k_EButton_System = 0,
	EVRButtonId_k_EButton_ApplicationMenu = 1,
	EVRButtonId_k_EButton_Grip = 2,
	EVRButtonId_k_EButton_DPad_Left = 3,
	EVRButtonId_k_EButton_DPad_Up = 4,
	EVRButtonId_k_EButton_DPad_Right = 5,
	EVRButtonId_k_EButton_DPad_Down = 6,
	EVRButtonId_k_EButton_A = 7,
	EVRButtonId_k_EButton_Axis0 = 32,
	EVRButtonId_k_EButton_Axis1 = 33,
	EVRButtonId_k_EButton_Axis2 = 34,
	EVRButtonId_k_EButton_Axis3 = 35,
	EVRButtonId_k_EButton_Axis4 = 36,
	EVRButtonId_k_EButton_SteamVR_Touchpad = 32,
	EVRButtonId_k_EButton_SteamVR_Trigger = 33,
	EVRButtonId_k_EButton_Dashboard_Back = 2,
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
	VROverlayError_InvalidParameter = 20,
	VROverlayError_ThumbnailCantBeDestroyed = 21,
	VROverlayError_ArrayTooSmall = 22,
	VROverlayError_RequestFailed = 23,
	VROverlayError_InvalidTexture = 24,
	VROverlayError_UnableToLoadFile = 25,
	VROverlayError_VROVerlayError_KeyboardAlreadyInUse = 26,
	VROverlayError_NoNeighbor = 27,
};
typedef enum EVRApplicationType
{
	EVRApplicationType_VRApplication_Other = 0,
	EVRApplicationType_VRApplication_Scene = 1,
	EVRApplicationType_VRApplication_Overlay = 2,
};
typedef enum VRFirmwareError
{
	VRFirmwareError_None = 0,
	VRFirmwareError_Success = 1,
	VRFirmwareError_Fail = 2,
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
	HmdError_Init_AppInfoInitFailed = 114,
	HmdError_Init_Retry = 115,
	HmdError_Init_InitCanceledByUser = 116,
	HmdError_Init_AnotherAppLaunching = 117,
	HmdError_Init_SettingsInitFailed = 118,
	HmdError_Init_ShuttingDown = 119,
	HmdError_Init_TooManyObjects = 120,
	HmdError_Driver_Failed = 200,
	HmdError_Driver_Unknown = 201,
	HmdError_Driver_HmdUnknown = 202,
	HmdError_Driver_NotLoaded = 203,
	HmdError_Driver_RuntimeOutOfDate = 204,
	HmdError_Driver_HmdInUse = 205,
	HmdError_Driver_NotCalibrated = 206,
	HmdError_Driver_CalibrationInvalid = 207,
	HmdError_Driver_HmdDisplayNotFound = 208,
	HmdError_IPC_ServerInitFailed = 300,
	HmdError_IPC_ConnectFailed = 301,
	HmdError_IPC_SharedStateInitFailed = 302,
	HmdError_IPC_CompositorInitFailed = 303,
	HmdError_IPC_MutexInitFailed = 304,
	HmdError_IPC_Failed = 305,
	HmdError_VendorSpecific_UnableToConnectToOculusRuntime = 1000,
	HmdError_VendorSpecific_HmdFound_But = 1100,
	HmdError_VendorSpecific_HmdFound_CantOpenDevice = 1101,
	HmdError_VendorSpecific_HmdFound_UnableToRequestConfigStart = 1102,
	HmdError_VendorSpecific_HmdFound_NoStoredConfig = 1103,
	HmdError_VendorSpecific_HmdFound_ConfigTooBig = 1104,
	HmdError_VendorSpecific_HmdFound_ConfigTooSmall = 1105,
	HmdError_VendorSpecific_HmdFound_UnableToInitZLib = 1106,
	HmdError_VendorSpecific_HmdFound_CantReadFirmwareVersion = 1107,
	HmdError_VendorSpecific_HmdFound_UnableToSendUserDataStart = 1108,
	HmdError_VendorSpecific_HmdFound_UnableToGetUserDataStart = 1109,
	HmdError_VendorSpecific_HmdFound_UnableToGetUserDataNext = 1110,
	HmdError_VendorSpecific_HmdFound_UserDataAddressRange = 1111,
	HmdError_VendorSpecific_HmdFound_UserDataError = 1112,
	HmdError_Steam_SteamInstallationNotFound = 2000,
};
typedef enum EVRApplicationError
{
	EVRApplicationError_VRApplicationError_None = 0,
	EVRApplicationError_VRApplicationError_AppKeyAlreadyExists = 100,
	EVRApplicationError_VRApplicationError_NoManifest = 101,
	EVRApplicationError_VRApplicationError_NoApplication = 102,
	EVRApplicationError_VRApplicationError_InvalidIndex = 103,
	EVRApplicationError_VRApplicationError_UnknownApplication = 104,
	EVRApplicationError_VRApplicationError_IPCFailed = 105,
	EVRApplicationError_VRApplicationError_ApplicationAlreadyRunning = 106,
	EVRApplicationError_VRApplicationError_InvalidManifest = 107,
	EVRApplicationError_VRApplicationError_InvalidApplication = 108,
	EVRApplicationError_VRApplicationError_LaunchFailed = 109,
	EVRApplicationError_VRApplicationError_ApplicationAlreadyStarting = 110,
	EVRApplicationError_VRApplicationError_LaunchInProgress = 111,
	EVRApplicationError_VRApplicationError_OldApplicationQuitting = 112,
	EVRApplicationError_VRApplicationError_TransitionAborted = 113,
	EVRApplicationError_VRApplicationError_BufferTooSmall = 200,
	EVRApplicationError_VRApplicationError_PropertyNotSet = 201,
	EVRApplicationError_VRApplicationError_UnknownProperty = 202,
};
typedef enum EVRApplicationProperty
{
	EVRApplicationProperty_VRApplicationProperty_Name_String = 0,
	EVRApplicationProperty_VRApplicationProperty_LaunchType_String = 11,
	EVRApplicationProperty_VRApplicationProperty_WorkingDirectory_String = 12,
	EVRApplicationProperty_VRApplicationProperty_BinaryPath_String = 13,
	EVRApplicationProperty_VRApplicationProperty_Arguments_String = 14,
	EVRApplicationProperty_VRApplicationProperty_URL_String = 15,
	EVRApplicationProperty_VRApplicationProperty_Description_String = 50,
	EVRApplicationProperty_VRApplicationProperty_NewsURL_String = 51,
	EVRApplicationProperty_VRApplicationProperty_ImagePath_String = 52,
	EVRApplicationProperty_VRApplicationProperty_Source_String = 53,
	EVRApplicationProperty_VRApplicationProperty_IsDashboardOverlay_Bool = 60,
};
typedef enum EVRApplicationTransitionState
{
	EVRApplicationTransitionState_VRApplicationTransition_None = 0,
	EVRApplicationTransitionState_VRApplicationTransition_OldAppQuitSent = 10,
	EVRApplicationTransitionState_VRApplicationTransition_WaitingForExternalLaunch = 11,
	EVRApplicationTransitionState_VRApplicationTransition_NewAppLaunched = 20,
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
	ChaperoneCalibrationState_Error_PlayAreaInvalid = 203,
	ChaperoneCalibrationState_Error_CollisionBoundsInvalid = 204,
};
typedef enum VRCompositorError
{
	VRCompositorError_None = 0,
	VRCompositorError_IncompatibleVersion = 100,
	VRCompositorError_DoNotHaveFocus = 101,
	VRCompositorError_InvalidTexture = 102,
	VRCompositorError_IsNotSceneApplication = 103,
	VRCompositorError_TextureIsOnWrongDevice = 104,
};
typedef enum VROverlayInputMethod
{
	VROverlayInputMethod_None = 0,
	VROverlayInputMethod_Mouse = 1,
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
	VROverlayFlags_NoDashboardTab = 3,
	VROverlayFlags_AcceptsGamepadEvents = 4,
};
typedef enum EGamepadTextInputMode
{
	EGamepadTextInputMode_k_EGamepadTextInputModeNormal = 0,
	EGamepadTextInputMode_k_EGamepadTextInputModePassword = 1,
};
typedef enum EGamepadTextInputLineMode
{
	EGamepadTextInputLineMode_k_EGamepadTextInputLineModeSingleLine = 0,
	EGamepadTextInputLineMode_k_EGamepadTextInputLineModeMultipleLines = 1,
};
typedef enum EOverlayDirection
{
	EOverlayDirection_OverlayDirection_Up = 0,
	EOverlayDirection_OverlayDirection_Down = 1,
	EOverlayDirection_OverlayDirection_Left = 2,
	EOverlayDirection_OverlayDirection_Right = 3,
	EOverlayDirection_OverlayDirection_Count = 4,
};
typedef enum NotificationError_t
{
	NotificationError_t_k_ENotificationError_OK = 0,
	NotificationError_t_k_ENotificationError_Fail = 1,
	NotificationError_t_k_eNotificationError_InvalidParam = 2,
};
typedef enum EVRSettingsError
{
	EVRSettingsError_VRSettingsError_None = 0,
	EVRSettingsError_VRSettingsError_IPCFailed = 1,
	EVRSettingsError_VRSettingsError_WriteFailed = 2,
	EVRSettingsError_VRSettingsError_ReadFailed = 3,
};
typedef enum ECameraVideoStreamFormat
{
	ECameraVideoStreamFormat_CVS_FORMAT_UNKNOWN = 0,
	ECameraVideoStreamFormat_CVS_FORMAT_RAW10 = 1,
	ECameraVideoStreamFormat_CVS_FORMAT_NV12 = 2,
	ECameraVideoStreamFormat_CVS_FORMAT_RGB24 = 3,
	ECameraVideoStreamFormat_CVS_MAX_FORMATS = 4,
};
typedef enum EChaperoneConfigFile
{
	EChaperoneConfigFile_Live = 1,
	EChaperoneConfigFile_Temp = 2,
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
typedef struct HmdVector4_t
{
	float v[4]; //float[4]
} HmdVector4_t;
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
typedef struct HmdColor_t
{
	float r;
	float g;
	float b;
	float a;
} HmdColor_t;
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
	float prediction;
	float m_flFrameIntervalMs;
	float m_flSceneRenderCpuMs;
	float m_flSceneRenderGpuMs;
	float m_flCompositorRenderCpuMs;
	float m_flCompositorRenderGpuMs;
	float m_flPresentCallCpuMs;
	float m_flRunningStartMs;
	float m_flHandoffStartMs;
	float m_flHandoffEndMs;
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
typedef struct ComponentState_t
{
	HmdMatrix34_t mTrackingToComponentRenderModel;
	HmdMatrix34_t mTrackingToComponentLocal;
	bool bIsStatic;
	bool bIsVisible;
} ComponentState_t;
typedef struct NotificationBitmap
{
	void * bytes; // void *
	int width;
	int height;
	int depth;
} NotificationBitmap;
typedef struct NotificationItem
{
	unsigned int notificationId;
} NotificationItem;
typedef struct CameraVideoStreamFrame_t
{
	unsigned int m_nStreamFormat;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nFrameSequence;
	unsigned int m_nTimeStamp;
	unsigned int m_nBufferIndex;
	unsigned int m_nBufferCount;
	unsigned int m_nImageDataSize;
	double m_flFrameTime;
	bool m_bPoseValid;
	float m_HMDPoseMatrix[16]; //float[16]
	void * m_pImageData; // void *
} CameraVideoStreamFrame_t;
typedef struct TrackedCameraCalibrationDevOnly_t
{
	double m_flIntrinsicsFX;
	double m_flIntrinsicsFY;
	double m_flIntrinsicsCX;
	double m_flIntrinsicsCY;
	double m_flIntrinsicsK1;
	double m_flIntrinsicsK2;
	double m_flIntrinsicsP1;
	double m_flIntrinsicsP2;
	double m_flIntrinsicsK3;
} TrackedCameraCalibrationDevOnly_t;



S_API void VR_IVRSystem_GetWindowBounds(intptr_t instancePtr, int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void VR_IVRSystem_GetRecommendedRenderTargetSize(intptr_t instancePtr, uint32_t * pnWidth, uint32_t * pnHeight);
S_API void VR_IVRSystem_GetEyeOutputViewport(intptr_t instancePtr, Hmd_Eye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight);
S_API struct HmdMatrix44_t VR_IVRSystem_GetProjectionMatrix(intptr_t instancePtr, Hmd_Eye eEye, float fNearZ, float fFarZ, GraphicsAPIConvention eProjType);
S_API void VR_IVRSystem_GetProjectionRaw(intptr_t instancePtr, Hmd_Eye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom);
S_API struct DistortionCoordinates_t VR_IVRSystem_ComputeDistortion(intptr_t instancePtr, Hmd_Eye eEye, float fU, float fV);
S_API struct HmdMatrix34_t VR_IVRSystem_GetEyeToHeadTransform(intptr_t instancePtr, Hmd_Eye eEye);
S_API bool VR_IVRSystem_GetTimeSinceLastVsync(intptr_t instancePtr, float * pfSecondsSinceLastVsync, uint64_t * pulFrameCounter);
S_API int32_t VR_IVRSystem_GetD3D9AdapterIndex(intptr_t instancePtr);
S_API void VR_IVRSystem_GetDXGIOutputInfo(intptr_t instancePtr, int32_t * pnAdapterIndex, int32_t * pnAdapterOutputIndex);
S_API bool VR_IVRSystem_AttachToWindow(intptr_t instancePtr, void * hWnd);
S_API void VR_IVRSystem_GetDeviceToAbsoluteTrackingPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, struct TrackedDevicePose_t * pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount);
S_API void VR_IVRSystem_ResetSeatedZeroPose(intptr_t instancePtr);
S_API struct HmdMatrix34_t VR_IVRSystem_GetSeatedZeroPoseToStandingAbsoluteTrackingPose(intptr_t instancePtr);
S_API struct HmdMatrix34_t VR_IVRSystem_GetRawZeroPoseToStandingAbsoluteTrackingPose(intptr_t instancePtr);
S_API uint32_t VR_IVRSystem_GetSortedTrackedDeviceIndicesOfClass(intptr_t instancePtr, TrackedDeviceClass eTrackedDeviceClass, TrackedDeviceIndex_t * punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex);
S_API EDeviceActivityLevel VR_IVRSystem_GetTrackedDeviceActivityLevel(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceId);
S_API void VR_IVRSystem_ApplyTransform(intptr_t instancePtr, struct TrackedDevicePose_t * pOutputPose, const struct TrackedDevicePose_t & trackedDevicePose, const struct HmdMatrix34_t & transform);
S_API TrackedDeviceClass VR_IVRSystem_GetTrackedDeviceClass(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex);
S_API bool VR_IVRSystem_IsTrackedDeviceConnected(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex);
S_API bool VR_IVRSystem_GetBoolTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API float VR_IVRSystem_GetFloatTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API int32_t VR_IVRSystem_GetInt32TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API uint64_t VR_IVRSystem_GetUint64TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API struct HmdMatrix34_t VR_IVRSystem_GetMatrix34TrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError * pError);
S_API uint32_t VR_IVRSystem_GetStringTrackedDeviceProperty(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, TrackedPropertyError * pError);
S_API char * VR_IVRSystem_GetPropErrorNameFromEnum(intptr_t instancePtr, TrackedPropertyError error);
S_API bool VR_IVRSystem_PollNextEvent(intptr_t instancePtr, struct VREvent_t * pEvent);
S_API bool VR_IVRSystem_PollNextEventWithPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, VREvent_t * pEvent, TrackedDevicePose_t * pTrackedDevicePose);
S_API char * VR_IVRSystem_GetEventTypeNameFromEnum(intptr_t instancePtr, EVREventType eType);
S_API struct HiddenAreaMesh_t VR_IVRSystem_GetHiddenAreaMesh(intptr_t instancePtr, Hmd_Eye eEye);
S_API bool VR_IVRSystem_GetControllerState(intptr_t instancePtr, TrackedDeviceIndex_t unControllerDeviceIndex, VRControllerState_t * pControllerState);
S_API bool VR_IVRSystem_GetControllerStateWithPose(intptr_t instancePtr, TrackingUniverseOrigin eOrigin, TrackedDeviceIndex_t unControllerDeviceIndex, VRControllerState_t * pControllerState, struct TrackedDevicePose_t * pTrackedDevicePose);
S_API void VR_IVRSystem_TriggerHapticPulse(intptr_t instancePtr, TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec);
S_API char * VR_IVRSystem_GetButtonIdNameFromEnum(intptr_t instancePtr, EVRButtonId eButtonId);
S_API char * VR_IVRSystem_GetControllerAxisTypeNameFromEnum(intptr_t instancePtr, EVRControllerAxisType eAxisType);
S_API bool VR_IVRSystem_CaptureInputFocus(intptr_t instancePtr);
S_API void VR_IVRSystem_ReleaseInputFocus(intptr_t instancePtr);
S_API bool VR_IVRSystem_IsInputFocusCapturedByAnotherProcess(intptr_t instancePtr);
S_API uint32_t VR_IVRSystem_DriverDebugRequest(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex, const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize);
S_API VRFirmwareError VR_IVRSystem_PerformFirmwareUpdate(intptr_t instancePtr, TrackedDeviceIndex_t unDeviceIndex);
S_API bool VR_IVRSystem_IsDisplayOnDesktop(intptr_t instancePtr);
S_API bool VR_IVRSystem_SetDisplayVisibility(intptr_t instancePtr, bool bIsVisibleOnDesktop);
S_API EVRApplicationError VR_IVRApplications_AddApplicationManifest(intptr_t instancePtr, const char * pchApplicationManifestFullPath, bool bTemporary);
S_API EVRApplicationError VR_IVRApplications_RemoveApplicationManifest(intptr_t instancePtr, const char * pchApplicationManifestFullPath);
S_API bool VR_IVRApplications_IsApplicationInstalled(intptr_t instancePtr, const char * pchAppKey);
S_API uint32_t VR_IVRApplications_GetApplicationCount(intptr_t instancePtr);
S_API EVRApplicationError VR_IVRApplications_GetApplicationKeyByIndex(intptr_t instancePtr, uint32_t unApplicationIndex, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen);
S_API EVRApplicationError VR_IVRApplications_GetApplicationKeyByProcessId(intptr_t instancePtr, uint32_t unProcessId, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen);
S_API EVRApplicationError VR_IVRApplications_LaunchApplication(intptr_t instancePtr, const char * pchAppKey);
S_API EVRApplicationError VR_IVRApplications_LaunchDashboardOverlay(intptr_t instancePtr, const char * pchAppKey);
S_API EVRApplicationError VR_IVRApplications_IdentifyApplication(intptr_t instancePtr, uint32_t unProcessId, const char * pchAppKey);
S_API uint32_t VR_IVRApplications_GetApplicationProcessId(intptr_t instancePtr, const char * pchAppKey);
S_API char * VR_IVRApplications_GetApplicationsErrorNameFromEnum(intptr_t instancePtr, EVRApplicationError error);
S_API uint32_t VR_IVRApplications_GetApplicationPropertyString(intptr_t instancePtr, const char * pchAppKey, EVRApplicationProperty eProperty, char * pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError * peError);
S_API bool VR_IVRApplications_GetApplicationPropertyBool(intptr_t instancePtr, const char * pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError * peError);
S_API EVRApplicationError VR_IVRApplications_GetHomeApplication(intptr_t instancePtr, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen);
S_API EVRApplicationError VR_IVRApplications_SetHomeApplication(intptr_t instancePtr, const char * pchAppKey);
S_API EVRApplicationError VR_IVRApplications_SetApplicationAutoLaunch(intptr_t instancePtr, const char * pchAppKey, bool bAutoLaunch);
S_API bool VR_IVRApplications_GetApplicationAutoLaunch(intptr_t instancePtr, const char * pchAppKey);
S_API EVRApplicationError VR_IVRApplications_GetStartingApplication(intptr_t instancePtr, char * pchAppKeyBuffer, uint32_t unAppKeyBufferLen);
S_API EVRApplicationTransitionState VR_IVRApplications_GetTransitionState(intptr_t instancePtr);
S_API EVRApplicationError VR_IVRApplications_PerformApplicationPrelaunchCheck(intptr_t instancePtr, const char * pchAppKey);
S_API char * VR_IVRApplications_GetApplicationsTransitionStateNameFromEnum(intptr_t instancePtr, EVRApplicationTransitionState state);
S_API ChaperoneCalibrationState VR_IVRChaperone_GetCalibrationState(intptr_t instancePtr);
S_API bool VR_IVRChaperone_GetPlayAreaSize(intptr_t instancePtr, float * pSizeX, float * pSizeZ);
S_API bool VR_IVRChaperone_GetPlayAreaRect(intptr_t instancePtr, struct HmdQuad_t * rect);
S_API void VR_IVRChaperone_ReloadInfo(intptr_t instancePtr);
S_API void VR_IVRChaperone_SetSceneColor(intptr_t instancePtr, struct HmdColor_t color);
S_API void VR_IVRChaperone_GetBoundsColor(intptr_t instancePtr, struct HmdColor_t * pOutputColorArray, int nNumOutputColors);
S_API bool VR_IVRChaperone_AreBoundsVisible(intptr_t instancePtr);
S_API void VR_IVRChaperone_ForceBoundsVisible(intptr_t instancePtr, bool bForce);
S_API uint32_t VR_IVRCompositor_GetLastError(intptr_t instancePtr, char * pchBuffer, uint32_t unBufferSize);
S_API void VR_IVRCompositor_SetVSync(intptr_t instancePtr, bool bVSync);
S_API bool VR_IVRCompositor_GetVSync(intptr_t instancePtr);
S_API void VR_IVRCompositor_SetGamma(intptr_t instancePtr, float fGamma);
S_API float VR_IVRCompositor_GetGamma(intptr_t instancePtr);
S_API VRCompositorError VR_IVRCompositor_WaitGetPoses(intptr_t instancePtr, struct TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount);
S_API VRCompositorError VR_IVRCompositor_Submit(intptr_t instancePtr, Hmd_Eye eEye, GraphicsAPIConvention eTextureType, void * pTexture, const struct VRTextureBounds_t * pBounds, VRSubmitFlags_t nSubmitFlags);
S_API void VR_IVRCompositor_ClearLastSubmittedFrame(intptr_t instancePtr);
S_API bool VR_IVRCompositor_GetFrameTiming(intptr_t instancePtr, struct Compositor_FrameTiming * pTiming, uint32_t unFramesAgo);
S_API void VR_IVRCompositor_FadeToColor(intptr_t instancePtr, float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground);
S_API void VR_IVRCompositor_FadeGrid(intptr_t instancePtr, float fSeconds, bool bFadeIn);
S_API void VR_IVRCompositor_SetSkyboxOverride(intptr_t instancePtr, GraphicsAPIConvention eTextureType, void * pFront, void * pBack, void * pLeft, void * pRight, void * pTop, void * pBottom);
S_API void VR_IVRCompositor_ClearSkyboxOverride(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorBringToFront(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorGoToBack(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorQuit(intptr_t instancePtr);
S_API bool VR_IVRCompositor_IsFullscreen(intptr_t instancePtr);
S_API void VR_IVRCompositor_SetTrackingSpace(intptr_t instancePtr, TrackingUniverseOrigin eOrigin);
S_API TrackingUniverseOrigin VR_IVRCompositor_GetTrackingSpace(intptr_t instancePtr);
S_API uint32_t VR_IVRCompositor_GetCurrentSceneFocusProcess(intptr_t instancePtr);
S_API bool VR_IVRCompositor_CanRenderScene(intptr_t instancePtr);
S_API void VR_IVRCompositor_ShowMirrorWindow(intptr_t instancePtr);
S_API void VR_IVRCompositor_HideMirrorWindow(intptr_t instancePtr);
S_API void VR_IVRCompositor_CompositorDumpImages(intptr_t instancePtr);
S_API float VR_IVRCompositor_GetFrameTimeRemaining(intptr_t instancePtr);
S_API uint32_t VR_IVRCompositor_GetLastFrameRenderer(intptr_t instancePtr);
S_API VRCompositorError VR_IVRCompositor_GetLastPoses(intptr_t instancePtr, struct TrackedDevicePose_t * pRenderPoseArray, uint32_t unRenderPoseArrayCount, struct TrackedDevicePose_t * pGamePoseArray, uint32_t unGamePoseArrayCount);
S_API void VR_IVRCompositor_PostPresentHandoff(intptr_t instancePtr);
S_API VROverlayError VR_IVROverlay_FindOverlay(intptr_t instancePtr, const char * pchOverlayKey, VROverlayHandle_t * pOverlayHandle);
S_API VROverlayError VR_IVROverlay_CreateOverlay(intptr_t instancePtr, const char * pchOverlayKey, const char * pchOverlayFriendlyName, VROverlayHandle_t * pOverlayHandle);
S_API VROverlayError VR_IVROverlay_DestroyOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError VR_IVROverlay_SetHighQualityOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayHandle_t VR_IVROverlay_GetHighQualityOverlay(intptr_t instancePtr);
S_API uint32_t VR_IVROverlay_GetOverlayKey(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, VROverlayError * pError);
S_API uint32_t VR_IVROverlay_GetOverlayName(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, char * pchValue, uint32_t unBufferSize, VROverlayError * pError);
S_API VROverlayError VR_IVROverlay_GetOverlayImageData(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unBufferSize, uint32_t * punWidth, uint32_t * punHeight);
S_API char * VR_IVROverlay_GetOverlayErrorNameFromEnum(intptr_t instancePtr, VROverlayError error);
S_API VROverlayError VR_IVROverlay_SetOverlayFlag(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled);
S_API VROverlayError VR_IVROverlay_GetOverlayFlag(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool * pbEnabled);
S_API VROverlayError VR_IVROverlay_SetOverlayColor(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue);
S_API VROverlayError VR_IVROverlay_GetOverlayColor(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfRed, float * pfGreen, float * pfBlue);
S_API VROverlayError VR_IVROverlay_SetOverlayAlpha(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fAlpha);
S_API VROverlayError VR_IVROverlay_GetOverlayAlpha(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfAlpha);
S_API VROverlayError VR_IVROverlay_SetOverlayGamma(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fGamma);
S_API VROverlayError VR_IVROverlay_GetOverlayGamma(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfGamma);
S_API VROverlayError VR_IVROverlay_SetOverlayWidthInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fWidthInMeters);
S_API VROverlayError VR_IVROverlay_GetOverlayWidthInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfWidthInMeters);
S_API VROverlayError VR_IVROverlay_SetOverlayAutoCurveDistanceRangeInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float fMinDistanceInMeters, float fMaxDistanceInMeters);
S_API VROverlayError VR_IVROverlay_GetOverlayAutoCurveDistanceRangeInMeters(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, float * pfMinDistanceInMeters, float * pfMaxDistanceInMeters);
S_API VROverlayError VR_IVROverlay_SetOverlayTextureBounds(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct VRTextureBounds_t * pOverlayTextureBounds);
S_API VROverlayError VR_IVROverlay_GetOverlayTextureBounds(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct VRTextureBounds_t * pOverlayTextureBounds);
S_API VROverlayError VR_IVROverlay_GetOverlayTransformType(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayTransformType * peTransformType);
S_API VROverlayError VR_IVROverlay_SetOverlayTransformAbsolute(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackingUniverseOrigin eTrackingOrigin, const struct HmdMatrix34_t * pmatTrackingOriginToOverlayTransform);
S_API VROverlayError VR_IVROverlay_GetOverlayTransformAbsolute(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackingUniverseOrigin * peTrackingOrigin, struct HmdMatrix34_t * pmatTrackingOriginToOverlayTransform);
S_API VROverlayError VR_IVROverlay_SetOverlayTransformTrackedDeviceRelative(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const struct HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform);
S_API VROverlayError VR_IVROverlay_GetOverlayTransformTrackedDeviceRelative(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t * punTrackedDevice, struct HmdMatrix34_t * pmatTrackedDeviceToOverlayTransform);
S_API VROverlayError VR_IVROverlay_ShowOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError VR_IVROverlay_HideOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API bool VR_IVROverlay_IsOverlayVisible(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API bool VR_IVROverlay_PollNextOverlayEvent(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct VREvent_t * pEvent);
S_API VROverlayError VR_IVROverlay_GetOverlayInputMethod(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod * peInputMethod);
S_API VROverlayError VR_IVROverlay_SetOverlayInputMethod(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod);
S_API VROverlayError VR_IVROverlay_GetOverlayMouseScale(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, struct HmdVector2_t * pvecMouseScale);
S_API VROverlayError VR_IVROverlay_SetOverlayMouseScale(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct HmdVector2_t * pvecMouseScale);
S_API bool VR_IVROverlay_ComputeOverlayIntersection(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const struct VROverlayIntersectionParams_t * pParams, struct VROverlayIntersectionResults_t * pResults);
S_API bool VR_IVROverlay_HandleControllerOverlayInteractionAsMouse(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unControllerDeviceIndex);
S_API bool VR_IVROverlay_IsHoverTargetOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayHandle_t VR_IVROverlay_GetGamepadFocusOverlay(intptr_t instancePtr);
S_API VROverlayError VR_IVROverlay_SetGamepadFocusOverlay(intptr_t instancePtr, VROverlayHandle_t ulNewFocusOverlay);
S_API VROverlayError VR_IVROverlay_SetOverlayNeighbor(intptr_t instancePtr, EOverlayDirection eDirection, VROverlayHandle_t ulFrom, VROverlayHandle_t ulTo);
S_API VROverlayError VR_IVROverlay_MoveGamepadFocusToNeighbor(intptr_t instancePtr, EOverlayDirection eDirection, VROverlayHandle_t ulFrom);
S_API VROverlayError VR_IVROverlay_SetOverlayTexture(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, GraphicsAPIConvention eTextureType, void * pTexture);
S_API VROverlayError VR_IVROverlay_ClearOverlayTexture(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError VR_IVROverlay_SetOverlayRaw(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, void * pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unDepth);
S_API VROverlayError VR_IVROverlay_SetOverlayFromFile(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, const char * pchFilePath);
S_API VROverlayError VR_IVROverlay_CreateDashboardOverlay(intptr_t instancePtr, const char * pchOverlayKey, const char * pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t * pThumbnailHandle);
S_API bool VR_IVROverlay_IsDashboardVisible(intptr_t instancePtr);
S_API bool VR_IVROverlay_IsActiveDashboardOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle);
S_API VROverlayError VR_IVROverlay_SetDashboardOverlaySceneProcess(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId);
S_API VROverlayError VR_IVROverlay_GetDashboardOverlaySceneProcess(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, uint32_t * punProcessId);
S_API void VR_IVROverlay_ShowDashboard(intptr_t instancePtr, const char * pchOverlayToShow);
S_API VROverlayError VR_IVROverlay_ShowKeyboard(intptr_t instancePtr, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue);
S_API VROverlayError VR_IVROverlay_ShowKeyboardForOverlay(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, const char * pchDescription, uint32_t unCharMax, const char * pchExistingText, bool bUseMinimalMode, uint64_t uUserValue);
S_API uint32_t VR_IVROverlay_GetKeyboardText(intptr_t instancePtr, char * pchText, uint32_t cchText);
S_API void VR_IVROverlay_HideKeyboard(intptr_t instancePtr);
S_API bool VR_IVRRenderModels_LoadRenderModel(intptr_t instancePtr, const char * pchRenderModelName, struct RenderModel_t * pRenderModel);
S_API void VR_IVRRenderModels_FreeRenderModel(intptr_t instancePtr, struct RenderModel_t * pRenderModel);
S_API uint32_t VR_IVRRenderModels_GetRenderModelName(intptr_t instancePtr, uint32_t unRenderModelIndex, char * pchRenderModelName, uint32_t unRenderModelNameLen);
S_API uint32_t VR_IVRRenderModels_GetRenderModelCount(intptr_t instancePtr);
S_API uint32_t VR_IVRRenderModels_GetComponentCount(intptr_t instancePtr, const char * pchRenderModelName);
S_API uint32_t VR_IVRRenderModels_GetComponentName(intptr_t instancePtr, const char * pchRenderModelName, uint32_t unComponentIndex, char * pchComponentName, uint32_t unComponentNameLen);
S_API uint64_t VR_IVRRenderModels_GetComponentButtonMask(intptr_t instancePtr, const char * pchRenderModelName, const char * pchComponentName);
S_API uint32_t VR_IVRRenderModels_GetComponentRenderModelName(intptr_t instancePtr, const char * pchRenderModelName, const char * pchComponentName, char * pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen);
S_API bool VR_IVRRenderModels_GetComponentState(intptr_t instancePtr, const char * pchRenderModelName, const char * pchComponentName, VRControllerState_t controllerState, struct ComponentState_t * pComponentState);
S_API uint32_t VR_IVRControlPanel_GetDriverCount(intptr_t instancePtr);
S_API uint32_t VR_IVRControlPanel_GetDriverId(intptr_t instancePtr, uint32_t unDriverIndex, char * pchBuffer, uint32_t unBufferLen);
S_API uint32_t VR_IVRControlPanel_GetDriverDisplayCount(intptr_t instancePtr, const char * pchDriverId);
S_API uint32_t VR_IVRControlPanel_GetDriverDisplayId(intptr_t instancePtr, const char * pchDriverId, uint32_t unDisplayIndex, char * pchBuffer, uint32_t unBufferLen);
S_API uint32_t VR_IVRControlPanel_GetDriverDisplayModelNumber(intptr_t instancePtr, const char * pchDriverId, const char * pchDisplayId, char * pchBuffer, uint32_t unBufferLen);
S_API uint32_t VR_IVRControlPanel_GetDriverDisplaySerialNumber(intptr_t instancePtr, const char * pchDriverId, const char * pchDisplayId, char * pchBuffer, uint32_t unBufferLen);
S_API uint32_t VR_IVRControlPanel_LoadSharedResource(intptr_t instancePtr, const char * pchResourceName, char * pchBuffer, uint32_t unBufferLen);
S_API float VR_IVRControlPanel_GetIPD(intptr_t instancePtr);
S_API void VR_IVRControlPanel_SetIPD(intptr_t instancePtr, float fIPD);
S_API class IVRCompositor * VR_IVRControlPanel_GetCurrentCompositorInterface(intptr_t instancePtr, const char * pchInterfaceVersion);
S_API bool VR_IVRControlPanel_QuitProcess(intptr_t instancePtr, uint32_t pidProcessToQuit);
S_API uint32_t VR_IVRControlPanel_StartVRProcess(intptr_t instancePtr, const char * pchExecutable, const char ** pchArguments, uint32_t unArgumentCount, const char * pchWorkingDirectory);
S_API void VR_IVRControlPanel_SetMasterProcessToThis(intptr_t instancePtr);
S_API uint32_t VR_IVRNotifications_GetErrorString(intptr_t instancePtr, NotificationError_t error, char * pchBuffer, uint32_t unBufferSize);
S_API NotificationError_t VR_IVRNotifications_CreateNotification(intptr_t instancePtr, VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, const char * strType, const char * strText, const char * strCategory, const struct NotificationBitmap * photo, VRNotificationId * notificationId);
S_API NotificationError_t VR_IVRNotifications_DismissNotification(intptr_t instancePtr, VRNotificationId notificationId);
S_API char * VR_IVRSettings_GetSettingsErrorNameFromEnum(intptr_t instancePtr, EVRSettingsError eError);
S_API void VR_IVRSettings_Sync(intptr_t instancePtr, EVRSettingsError * peError);
S_API bool VR_IVRSettings_GetBool(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, bool bDefaultValue, EVRSettingsError * peError);
S_API void VR_IVRSettings_SetBool(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, bool bValue, EVRSettingsError * peError);
S_API int32_t VR_IVRSettings_GetInt32(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, int32_t nDefaultValue, EVRSettingsError * peError);
S_API void VR_IVRSettings_SetInt32(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, int32_t nValue, EVRSettingsError * peError);
S_API float VR_IVRSettings_GetFloat(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, float flDefaultValue, EVRSettingsError * peError);
S_API void VR_IVRSettings_SetFloat(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, float flValue, EVRSettingsError * peError);
S_API void VR_IVRSettings_GetString(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, char * pchValue, uint32_t unValueLen, const char * pchDefaultValue, EVRSettingsError * peError);
S_API void VR_IVRSettings_SetString(intptr_t instancePtr, const char * pchSection, const char * pchSettingsKey, const char * pchValue, EVRSettingsError * peError);
S_API bool VR_IVRTrackedCamera_HasCamera(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_GetCameraFirmwareDescription(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, char * pBuffer, uint32_t nBufferLen);
S_API bool VR_IVRTrackedCamera_GetCameraFrameDimensions(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, ECameraVideoStreamFormat nVideoStreamFormat, uint32_t * pWidth, uint32_t * pHeight);
S_API bool VR_IVRTrackedCamera_SetCameraVideoStreamFormat(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, ECameraVideoStreamFormat nVideoStreamFormat);
S_API ECameraVideoStreamFormat VR_IVRTrackedCamera_GetCameraVideoStreamFormat(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_EnableCameraForStreaming(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, bool bEnable);
S_API bool VR_IVRTrackedCamera_StartVideoStream(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_StopVideoStream(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_IsVideoStreamActive(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API float VR_IVRTrackedCamera_GetVideoStreamElapsedTime(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API CameraVideoStreamFrame_t * VR_IVRTrackedCamera_GetVideoStreamFrame(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_ReleaseVideoStreamFrame(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, const CameraVideoStreamFrame_t * pFrameImage);
S_API bool VR_IVRTrackedCamera_SetAutoExposure(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex, bool bEnable);
S_API bool VR_IVRTrackedCamera_SupportsPauseResume(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_PauseVideoStream(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_ResumeVideoStream(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRTrackedCamera_IsVideoStreamPaused(intptr_t instancePtr, TrackedDeviceIndex_t nDeviceIndex);
S_API bool VR_IVRChaperoneSetup_CommitWorkingCopy(intptr_t instancePtr, EChaperoneConfigFile configFile);
S_API void VR_IVRChaperoneSetup_RevertWorkingCopy(intptr_t instancePtr);
S_API bool VR_IVRChaperoneSetup_GetWorkingPlayAreaSize(intptr_t instancePtr, float * pSizeX, float * pSizeZ);
S_API bool VR_IVRChaperoneSetup_GetWorkingPlayAreaRect(intptr_t instancePtr, struct HmdQuad_t * rect);
S_API bool VR_IVRChaperoneSetup_GetWorkingCollisionBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool VR_IVRChaperoneSetup_GetLiveCollisionBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t * punQuadsCount);
S_API bool VR_IVRChaperoneSetup_GetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, struct HmdMatrix34_t * pmatSeatedZeroPoseToRawTrackingPose);
S_API bool VR_IVRChaperoneSetup_GetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, struct HmdMatrix34_t * pmatStandingZeroPoseToRawTrackingPose);
S_API void VR_IVRChaperoneSetup_SetWorkingPlayAreaSize(intptr_t instancePtr, float sizeX, float sizeZ);
S_API void VR_IVRChaperoneSetup_SetWorkingCollisionBoundsInfo(intptr_t instancePtr, struct HmdQuad_t * pQuadsBuffer, uint32_t unQuadsCount);
S_API void VR_IVRChaperoneSetup_SetWorkingSeatedZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct HmdMatrix34_t & matSeatedZeroPoseToRawTrackingPose);
S_API void VR_IVRChaperoneSetup_SetWorkingStandingZeroPoseToRawTrackingPose(intptr_t instancePtr, const struct HmdMatrix34_t & matStandingZeroPoseToRawTrackingPose);
S_API void VR_IVRChaperoneSetup_ReloadFromDisk(intptr_t instancePtr, EChaperoneConfigFile configFile);

// Global entry points
S_API intptr_t VR_Init( HmdError *peError, EVRApplicationType eType );
S_API void VR_Shutdown();
S_API bool VR_IsHmdPresent();
S_API const char *VR_GetStringForHmdError( HmdError error );
S_API intptr_t VR_GetGenericInterface( const char *pchInterfaceVersion, HmdError *peError );
S_API bool VR_IsRuntimeInstalled();

#endif // __OPENVR_API_FLAT_H__


