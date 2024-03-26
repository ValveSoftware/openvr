import sys
import api_shared
calculateinteropmethodname = api_shared.calculateinteropmethodname

print ("""//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: This file contains C#/managed code bindings for the OpenVR interfaces
// This file is auto-generated, do not edit it.
//
//=============================================================================
#if !OPENVR_XR_API

using System;
using System.Runtime.InteropServices;
using Valve.VR;

#if UNITY_5_3_OR_NEWER
using UnityEngine;
#endif

namespace Valve.VR
{
""")

if len(sys.argv) != 2:
	sys.exit(-1);
json_path = sys.argv[1]

data = api_shared.loadfile(json_path, 'vr')
converttype = api_shared.converttype
structlist = api_shared.structlist
typedeflist = api_shared.typedeflist
enumlist = api_shared.enumlist
lastmethod = ''


############## define Csharp interfaces ######################

api_shared.outputinterfaces('vr', data)

############ define csharp classes

api_shared.outputclasses('vr', data)

print("}\n\n");

# Output the OpenVRInterop class 
print('public class OpenVRInterop\n{')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_InitInternal", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern uint InitInternal(ref EVRInitError peError, EVRApplicationType eApplicationType);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_InitInternal2", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern uint InitInternal2(ref EVRInitError peError, EVRApplicationType eApplicationType,[In, MarshalAs(UnmanagedType.LPStr)] string pStartupInfo);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_ShutdownInternal", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern void ShutdownInternal();')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_IsHmdPresent", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern bool IsHmdPresent();')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_IsRuntimeInstalled", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern bool IsRuntimeInstalled();')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_RuntimePath", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern string RuntimePath();')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_GetRuntimePath", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern bool GetRuntimePath(System.Text.StringBuilder pchPathBuffer, uint unBufferSize, ref uint punRequiredBufferSize);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_GetStringForHmdError", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern IntPtr GetStringForHmdError(EVRInitError error);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_GetGenericInterface", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern IntPtr GetGenericInterface([In, MarshalAs(UnmanagedType.LPStr)] string pchInterfaceVersion, ref EVRInitError peError);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_IsInterfaceVersionValid", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern bool IsInterfaceVersionValid([In, MarshalAs(UnmanagedType.LPStr)] string pchInterfaceVersion);')
print('[DllImportAttribute("openvr_api", EntryPoint = "VR_GetInitToken", CallingConvention = CallingConvention.Cdecl)]')
print('internal static extern uint GetInitToken();')
print("}\n\n");

api_shared.outputenums('vr', data)

print("""
	[StructLayout(LayoutKind.Explicit)] public struct VREvent_Data_t
	{
		[FieldOffset(0)] public VREvent_Reserved_t reserved;
		[FieldOffset(0)] public VREvent_Controller_t controller;
		[FieldOffset(0)] public VREvent_Mouse_t mouse;
		[FieldOffset(0)] public VREvent_Scroll_t scroll;
		[FieldOffset(0)] public VREvent_Process_t process;
		[FieldOffset(0)] public VREvent_Notification_t notification;
		[FieldOffset(0)] public VREvent_Overlay_t overlay;
		[FieldOffset(0)] public VREvent_Status_t status;
		[FieldOffset(0)] public VREvent_Ipd_t ipd;
		[FieldOffset(0)] public VREvent_Chaperone_t chaperone;
		[FieldOffset(0)] public VREvent_PerformanceTest_t performanceTest;
		[FieldOffset(0)] public VREvent_TouchPadMove_t touchPadMove;
		[FieldOffset(0)] public VREvent_SeatedZeroPoseReset_t seatedZeroPoseReset;
		[FieldOffset(0)] public VREvent_Screenshot_t screenshot;
		[FieldOffset(0)] public VREvent_ScreenshotProgress_t screenshotProgress;
		[FieldOffset(0)] public VREvent_ApplicationLaunch_t applicationLaunch;
		[FieldOffset(0)] public VREvent_EditingCameraSurface_t cameraSurface;
		[FieldOffset(0)] public VREvent_MessageOverlay_t messageOverlay;
		[FieldOffset(0)] public VREvent_Property_t property;
		[FieldOffset(0)] public VREvent_HapticVibration_t hapticVibration;
		[FieldOffset(0)] public VREvent_WebConsole_t webConsole;
		[FieldOffset(0)] public VREvent_InputBindingLoad_t inputBinding;
		[FieldOffset(0)] public VREvent_SpatialAnchor_t spatialAnchor;
		[FieldOffset(0)] public VREvent_InputActionManifestLoad_t actionManifest;
		[FieldOffset(0)] public VREvent_ProgressUpdate_t progressUpdate;
		[FieldOffset(0)] public VREvent_ShowUI_t showUi;
		[FieldOffset(0)] public VREvent_ShowDevTools_t showDevTools;
		[FieldOffset(0)] public VREvent_HDCPError_t hdcpError;
		[FieldOffset(0)] public VREvent_AudioVolumeControl_t audioVolumeControl;
		[FieldOffset(0)] public VREvent_AudioMuteControl_t audioMuteControl;
		[FieldOffset(0)] public VREvent_Keyboard_t keyboard; // This has to be at the end due to a mono bug
	}
	""")

