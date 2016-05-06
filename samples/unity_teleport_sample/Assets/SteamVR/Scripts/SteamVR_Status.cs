//========= Copyright 2014, Valve Corporation, All rights reserved. ===========
//
// Purpose: Base class for fading things based on SteamVR events
//
//=============================================================================

using UnityEngine;

public abstract class SteamVR_Status : MonoBehaviour
{
	public string message; // name of event to respond to
	public float duration, fade;

	protected float timer;
	protected bool status;

	public enum Mode
	{
		OnTrue,
		OnFalse,
		WhileTrue,
		WhileFalse
	}

	public Mode mode;

	protected abstract void SetAlpha(float a);

	void OnEnable()
	{
		SteamVR_Utils.Event.Listen(message, OnEvent);
	}

	void OnDisable()
	{
		SteamVR_Utils.Event.Remove(message, OnEvent);
	}

	void OnEvent(params object[] args)
	{
		status = (bool)args[0];
		if (status)
		{
			if (mode == Mode.OnTrue)
				timer = duration;
		}
		else
		{
			if (mode == Mode.OnFalse)
				timer = duration;
		}
	}

	void Update()
	{
		if (mode == Mode.OnTrue || mode == Mode.OnFalse)
		{
			timer -= Time.deltaTime;
			if (timer < 0.0f)
			{
				SetAlpha(0.0f);
			}
			else
			{
				var a = 1.0f;
				if (timer < fade)
					a = timer / fade;
				if (timer > duration - fade)
					a = Mathf.InverseLerp(duration, duration - fade, timer);
				SetAlpha(a);
			}
		}
		else
		{
			var show = (mode == Mode.WhileTrue && status == true) || (mode == Mode.WhileFalse && status == false);
			timer = show ? Mathf.Min(fade, timer + Time.deltaTime) : Mathf.Max(0.0f, timer - Time.deltaTime);
			SetAlpha(timer / fade);
		}
	}
}

