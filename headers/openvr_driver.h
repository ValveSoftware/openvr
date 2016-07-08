#pragma once

// openvr_driver.h
//========= Copyright Valve Corporation ============//
// Dynamically generated file. Do not modify this file directly.

#ifndef _OPENVR_DRIVER_API
#define _OPENVR_DRIVER_API

#include <stdint.h>



// vrtypes.h
#ifndef _INCLUDE_VRTYPES_H
#define _INCLUDE_VRTYPES_H 

namespace vr
{

#if defined(__linux__) || defined(__APPLE__) 
	// The 32-bit version of gcc has the alignment requirement for uint64 and double set to
	// 4 meaning that even with #pragma pack(8) these types will only be four-byte aligned.
	// The 64-bit version of gcc has the alignment requirement for these types set to
	// 8 meaning that unless we use #pragma pack(4) our structures will get bigger.
	// The 64-bit structure packing has to match the 32-bit structure packing for each platform.
	#pragma pack( push, 4 )
#else
	#pragma pack( push, 8 )
#endif

typedef void* glSharedTextureHandle_t;
typedef int32_t glInt_t;
typedef uint32_t glUInt_t;

// right-handed system
// +y is up
// +x is to the right
// -z is going away from you
// Distance unit is  meters
struct HmdMatrix34_t
{
	float m[3][4];
};

struct HmdMatrix44_t
{
	float m[4][4];
};

struct HmdVector3_t
{
	float v[3];
};

struct HmdVector4_t
{
	float v[4];
};

struct HmdVector3d_t
{
	double v[3];
};

struct HmdVector2_t
{
	float v[2];
};

struct HmdQuaternion_t
{
	double w, x, y, z;
};

struct HmdColor_t
{
	float r, g, b, a;
};

struct HmdQuad_t
{
	HmdVector3_t vCorners[ 4 ];
};

struct HmdRect2_t
{
	HmdVector2_t vTopLeft;
	HmdVector2_t vBottomRight;
};

/** Used to return the post-distortion UVs for each color channel. 
* UVs range from 0 to 1 with 0,0 in the upper left corner of the 
* source render target. The 0,0 to 1,1 range covers a single eye. */
struct DistortionCoordinates_t
{
	float rfRed[2];
	float rfGreen[2];
	float rfBlue[2];
};

enum EVREye
{
	Eye_Left = 0,
	Eye_Right = 1
};

enum EGraphicsAPIConvention
{
	API_DirectX = 0, // Normalized Z goes from 0 at the viewer to 1 at the far clip plane
	API_OpenGL = 1,  // Normalized Z goes from 1 at the viewer to -1 at the far clip plane
};

enum EColorSpace
{
	ColorSpace_Auto = 0,	// Assumes 'gamma' for 8-bit per component formats, otherwise 'linear'.  This mirrors the DXGI formats which have _SRGB variants.
	ColorSpace_Gamma = 1,	// Texture data can be displayed directly on the display without any conversion (a.k.a. display native format).
	ColorSpace_Linear = 2,	// Same as gamma but has been converted to a linear representation using DXGI's sRGB conversion algorithm.
};

struct Texture_t
{
	void* handle; // Native d3d texture pointer or GL texture id.
	EGraphicsAPIConvention eType;
	EColorSpace eColorSpace;
};

enum ETrackingResult
{
	TrackingResult_Uninitialized			= 1,

	TrackingResult_Calibrating_InProgress	= 100,
	TrackingResult_Calibrating_OutOfRange	= 101,

	TrackingResult_Running_OK				= 200,
	TrackingResult_Running_OutOfRange		= 201,
};

static const uint32_t k_unTrackingStringSize = 32;
static const uint32_t k_unMaxDriverDebugResponseSize = 32768;

/** Used to pass device IDs to API calls */
typedef uint32_t TrackedDeviceIndex_t;
static const uint32_t k_unTrackedDeviceIndex_Hmd = 0;
static const uint32_t k_unMaxTrackedDeviceCount = 16;
static const uint32_t k_unTrackedDeviceIndexOther = 0xFFFFFFFE;
static const uint32_t k_unTrackedDeviceIndexInvalid = 0xFFFFFFFF;

/** Describes what kind of object is being tracked at a given ID */
enum ETrackedDeviceClass
{
	TrackedDeviceClass_Invalid = 0,				// the ID was not valid.
	TrackedDeviceClass_HMD = 1,					// Head-Mounted Displays
	TrackedDeviceClass_Controller = 2,			// Tracked controllers
	TrackedDeviceClass_TrackingReference = 4,	// Camera and base stations that serve as tracking reference points

	TrackedDeviceClass_Other = 1000,
};


/** Describes what specific role associated with a tracked device */
enum ETrackedControllerRole
{
	TrackedControllerRole_Invalid = 0,					// Invalid value for controller type
	TrackedControllerRole_LeftHand = 1,					// Tracked device associated with the left hand
	TrackedControllerRole_RightHand = 2,				// Tracked device associated with the right hand
};


/** describes a single pose for a tracked object */
struct TrackedDevicePose_t
{
	HmdMatrix34_t mDeviceToAbsoluteTracking;
	HmdVector3_t vVelocity;				// velocity in tracker space in m/s
	HmdVector3_t vAngularVelocity;		// angular velocity in radians/s (?)
	ETrackingResult eTrackingResult;
	bool bPoseIsValid;

	// This indicates that there is a device connected for this spot in the pose array.
	// It could go from true to false if the user unplugs the device.
	bool bDeviceIsConnected;
};

/** Identifies which style of tracking origin the application wants to use
* for the poses it is requesting */
enum ETrackingUniverseOrigin
{
	TrackingUniverseSeated = 0,		// Poses are provided relative to the seated zero pose
	TrackingUniverseStanding = 1,	// Poses are provided relative to the safe bounds configured by the user
	TrackingUniverseRawAndUncalibrated = 2,	// Poses are provided in the coordinate system defined by the driver. You probably don't want this one.
};


/** Each entry in this enum represents a property that can be retrieved about a
* tracked device. Many fields are only valid for one ETrackedDeviceClass. */
enum ETrackedDeviceProperty
{
	// general properties that apply to all device classes
	Prop_TrackingSystemName_String				= 1000,
	Prop_ModelNumber_String						= 1001,
	Prop_SerialNumber_String					= 1002,
	Prop_RenderModelName_String					= 1003,
	Prop_WillDriftInYaw_Bool					= 1004,
	Prop_ManufacturerName_String				= 1005,
	Prop_TrackingFirmwareVersion_String			= 1006,
	Prop_HardwareRevision_String				= 1007,
	Prop_AllWirelessDongleDescriptions_String	= 1008,
	Prop_ConnectedWirelessDongle_String			= 1009,
	Prop_DeviceIsWireless_Bool					= 1010,
	Prop_DeviceIsCharging_Bool					= 1011,
	Prop_DeviceBatteryPercentage_Float			= 1012, // 0 is empty, 1 is full
	Prop_StatusDisplayTransform_Matrix34		= 1013,
	Prop_Firmware_UpdateAvailable_Bool			= 1014,
	Prop_Firmware_ManualUpdate_Bool				= 1015,
	Prop_Firmware_ManualUpdateURL_String		= 1016,
	Prop_HardwareRevision_Uint64				= 1017,
	Prop_FirmwareVersion_Uint64					= 1018,
	Prop_FPGAVersion_Uint64						= 1019,
	Prop_VRCVersion_Uint64						= 1020,
	Prop_RadioVersion_Uint64					= 1021,
	Prop_DongleVersion_Uint64					= 1022,
	Prop_BlockServerShutdown_Bool				= 1023,
	Prop_CanUnifyCoordinateSystemWithHmd_Bool	= 1024,
	Prop_ContainsProximitySensor_Bool			= 1025,
	Prop_DeviceProvidesBatteryStatus_Bool		= 1026,
	Prop_DeviceCanPowerOff_Bool					= 1027,
	Prop_Firmware_ProgrammingTarget_String		= 1028,
	Prop_DeviceClass_Int32						= 1029,
	Prop_HasCamera_Bool							= 1030,
	Prop_DriverVersion_String                   = 1031,
	Prop_Firmware_ForceUpdateRequired_Bool      = 1032,

	// Properties that are unique to TrackedDeviceClass_HMD
	Prop_ReportsTimeSinceVSync_Bool				= 2000,
	Prop_SecondsFromVsyncToPhotons_Float		= 2001,
	Prop_DisplayFrequency_Float					= 2002,
	Prop_UserIpdMeters_Float					= 2003,
	Prop_CurrentUniverseId_Uint64				= 2004, 
	Prop_PreviousUniverseId_Uint64				= 2005, 
	Prop_DisplayFirmwareVersion_Uint64			= 2006,
	Prop_IsOnDesktop_Bool						= 2007,
	Prop_DisplayMCType_Int32					= 2008,
	Prop_DisplayMCOffset_Float					= 2009,
	Prop_DisplayMCScale_Float					= 2010,
	Prop_EdidVendorID_Int32						= 2011,
	Prop_DisplayMCImageLeft_String              = 2012,
	Prop_DisplayMCImageRight_String             = 2013,
	Prop_DisplayGCBlackClamp_Float				= 2014,
	Prop_EdidProductID_Int32					= 2015,
	Prop_CameraToHeadTransform_Matrix34			= 2016,
	Prop_DisplayGCType_Int32					= 2017,
	Prop_DisplayGCOffset_Float					= 2018,
	Prop_DisplayGCScale_Float					= 2019,
	Prop_DisplayGCPrescale_Float				= 2020,
	Prop_DisplayGCImage_String					= 2021,
	Prop_LensCenterLeftU_Float					= 2022,
	Prop_LensCenterLeftV_Float					= 2023,
	Prop_LensCenterRightU_Float					= 2024,
	Prop_LensCenterRightV_Float					= 2025,
	Prop_UserHeadToEyeDepthMeters_Float			= 2026,
	Prop_CameraFirmwareVersion_Uint64			= 2027,
	Prop_CameraFirmwareDescription_String		= 2028,
	Prop_DisplayFPGAVersion_Uint64				= 2029,
	Prop_DisplayBootloaderVersion_Uint64		= 2030,
	Prop_DisplayHardwareVersion_Uint64			= 2031,
	Prop_AudioFirmwareVersion_Uint64			= 2032,
	Prop_CameraCompatibilityMode_Int32			= 2033,
	Prop_ScreenshotHorizontalFieldOfViewDegrees_Float = 2034,
	Prop_ScreenshotVerticalFieldOfViewDegrees_Float = 2035,
	Prop_DisplaySuppressed_Bool					= 2036,