print("""
	[StructLayout(LayoutKind.Explicit)] public struct VROverlayIntersectionMaskPrimitive_Data_t
	{
		[FieldOffset(0)] public IntersectionMaskRectangle_t m_Rectangle;
		[FieldOffset(0)] public IntersectionMaskCircle_t m_Circle;
	}
	""")

api_shared.outputstructs('vr', data)


# Output the OpenVR class
print('\npublic class OpenVR\n{')
print("""
	public static uint InitInternal(ref EVRInitError peError, EVRApplicationType eApplicationType)
	{
		return OpenVRInterop.InitInternal(ref peError, eApplicationType);
	}

	public static uint InitInternal2(ref EVRInitError peError, EVRApplicationType eApplicationType, string pchStartupInfo)
	{
		return OpenVRInterop.InitInternal2(ref peError, eApplicationType, pchStartupInfo);
	}

	public static void ShutdownInternal()
	{
		OpenVRInterop.ShutdownInternal();
	}

	public static bool IsHmdPresent()
	{
		return OpenVRInterop.IsHmdPresent();
	}

	public static bool IsRuntimeInstalled()
	{
		return OpenVRInterop.IsRuntimeInstalled();
	}
	
    public static string RuntimePath()
	{
        try
        {
            uint pathSize = 512;
            uint requiredPathSize = 512;
            System.Text.StringBuilder path = new System.Text.StringBuilder((int)pathSize);
            bool success = OpenVRInterop.GetRuntimePath(path, pathSize, ref requiredPathSize);
            if (success == false)
            {
                return null;
            }

            return path.ToString();
        } catch
        {
            return OpenVRInterop.RuntimePath(); //this api is deprecated but here to support older unity versions
        }
	}

	public static string GetStringForHmdError(EVRInitError error)
	{
		return Marshal.PtrToStringAnsi(OpenVRInterop.GetStringForHmdError(error));
	}

	public static IntPtr GetGenericInterface(string pchInterfaceVersion, ref EVRInitError peError)
	{
		return OpenVRInterop.GetGenericInterface(pchInterfaceVersion, ref peError);
	}

	public static bool IsInterfaceVersionValid(string pchInterfaceVersion)
	{
		return OpenVRInterop.IsInterfaceVersionValid(pchInterfaceVersion);
	}

	public static uint GetInitToken()
	{
		return OpenVRInterop.GetInitToken();
	}
""")

########
# Output constants into OpenVR class

for const in data['consts']:
	if(len(const) > 0):
		consttype = converttype(const['consttype'])
		constval = const['constval']
		if(consttype == 'string'):
			constval = '\"' + constval + '\"'
		print('public const '+consttype+' '+const['constname']+' = '+constval+';')

########
# Output static constructors into OpenVR class
#lastclass = ''
#lastmethod = ''
#for method in data['methods']:
#	if (len(method) > 0):
#		returntype = converttype(method['returntype'])
#
#		methodname = method['methodname']
#		if(methodname == lastmethod):
#			methodname = methodname + `count`
#			count = count + 1
#		else:
#			count = 0
#		lastmethod = method['methodname']
#
#		interfacename = method['classname']
#		namespace = api_shared.getnamespace(interfacename)
#		if(interfacename.find('::') == -1):
#			classname= 'C' + interfacename[1:]
#		else:
#			interfacename = interfacename[interfacename.find('::') + 2:]
#			classname = 'C' + interfacename[1:]
#		if(namespace != ''):
#			if(interfacename != lastclass):
#				shortname = interfacename[1:]
#				print('public static '+interfacename+' '+shortname+'()\n{\nreturn new '+classname+'(OpenVRInterop.'+shortname+'());\n}\n')
#				lastclass = interfacename

########
# Output interface wrappers into OpenVR class

