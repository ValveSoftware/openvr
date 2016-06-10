//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: Helper to update poses when using native OpenVR integration.
//
//=============================================================================

using UnityEngine;
using Valve.VR;

[RequireComponent(typeof(Camera))]
public class SteamVR_UpdatePoses : MonoBehaviour
{
	void Awake()
	{
		var camera = GetComponent<Camera>();
#if !(UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0)
		camera.stereoTargetEye = StereoTargetEyeMask.None;
#endif
		camera.clearFlags = CameraClearFlags.Nothing;
		camera.useOcclusionCulling = false;
		camera.cullingMask = 0;
		camera.depth = -9999;
	}

	void OnPreCull()
	{
		var compositor = OpenVR.Compositor;
		if (compositor != null)
		{
			var render = SteamVR_Render.instance;
			compositor.GetLastPoses(render.poses, render.gamePoses);
			SteamVR_Utils.Event.Send("new_poses", render.poses);
			SteamVR_Utils.Event.Send("new_poses_applied");
		}
	}
}