	// Properties that are unique to TrackedDeviceClass_Controller
	Prop_AttachedDeviceId_String				= 3000,
	Prop_SupportedButtons_Uint64				= 3001,
	Prop_Axis0Type_Int32						= 3002, // Return value is of type EVRControllerAxisType
	Prop_Axis1Type_Int32						= 3003, // Return value is of type EVRControllerAxisType
	Prop_Axis2Type_Int32						= 3004, // Return value is of type EVRControllerAxisType
	Prop_Axis3Type_Int32						= 3005, // Return value is of type EVRControllerAxisType
	Prop_Axis4Type_Int32						= 3006, // Return value is of type EVRControllerAxisType
	Prop_ControllerRoleHint_Int32				= 3007, // Return value is of type ETrackedControllerRole

	// Properties that are unique to TrackedDeviceClass_TrackingReference
	Prop_FieldOfViewLeftDegrees_Float			= 4000,
	Prop_FieldOfViewRightDegrees_Float			= 4001,
	Prop_FieldOfViewTopDegrees_Float			= 4002,
	Prop_FieldOfViewBottomDegrees_Float			= 4003,
	Prop_TrackingRangeMinimumMeters_Float		= 4004,
	Prop_TrackingRangeMaximumMeters_Float		= 4005,
	Prop_ModeLabel_String						= 4006,

	// Vendors are free to expose private debug data in this reserved region
	Prop_VendorSpecific_Reserved_Start			= 10000,
	Prop_VendorSpecific_Reserved_End			= 10999,
};

/** No string property will ever be longer than this length */
static const uint32_t k_unMaxPropertyStringSize = 32 * 1024;

/** Used to return errors that occur when reading properties. */
enum ETrackedPropertyError
{
	TrackedProp_Success						= 0,
	TrackedProp_WrongDataType				= 1,
	TrackedProp_WrongDeviceClass			= 2,
	TrackedProp_BufferTooSmall				= 3,
	TrackedProp_UnknownProperty				= 4,
	TrackedProp_InvalidDevice				= 5,
	TrackedProp_CouldNotContactServer		= 6,
	TrackedProp_ValueNotProvidedByDevice	= 7,
	TrackedProp_StringExceedsMaximumLength	= 8,
	TrackedProp_NotYetAvailable				= 9, // The property value isn't known yet, but is expected soon. Call again later.
};

/** Allows the application to control what part of the provided texture will be used in the
* frame buffer. */
struct VRTextureBounds_t
{
	float uMin, vMin;
	float uMax, vMax;
};


/** Allows the application to control how scene textures are used by the compositor when calling Submit. */
enum EVRSubmitFlags
{
	// Simple render path. App submits rendered left and right eye images with no lens distortion correction applied.
	Submit_Default = 0x00,

	// App submits final left and right eye images with lens distortion already applied (lens distortion makes the images appear
	// barrel distorted with chromatic aberration correction applied). The app would have used the data returned by
	// vr::IVRSystem::ComputeDistortion() to apply the correct distortion to the rendered images before calling Submit().
	Submit_LensDistortionAlreadyApplied = 0x01,

	// If the texture pointer passed in is actually a renderbuffer (e.g. for MSAA in OpenGL) then set this flag.
	Submit_GlRenderBuffer = 0x02,
};


/** Status of the overall system or tracked objects */
enum EVRState
{
	VRState_Undefined = -1,
	VRState_Off = 0,
	VRState_Searching = 1,
	VRState_Searching_Alert = 2,
	VRState_Ready = 3,
	VRState_Ready_Alert = 4,
	VRState_NotReady = 5,
	VRState_Standby = 6,
	VRState_Ready_Alert_Low = 7,
};

/** The types of events that could be posted (and what the parameters mean for each event type) */
enum EVREventType
{
	VREvent_None = 0,

	VREvent_TrackedDeviceActivated		= 100,
	VREvent_TrackedDeviceDeactivated	= 101,
	VREvent_TrackedDeviceUpdated		= 102,
	VREvent_TrackedDeviceUserInteractionStarted		= 103,
	VREvent_TrackedDeviceUserInteractionEnded	= 104,
	VREvent_IpdChanged					= 105,
	VREvent_EnterStandbyMode			= 106,
	VREvent_LeaveStandbyMode			= 107,
	VREvent_TrackedDeviceRoleChanged	= 108,
	VREvent_WatchdogWakeUpRequested		= 109,

	VREvent_ButtonPress					= 200, // data is controller
	VREvent_ButtonUnpress				= 201, // data is controller
	VREvent_ButtonTouch					= 202, // data is controller
	VREvent_ButtonUntouch				= 203, // data is controller

	VREvent_MouseMove					= 300, // data is mouse
	VREvent_MouseButtonDown				= 301, // data is mouse
	VREvent_MouseButtonUp				= 302, // data is mouse
	VREvent_FocusEnter					= 303, // data is overlay
	VREvent_FocusLeave					= 304, // data is overlay
	VREvent_Scroll						= 305, // data is mouse
	VREvent_TouchPadMove				= 306, // data is mouse
	VREvent_OverlayFocusChanged			= 307, // data is overlay, global event

	VREvent_InputFocusCaptured			= 400, // data is process DEPRECATED
	VREvent_InputFocusReleased			= 401, // data is process DEPRECATED
	VREvent_SceneFocusLost				= 402, // data is process
	VREvent_SceneFocusGained			= 403, // data is process
	VREvent_SceneApplicationChanged		= 404, // data is process - The App actually drawing the scene changed (usually to or from the compositor)
	VREvent_SceneFocusChanged			= 405, // data is process - New app got access to draw the scene
	VREvent_InputFocusChanged			= 406, // data is process
	VREvent_SceneApplicationSecondaryRenderingStarted = 407, // data is process

	VREvent_HideRenderModels			= 410, // Sent to the scene application to request hiding render models temporarily
	VREvent_ShowRenderModels			= 411, // Sent to the scene application to request restoring render model visibility

	VREvent_OverlayShown				= 500,
	VREvent_OverlayHidden				= 501,
	VREvent_DashboardActivated		= 502,
	VREvent_DashboardDeactivated	= 503,
	VREvent_DashboardThumbSelected	= 504, // Sent to the overlay manager - data is overlay
	VREvent_DashboardRequested		= 505, // Sent to the overlay manager - data is overlay
	VREvent_ResetDashboard			= 506, // Send to the overlay manager
	VREvent_RenderToast				= 507, // Send to the dashboard to render a toast - data is the notification ID
	VREvent_ImageLoaded				= 508, // Sent to overlays when a SetOverlayRaw or SetOverlayFromFile call finishes loading
	VREvent_ShowKeyboard = 509, // Sent to keyboard renderer in the dashboard to invoke it
	VREvent_HideKeyboard = 510, // Sent to keyboard renderer in the dashboard to hide it
	VREvent_OverlayGamepadFocusGained		= 511, // Sent to an overlay when IVROverlay::SetFocusOverlay is called on it
	VREvent_OverlayGamepadFocusLost = 512, // Send to an overlay when it previously had focus and IVROverlay::SetFocusOverlay is called on something else
	VREvent_OverlaySharedTextureChanged = 513,
	VREvent_DashboardGuideButtonDown = 514,
	VREvent_DashboardGuideButtonUp = 515,
	VREvent_ScreenshotTriggered	= 516, // Screenshot button combo was pressed, Dashboard should request a screenshot
	VREvent_ImageFailed				= 517, // Sent to overlays when a SetOverlayRaw or SetOverlayfromFail fails to load

	// Screenshot API
	VREvent_RequestScreenshot = 520, // Sent by vrclient application to compositor to take a screenshot
	VREvent_ScreenshotTaken = 521, // Sent by compositor to the application that the screenshot has been taken
	VREvent_ScreenshotFailed = 522, // Sent by compositor to the application that the screenshot failed to be taken
	VREvent_SubmitScreenshotToDashboard = 523, // Sent by compositor to the dashboard that a completed screenshot was submitted
	VREvent_ScreenshotProgressToDashboard = 524, // Sent by compositor to the dashboard that a completed screenshot was submitted

	VREvent_Notification_Shown				= 600,
	VREvent_Notification_Hidden				= 601,
	VREvent_Notification_BeginInteraction	= 602,
	VREvent_Notification_Destroyed			= 603,

	VREvent_Quit						= 700, // data is process
	VREvent_ProcessQuit					= 701, // data is process
	VREvent_QuitAborted_UserPrompt		= 702, // data is process
	VREvent_QuitAcknowledged			= 703, // data is process
	VREvent_DriverRequestedQuit			= 704, // The driver has requested that SteamVR shut down

	VREvent_ChaperoneDataHasChanged		= 800,
	VREvent_ChaperoneUniverseHasChanged	= 801,
	VREvent_ChaperoneTempDataHasChanged = 802,
	VREvent_ChaperoneSettingsHaveChanged = 803,
	VREvent_SeatedZeroPoseReset			= 804,

	VREvent_AudioSettingsHaveChanged	= 820,

	VREvent_BackgroundSettingHasChanged	= 850,
	VREvent_CameraSettingsHaveChanged	= 851,
	VREvent_ReprojectionSettingHasChanged = 852,
	VREvent_ModelSkinSettingsHaveChanged = 853,
	VREvent_EnvironmentSettingsHaveChanged = 854,

	VREvent_StatusUpdate				= 900,

	VREvent_MCImageUpdated				= 1000,

	VREvent_FirmwareUpdateStarted	= 1100,
	VREvent_FirmwareUpdateFinished	= 1101,

	VREvent_KeyboardClosed				= 1200,
	VREvent_KeyboardCharInput			= 1201,
	VREvent_KeyboardDone				= 1202, // Sent when DONE button clicked on keyboard

	VREvent_ApplicationTransitionStarted	= 1300,
	VREvent_ApplicationTransitionAborted	= 1301,
	VREvent_ApplicationTransitionNewAppStarted = 1302,
	VREvent_ApplicationListUpdated			= 1303,
	VREvent_ApplicationMimeTypeLoad			= 1304,

	VREvent_Compositor_MirrorWindowShown	= 1400,
	VREvent_Compositor_MirrorWindowHidden	= 1401,
	VREvent_Compositor_ChaperoneBoundsShown = 1410,
	VREvent_Compositor_ChaperoneBoundsHidden = 1411,

