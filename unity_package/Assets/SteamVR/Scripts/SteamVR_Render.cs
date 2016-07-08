﻿//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: Handles rendering of all SteamVR_Cameras
//
//=============================================================================

using UnityEngine;
using System.Collections;
using Valve.VR;

public class SteamVR_Render : MonoBehaviour
{
	public bool pauseGameWhenDashboardIsVisible = true;
	public bool lockPhysicsUpdateRateToRenderFrequency = true;

	public SteamVR_ExternalCamera externalCamera;
	public string externalCameraConfigPath = "externalcamera.cfg";

#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
	public LayerMask leftMask, rightMask;

	SteamVR_CameraMask cameraMask;
#endif
	public ETrackingUniverseOrigin trackingSpace = ETrackingUniverseOrigin.TrackingUniverseStanding;

	static public EVREye eye { get; private set; }

	static private SteamVR_Render _instance;
	static public SteamVR_Render instance
	{
		get
		{
			if (_instance == null)
			{
				_instance = GameObject.FindObjectOfType<SteamVR_Render>();

				if (_instance == null)
					_instance = new GameObject("[SteamVR]").AddComponent<SteamVR_Render>();
			}
			return _instance;
		}
	}

	void OnDestroy()
	{
		_instance = null;
	}

	static private bool isQuitting;
	void OnApplicationQuit()
	{
		isQuitting = true;
		SteamVR.SafeDispose();
	}

	static public void Add(SteamVR_Camera vrcam)
	{
		if (!isQuitting)
			instance.AddInternal(vrcam);
	}

	static public void Remove(SteamVR_Camera vrcam)
	{
		if (!isQuitting && _instance != null)
			instance.RemoveInternal(vrcam);
	}

	static public SteamVR_Camera Top()
	{
		if (!isQuitting)
			return instance.TopInternal();

		return null;
	}

	private SteamVR_Camera[] cameras = new SteamVR_Camera[0];

	void AddInternal(SteamVR_Camera vrcam)
	{
		var camera = vrcam.GetComponent<Camera>();
		var length = cameras.Length;
		var sorted = new SteamVR_Camera[length + 1];
		int insert = 0;
		for (int i = 0; i < length; i++)
		{
			var c = cameras[i].GetComponent<Camera>();
			if (i == insert && c.depth > camera.depth)
				sorted[insert++] = vrcam;

			sorted[insert++] = cameras[i];
		}
		if (insert == length)
			sorted[insert] = vrcam;

		cameras = sorted;

#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		enabled = true;
#endif
	}

	void RemoveInternal(SteamVR_Camera vrcam)
	{
		var length = cameras.Length;
		int count = 0;
		for (int i = 0; i < length; i++)
		{
			var c = cameras[i];
			if (c == vrcam)
				++count;
		}
		if (count == 0)
			return;

		var sorted = new SteamVR_Camera[length - count];
		int insert = 0;
		for (int i = 0; i < length; i++)
		{
			var c = cameras[i];
			if (c != vrcam)
				sorted[insert++] = c;
		}

		cameras = sorted;
	}

	SteamVR_Camera TopInternal()
	{
		if (cameras.Length > 0)
			return cameras[cameras.Length - 1];

		return null;
	}

	public TrackedDevicePose_t[] poses = new TrackedDevicePose_t[OpenVR.k_unMaxTrackedDeviceCount];
	public TrackedDevicePose_t[] gamePoses = new TrackedDevicePose_t[0];

	static private bool _pauseRendering;
	static public bool pauseRendering
	{
		get { return _pauseRendering; }
		set
		{
			_pauseRendering = value;
#if !(UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
			var compositor = OpenVR.Compositor;
			if (compositor != null)
				compositor.SuspendRendering(value);
#endif
		}
	}

