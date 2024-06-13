using UnityEngine;
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
        SteamVR_Events.System(EVREventType.VREvent_KeyboardCharInput).Listen(OnKeyboard);
        SteamVR_Events.System(EVREventType.VREvent_KeyboardClosed).Listen(OnKeyboardClosed);
    }

    private void OnKeyboard(VREvent_t args)
    {
        if (activeKeyboard != this)
			return;
		VREvent_Keyboard_t keyboard = args.data.keyboard;
		byte[] inputBytes = new byte[] { keyboard.cNewInput0, keyboard.cNewInput1, keyboard.cNewInput2, keyboard.cNewInput3, keyboard.cNewInput4, keyboard.cNewInput5, keyboard.cNewInput6, keyboard.cNewInput7 };
		int len = 0;
		for (; inputBytes[len] != 0 && len < 7; len++) ;
		string input = System.Text.Encoding.UTF8.GetString(inputBytes, 0, len);

		if (minimalMode)
		{
			if (input == "\b")
			{
				if (text.Length > 0)
				{
					text = text.Substring(0, text.Length - 1);
				}
			}
			else if (input == "\x1b")
			{
				// Close the keyboard
				var vr = SteamVR.instance;
				vr.overlay.HideKeyboard();
				keyboardShowing = false;
			}
			else
			{
				text += input;
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

    private void OnKeyboardClosed(VREvent_t args)
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
            int inputMode = (int)EGamepadTextInputMode.k_EGamepadTextInputModeNormal;
            int lineMode = (int)EGamepadTextInputLineMode.k_EGamepadTextInputLineModeSingleLine;
            SteamVR.instance.overlay.ShowKeyboard(inputMode, lineMode, "Description", 256, textEntry.text, minimalMode, 0);
		}
	}

}
