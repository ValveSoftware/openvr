//======= Copyright (c) Valve Corporation, All rights reserved. ===============
//
// Purpose: Flips the camera output back to normal for D3D.
//
//=============================================================================

using UnityEngine;
using System.Collections;

public class SteamVR_CameraFlip : MonoBehaviour
{
	static Material blitMaterial;

	void OnEnable()
	{
		if (blitMaterial == null)
			blitMaterial = new Material(Shader.Find("Custom/SteamVR_BlitFlip"));
	}

	void OnRenderImage(RenderTexture src, RenderTexture dest)
	{
		Graphics.Blit(src, dest, blitMaterial);
	}
}

