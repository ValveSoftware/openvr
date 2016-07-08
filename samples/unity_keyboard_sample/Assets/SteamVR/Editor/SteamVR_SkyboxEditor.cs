//========= Copyright 2014, Valve Corporation, All rights reserved. ===========
//
// Purpose: Custom inspector display for SteamVR_Skybox
//
//=============================================================================

using UnityEngine;
using UnityEditor;
using System.Text;
using System.Collections.Generic;
using Valve.VR;
using System.IO;

[CustomEditor(typeof(SteamVR_Skybox)), CanEditMultipleObjects]
public class SteamVR_SkyboxEditor : Editor
{
	private const string nameFormat = "{0}/{1}-{2}.png";
	private const string helpText = "Take snapshot will use the current " +
		"position and rotation to capture six directional screenshots to use as this " +
		"skybox's textures.  Note: This skybox is only used to override what shows up " +
		"in the compositor (e.g. when loading levels).  Add a Camera component to this " +
		"object to override default settings like which layers to render.  Additionally, " +
		"by specifying your own targetTexture, you can control the size of the textures " +
		"and other properties like antialiasing.  Don't forget to disable the camera.";

	public override void OnInspectorGUI()
	{
		DrawDefaultInspector();
#if !(UNITY_5_0 || UNITY_5_1)
		EditorGUILayout.HelpBox(helpText, MessageType.Info);

		if (GUILayout.Button("Take snapshot"))
		{
#if (UNITY_5_2)
			var sceneName = Path.GetFileNameWithoutExtension(EditorApplication.currentScene);
			var scenePath = Path.GetDirectoryName(EditorApplication.currentScene);
			var assetPath = scenePath +"/" + sceneName;
			if (!AssetDatabase.IsValidFolder(assetPath))
			{
				var guid = AssetDatabase.CreateFolder(scenePath, sceneName);
				assetPath = AssetDatabase.GUIDToAssetPath(guid);
			}
#endif
			var directions = new Quaternion[] {
				Quaternion.LookRotation(Vector3.forward),
				Quaternion.LookRotation(Vector3.back),
				Quaternion.LookRotation(Vector3.left),
				Quaternion.LookRotation(Vector3.right),
				Quaternion.LookRotation(Vector3.up, Vector3.back),
				Quaternion.LookRotation(Vector3.down, Vector3.forward)
			};

			Camera tempCamera = null;
			foreach (SteamVR_Skybox target in targets)
			{
#if !(UNITY_5_2)
				var targetScene = target.gameObject.scene;
                var sceneName = Path.GetFileNameWithoutExtension(targetScene.name);
				var scenePath = Path.GetDirectoryName(targetScene.name);
				var assetPath = scenePath + "/" + sceneName;
				if (!AssetDatabase.IsValidFolder(assetPath))
				{
					var guid = AssetDatabase.CreateFolder(scenePath, sceneName);
					assetPath = AssetDatabase.GUIDToAssetPath(guid);
				}
#endif
				var camera = target.GetComponent<Camera>();
				if (camera == null)
				{
					if (tempCamera == null)
						tempCamera = new GameObject().AddComponent<Camera>();
					camera = tempCamera;
				}

				var targetTexture = camera.targetTexture;
				if (camera.targetTexture == null)
				{
					targetTexture = new RenderTexture(1024, 1024, 24);
					targetTexture.antiAliasing = 8;
					camera.targetTexture = targetTexture;
				}

				var oldPosition = target.transform.localPosition;
				var oldRotation = target.transform.localRotation;
				var baseRotation = target.transform.rotation;

				var t = camera.transform;
				t.position = target.transform.position;
				camera.orthographic = false;
				camera.fieldOfView = 90;

				for (int i = 0; i < directions.Length; i++)
				{
					t.rotation = baseRotation * directions[i];
					camera.Render();

					// Copy to texture and save to disk.
					RenderTexture.active = targetTexture;
					var texture = new Texture2D(targetTexture.width, targetTexture.height, TextureFormat.ARGB32, false);
					texture.ReadPixels(new Rect(0, 0, texture.width, texture.height), 0, 0);
					texture.Apply();
					RenderTexture.active = null;

					var assetName = string.Format(nameFormat, assetPath, target.name, i);
					System.IO.File.WriteAllBytes(assetName, texture.EncodeToPNG());
				}
	
				if (camera != tempCamera)
				{
					target.transform.localPosition = oldPosition;
					target.transform.localRotation = oldRotation;
				}
			}

			if (tempCamera != null)
			{
				Object.DestroyImmediate(tempCamera.gameObject);
			}

			// Now that everything has be written out, reload the associated assets and assign them.
			AssetDatabase.Refresh();
			foreach (SteamVR_Skybox target in targets)
			{
#if !(UNITY_5_2)
				var targetScene = target.gameObject.scene;
				var sceneName = Path.GetFileNameWithoutExtension(targetScene.name);
				var scenePath = Path.GetDirectoryName(targetScene.name);
				var assetPath = scenePath + "/" + sceneName;
#endif
				for (int i = 0; i < directions.Length; i++)
				{
					var assetName = string.Format(nameFormat, assetPath, target.name, i);
					var importer = AssetImporter.GetAtPath(assetName) as TextureImporter;
					importer.textureFormat = TextureImporterFormat.ARGB32;
					importer.wrapMode = TextureWrapMode.Clamp;
					importer.SaveAndReimport();

					var texture = AssetDatabase.LoadAssetAtPath<Texture>(assetName);
					target.SetTextureByIndex(i, texture);
				}
			}
		}
#endif
	}
}