	VREvent_TrackedCamera_StartVideoStream  = 1500,
	VREvent_TrackedCamera_StopVideoStream   = 1501,
	VREvent_TrackedCamera_PauseVideoStream  = 1502,
	VREvent_TrackedCamera_ResumeVideoStream = 1503,

	VREvent_PerformanceTest_EnableCapture = 1600,
	VREvent_PerformanceTest_DisableCapture = 1601,
	VREvent_PerformanceTest_FidelityLevel = 1602,
	
	// Vendors are free to expose private events in this reserved region
	VREvent_VendorSpecific_Reserved_Start = 10000,
	VREvent_VendorSpecific_Reserved_End = 19999,
};


/** Level of Hmd activity */
enum EDeviceActivityLevel
{
	k_EDeviceActivityLevel_Unknown = -1,
	k_EDeviceActivityLevel_Idle = 0,
	k_EDeviceActivityLevel_UserInteraction = 1,
	k_EDeviceActivityLevel_UserInteraction_Timeout = 2,
	k_EDeviceActivityLevel_Standby = 3,
};


/** VR controller button and axis IDs */
enum EVRButtonId
{
	k_EButton_System			= 0,
	k_EButton_ApplicationMenu	= 1,
	k_EButton_Grip				= 2,
	k_EButton_DPad_Left			= 3,
	k_EButton_DPad_Up			= 4,
	k_EButton_DPad_Right		= 5,
	k_EButton_DPad_Down			= 6,
	k_EButton_A					= 7,

	k_EButton_Axis0				= 32,
	k_EButton_Axis1				= 33,
	k_EButton_Axis2				= 34,
	k_EButton_Axis3				= 35,
	k_EButton_Axis4				= 36,

	// aliases for well known controllers
	k_EButton_SteamVR_Touchpad	= k_EButton_Axis0,
	k_EButton_SteamVR_Trigger	= k_EButton_Axis1,

	k_EButton_Dashboard_Back	= k_EButton_Grip,

	k_EButton_Max				= 64
};

inline uint64_t ButtonMaskFromId( EVRButtonId id ) { return 1ull << id; }

/** used for controller button events */
struct VREvent_Controller_t
{
	uint32_t button; // EVRButtonId enum
};


/** used for simulated mouse events in overlay space */
enum EVRMouseButton
{
	VRMouseButton_Left					= 0x0001,
	VRMouseButton_Right					= 0x0002,
	VRMouseButton_Middle				= 0x0004,
};


/** used for simulated mouse events in overlay space */
struct VREvent_Mouse_t
{
	float x, y; // co-ords are in GL space, bottom left of the texture is 0,0
	uint32_t button; // EVRMouseButton enum
};

/** used for simulated mouse wheel scroll in overlay space */
struct VREvent_Scroll_t
{
	float xdelta, ydelta; // movement in fraction of the pad traversed since last delta, 1.0 for a full swipe
	uint32_t repeatCount;
};

/** when in mouse input mode you can receive data from the touchpad, these events are only sent if the users finger
   is on the touchpad (or just released from it) 
**/
struct VREvent_TouchPadMove_t
{
	// true if the users finger is detected on the touch pad
	bool bFingerDown;

	// How long the finger has been down in seconds
	float flSecondsFingerDown;

	// These values indicate the starting finger position (so you can do some basic swipe stuff)
	float fValueXFirst;
	float fValueYFirst;

	// This is the raw sampled coordinate without deadzoning
	float fValueXRaw;
	float fValueYRaw;
};

/** notification related events. Details will still change at this point */
struct VREvent_Notification_t
{
	uint64_t ulUserValue;
	uint32_t notificationId;
};

/** Used for events about processes */
struct VREvent_Process_t
{
	uint32_t pid;
	uint32_t oldPid;
	bool bForced;
};


/** Used for a few events about overlays */
struct VREvent_Overlay_t
{
	uint64_t overlayHandle;
};


/** Used for a few events about overlays */
struct VREvent_Status_t
{
	uint32_t statusState; // EVRState enum
};

/** Used for keyboard events **/
struct VREvent_Keyboard_t
{
	char cNewInput[8];	// Up to 11 bytes of new input
	uint64_t uUserValue;	// Possible flags about the new input
};

struct VREvent_Ipd_t
{
	float ipdMeters;
};

struct VREvent_Chaperone_t
{
	uint64_t m_nPreviousUniverse;
	uint64_t m_nCurrentUniverse;
};

/** Not actually used for any events */
struct VREvent_Reserved_t
{
	uint64_t reserved0;
	uint64_t reserved1;
};

struct VREvent_PerformanceTest_t
{
	uint32_t m_nFidelityLevel;
};

struct VREvent_SeatedZeroPoseReset_t
{
	bool bResetBySystemMenu;
};

struct VREvent_Screenshot_t
{
	uint32_t handle;
	uint32_t type;
};

struct VREvent_ScreenshotProgress_t
{
	float progress;
};

struct VREvent_ApplicationLaunch_t
{
	uint32_t pid;
	uint32_t unArgsHandle;
};

/** If you change this you must manually update openvr_interop.cs.py */
typedef union
{
	VREvent_Reserved_t reserved;
	VREvent_Controller_t controller;
	VREvent_Mouse_t mouse;
	VREvent_Scroll_t scroll;
	VREvent_Process_t process;
	VREvent_Notification_t notification;
	VREvent_Overlay_t overlay;
	VREvent_Status_t status;
	VREvent_Keyboard_t keyboard;
	VREvent_Ipd_t ipd;
	VREvent_Chaperone_t chaperone;
	VREvent_PerformanceTest_t performanceTest;
	VREvent_TouchPadMove_t touchPadMove;
	VREvent_SeatedZeroPoseReset_t seatedZeroPoseReset;
	VREvent_Screenshot_t screenshot;
	VREvent_ScreenshotProgress_t screenshotProgress;
	VREvent_ApplicationLaunch_t applicationLaunch;
} VREvent_Data_t;

/** An event posted by the server to all running applications */
struct VREvent_t
{
	uint32_t eventType; // EVREventType enum
	TrackedDeviceIndex_t trackedDeviceIndex;
	float eventAgeSeconds;
	// event data must be the end of the struct as its size is variable
	VREvent_Data_t data;
};


/** The mesh to draw into the stencil (or depth) buffer to perform 
* early stencil (or depth) kills of pixels that will never appear on the HMD.
* This mesh draws on all the pixels that will be hidden after distortion. 
*
* If the HMD does not provide a visible area mesh pVertexData will be
* NULL and unTriangleCount will be 0. */
struct HiddenAreaMesh_t
{
	const HmdVector2_t *pVertexData;
	uint32_t unTriangleCount;
};


/** Identifies what kind of axis is on the controller at index n. Read this type 
* with pVRSystem->Get( nControllerDeviceIndex, Prop_Axis0Type_Int32 + n );
*/
enum EVRControllerAxisType
{
	k_eControllerAxis_None = 0,
	k_eControllerAxis_TrackPad = 1,
	k_eControllerAxis_Joystick = 2,
	k_eControllerAxis_Trigger = 3, // Analog trigger data is in the X axis
};


/** contains information about one axis on the controller */
struct VRControllerAxis_t
{
	float x; // Ranges from -1.0 to 1.0 for joysticks and track pads. Ranges from 0.0 to 1.0 for triggers were 0 is fully released.
	float y; // Ranges from -1.0 to 1.0 for joysticks and track pads. Is always 0.0 for triggers.
};


/** the number of axes in the controller state */
static const uint32_t k_unControllerStateAxisCount = 5;


/** Holds all the state of a controller at one moment in time. */
struct VRControllerState001_t
{
	// If packet num matches that on your prior call, then the controller state hasn't been changed since 
	// your last call and there is no need to process it
	uint32_t unPacketNum;

	// bit flags for each of the buttons. Use ButtonMaskFromId to turn an ID into a mask
	uint64_t ulButtonPressed;
	uint64_t ulButtonTouched;

	// Axis data for the controller's analog inputs
	VRControllerAxis_t rAxis[ k_unControllerStateAxisCount ];
};


typedef VRControllerState001_t VRControllerState_t;


/** determines how to provide output to the application of various event processing functions. */
enum EVRControllerEventOutputType
{
	ControllerEventOutput_OSEvents = 0,
	ControllerEventOutput_VREvents = 1,
};



/** Collision Bounds Style */
enum ECollisionBoundsStyle
{
	COLLISION_BOUNDS_STYLE_BEGINNER = 0,
	COLLISION_BOUNDS_STYLE_INTERMEDIATE,
	COLLISION_BOUNDS_STYLE_SQUARES,
	COLLISION_BOUNDS_STYLE_ADVANCED,
	COLLISION_BOUNDS_STYLE_NONE,

	COLLISION_BOUNDS_STYLE_COUNT
};

/** Allows the application to customize how the overlay appears in the compositor */
struct Compositor_OverlaySettings
{
	uint32_t size; // sizeof(Compositor_OverlaySettings)
	bool curved, antialias;
	float scale, distance, alpha;
	float uOffset, vOffset, uScale, vScale;
	float gridDivs, gridWidth, gridScale;
	HmdMatrix44_t transform;
};

/** used to refer to a single VR overlay */
typedef uint64_t VROverlayHandle_t;

static const VROverlayHandle_t k_ulOverlayHandleInvalid = 0;

/** Errors that can occur around VR overlays */
enum EVROverlayError
{
	VROverlayError_None					= 0,

	VROverlayError_UnknownOverlay		= 10,
	VROverlayError_InvalidHandle		= 11,
	VROverlayError_PermissionDenied		= 12,
	VROverlayError_OverlayLimitExceeded = 13, // No more overlays could be created because the maximum number already exist
	VROverlayError_WrongVisibilityType	= 14,
	VROverlayError_KeyTooLong			= 15,
	VROverlayError_NameTooLong			= 16,
	VROverlayError_KeyInUse				= 17,
	VROverlayError_WrongTransformType	= 18,
	VROverlayError_InvalidTrackedDevice = 19,
	VROverlayError_InvalidParameter		= 20,
	VROverlayError_ThumbnailCantBeDestroyed = 21,
	VROverlayError_ArrayTooSmall		= 22,
	VROverlayError_RequestFailed		= 23,
	VROverlayError_InvalidTexture		= 24,
	VROverlayError_UnableToLoadFile		= 25,
	VROVerlayError_KeyboardAlreadyInUse = 26,
	VROverlayError_NoNeighbor			= 27,
};

/** enum values to pass in to VR_Init to identify whether the application will 
* draw a 3D scene. */
enum EVRApplicationType
{
	VRApplication_Other = 0,		// Some other kind of application that isn't covered by the other entries 
	VRApplication_Scene	= 1,		// Application will submit 3D frames 
	VRApplication_Overlay = 2,		// Application only interacts with overlays
	VRApplication_Background = 3,	// Application should not start SteamVR if it's not already running, and should not
									// keep it running if everything else quits.
	VRApplication_Utility = 4,		// Init should not try to load any drivers. The application needs access to utility
									// interfaces (like IVRSettings and IVRApplications) but not hardware.
	VRApplication_VRMonitor = 5,	// Reserved for vrmonitor
	VRApplication_SteamWatchdog = 6,// Reserved for Steam