print("""
	static uint VRToken { get; set; }

	const string FnTable_Prefix = "FnTable:";

	class COpenVRContext
	{
		public COpenVRContext() { Clear(); }

		public void Clear()
		{
			m_pVRSystem = null;
			m_pVRChaperone = null;
			m_pVRChaperoneSetup = null;
			m_pVRCompositor = null;
			m_pVRHeadsetView = null;
			m_pVROverlay = null;
			m_pVROverlayView = null;
			m_pVRRenderModels = null;
			m_pVRExtendedDisplay = null;
			m_pVRSettings = null;
			m_pVRApplications = null;
			m_pVRScreenshots = null;
			m_pVRTrackedCamera = null;
			m_pVRInput = null;
			m_pVRIOBuffer = null;
			m_pVRSpatialAnchors = null;
			m_pVRNotifications = null;
			m_pVRDebug = null;
		}

		void CheckClear()
		{
			if (VRToken != GetInitToken())
			{
				Clear();
				VRToken = GetInitToken();
			}
		}

		public CVRSystem VRSystem()
		{
			CheckClear();
			if (m_pVRSystem == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRSystem_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRSystem = new CVRSystem(pInterface);
			}
			return m_pVRSystem;
		}

		public CVRChaperone VRChaperone()
		{
			CheckClear();
			if (m_pVRChaperone == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRChaperone_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRChaperone = new CVRChaperone(pInterface);
			}
			return m_pVRChaperone;
		}

		public CVRChaperoneSetup VRChaperoneSetup()
		{
			CheckClear();
			if (m_pVRChaperoneSetup == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRChaperoneSetup_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRChaperoneSetup = new CVRChaperoneSetup(pInterface);
			}
			return m_pVRChaperoneSetup;
		}

		public CVRCompositor VRCompositor()
		{
			CheckClear();
			if (m_pVRCompositor == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRCompositor_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRCompositor = new CVRCompositor(pInterface);
			}
			return m_pVRCompositor;
		}

		public CVRHeadsetView VRHeadsetView()
		{
			CheckClear();
			if (m_pVRHeadsetView == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRHeadsetView_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRHeadsetView = new CVRHeadsetView(pInterface);
			}
			return m_pVRHeadsetView;
		}

		public CVROverlay VROverlay()
		{
			CheckClear();
			if (m_pVROverlay == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVROverlay_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVROverlay = new CVROverlay(pInterface);
			}
			return m_pVROverlay;
		}

		public CVROverlayView VROverlayView()
		{
			CheckClear();
			if (m_pVROverlayView == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVROverlayView_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVROverlayView = new CVROverlayView(pInterface);
			}
			return m_pVROverlayView;
		}

		public CVRRenderModels VRRenderModels()
		{
			CheckClear();
			if (m_pVRRenderModels == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRRenderModels_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRRenderModels = new CVRRenderModels(pInterface);
			}
			return m_pVRRenderModels;
		}

		public CVRExtendedDisplay VRExtendedDisplay()
		{
			CheckClear();
			if (m_pVRExtendedDisplay == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRExtendedDisplay_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRExtendedDisplay = new CVRExtendedDisplay(pInterface);
			}
			return m_pVRExtendedDisplay;
		}

		public CVRSettings VRSettings()
		{
			CheckClear();
			if (m_pVRSettings == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRSettings_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRSettings = new CVRSettings(pInterface);
			}
			return m_pVRSettings;
		}

		public CVRApplications VRApplications()
		{
			CheckClear();
			if (m_pVRApplications == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRApplications_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRApplications = new CVRApplications(pInterface);
			}
			return m_pVRApplications;
		}

		public CVRScreenshots VRScreenshots()
		{
			CheckClear();
			if (m_pVRScreenshots == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRScreenshots_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRScreenshots = new CVRScreenshots(pInterface);
			}
			return m_pVRScreenshots;
		}

		public CVRTrackedCamera VRTrackedCamera()
		{
			CheckClear();
			if (m_pVRTrackedCamera == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRTrackedCamera_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRTrackedCamera = new CVRTrackedCamera(pInterface);
			}
			return m_pVRTrackedCamera;
		}

		public CVRInput VRInput()
		{
			CheckClear();
			if (m_pVRInput == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix+IVRInput_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
					m_pVRInput = new CVRInput(pInterface);
			}
			return m_pVRInput;
		}

		public CVRIOBuffer VRIOBuffer()
		{
			CheckClear();
			if (m_pVRIOBuffer == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix + IVRIOBuffer_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
						m_pVRIOBuffer = new CVRIOBuffer(pInterface);
			}
			return m_pVRIOBuffer;
		}

		public CVRSpatialAnchors VRSpatialAnchors()
		{
			CheckClear();
			if (m_pVRSpatialAnchors == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix + IVRSpatialAnchors_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
						m_pVRSpatialAnchors = new CVRSpatialAnchors(pInterface);
			}
			return m_pVRSpatialAnchors;
		}

		public CVRDebug VRDebug()
		{
			CheckClear();
			if (m_pVRDebug == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix + IVRDebug_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
						m_pVRDebug = new CVRDebug(pInterface);
			}
			return m_pVRDebug;
		}

		public CVRNotifications VRNotifications()
		{
			CheckClear();
			if (m_pVRNotifications == null)
			{
				var eError = EVRInitError.None;
				var pInterface = OpenVRInterop.GetGenericInterface(FnTable_Prefix + IVRNotifications_Version, ref eError);
				if (pInterface != IntPtr.Zero && eError == EVRInitError.None)
						m_pVRNotifications = new CVRNotifications(pInterface);
			}
			return m_pVRNotifications;
		}

		private CVRSystem m_pVRSystem;
		private CVRChaperone m_pVRChaperone;
		private CVRChaperoneSetup m_pVRChaperoneSetup;
		private CVRCompositor m_pVRCompositor;
		private CVRHeadsetView m_pVRHeadsetView;
		private CVROverlay m_pVROverlay;
		private CVROverlayView m_pVROverlayView;
		private CVRRenderModels m_pVRRenderModels;
		private CVRExtendedDisplay m_pVRExtendedDisplay;
		private CVRSettings m_pVRSettings;
		private CVRApplications m_pVRApplications;
		private CVRScreenshots m_pVRScreenshots;
		private CVRTrackedCamera m_pVRTrackedCamera;
		private CVRInput m_pVRInput;
		private CVRIOBuffer m_pVRIOBuffer;
		private CVRSpatialAnchors m_pVRSpatialAnchors;
		private CVRNotifications m_pVRNotifications;
		private CVRDebug m_pVRDebug;
	};

	private static COpenVRContext _OpenVRInternal_ModuleContext = null;
	static COpenVRContext OpenVRInternal_ModuleContext
	{
		get
		{
			if (_OpenVRInternal_ModuleContext == null)
				_OpenVRInternal_ModuleContext = new COpenVRContext();
			return _OpenVRInternal_ModuleContext;
		}
	}

	public static CVRSystem System { get { return OpenVRInternal_ModuleContext.VRSystem(); } }
	public static CVRChaperone Chaperone { get { return OpenVRInternal_ModuleContext.VRChaperone(); } }
	public static CVRChaperoneSetup ChaperoneSetup { get { return OpenVRInternal_ModuleContext.VRChaperoneSetup(); } }
	public static CVRCompositor Compositor { get { return OpenVRInternal_ModuleContext.VRCompositor(); } }
	public static CVRHeadsetView HeadsetView { get { return OpenVRInternal_ModuleContext.VRHeadsetView(); } }
	public static CVROverlay Overlay { get { return OpenVRInternal_ModuleContext.VROverlay(); } }
	public static CVROverlayView OverlayView { get { return OpenVRInternal_ModuleContext.VROverlayView(); } }
	public static CVRRenderModels RenderModels { get { return OpenVRInternal_ModuleContext.VRRenderModels(); } }
	public static CVRExtendedDisplay ExtendedDisplay { get { return OpenVRInternal_ModuleContext.VRExtendedDisplay(); } }
	public static CVRSettings Settings { get { return OpenVRInternal_ModuleContext.VRSettings(); } }
	public static CVRApplications Applications { get { return OpenVRInternal_ModuleContext.VRApplications(); } }
	public static CVRScreenshots Screenshots { get { return OpenVRInternal_ModuleContext.VRScreenshots(); } }
	public static CVRTrackedCamera TrackedCamera { get { return OpenVRInternal_ModuleContext.VRTrackedCamera(); } }
	public static CVRInput Input { get { return OpenVRInternal_ModuleContext.VRInput(); } }
	public static CVRIOBuffer IOBuffer { get { return OpenVRInternal_ModuleContext.VRIOBuffer(); } }
	public static CVRSpatialAnchors SpatialAnchors { get { return OpenVRInternal_ModuleContext.VRSpatialAnchors(); } }
	public static CVRNotifications Notifications { get { return OpenVRInternal_ModuleContext.VRNotifications(); } }
	public static CVRDebug Debug { get { return OpenVRInternal_ModuleContext.VRDebug(); } }


	/** Finds the active installation of vrclient.dll and initializes it */
	public static CVRSystem Init(ref EVRInitError peError, EVRApplicationType eApplicationType = EVRApplicationType.VRApplication_Scene, string pchStartupInfo= "")
	{
		try 
		{
			VRToken = InitInternal2(ref peError, eApplicationType, pchStartupInfo);
		} 
		catch (EntryPointNotFoundException)
		{
			VRToken = InitInternal(ref peError, eApplicationType);
		}

		OpenVRInternal_ModuleContext.Clear();

		if (peError != EVRInitError.None)
			return null;

		bool bInterfaceValid = IsInterfaceVersionValid(IVRSystem_Version);
		if (!bInterfaceValid)
		{
			ShutdownInternal();
			peError = EVRInitError.Init_InterfaceNotFound;
			return null;
		}

		return OpenVR.System;
	}

	/** unloads vrclient.dll. Any interface pointers from the interface are
	* invalid after this point */
	public static void Shutdown()
	{
		ShutdownInternal();
	}
""")

print("}\n\n");

print ("""
}
#endif
""")
