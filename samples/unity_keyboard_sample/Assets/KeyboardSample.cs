using UnityEngine;
using System.Collections;
using Valve.VR;

public class KeyboardSample : MonoBehaviour
{
	public UnityEngine.UI.InputField textEntry;
	public bool minimalMode;
	static bool keyboardShowing;
	string text = "";
	static KeyboardSample activeKeyboard = null;

	// Use this for initialization
	void Start ()
	{
		GetComponent<UIClicker>().Clicked += KeyboardDemo_Clicked;
	}

	void OnEnable()
	{
		SteamVR_Utils.Event.Listen("KeyboardCharInput", OnKeyboard);
		SteamVR_Utils.Event.Listen("KeyboardClosed", OnKeyboardClosed);
	}

	private void OnKeyboard(object[] args)
	{
		if (activeKeyboard != this)
			return;
		Valve.VR.VREvent_t ev = (Valve.VR.VREvent_t) args[0];
		if (minimalMode)
		{
			VREvent_t evt = (VREvent_t)args[0];
			if (evt.data.keyboard.cNewInput == "\b")
			{
				if (text.Length > 0)
				{
					text = text.Substring(0, text.Length - 1);
				}
			}
			else if (evt.data.keyboard.cNewInput == "\x1b")
			{
				// Close the keyboard
				var vr = SteamVR.instance;
				vr.overlay.HideKeyboard();
				keyboardShowing = false;
			}
			else
			{
				text += evt.data.keyboard.cNewInput;
			}
			textEntry.text = text;
		}
		else
		{
			System.Text.StringBuilder textBuilder = new System.Text.StringBuilder(1024);
			uint size = SteamVR.instance.overlay.GetKeyboardText(textBuilder, 1024);
			text = textBuilder.ToString();
            textEntry.text = text;
		}
	}

	private void OnKeyboardClosed(object[] args)
	{
		if (activeKeyboard != this)
			return;
		keyboardShowing = false;
		activeKeyboard = null;
    }

	private void KeyboardDemo_Clicked()
	{
		if(!keyboardShowing)
		{
			keyboardShowing = true;
			activeKeyboard = this;
			SteamVR.instance.overlay.ShowKeyboard(0, 0, "Description", 256, text, minimalMode, 0);
		}
	}

	// Update is called once per frame
	void Update ()
	{
		
	}
}