	private IEnumerator RenderLoop()
	{
		while (true)
		{
			yield return new WaitForEndOfFrame();

			if (pauseRendering)
				continue;

			var compositor = OpenVR.Compositor;
			if (compositor != null)
			{
				if (!compositor.CanRenderScene())
					continue;

				compositor.SetTrackingSpace(trackingSpace);

#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
				SteamVR_Utils.QueueEventOnRenderThread(SteamVR.Unity.k_nRenderEventID_WaitGetPoses);

				// Hack to flush render event that was queued in Update (this ensures WaitGetPoses has returned before we grab the new values).
				SteamVR.Unity.EventWriteString("[UnityMain] GetNativeTexturePtr - Begin");
				SteamVR_Camera.GetSceneTexture(cameras[0].GetComponent<Camera>().hdr).GetNativeTexturePtr();
				SteamVR.Unity.EventWriteString("[UnityMain] GetNativeTexturePtr - End");

				compositor.GetLastPoses(poses, gamePoses);
				SteamVR_Utils.Event.Send("new_poses", poses);
				SteamVR_Utils.Event.Send("new_poses_applied");
#endif
			}

			var overlay = SteamVR_Overlay.instance;
			if (overlay != null)
				overlay.UpdateOverlay();

			RenderExternalCamera();

#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
			var vr = SteamVR.instance;
			RenderEye(vr, EVREye.Eye_Left);
			RenderEye(vr, EVREye.Eye_Right);

			// Move cameras back to head position so they can be tracked reliably
			foreach (var c in cameras)
			{
				c.transform.localPosition = Vector3.zero;
				c.transform.localRotation = Quaternion.identity;
			}

			if (cameraMask != null)
				cameraMask.Clear();
#endif
		}
	}

#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
	void RenderEye(SteamVR vr, EVREye eye)
	{
		int i = (int)eye;
		SteamVR_Render.eye = eye;

		if (cameraMask != null)
			cameraMask.Set(vr, eye);

		foreach (var c in cameras)
		{
			c.transform.localPosition = vr.eyes[i].pos;
			c.transform.localRotation = vr.eyes[i].rot;

			// Update position to keep from getting culled
			cameraMask.transform.position = c.transform.position;

			var camera = c.GetComponent<Camera>();
			camera.targetTexture = SteamVR_Camera.GetSceneTexture(camera.hdr);
			int cullingMask = camera.cullingMask;
			if (eye == EVREye.Eye_Left)
			{
				camera.cullingMask &= ~rightMask;
				camera.cullingMask |= leftMask;
			}
			else
			{
				camera.cullingMask &= ~leftMask;
				camera.cullingMask |= rightMask;
			}
			camera.Render();
			camera.cullingMask = cullingMask;
		}
	}
#endif

	void RenderExternalCamera()
	{
		if (externalCamera == null)
			return;

		if (!externalCamera.gameObject.activeInHierarchy)
			return;

		var frameSkip = (int)Mathf.Max(externalCamera.config.frameSkip, 0.0f);
		if (Time.frameCount % (frameSkip + 1) != 0)
			return;

		// Keep external camera relative to the most relevant vr camera.
		externalCamera.AttachToCamera(TopInternal());

		externalCamera.RenderNear();
		externalCamera.RenderFar();
	}

	float sceneResolutionScale = 1.0f, timeScale = 1.0f;

    private void OnInputFocus(params object[] args)
    {
        bool hasFocus = (bool)args[0];
		if (hasFocus)
		{
			if (pauseGameWhenDashboardIsVisible)
			{
				Time.timeScale = timeScale;
			}

			SteamVR_Camera.sceneResolutionScale = sceneResolutionScale;
		}
		else
		{
			if (pauseGameWhenDashboardIsVisible)
			{
				timeScale = Time.timeScale;
				Time.timeScale = 0.0f;
			}

			sceneResolutionScale = SteamVR_Camera.sceneResolutionScale;
			SteamVR_Camera.sceneResolutionScale = 0.5f;
		}
	}

	void OnQuit(params object[] args)
	{
#if UNITY_EDITOR
		foreach (System.Reflection.Assembly a in System.AppDomain.CurrentDomain.GetAssemblies())
		{
			var t = a.GetType("UnityEditor.EditorApplication");
			if (t != null)
			{
				t.GetProperty("isPlaying").SetValue(null, false, null);
				break;
			}
		}
#else
		Application.Quit();
#endif
	}

    private string GetScreenshotFilename(uint screenshotHandle, EVRScreenshotPropertyFilenames screenshotPropertyFilename)
    {
        var error = EVRScreenshotError.None;
        var capacity = OpenVR.Screenshots.GetScreenshotPropertyFilename(screenshotHandle, screenshotPropertyFilename, null, 0, ref error);
        if (error != EVRScreenshotError.None && error != EVRScreenshotError.BufferTooSmall )
            return null;
        if (capacity > 1)
        {
            var result = new System.Text.StringBuilder((int)capacity);
            OpenVR.Screenshots.GetScreenshotPropertyFilename(screenshotHandle, screenshotPropertyFilename, result, capacity, ref error);
            if (error != EVRScreenshotError.None)
                return null;
            return result.ToString();
        }
        return null;
    }

    private void OnRequestScreenshot(params object[] args)
    {
        var vrEvent = (VREvent_t)args[0];
        var screenshotHandle = vrEvent.data.screenshot.handle;
        var screenshotType = (EVRScreenshotType)vrEvent.data.screenshot.type;

        if ( screenshotType == EVRScreenshotType.StereoPanorama )
        {
            string previewFilename = GetScreenshotFilename(screenshotHandle, EVRScreenshotPropertyFilenames.Preview);
            string VRFilename = GetScreenshotFilename(screenshotHandle, EVRScreenshotPropertyFilenames.VR);

            if (previewFilename == null || VRFilename == null)
                return;

            // Do the stereo panorama screenshot
            // Figure out where the view is
            GameObject screenshotPosition = new GameObject("screenshotPosition");
            screenshotPosition.transform.position = SteamVR_Render.Top().transform.position;
            screenshotPosition.transform.rotation = SteamVR_Render.Top().transform.rotation;
            screenshotPosition.transform.localScale = SteamVR_Render.Top().transform.lossyScale;
            SteamVR_Utils.TakeStereoScreenshot(screenshotHandle, screenshotPosition, 32, 0.064f, ref previewFilename, ref VRFilename);

            // and submit it
            OpenVR.Screenshots.SubmitScreenshot(screenshotHandle, screenshotType, previewFilename, VRFilename);
        }
    }

