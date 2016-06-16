//========= Copyright 2016, Valve Corporation, All rights reserved. ===========
//
// Purpose: Used to render an external camera of vr player (split front/back).
//
//=============================================================================

using UnityEngine;
using UnityEngine.Rendering;
using Valve.VR;

public class SteamVR_ExternalCamera : MonoBehaviour
{
	public struct Config
	{
		public float x, y, z;
		public float rx, ry, rz;
		public float fov;
		public float near, far;
		public float sceneResolutionScale;
		public float frameSkip;
		public float nearOffset, farOffset;
		public float hmdOffset;
		public bool disableStandardAssets;
	}

	public Config config;
	public string configPath;

	public void ReadConfig()
	{
		try
		{
			var mCam = new HmdMatrix34_t();
			var readCamMatrix = false;

			object c = config; // box
			var lines = System.IO.File.ReadAllLines(configPath);
			foreach (var line in lines)
			{
				var split = line.Split('=');
				if (split.Length == 2)
				{
					var key = split[0];
					if (key == "m")
					{
						var values = split[1].Split(',');
						if (values.Length == 12)
						{
							mCam.m0 = float.Parse(values[0]);
							mCam.m1 = float.Parse(values[1]);
							mCam.m2 = float.Parse(values[2]);
							mCam.m3 = float.Parse(values[3]);
							mCam.m4 = float.Parse(values[4]);
							mCam.m5 = float.Parse(values[5]);
							mCam.m6 = float.Parse(values[6]);
							mCam.m7 = float.Parse(values[7]);
							mCam.m8 = float.Parse(values[8]);
							mCam.m9 = float.Parse(values[9]);
							mCam.m10 = float.Parse(values[10]);
							mCam.m11 = float.Parse(values[11]);
							readCamMatrix = true;
						}
					}
					else if (key == "disableStandardAssets")
					{
						var field = c.GetType().GetField(key);
						if (field != null)
							field.SetValue(c, bool.Parse(split[1]));
					}
					else
					{
						var field = c.GetType().GetField(key);
						if (field != null)
							field.SetValue(c, float.Parse(split[1]));
					}
				}
			}
			config = (Config)c; //unbox

			// Convert calibrated camera matrix settings.
			if (readCamMatrix)
			{
				var t = new SteamVR_Utils.RigidTransform(mCam);
				config.x = t.pos.x;
				config.y = t.pos.y;
				config.z = t.pos.z;
				var angles = t.rot.eulerAngles;
				config.rx = angles.x;
				config.ry = angles.y;
				config.rz = angles.z;
			}
		}
		catch { }
	}

	Camera cam;
	Transform target;
	GameObject clipQuad;
	Material clipMaterial;

	public void AttachToCamera(SteamVR_Camera vrcam)
	{
		if (target == vrcam.head)
			return;

		target = vrcam.head;

		var root = transform.parent;
		var origin = vrcam.head.parent;
		root.parent = origin;
		root.localPosition = Vector3.zero;
		root.localRotation = Quaternion.identity;
		root.localScale = Vector3.one;

		// Make a copy of the eye camera to pick up any camera fx.
		vrcam.enabled = false;
		var go = Instantiate(vrcam.gameObject);
		vrcam.enabled = true;
		go.name = "camera";

		DestroyImmediate(go.GetComponent<SteamVR_Camera>());
		DestroyImmediate(go.GetComponent<SteamVR_CameraFlip>());

		cam = go.GetComponent<Camera>();
		cam.fieldOfView = config.fov;
		cam.useOcclusionCulling = false;
		cam.enabled = false; // manually rendered

		colorMat = new Material(Shader.Find("Custom/SteamVR_ColorOut"));
		alphaMat = new Material(Shader.Find("Custom/SteamVR_AlphaOut"));
		clipMaterial = new Material(Shader.Find("Custom/SteamVR_ClearAll"));

		var offset = go.transform;
		offset.parent = transform;
		offset.localPosition = new Vector3(config.x, config.y, config.z);
		offset.localRotation = Quaternion.Euler(config.rx, config.ry, config.rz);
		offset.localScale = Vector3.one;

		// Strip children of cloned object (AudioListener in particular).
		while (offset.childCount > 0)
			DestroyImmediate(offset.GetChild(0).gameObject);

		// Setup clipping quad (using camera clip causes problems with shadows).
		clipQuad = GameObject.CreatePrimitive(PrimitiveType.Quad);
		clipQuad.name = "ClipQuad";
		DestroyImmediate(clipQuad.GetComponent<MeshCollider>());

		var clipRenderer = clipQuad.GetComponent<MeshRenderer>();
		clipRenderer.material = clipMaterial;
		clipRenderer.shadowCastingMode = ShadowCastingMode.Off;
		clipRenderer.receiveShadows = false;
#if !(UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		clipRenderer.lightProbeUsage = LightProbeUsage.Off;
#else
		clipRenderer.useLightProbes = false;
#endif
		clipRenderer.reflectionProbeUsage = ReflectionProbeUsage.Off;

		var clipTransform = clipQuad.transform;
		clipTransform.parent = offset;
		clipTransform.localScale = new Vector3(1000.0f, 1000.0f, 1.0f);
		clipTransform.localRotation = Quaternion.identity;

		clipQuad.SetActive(false);
	}

