using UnityEngine;
using System.Collections;

public class SteamVR_Keyboard : MonoBehaviour
{
	public UnityEngine.UI.Text target = null;

	// Use this for initialization
	void Start ()
	{
	
	}
	
	// Update is called once per frame
	void Update ()
	{
	
	}

	public void Show()
	{
		string text = "";
		SteamVR.instance.overlay.ShowKeyboard(0, 0, "", 256, text, true, 0);
	}

	public void Close()
	{
		SteamVR.instance.overlay.HideKeyboard();
	}

	public void UpdatePos()
	{
		SteamVR_Utils.RigidTransform rigid = new SteamVR_Utils.RigidTransform(this.transform);
		Valve.VR.HmdMatrix34_t trans = rigid.ToHmdMatrix34();
		SteamVR.instance.overlay.SetKeyboardTransformAbsolute(Valve.VR.ETrackingUniverseOrigin.TrackingUniverseStanding, ref trans);
    }
}
