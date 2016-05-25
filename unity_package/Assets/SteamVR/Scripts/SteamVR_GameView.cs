//========= Copyright 2014, Valve Corporation, All rights reserved. ===========
//
// Purpose: Handles rendering to the game view window
//
//=============================================================================

using UnityEngine;

[RequireComponent(typeof(Camera))]
public class SteamVR_GameView : MonoBehaviour
{
#if (UNITY_5_3 || UNITY_5_2 || UNITY_5_1 || UNITY_5_0) // DEPRECATED in Unity 5.4+
	public float scale = 1.5f;
	public bool drawOverlay = true;

	static Material overlayMaterial;

	void OnEnable()
	{
		if (overlayMaterial == null)
		{
			overlayMaterial = new Material(Shader.Find("Custom/SteamVR_Overlay"));
		}
	}

	void OnPostRender()
	{
		var vr = SteamVR.instance;
		var camera = GetComponent<Camera>();
		var aspect = scale * camera.aspect / vr.aspect;

		var x0 = -scale;
		var x1 = scale;
		var y0 = aspect;
		var y1 = -aspect;

		var blitMaterial = SteamVR_Camera.blitMaterial;
		blitMaterial.mainTexture = SteamVR_Camera.GetSceneTexture(camera.hdr);

		GL.PushMatrix();
		GL.LoadOrtho();
#if !(UNITY_5_0)
		blitMaterial.SetPass(0);
#else
		blitMaterial.SetPass(QualitySettings.activeColorSpace == ColorSpace.Linear ? 1 : 0);
#endif
		GL.Begin(GL.QUADS);
		GL.TexCoord2(0.0f, 0.0f); GL.Vertex3(x0, y0, 0);
		GL.TexCoord2(1.0f, 0.0f); GL.Vertex3(x1, y0, 0);
		GL.TexCoord2(1.0f, 1.0f); GL.Vertex3(x1, y1, 0);
		GL.TexCoord2(0.0f, 1.0f); GL.Vertex3(x0, y1, 0);
		GL.End();
		GL.PopMatrix();

		var overlay = SteamVR_Overlay.instance;
		if (overlay && overlay.texture && overlayMaterial && drawOverlay)
		{
			var texture = overlay.texture;
			overlayMaterial.mainTexture = texture;

			var u0 = 0.0f;
			var v0 = 1.0f - (float)Screen.height / texture.height;
			var u1 = (float)Screen.width / texture.width;
			var v1 = 1.0f;

			GL.PushMatrix();
			GL.LoadOrtho();
#if !(UNITY_5_0)
			overlayMaterial.SetPass(QualitySettings.activeColorSpace == ColorSpace.Linear ? 1 : 0);
#else
			overlayMaterial.SetPass(0);
#endif
			GL.Begin(GL.QUADS);
			GL.TexCoord2(u0, v0); GL.Vertex3(-1, -1, 0);
			GL.TexCoord2(u1, v0); GL.Vertex3( 1, -1, 0);
			GL.TexCoord2(u1, v1); GL.Vertex3( 1,  1, 0);
			GL.TexCoord2(u0, v1); GL.Vertex3(-1,  1, 0);
			GL.End();
			GL.PopMatrix();
		}
	}
#endif
}