	VRApplication_Max
};


/** error codes for firmware */
enum EVRFirmwareError
{
	VRFirmwareError_None = 0,
	VRFirmwareError_Success = 1,
	VRFirmwareError_Fail = 2,
};


/** error codes for notifications */
enum EVRNotificationError
{
	VRNotificationError_OK = 0,
	VRNotificationError_InvalidNotificationId = 100,
	VRNotificationError_NotificationQueueFull = 101,
	VRNotificationError_InvalidOverlayHandle = 102,
	VRNotificationError_SystemWithUserValueAlreadyExists = 103,
};


/** error codes returned by Vr_Init */

// Please add adequate error description to https://developer.valvesoftware.com/w/index.php?title=Category:SteamVRHelp
enum EVRInitError
{
	VRInitError_None	= 0,
	VRInitError_Unknown = 1,

	VRInitError_Init_InstallationNotFound		= 100,
	VRInitError_Init_InstallationCorrupt		= 101,
	VRInitError_Init_VRClientDLLNotFound		= 102,
	VRInitError_Init_FileNotFound				= 103,
	VRInitError_Init_FactoryNotFound			= 104,
	VRInitError_Init_InterfaceNotFound			= 105,
	VRInitError_Init_InvalidInterface			= 106,
	VRInitError_Init_UserConfigDirectoryInvalid = 107,
	VRInitError_Init_HmdNotFound				= 108,
	VRInitError_Init_NotInitialized				= 109,
	VRInitError_Init_PathRegistryNotFound		= 110,
	VRInitError_Init_NoConfigPath				= 111,
	VRInitError_Init_NoLogPath					= 112,
	VRInitError_Init_PathRegistryNotWritable	= 113,
	VRInitError_Init_AppInfoInitFailed			= 114,
	VRInitError_Init_Retry						= 115, // Used internally to cause retries to vrserver
	VRInitError_Init_InitCanceledByUser			= 116, // The calling application should silently exit. The user canceled app startup
	VRInitError_Init_AnotherAppLaunching		= 117, 
	VRInitError_Init_SettingsInitFailed			= 118, 
	VRInitError_Init_ShuttingDown				= 119,
	VRInitError_Init_TooManyObjects				= 120,
	VRInitError_Init_NoServerForBackgroundApp	= 121,
	VRInitError_Init_NotSupportedWithCompositor	= 122,
	VRInitError_Init_NotAvailableToUtilityApps	= 123,
	VRInitError_Init_Internal				 	= 124,
	VRInitError_Init_HmdDriverIdIsNone		 	= 125,
	VRInitError_Init_HmdNotFoundPresenceFailed 	= 126,
	VRInitError_Init_VRMonitorNotFound			= 127,
	VRInitError_Init_VRMonitorStartupFailed		= 128,
	VRInitError_Init_LowPowerWatchdogNotSupported = 129, 
	VRInitError_Init_InvalidApplicationType		= 130,
	VRInitError_Init_NotAvailableToWatchdogApps = 131,
	VRInitError_Init_WatchdogDisabledInSettings = 132,

	VRInitError_Driver_Failed				= 200,
	VRInitError_Driver_Unknown				= 201,
	VRInitError_Driver_HmdUnknown			= 202,
	VRInitError_Driver_NotLoaded			= 203,
	VRInitError_Driver_RuntimeOutOfDate		= 204,
	VRInitError_Driver_HmdInUse				= 205,
	VRInitError_Driver_NotCalibrated		= 206,
	VRInitError_Driver_CalibrationInvalid	= 207,
	VRInitError_Driver_HmdDisplayNotFound	= 208,
	VRInitError_Driver_TrackedDeviceInterfaceUnknown = 209,
	// VRInitError_Driver_HmdDisplayNotFoundAfterFix	 = 210, // not needed: here for historic reasons
	VRInitError_Driver_HmdDriverIdOutOfBounds = 211,
	VRInitError_Driver_HmdDisplayMirrored  = 212,

	VRInitError_IPC_ServerInitFailed		= 300,
	VRInitError_IPC_ConnectFailed			= 301,
	VRInitError_IPC_SharedStateInitFailed	= 302,
	VRInitError_IPC_CompositorInitFailed	= 303,
	VRInitError_IPC_MutexInitFailed			= 304,
	VRInitError_IPC_Failed					= 305,
	VRInitError_IPC_CompositorConnectFailed	= 306,
	VRInitError_IPC_CompositorInvalidConnectResponse = 307,
	VRInitError_IPC_ConnectFailedAfterMultipleAttempts = 308,

	VRInitError_Compositor_Failed					= 400,
	VRInitError_Compositor_D3D11HardwareRequired	= 401,
	VRInitError_Compositor_FirmwareRequiresUpdate	= 402,
	VRInitError_Compositor_OverlayInitFailed		= 403,
	VRInitError_Compositor_ScreenshotsInitFailed	= 404,

	VRInitError_VendorSpecific_UnableToConnectToOculusRuntime = 1000,

	VRInitError_VendorSpecific_HmdFound_CantOpenDevice 				= 1101,
	VRInitError_VendorSpecific_HmdFound_UnableToRequestConfigStart	= 1102,
	VRInitError_VendorSpecific_HmdFound_NoStoredConfig 				= 1103,
	VRInitError_VendorSpecific_HmdFound_ConfigTooBig 				= 1104,
	VRInitError_VendorSpecific_HmdFound_ConfigTooSmall 				= 1105,
	VRInitError_VendorSpecific_HmdFound_UnableToInitZLib 			= 1106,
	VRInitError_VendorSpecific_HmdFound_CantReadFirmwareVersion 	= 1107,
	VRInitError_VendorSpecific_HmdFound_UnableToSendUserDataStart	= 1108,
	VRInitError_VendorSpecific_HmdFound_UnableToGetUserDataStart	= 1109,
	VRInitError_VendorSpecific_HmdFound_UnableToGetUserDataNext		= 1110,
	VRInitError_VendorSpecific_HmdFound_UserDataAddressRange		= 1111,
	VRInitError_VendorSpecific_HmdFound_UserDataError				= 1112,
	VRInitError_VendorSpecific_HmdFound_ConfigFailedSanityCheck		= 1113,

	VRInitError_Steam_SteamInstallationNotFound = 2000,
};

enum EVRScreenshotType
{
	VRScreenshotType_None = 0,
	VRScreenshotType_Mono = 1, // left eye only
	VRScreenshotType_Stereo = 2,
	VRScreenshotType_Cubemap = 3,
	VRScreenshotType_MonoPanorama = 4,
	VRScreenshotType_StereoPanorama = 5
};

enum EVRScreenshotPropertyFilenames
{
	VRScreenshotPropertyFilenames_Preview = 0,
	VRScreenshotPropertyFilenames_VR = 1,
};

enum EVRTrackedCameraError
{
	VRTrackedCameraError_None                       = 0,
	VRTrackedCameraError_OperationFailed            = 100,
	VRTrackedCameraError_InvalidHandle              = 101,	
	VRTrackedCameraError_InvalidFrameHeaderVersion  = 102,
	VRTrackedCameraError_OutOfHandles               = 103,
	VRTrackedCameraError_IPCFailure                 = 104,
	VRTrackedCameraError_NotSupportedForThisDevice  = 105,
	VRTrackedCameraError_SharedMemoryFailure        = 106,
	VRTrackedCameraError_FrameBufferingFailure      = 107,
	VRTrackedCameraError_StreamSetupFailure         = 108,
	VRTrackedCameraError_InvalidGLTextureId         = 109,
	VRTrackedCameraError_InvalidSharedTextureHandle = 110,
	VRTrackedCameraError_FailedToGetGLTextureId     = 111,
	VRTrackedCameraError_SharedTextureFailure       = 112,
	VRTrackedCameraError_NoFrameAvailable           = 113,
	VRTrackedCameraError_InvalidArgument            = 114,
	VRTrackedCameraError_InvalidFrameBufferSize     = 115,
};

enum EVRTrackedCameraFrameType
{
	VRTrackedCameraFrameType_Distorted = 0,			// This is the camera video frame size in pixels, still distorted.
	VRTrackedCameraFrameType_Undistorted,			// In pixels, an undistorted inscribed rectangle region without invalid regions. This size is subject to changes shortly.
	VRTrackedCameraFrameType_MaximumUndistorted,	// In pixels, maximum undistorted with invalid regions. Non zero alpha component identifies valid regions.
	MAX_CAMERA_FRAME_TYPES
};

typedef uint64_t TrackedCameraHandle_t;
#define INVALID_TRACKED_CAMERA_HANDLE	((vr::TrackedCameraHandle_t)0)

struct CameraVideoStreamFrameHeader_t
{
	EVRTrackedCameraFrameType eFrameType;

	uint32_t nWidth;
	uint32_t nHeight;
	uint32_t nBytesPerPixel;

	uint32_t nFrameSequence;

	TrackedDevicePose_t standingTrackedDevicePose;
};

// Screenshot types
typedef uint32_t ScreenshotHandle_t;

static const uint32_t k_unScreenshotHandleInvalid = 0;

#pragma pack( pop )

// figure out how to import from the VR API dll
#if defined(_WIN32)

#ifdef VR_API_EXPORT
#define VR_INTERFACE extern "C" __declspec( dllexport )
#else
#define VR_INTERFACE extern "C" __declspec( dllimport )
#endif

#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)

#ifdef VR_API_EXPORT
#define VR_INTERFACE extern "C" __attribute__((visibility("default")))
#else
#define VR_INTERFACE extern "C" 
#endif

#else
#error "Unsupported Platform."
#endif


#if defined( _WIN32 )
#define VR_CALLTYPE __cdecl
#else
#define VR_CALLTYPE 
#endif

} // namespace vr

