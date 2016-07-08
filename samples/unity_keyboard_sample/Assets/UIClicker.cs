using UnityEngine;
using System.Collections;

public class UIClicker : MonoBehaviour
{
	public delegate void ClickHandler();

	public event ClickHandler Clicked;

	// Use this for initialization
	void Start ()
	{
	
	}
	
	// Update is called once per frame
	void Update ()
	{
		foreach(SteamVR_LaserPointer pointer in FindObjectsOfType<SteamVR_LaserPointer>())
		{
			Ray r = new Ray(pointer.transform.position, pointer.transform.forward);
			Plane p = new Plane(transform.forward, transform.position);
			float enter;
			if (p.Raycast(r, out enter))
			{
				Vector3 target = pointer.transform.position + enter * pointer.transform.forward;
				Vector3 localTarget = ((RectTransform)transform).InverseTransformPoint(target);
				bool hit = ((RectTransform)transform).rect.Contains(new Vector2(localTarget.x, localTarget.y));


				uint controllerIndex = (uint)pointer.GetComponent<SteamVR_TrackedObject>().index;
				Valve.VR.VRControllerState_t controllerState = new Valve.VR.VRControllerState_t();
				if (SteamVR.instance.hmd.GetControllerState(controllerIndex, ref controllerState))
				{
					bool trigger = (controllerState.ulButtonPressed & (1UL << ((int)Valve.VR.EVRButtonId.k_EButton_SteamVR_Trigger))) > 0;
					if (hit && Clicked != null && trigger)
					{
						Clicked();
					}
				}
			}
		}
	}
}