	public float GetTargetDistance()
	{
		if (target == null)
			return config.near + 0.01f;

		var offset = cam.transform;
		var forward = new Vector3(offset.forward.x, 0.0f, offset.forward.z).normalized;
		var targetPos = target.position + new Vector3(target.forward.x, 0.0f, target.forward.z).normalized * config.hmdOffset;

		var distance = -(new Plane(forward, targetPos)).GetDistanceToPoint(offset.position);
		return Mathf.Clamp(distance, config.near + 0.01f, config.far - 0.01f);
	}

	Material colorMat, alphaMat;

	public void RenderNear()
	{
		var w = Screen.width / 2;
		var h = Screen.height / 2;

		if (cam.targetTexture == null || cam.targetTexture.width != w || cam.targetTexture.height != h)
			cam.targetTexture = new RenderTexture(w, h, 16, RenderTextureFormat.ARGB32);

		cam.nearClipPlane = config.near;
		cam.farClipPlane = config.far;

		var clearFlags = cam.clearFlags;
		var backgroundColor = cam.backgroundColor;

		cam.clearFlags = CameraClearFlags.Color;
		cam.backgroundColor = Color.clear;

		float dist = Mathf.Clamp(GetTargetDistance() + config.nearOffset, config.near, config.far);
		var clipParent = clipQuad.transform.parent;
		clipQuad.transform.position = clipParent.position + clipParent.forward * dist;

		MonoBehaviour[] behaviours = null;
		bool[] wasEnabled = null;
		if (config.disableStandardAssets)
		{
			behaviours = cam.gameObject.GetComponents<MonoBehaviour>();
			wasEnabled = new bool[behaviours.Length];
			for (int i = 0; i < behaviours.Length; i++)
			{
				var behaviour = behaviours[i];
				if (behaviour.enabled && behaviour.GetType().ToString().StartsWith("UnityStandardAssets."))
				{
					behaviour.enabled = false;
					wasEnabled[i] = true;
				}
			}
		}

		clipQuad.SetActive(true);
		cam.Render();
		clipQuad.SetActive(false);

		if (behaviours != null)
		{
			for (int i = 0; i < behaviours.Length; i++)
			{
				if (wasEnabled[i])
				{
					behaviours[i].enabled = true;
				}
			}
		}

		cam.clearFlags = clearFlags;
		cam.backgroundColor = backgroundColor;

		Graphics.DrawTexture(new Rect(0, 0, w, h), cam.targetTexture, colorMat);
		Graphics.DrawTexture(new Rect(w, 0, w, h), cam.targetTexture, alphaMat);
	}

	public void RenderFar()
	{
		cam.nearClipPlane = config.near;
		cam.farClipPlane = config.far;
		cam.Render();

		var w = Screen.width / 2;
		var h = Screen.height / 2;
		Graphics.DrawTexture(new Rect(0, h, w, h), cam.targetTexture, colorMat);
	}

	Camera[] cameras;
	Rect[] cameraRects;
	float sceneResolutionScale;

	void OnEnable()
	{
		// Move game view cameras to lower-right quadrant.
		cameras = FindObjectsOfType<Camera>() as Camera[];
		if (cameras != null)
		{
			var numCameras = cameras.Length;
			cameraRects = new Rect[numCameras];
			for (int i = 0; i < numCameras; i++)
			{
				var cam = cameras[i];
				cameraRects[i] = cam.rect;

				if (cam == this.cam)
					continue;

				if (cam.targetTexture != null)
					continue;

				if (cam.GetComponent<SteamVR_Camera>() != null)
					continue;

				cam.rect = new Rect(0.5f, 0.0f, 0.5f, 0.5f);
			}
		}

		if (config.sceneResolutionScale > 0.0f)
		{
			sceneResolutionScale = SteamVR_Camera.sceneResolutionScale;
			SteamVR_Camera.sceneResolutionScale = config.sceneResolutionScale;
		}
	}

	void OnDisable()
	{
		// Restore game view cameras.
		if (cameras != null)
		{
			var numCameras = cameras.Length;
			for (int i = 0; i < numCameras; i++)
			{
				var cam = cameras[i];
				if (cam != null)
					cam.rect = cameraRects[i];
			}
			cameras = null;
			cameraRects = null;
		}

		if (config.sceneResolutionScale > 0.0f)
		{
			SteamVR_Camera.sceneResolutionScale = sceneResolutionScale;
		}
	}
}