#endif // _INCLUDE_VRTYPES_H


// vrannotation.h
#ifdef API_GEN
# define VR_CLANG_ATTR(ATTR) __attribute__((annotate( ATTR )))
#else
# define VR_CLANG_ATTR(ATTR)
#endif

#define VR_METHOD_DESC(DESC) VR_CLANG_ATTR( "desc:" #DESC ";" )
#define VR_IGNOREATTR() VR_CLANG_ATTR( "ignore" )
#define VR_OUT_STRUCT() VR_CLANG_ATTR( "out_struct: ;" )
#define VR_OUT_STRING() VR_CLANG_ATTR( "out_string: ;" )
#define VR_OUT_ARRAY_CALL(COUNTER,FUNCTION,PARAMS) VR_CLANG_ATTR( "out_array_call:" #COUNTER "," #FUNCTION "," #PARAMS ";" )
#define VR_OUT_ARRAY_COUNT(COUNTER) VR_CLANG_ATTR( "out_array_count:" #COUNTER ";" )
#define VR_ARRAY_COUNT(COUNTER) VR_CLANG_ATTR( "array_count:" #COUNTER ";" )
#define VR_ARRAY_COUNT_D(COUNTER, DESC) VR_CLANG_ATTR( "array_count:" #COUNTER ";desc:" #DESC )
#define VR_BUFFER_COUNT(COUNTER) VR_CLANG_ATTR( "buffer_count:" #COUNTER ";" )
#define VR_OUT_BUFFER_COUNT(COUNTER) VR_CLANG_ATTR( "out_buffer_count:" #COUNTER ";" )
#define VR_OUT_STRING_COUNT(COUNTER) VR_CLANG_ATTR( "out_string_count:" #COUNTER ";" )

// vrtrackedcameratypes.h
#ifndef _VRTRACKEDCAMERATYPES_H
#define _VRTRACKEDCAMERATYPES_H 

namespace vr
{

#if defined(__linux__) || defined(__APPLE__) 
	// The 32-bit version of gcc has the alignment requirement for uint64 and double set to
	// 4 meaning that even with #pragma pack(8) these types will only be four-byte aligned.
	// The 64-bit version of gcc has the alignment requirement for these types set to
	// 8 meaning that unless we use #pragma pack(4) our structures will get bigger.
	// The 64-bit structure packing has to match the 32-bit structure packing for each platform.
	#pragma pack( push, 4 )
#else
	#pragma pack( push, 8 )
#endif

enum ECameraVideoStreamFormat
{
	CVS_FORMAT_UNKNOWN = 0,
	CVS_FORMAT_RAW10 = 1,		// 10 bits per pixel
	CVS_FORMAT_NV12 = 2,		// 12 bits per pixel
	CVS_FORMAT_RGB24 = 3,		// 24 bits per pixel
	CVS_MAX_FORMATS
};

enum ECameraCompatibilityMode
{
	CAMERA_COMPAT_MODE_BULK_DEFAULT = 0,
	CAMERA_COMPAT_MODE_BULK_64K_DMA,
	CAMERA_COMPAT_MODE_BULK_16K_DMA,
	CAMERA_COMPAT_MODE_BULK_8K_DMA,
	CAMERA_COMPAT_MODE_ISO_52FPS,
	CAMERA_COMPAT_MODE_ISO_50FPS,
	CAMERA_COMPAT_MODE_ISO_48FPS,
	CAMERA_COMPAT_MODE_ISO_46FPS,
	CAMERA_COMPAT_MODE_ISO_44FPS,
	CAMERA_COMPAT_MODE_ISO_42FPS,
	CAMERA_COMPAT_MODE_ISO_40FPS,
	CAMERA_COMPAT_MODE_ISO_35FPS,
	CAMERA_COMPAT_MODE_ISO_30FPS,
	MAX_CAMERA_COMPAT_MODES
};

#ifdef _MSC_VER
#define VR_CAMERA_DECL_ALIGN( x ) __declspec( align( x ) )
#else
#define VR_CAMERA_DECL_ALIGN( x ) //
#endif

#define MAX_CAMERA_FRAME_SHARED_HANDLES	4

VR_CAMERA_DECL_ALIGN( 8 ) struct CameraVideoStreamFrame_t
{
	ECameraVideoStreamFormat m_nStreamFormat;

	uint32_t m_nWidth;
	uint32_t m_nHeight;

	uint32_t m_nImageDataSize;						// Based on stream format, width, height

	uint32_t m_nFrameSequence;						// Starts from 0 when stream starts.

	uint32_t m_nBufferIndex;						// Identifies which buffer the image data is hosted
	uint32_t m_nBufferCount;						// Total number of configured buffers

	uint32_t m_nExposureTime;

	uint32_t m_nISPFrameTimeStamp;					// Driver provided time stamp per driver centric time base
	uint32_t m_nISPReferenceTimeStamp;
	uint32_t m_nSyncCounter;

	uint32_t m_nCamSyncEvents;
	uint32_t m_nISPSyncEvents;

	double m_flReferenceCamSyncTime;

	double m_flFrameElapsedTime;					// Starts from 0 when stream starts. In seconds.
	double m_flFrameDeliveryRate;

	double m_flFrameCaptureTime_DriverAbsolute;		// In USB time, via AuxEvent
	double m_flFrameCaptureTime_ServerRelative;		// In System time within the server
	uint64_t m_nFrameCaptureTicks_ServerAbsolute;	// In system ticks within the server
	double m_flFrameCaptureTime_ClientRelative;		// At the client, relative to when the frame was exposed/captured.

	double m_flSyncMarkerError;

	TrackedDevicePose_t m_StandingTrackedDevicePose;	// Supplied by HMD layer when used as a tracked camera

	uint64_t m_pImageData;
};

#pragma pack( pop )

}

#endif // _VRTRACKEDCAMERATYPES_H
// ivrsettings.h
namespace vr
{
	enum EVRSettingsError
	{
		VRSettingsError_None = 0,
		VRSettingsError_IPCFailed = 1,
		VRSettingsError_WriteFailed = 2,
		VRSettingsError_ReadFailed = 3,
	};

	// The maximum length of a settings key
	static const uint32_t k_unMaxSettingsKeyLength = 128;

	class IVRSettings
	{
	public:
		virtual const char *GetSettingsErrorNameFromEnum( EVRSettingsError eError ) = 0;

		// Returns true if file sync occurred (force or settings dirty)
		virtual bool Sync( bool bForce = false, EVRSettingsError *peError = nullptr ) = 0;