    void OnEnable()
	{
        StartCoroutine("RenderLoop");
		SteamVR_Utils.Event.Listen("input_focus", OnInputFocus);
		SteamVR_Utils.Event.Listen("Quit", OnQuit);
        SteamVR_Utils.Event.Listen("RequestScreenshot", OnRequestScreenshot);

        var vr = SteamVR.instance;
        if (vr == null)
        {
            enabled = false;
            return;
        }
        var types = new EVRScreenshotType[] { EVRScreenshotType.StereoPanorama };
        OpenVR.Screenshots.HookScreenshot(types);
	}

	void OnDisable()
	{
		StopAllCoroutines();
        SteamVR_Utils.Event.Remove("RequestScreenshot", OnRequestScreenshot);
        SteamVR_Utils.Event.Remove("input_focus", OnInputFocus);
		SteamVR_Utils.Event.Remove("Quit", OnQuit);
	}

	void Awake()
	{
#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		var go = new GameObject("cameraMask");
		go.transform.parent = transform;
		cameraMask = go.AddComponent<SteamVR_CameraMask>();
#endif
		if (externalCamera == null && System.IO.File.Exists(externalCameraConfigPath))
		{
			var prefab = Resources.Load<GameObject>("SteamVR_ExternalCamera");
			var instance = Instantiate(prefab);
			instance.gameObject.name = "External Camera";

			externalCamera = instance.transform.GetChild(0).GetComponent<SteamVR_ExternalCamera>();
			externalCamera.configPath = externalCameraConfigPath;
			externalCamera.ReadConfig();
		}
	}

	void FixedUpdate()
	{
#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		// We want to call this as soon after Present as possible.
		SteamVR_Utils.QueueEventOnRenderThread(SteamVR.Unity.k_nRenderEventID_PostPresentHandoff);
#endif
	}

#if !(UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
	private SteamVR_UpdatePoses poseUpdater;
#endif

	void Update()
	{
#if !(UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		if (poseUpdater == null)
		{
			var go = new GameObject("poseUpdater");
			go.transform.parent = transform;
			poseUpdater = go.AddComponent<SteamVR_UpdatePoses>();
		}
#else
		if (cameras.Length == 0)
		{
			enabled = false;
			return;
		}

		// If our FixedUpdate rate doesn't match our render framerate, then catch the handoff here.
		SteamVR_Utils.QueueEventOnRenderThread(SteamVR.Unity.k_nRenderEventID_PostPresentHandoff);
#endif
		// Force controller update in case no one else called this frame to ensure prevState gets updated.
		SteamVR_Controller.Update();

		// Dispatch any OpenVR events.
		var system = OpenVR.System;
		if (system != null)
		{
			var vrEvent = new VREvent_t();
			var size = (uint)System.Runtime.InteropServices.Marshal.SizeOf(typeof(VREvent_t));
			for (int i = 0; i < 64; i++)
			{
				if (!system.PollNextEvent(ref vrEvent, size))
					break;

				switch ((EVREventType)vrEvent.eventType)
				{
					case EVREventType.VREvent_InputFocusCaptured: // another app has taken focus (likely dashboard)
						if (vrEvent.data.process.oldPid == 0)
						{
							SteamVR_Utils.Event.Send("input_focus", false);
						}
						break;
					case EVREventType.VREvent_InputFocusReleased: // that app has released input focus
						if (vrEvent.data.process.pid == 0)
						{
							SteamVR_Utils.Event.Send("input_focus", true);
						}
						break;
					case EVREventType.VREvent_ShowRenderModels:
						SteamVR_Utils.Event.Send("hide_render_models", false);
						break;
					case EVREventType.VREvent_HideRenderModels:
						SteamVR_Utils.Event.Send("hide_render_models", true);
						break;
					default:
						var name = System.Enum.GetName(typeof(EVREventType), vrEvent.eventType);
						if (name != null)
							SteamVR_Utils.Event.Send(name.Substring(8) /*strip VREvent_*/, vrEvent);
						break;
				}
			}
		}

		// Ensure various settings to minimize latency.
		Application.targetFrameRate = -1;
		Application.runInBackground = true; // don't require companion window focus
		QualitySettings.maxQueuedFrames = -1;
		QualitySettings.vSyncCount = 0; // this applies to the companion window

		if (lockPhysicsUpdateRateToRenderFrequency && Time.timeScale > 0.0f)
		{
			var vr = SteamVR.instance;
			if (vr != null)
			{
				var timing = new Compositor_FrameTiming();
				timing.m_nSize = (uint)System.Runtime.InteropServices.Marshal.SizeOf(typeof(Compositor_FrameTiming));
				vr.compositor.GetFrameTiming(ref timing, 0);

				Time.fixedDeltaTime = Time.timeScale / vr.hmd_DisplayFrequency;
			}
		}
	}
}