		virtual bool GetBool( const char *pchSection, const char *pchSettingsKey, bool bDefaultValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual void SetBool( const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual int32_t GetInt32( const char *pchSection, const char *pchSettingsKey, int32_t nDefaultValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual void SetInt32( const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual float GetFloat( const char *pchSection, const char *pchSettingsKey, float flDefaultValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual void SetFloat( const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual void GetString( const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, const char *pchDefaultValue, EVRSettingsError *peError = nullptr ) = 0;
		virtual void SetString( const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError = nullptr ) = 0;
		
		virtual void RemoveSection( const char *pchSection, EVRSettingsError *peError = nullptr ) = 0;
		virtual void RemoveKeyInSection( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr ) = 0;
	};

	//-----------------------------------------------------------------------------
	static const char * const IVRSettings_Version = "IVRSettings_001";

	//-----------------------------------------------------------------------------
	// steamvr keys

	static const char * const k_pch_SteamVR_Section = "steamvr";
	static const char * const k_pch_SteamVR_RequireHmd_String = "requireHmd";
	static const char * const k_pch_SteamVR_ForcedDriverKey_String = "forcedDriver";
	static const char * const k_pch_SteamVR_ForcedHmdKey_String = "forcedHmd";
	static const char * const k_pch_SteamVR_DisplayDebug_Bool = "displayDebug";
	static const char * const k_pch_SteamVR_DebugProcessPipe_String = "debugProcessPipe";
	static const char * const k_pch_SteamVR_EnableDistortion_Bool = "enableDistortion";
	static const char * const k_pch_SteamVR_DisplayDebugX_Int32 = "displayDebugX";
	static const char * const k_pch_SteamVR_DisplayDebugY_Int32 = "displayDebugY";
	static const char * const k_pch_SteamVR_SendSystemButtonToAllApps_Bool= "sendSystemButtonToAllApps";
	static const char * const k_pch_SteamVR_LogLevel_Int32 = "loglevel";
	static const char * const k_pch_SteamVR_IPD_Float = "ipd";
	static const char * const k_pch_SteamVR_Background_String = "background";
	static const char * const k_pch_SteamVR_BackgroundCameraHeight_Float = "backgroundCameraHeight";
	static const char * const k_pch_SteamVR_BackgroundDomeRadius_Float = "backgroundDomeRadius";
	static const char * const k_pch_SteamVR_Environment_String = "environment";
	static const char * const k_pch_SteamVR_GridColor_String = "gridColor";
	static const char * const k_pch_SteamVR_PlayAreaColor_String = "playAreaColor";
	static const char * const k_pch_SteamVR_ShowStage_Bool = "showStage";
	static const char * const k_pch_SteamVR_ActivateMultipleDrivers_Bool = "activateMultipleDrivers";
	static const char * const k_pch_SteamVR_PowerOffOnExit_Bool = "powerOffOnExit";
	static const char * const k_pch_SteamVR_StandbyAppRunningTimeout_Float = "standbyAppRunningTimeout";
	static const char * const k_pch_SteamVR_StandbyNoAppTimeout_Float = "standbyNoAppTimeout";
	static const char * const k_pch_SteamVR_DirectMode_Bool = "directMode";
	static const char * const k_pch_SteamVR_DirectModeEdidVid_Int32 = "directModeEdidVid";
	static const char * const k_pch_SteamVR_DirectModeEdidPid_Int32 = "directModeEdidPid";
	static const char * const k_pch_SteamVR_UsingSpeakers_Bool = "usingSpeakers";
	static const char * const k_pch_SteamVR_SpeakersForwardYawOffsetDegrees_Float = "speakersForwardYawOffsetDegrees";
	static const char * const k_pch_SteamVR_BaseStationPowerManagement_Bool = "basestationPowerManagement";
	static const char * const k_pch_SteamVR_NeverKillProcesses_Bool = "neverKillProcesses";
	static const char * const k_pch_SteamVR_RenderTargetMultiplier_Float = "renderTargetMultiplier";
	static const char * const k_pch_SteamVR_AllowReprojection_Bool = "allowReprojection";
	static const char * const k_pch_SteamVR_ForceReprojection_Bool = "forceReprojection";
	static const char * const k_pch_SteamVR_ForceFadeOnBadTracking_Bool = "forceFadeOnBadTracking";
	static const char * const k_pch_SteamVR_DefaultMirrorView_Int32 = "defaultMirrorView";
	static const char * const k_pch_SteamVR_ShowMirrorView_Bool = "showMirrorView";
	static const char * const k_pch_SteamVR_StartMonitorFromAppLaunch = "startMonitorFromAppLaunch";
	static const char * const k_pch_SteamVR_AutoLaunchSteamVROnButtonPress = "autoLaunchSteamVROnButtonPress";
	static const char * const k_pch_SteamVR_UseGenericGraphcisDevice_Bool = "useGenericGraphicsDevice";


	//-----------------------------------------------------------------------------
	// lighthouse keys

	static const char * const k_pch_Lighthouse_Section = "driver_lighthouse";
	static const char * const k_pch_Lighthouse_DisableIMU_Bool = "disableimu";
	static const char * const k_pch_Lighthouse_UseDisambiguation_String = "usedisambiguation";
	static const char * const k_pch_Lighthouse_DisambiguationDebug_Int32 = "disambiguationdebug";

	static const char * const k_pch_Lighthouse_PrimaryBasestation_Int32 = "primarybasestation";
	static const char * const k_pch_Lighthouse_LighthouseName_String = "lighthousename";
	static const char * const k_pch_Lighthouse_MaxIncidenceAngleDegrees_Float = "maxincidenceangledegrees";
	static const char * const k_pch_Lighthouse_UseLighthouseDirect_Bool = "uselighthousedirect";
	static const char * const k_pch_Lighthouse_DBHistory_Bool = "dbhistory";

	//-----------------------------------------------------------------------------
	// null keys

	static const char * const k_pch_Null_Section = "driver_null";
	static const char * const k_pch_Null_EnableNullDriver_Bool = "enable";
	static const char * const k_pch_Null_SerialNumber_String = "serialNumber";
	static const char * const k_pch_Null_ModelNumber_String = "modelNumber";
	static const char * const k_pch_Null_WindowX_Int32 = "windowX";
	static const char * const k_pch_Null_WindowY_Int32 = "windowY";
	static const char * const k_pch_Null_WindowWidth_Int32 = "windowWidth";
	static const char * const k_pch_Null_WindowHeight_Int32 = "windowHeight";
	static const char * const k_pch_Null_RenderWidth_Int32 = "renderWidth";
	static const char * const k_pch_Null_RenderHeight_Int32 = "renderHeight";
	static const char * const k_pch_Null_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
	static const char * const k_pch_Null_DisplayFrequency_Float = "displayFrequency";

	//-----------------------------------------------------------------------------
	// user interface keys
	static const char * const k_pch_UserInterface_Section = "userinterface";
	static const char * const k_pch_UserInterface_StatusAlwaysOnTop_Bool = "StatusAlwaysOnTop";
	static const char * const k_pch_UserInterface_Screenshots_Bool = "screenshots";
	static const char * const k_pch_UserInterface_ScreenshotType_Int = "screenshotType";

	//-----------------------------------------------------------------------------
	// notification keys
	static const char * const k_pch_Notifications_Section = "notifications";
	static const char * const k_pch_Notifications_DoNotDisturb_Bool = "DoNotDisturb";

	//-----------------------------------------------------------------------------
	// keyboard keys
	static const char * const k_pch_Keyboard_Section = "keyboard";
	static const char * const k_pch_Keyboard_TutorialCompletions = "TutorialCompletions";
	static const char * const k_pch_Keyboard_ScaleX = "ScaleX";
	static const char * const k_pch_Keyboard_ScaleY = "ScaleY";
	static const char * const k_pch_Keyboard_OffsetLeftX = "OffsetLeftX";
	static const char * const k_pch_Keyboard_OffsetRightX = "OffsetRightX";
	static const char * const k_pch_Keyboard_OffsetY = "OffsetY";
	static const char * const k_pch_Keyboard_Smoothing = "Smoothing";

	//-----------------------------------------------------------------------------
	// perf keys
	static const char * const k_pch_Perf_Section = "perfcheck";
	static const char * const k_pch_Perf_HeuristicActive_Bool = "heuristicActive";
	static const char * const k_pch_Perf_NotifyInHMD_Bool = "warnInHMD";
	static const char * const k_pch_Perf_NotifyOnlyOnce_Bool = "warnOnlyOnce";
	static const char * const k_pch_Perf_AllowTimingStore_Bool = "allowTimingStore";
	static const char * const k_pch_Perf_SaveTimingsOnExit_Bool = "saveTimingsOnExit";
	static const char * const k_pch_Perf_TestData_Float = "perfTestData";

	//-----------------------------------------------------------------------------
	// collision bounds keys
	static const char * const k_pch_CollisionBounds_Section = "collisionBounds";
	static const char * const k_pch_CollisionBounds_Style_Int32 = "CollisionBoundsStyle";
	static const char * const k_pch_CollisionBounds_GroundPerimeterOn_Bool = "CollisionBoundsGroundPerimeterOn";
	static const char * const k_pch_CollisionBounds_CenterMarkerOn_Bool = "CollisionBoundsCenterMarkerOn";
	static const char * const k_pch_CollisionBounds_PlaySpaceOn_Bool = "CollisionBoundsPlaySpaceOn";
	static const char * const k_pch_CollisionBounds_FadeDistance_Float = "CollisionBoundsFadeDistance";
	static const char * const k_pch_CollisionBounds_ColorGammaR_Int32 = "CollisionBoundsColorGammaR";
	static const char * const k_pch_CollisionBounds_ColorGammaG_Int32 = "CollisionBoundsColorGammaG";
	static const char * const k_pch_CollisionBounds_ColorGammaB_Int32 = "CollisionBoundsColorGammaB";
	static const char * const k_pch_CollisionBounds_ColorGammaA_Int32 = "CollisionBoundsColorGammaA";

	//-----------------------------------------------------------------------------
	// camera keys
	static const char * const k_pch_Camera_Section = "camera";
	static const char * const k_pch_Camera_EnableCamera_Bool = "enableCamera";
	static const char * const k_pch_Camera_EnableCameraInDashboard_Bool = "enableCameraInDashboard";
	static const char * const k_pch_Camera_EnableCameraForCollisionBounds_Bool = "enableCameraForCollisionBounds";
	static const char * const k_pch_Camera_EnableCameraForRoomView_Bool = "enableCameraForRoomView";
	static const char * const k_pch_Camera_BoundsColorGammaR_Int32 = "cameraBoundsColorGammaR";
	static const char * const k_pch_Camera_BoundsColorGammaG_Int32 = "cameraBoundsColorGammaG";
	static const char * const k_pch_Camera_BoundsColorGammaB_Int32 = "cameraBoundsColorGammaB";
	static const char * const k_pch_Camera_BoundsColorGammaA_Int32 = "cameraBoundsColorGammaA";

	//-----------------------------------------------------------------------------
	// audio keys
	static const char * const k_pch_audio_Section = "audio";
	static const char * const k_pch_audio_OnPlaybackDevice_String = "onPlaybackDevice";
	static const char * const k_pch_audio_OnRecordDevice_String = "onRecordDevice";
	static const char * const k_pch_audio_OnPlaybackMirrorDevice_String = "onPlaybackMirrorDevice";
	static const char * const k_pch_audio_OffPlaybackDevice_String = "offPlaybackDevice";
	static const char * const k_pch_audio_OffRecordDevice_String = "offRecordDevice";
	static const char * const k_pch_audio_VIVEHDMIGain = "viveHDMIGain";

	//-----------------------------------------------------------------------------
	// model skin keys
	static const char * const k_pch_modelskin_Section = "modelskins";

} // namespace vr

// iservertrackeddevicedriver.h
namespace vr
{


struct DriverPoseQuaternion_t
{
	double w, x, y, z;
};

struct DriverPose_t
{
	/* Time offset of this pose, in seconds from the actual time of the pose,
	 * relative to the time of the PoseUpdated() call made by the driver.
	 */
	double poseTimeOffset;

	/* Generally, the pose maintained by a driver
	 * is in an inertial coordinate system different
	 * from the world system of x+ right, y+ up, z+ back.
	 * Also, the driver is not usually tracking the "head" position,
	 * but instead an internal IMU or another reference point in the HMD.
	 * The following two transforms transform positions and orientations
	 * to app world space from driver world space,
	 * and to HMD head space from driver local body space. 
	 *
	 * We maintain the driver pose state in its internal coordinate system,
	 * so we can do the pose prediction math without having to
	 * use angular acceleration.  A driver's angular acceleration is generally not measured,
	 * and is instead calculated from successive samples of angular velocity.
	 * This leads to a noisy angular acceleration values, which are also
	 * lagged due to the filtering required to reduce noise to an acceptable level.
	 */
	vr::HmdQuaternion_t qWorldFromDriverRotation;
	double vecWorldFromDriverTranslation[ 3 ];

	vr::HmdQuaternion_t qDriverFromHeadRotation;
	double vecDriverFromHeadTranslation[ 3 ];

	/* State of driver pose, in meters and radians. */
	/* Position of the driver tracking reference in driver world space
	* +[0] (x) is right
	* +[1] (y) is up
	* -[2] (z) is forward
	*/
	double vecPosition[ 3 ];

	/* Velocity of the pose in meters/second */
	double vecVelocity[ 3 ];

	/* Acceleration of the pose in meters/second */
	double vecAcceleration[ 3 ];

	/* Orientation of the tracker, represented as a quaternion */
	vr::HmdQuaternion_t qRotation;

	/* Angular velocity of the pose in axis-angle 
	* representation. The direction is the angle of
	* rotation and the magnitude is the angle around
	* that axis in radians/second. */
	double vecAngularVelocity[ 3 ];

	/* Angular acceleration of the pose in axis-angle 
	* representation. The direction is the angle of
	* rotation and the magnitude is the angle around
	* that axis in radians/second^2. */
	double vecAngularAcceleration[ 3 ];

	ETrackingResult result;

	bool poseIsValid;
	bool willDriftInYaw;
	bool shouldApplyHeadModel;
	bool deviceIsConnected;
};


// ----------------------------------------------------------------------------------------------
// Purpose: Represents a single tracked device in a driver
// ----------------------------------------------------------------------------------------------
class ITrackedDeviceServerDriver
{
public:

	// ------------------------------------
	// Management Methods
	// ------------------------------------
	/** This is called before an HMD is returned to the application. It will always be
	* called before any display or tracking methods. Memory and processor use by the
	* ITrackedDeviceServerDriver object should be kept to a minimum until it is activated.
	* The pose listener is guaranteed to be valid until Deactivate is called, but
	* should not be used after that point. */
	virtual EVRInitError Activate( uint32_t unObjectId ) = 0;

	/** This is called when The VR system is switching from this Hmd being the active display
	* to another Hmd being the active display. The driver should clean whatever memory
	* and thread use it can when it is deactivated */
	virtual void Deactivate() = 0;

	/** Handles a request from the system to power off this device */
	virtual void PowerOff() = 0;

	/** Requests a component interface of the driver for device-specific functionality. The driver should return NULL
	* if the requested interface or version is not supported. */
	virtual void *GetComponent( const char *pchComponentNameAndVersion ) = 0;

	/** A VR Client has made this debug request of the driver. The set of valid requests is entirely
	* up to the driver and the client to figure out, as is the format of the response. Responses that
	* exceed the length of the supplied buffer should be truncated and null terminated */
	virtual void DebugRequest( const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize ) = 0;

	// ------------------------------------
	// Tracking Methods
	// ------------------------------------
	virtual DriverPose_t GetPose() = 0;

	// ------------------------------------
	// Property Methods
	// ------------------------------------

	/** Returns a bool property. If the property is not available this function will return false. */
	virtual bool GetBoolTrackedDeviceProperty( ETrackedDeviceProperty prop, ETrackedPropertyError *pError ) = 0;

	/** Returns a float property. If the property is not available this function will return 0. */
	virtual float GetFloatTrackedDeviceProperty( ETrackedDeviceProperty prop, ETrackedPropertyError *pError ) = 0;

	/** Returns an int property. If the property is not available this function will return 0. */
	virtual int32_t GetInt32TrackedDeviceProperty( ETrackedDeviceProperty prop, ETrackedPropertyError *pError ) = 0;

	/** Returns a uint64 property. If the property is not available this function will return 0. */
	virtual uint64_t GetUint64TrackedDeviceProperty( ETrackedDeviceProperty prop, ETrackedPropertyError *pError ) = 0;

	/** Returns a matrix property. If the device index is not valid or the property is not a matrix type, this function will return identity. */
	virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty( ETrackedDeviceProperty prop, ETrackedPropertyError *pError ) = 0;

	/** Returns a string property. If the property is not available this function will return 0 and pError will be 
	* set to an error. Otherwise it returns the length of the number of bytes necessary to hold this string including 
	* the trailing null. If the buffer is too small the error will be TrackedProp_BufferTooSmall. Strings will 
	* generally fit in buffers of k_unTrackingStringSize characters. Drivers may not return strings longer than 
	* k_unMaxPropertyStringSize. */
	virtual uint32_t GetStringTrackedDeviceProperty( ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError ) = 0;

};



static const char *ITrackedDeviceServerDriver_Version = "ITrackedDeviceServerDriver_004";

}
// ivrdisplaycomponent.h
namespace vr
{


	// ----------------------------------------------------------------------------------------------
	// Purpose: The display component on a single tracked device
	// ----------------------------------------------------------------------------------------------
	class IVRDisplayComponent
	{
	public:

		// ------------------------------------
		// Display Methods
		// ------------------------------------

		/** Size and position that the window needs to be on the VR display. */
		virtual void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) = 0;

		/** Returns true if the display is extending the desktop. */
		virtual bool IsDisplayOnDesktop( ) = 0;

		/** Returns true if the display is real and not a fictional display. */
		virtual bool IsDisplayRealDisplay( ) = 0;

		/** Suggested size for the intermediate render target that the distortion pulls from. */
		virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) = 0;

		/** Gets the viewport in the frame buffer to draw the output of the distortion into */
		virtual void GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) = 0;

		/** The components necessary to build your own projection matrix in case your
		* application is doing something fancy like infinite Z */
		virtual void GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) = 0;

		/** Returns the result of the distortion function for the specified eye and input UVs. UVs go from 0,0 in
		* the upper left of that eye's viewport and 1,1 in the lower right of that eye's viewport. */
		virtual DistortionCoordinates_t ComputeDistortion( EVREye eEye, float fU, float fV ) = 0;

	};

	static const char *IVRDisplayComponent_Version = "IVRDisplayComponent_002";

}

// ivrdriverdirectmodecomponent.h
namespace vr
{


	// ----------------------------------------------------------------------------------------------
	// Purpose: This component is used for drivers that implement direct mode entirely on their own
	//			without allowing the VR Compositor to own the window/device. Chances are you don't
	//			need to implement this component in your driver.
	// ----------------------------------------------------------------------------------------------
	class IVRDriverDirectModeComponent
	{
	public:

		// -----------------------------------
		// Direct mode methods
		// -----------------------------------

		/** Specific to Oculus compositor support, textures supplied must be created using this method. */
		virtual void CreateSwapTextureSet( uint32_t unPid, uint32_t unFormat, uint32_t unWidth, uint32_t unHeight, void *(*pSharedTextureHandles)[3] ) {}

		/** Used to textures created using CreateSwapTextureSet.  Only one of the set's handles needs to be used to destroy the entire set. */
		virtual void DestroySwapTextureSet( void *pSharedTextureHandle ) {}

		/** Used to purge all texture sets for a given process. */
		virtual void DestroyAllSwapTextureSets( uint32_t unPid ) {}

		/** After Present returns, calls this to get the next index to use for rendering. */
		virtual void GetNextSwapTextureSetIndex( void *pSharedTextureHandles[ 2 ], uint32_t( *pIndices )[ 2 ] ) {}

		/** Call once per layer to draw for this frame.  One shared texture handle per eye.  Textures must be created
		* using CreateSwapTextureSet and should be alternated per frame.  Call Present once all layers have been submitted. */
		virtual void SubmitLayer( void *pSharedTextureHandles[ 2 ], const vr::VRTextureBounds_t( &bounds )[ 2 ], const vr::HmdMatrix34_t *pPose ) {}

		/** Submits queued layers for display. */
		virtual void Present( void *hSyncTexture ) {}

	};

	static const char *IVRDriverDirectModeComponent_Version = "IVRDriverDirectModeComponent_001";

}

// ivrcontrollercomponent.h
namespace vr
{


	// ----------------------------------------------------------------------------------------------
	// Purpose: Controller access on a single tracked device.
	// ----------------------------------------------------------------------------------------------
	class IVRControllerComponent
	{
	public:

		// ------------------------------------
		// Controller Methods
		// ------------------------------------

		/** Gets the current state of a controller. */
		virtual VRControllerState_t GetControllerState( ) = 0;

		/** Returns a uint64 property. If the property is not available this function will return 0. */
		virtual bool TriggerHapticPulse( uint32_t unAxisId, uint16_t usPulseDurationMicroseconds ) = 0;

	};



	static const char *IVRControllerComponent_Version = "IVRControllerComponent_001";

}
// ivrcameracomponent.h
namespace vr
{
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	class ICameraVideoSinkCallback
	{
	public:
		virtual void OnCameraVideoSinkCallback() = 0;
	};

	// ----------------------------------------------------------------------------------------------
	// Purpose: The camera on a single tracked device
	// ----------------------------------------------------------------------------------------------
	class IVRCameraComponent
	{
	public:
		// ------------------------------------
		// Camera Methods
		// ------------------------------------
		virtual bool GetCameraFrameDimensions( vr::ECameraVideoStreamFormat nVideoStreamFormat, uint32_t *pWidth, uint32_t *pHeight ) = 0;
		virtual bool GetCameraFrameBufferingRequirements( int *pDefaultFrameQueueSize, uint32_t *pFrameBufferDataSize ) = 0;
		virtual bool SetCameraFrameBuffering( int nFrameBufferCount, void **ppFrameBuffers, uint32_t nFrameBufferDataSize ) = 0;
		virtual bool SetCameraVideoStreamFormat( vr::ECameraVideoStreamFormat nVideoStreamFormat ) = 0;
		virtual vr::ECameraVideoStreamFormat GetCameraVideoStreamFormat() = 0;
		virtual bool StartVideoStream() = 0;
		virtual void StopVideoStream() = 0;
		virtual bool IsVideoStreamActive( bool *pbPaused, float *pflElapsedTime ) = 0;
		virtual const vr::CameraVideoStreamFrame_t *GetVideoStreamFrame() = 0;
		virtual void ReleaseVideoStreamFrame( const vr::CameraVideoStreamFrame_t *pFrameImage ) = 0;
		virtual bool SetAutoExposure( bool bEnable ) = 0;
		virtual bool PauseVideoStream() = 0;
		virtual bool ResumeVideoStream() = 0;
		virtual bool GetCameraDistortion( float flInputU, float flInputV, float *pflOutputU, float *pflOutputV ) = 0;
		virtual bool GetCameraProjection( vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection ) = 0;
		virtual bool SetFrameRate( int nISPFrameRate, int nSensorFrameRate ) = 0;
		virtual bool SetCameraVideoSinkCallback( vr::ICameraVideoSinkCallback *pCameraVideoSinkCallback ) = 0;
		virtual bool GetCameraCompatibilityMode( vr::ECameraCompatibilityMode *pCameraCompatibilityMode ) = 0;
		virtual bool SetCameraCompatibilityMode( vr::ECameraCompatibilityMode nCameraCompatibilityMode ) = 0;
		virtual bool GetCameraFrameBounds( vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pLeft, uint32_t *pTop, uint32_t *pWidth, uint32_t *pHeight ) = 0;
		virtual bool GetCameraIntrinsics( vr::EVRTrackedCameraFrameType eFrameType, HmdVector2_t *pFocalLength, HmdVector2_t *pCenter ) = 0;
	};

	static const char *IVRCameraComponent_Version = "IVRCameraComponent_002";
}
// itrackeddevicedriverprovider.h
namespace vr
{

class ITrackedDeviceServerDriver;
struct TrackedDeviceDriverInfo_t;
struct DriverPose_t;

class IDriverLog
{
public:
	/** Writes a log message to the log file prefixed with the driver name */
	virtual void Log( const char *pchLogMessage ) = 0;
};

/** This interface is provided by vrserver to allow the driver to notify 
* the system when something changes about a device. These changes must
* not change the serial number or class of the device because those values
* are permanently associated with the device's index. */
class IServerDriverHost
{
public:
	/** Notifies the server that a tracked device has been added. If this function returns true
	* the server will call Activate on the device. If it returns false some kind of error
	* has occurred and the device will not be activated. */
	virtual bool TrackedDeviceAdded( const char *pchDeviceSerialNumber ) = 0;

	/** Notifies the server that a tracked device's pose has been updated */
	virtual void TrackedDevicePoseUpdated( uint32_t unWhichDevice, const DriverPose_t & newPose ) = 0;

	/** Notifies the server that the property cache for the specified device should be invalidated */
	virtual void TrackedDevicePropertiesChanged( uint32_t unWhichDevice ) = 0;

	/** Notifies the server that vsync has occurred on the the display attached to the device. This is
	* only permitted on devices of the HMD class. */
	virtual void VsyncEvent( double vsyncTimeOffsetSeconds ) = 0;

	/** notifies the server that the button was pressed */
	virtual void TrackedDeviceButtonPressed( uint32_t unWhichDevice, EVRButtonId eButtonId, double eventTimeOffset ) = 0;

	/** notifies the server that the button was unpressed */
	virtual void TrackedDeviceButtonUnpressed( uint32_t unWhichDevice, EVRButtonId eButtonId, double eventTimeOffset ) = 0;

	/** notifies the server that the button was pressed */
	virtual void TrackedDeviceButtonTouched( uint32_t unWhichDevice, EVRButtonId eButtonId, double eventTimeOffset ) = 0;

	/** notifies the server that the button was unpressed */
	virtual void TrackedDeviceButtonUntouched( uint32_t unWhichDevice, EVRButtonId eButtonId, double eventTimeOffset ) = 0;

	/** notifies the server than a controller axis changed */
	virtual void TrackedDeviceAxisUpdated( uint32_t unWhichDevice, uint32_t unWhichAxis, const VRControllerAxis_t & axisState ) = 0;

	/** Notifies the server that the MC image has been updated for the display attached to the device. This is
	* only permitted on devices of the HMD class. */
	virtual void MCImageUpdated() = 0;
	
	/** always returns a pointer to a valid interface pointer of IVRSettings */
	virtual IVRSettings *GetSettings( const char *pchInterfaceVersion ) = 0;

	/** Notifies the server that the physical IPD adjustment has been moved on the HMD */
	virtual void PhysicalIpdSet( uint32_t unWhichDevice, float fPhysicalIpdMeters ) = 0;

	/** Notifies the server that the proximity sensor on the specified device  */
	virtual void ProximitySensorState( uint32_t unWhichDevice, bool bProximitySensorTriggered ) = 0;

	/** Sends a vendor specific event (VREvent_VendorSpecific_Reserved_Start..VREvent_VendorSpecific_Reserved_End */
	virtual void VendorSpecificEvent( uint32_t unWhichDevice, vr::EVREventType eventType, const VREvent_Data_t & eventData, double eventTimeOffset ) = 0;

	/** Returns true if SteamVR is exiting */
	virtual bool IsExiting() = 0;
};


/** This interface must be implemented in each driver. It will be loaded in vrserver.exe */
class IServerTrackedDeviceProvider
{
public:
	/** initializes the driver. This will be called before any other methods are called.
	* If Init returns anything other than VRInitError_None the driver DLL will be unloaded.
	*
	* pDriverHost will never be NULL, and will always be a pointer to a IServerDriverHost interface
	*
	* pchUserDriverConfigDir - The absolute path of the directory where the driver should store user
	*	config files.
	* pchDriverInstallDir - The absolute path of the root directory for the driver.
	*/
	virtual EVRInitError Init( IDriverLog *pDriverLog, vr::IServerDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir ) = 0;

	/** cleans up the driver right before it is unloaded */
	virtual void Cleanup() = 0;

	/** Returns the version of the ITrackedDeviceServerDriver interface used by this driver */
	virtual const char * const *GetInterfaceVersions() = 0;

	/** returns the number of HMDs that this driver manages that are physically connected. */
	virtual uint32_t GetTrackedDeviceCount() = 0;

	/** returns a single HMD */
	virtual ITrackedDeviceServerDriver *GetTrackedDeviceDriver( uint32_t unWhich ) = 0;

	/** returns a single HMD by ID */
	virtual ITrackedDeviceServerDriver* FindTrackedDeviceDriver( const char *pchId ) = 0;

	/** Allows the driver do to some work in the main loop of the server. */
	virtual void RunFrame() = 0;


	// ------------  Power State Functions ----------------------- //

	/** Returns true if the driver wants to block Standby mode. */
	virtual bool ShouldBlockStandbyMode() = 0;

	/** Called when the system is entering Standby mode. The driver should switch itself into whatever sort of low-power
	* state it has. */
	virtual void EnterStandby() = 0;

	/** Called when the system is leaving Standby mode. The driver should switch itself back to
	full operation. */
	virtual void LeaveStandby() = 0;

};


static const char *IServerTrackedDeviceProvider_Version = "IServerTrackedDeviceProvider_003";


/** This interface is provided by vrclient to allow the driver call back and query various information */
class IClientDriverHost
{
public:
	/** Returns the device class of a tracked device. If there has not been a device connected in this slot
	* since the application started this function will return TrackedDevice_Invalid. For previous detected
	* devices the function will return the previously observed device class. 
	*
	* To determine which devices exist on the system, just loop from 0 to k_unMaxTrackedDeviceCount and check
	* the device class. Every device with something other than TrackedDevice_Invalid is associated with an 
	* actual tracked device. */
	virtual ETrackedDeviceClass GetTrackedDeviceClass( vr::TrackedDeviceIndex_t unDeviceIndex ) = 0;

	/** Returns true if there is a device connected in this slot. */
	virtual bool IsTrackedDeviceConnected( vr::TrackedDeviceIndex_t unDeviceIndex ) = 0;

	/** Returns a bool property. If the device index is not valid or the property is not a bool type this function will return false. */
	virtual bool GetBoolTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;

	/** Returns a float property. If the device index is not valid or the property is not a float type this function will return 0. */
	virtual float GetFloatTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;

	/** Returns an int property. If the device index is not valid or the property is not a int type this function will return 0. */
	virtual int32_t GetInt32TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;

	/** Returns a uint64 property. If the device index is not valid or the property is not a uint64 type this function will return 0. */
	virtual uint64_t GetUint64TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;

	/** Returns a string property. If the device index is not valid or the property is not a float type this function will 
	* return 0. Otherwise it returns the length of the number of bytes necessary to hold this string including the trailing
	* null. Strings will generally fit in buffers of k_unTrackingStringSize characters. */
	virtual uint32_t GetStringTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L ) = 0;

	/** always returns a pointer to a valid interface pointer of IVRSettings */
	virtual IVRSettings *GetSettings( const char *pchInterfaceVersion ) = 0; 

	/** Client drivers in watchdog mode should call this when they have received a signal from hardware that should
	* cause SteamVR to start */
	virtual void WatchdogWakeUp() = 0;
};


/** Defines the mode that the client driver should start in. */
enum EClientDriverMode
{
	ClientDriverMode_Normal = 0,
	ClientDriverMode_Watchdog = 1, // client should return VRInitError_Init_LowPowerWatchdogNotSupported if it can't support this mode
};


/** This interface must be implemented in each driver. It will be loaded in vrclient.dll */
class IClientTrackedDeviceProvider
{
public:
	/** initializes the driver. This will be called before any other methods are called,
	* except BIsHmdPresent(). BIsHmdPresent is called outside of the Init/Cleanup pair.
	* If Init returns anything other than VRInitError_None the driver DLL will be unloaded.
	*
	* pDriverHost will never be NULL, and will always be a pointer to a IClientDriverHost interface
	*
	* pchUserDriverConfigDir - The absolute path of the directory where the driver should store user
	*	config files.
	* pchDriverInstallDir - The absolute path of the root directory for the driver.
	*/
	virtual EVRInitError Init( EClientDriverMode eDriverMode, IDriverLog *pDriverLog, vr::IClientDriverHost *pDriverHost, const char *pchUserDriverConfigDir, const char *pchDriverInstallDir ) = 0;

	/** cleans up the driver right before it is unloaded */
	virtual void Cleanup() = 0;

	/** Called when the client needs to inform an application if an HMD is attached that uses
	* this driver. This method should be as lightweight as possible and should have no side effects
	* such as hooking process functions or leaving resources loaded. Init will not be called before 
	* this method and Cleanup will not be called after it.
	*/
	virtual bool BIsHmdPresent( const char *pchUserConfigDir ) = 0;

	/** called when the client inits an HMD to let the client driver know which one is in use */
	virtual EVRInitError SetDisplayId( const char *pchDisplayId ) = 0;

	/** Returns the stencil mesh information for the current HMD. If this HMD does not have a stencil mesh the vertex data and count will be
	* NULL and 0 respectively. This mesh is meant to be rendered into the stencil buffer (or into the depth buffer setting nearz) before rendering
	* each eye's view. The pixels covered by this mesh will never be seen by the user after the lens distortion is applied and based on visibility to the panels.
	* This will improve perf by letting the GPU early-reject pixels the user will never see before running the pixel shader.
	* NOTE: Render this mesh with backface culling disabled since the winding order of the vertices can be different per-HMD or per-eye.
	*/
	virtual HiddenAreaMesh_t GetHiddenAreaMesh( EVREye eEye ) = 0;

	/** Get the MC image for the current HMD.
	* Returns the size in bytes of the buffer required to hold the specified resource. */
	virtual uint32_t GetMCImage( uint32_t *pImgWidth, uint32_t *pImgHeight, uint32_t *pChannels, void *pDataBuffer, uint32_t unBufferLen ) = 0;
};

static const char *IClientTrackedDeviceProvider_Version = "IClientTrackedDeviceProvider_004";

}




namespace vr
{
	static const char * const k_InterfaceVersions[] =
	{
		IVRSettings_Version,
		ITrackedDeviceServerDriver_Version,
		IVRDisplayComponent_Version,
		IVRDriverDirectModeComponent_Version,
		IVRControllerComponent_Version,
		IVRCameraComponent_Version,
		IServerTrackedDeviceProvider_Version,
		IClientTrackedDeviceProvider_Version,
		nullptr
	};
}
// End

#endif // _OPENVR_DRIVER_API


