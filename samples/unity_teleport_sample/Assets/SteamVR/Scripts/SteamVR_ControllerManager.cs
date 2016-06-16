//========= Copyright 2016, Valve Corporation, All rights reserved. ===========
//
// Purpose: Enables/disables objects based on connectivity and assigned roles.
//
//=============================================================================

using UnityEngine;
using System.Collections.Generic;
using Valve.VR;

public class SteamVR_ControllerManager : MonoBehaviour
{
	public GameObject left, right;
	public GameObject[] objects; // populate with objects you want to assign to additional controllers

	uint[] indices; // assigned
	bool[] connected = new bool[OpenVR.k_unMaxTrackedDeviceCount]; // controllers only

	// cached roles - may or may not be connected
	uint leftIndex = OpenVR.k_unTrackedDeviceIndexInvalid;
	uint rightIndex = OpenVR.k_unTrackedDeviceIndexInvalid;

	void Awake()
	{
		// Add left and right entries to the head of the list so we only have to operate on the list itself.
		var additional = (this.objects != null) ? this.objects.Length : 0;
		var objects = new GameObject[2 + additional];
		indices = new uint[2 + additional];
		objects[0] = right;
		indices[0] = OpenVR.k_unTrackedDeviceIndexInvalid;
		objects[1] = left;
		indices[1] = OpenVR.k_unTrackedDeviceIndexInvalid;
		for (int i = 0; i < additional; i++)
		{
			objects[2 + i] = this.objects[i];
			indices[2 + i] = OpenVR.k_unTrackedDeviceIndexInvalid;
		}
		this.objects = objects;
	}

	void OnEnable()
	{
		for (int i = 0; i < objects.Length; i++)
		{
			var obj = objects[i];
			if (obj != null)
				obj.SetActive(false);
		}

		OnTrackedDeviceRoleChanged();

		for (int i = 0; i < SteamVR.connected.Length; i++)
			if (SteamVR.connected[i])
				OnDeviceConnected(i, true);

		SteamVR_Utils.Event.Listen("input_focus", OnInputFocus);
		SteamVR_Utils.Event.Listen("device_connected", OnDeviceConnected);
		SteamVR_Utils.Event.Listen("TrackedDeviceRoleChanged", OnTrackedDeviceRoleChanged);
	}

	void OnDisable()
	{
		SteamVR_Utils.Event.Remove("input_focus", OnInputFocus);
		SteamVR_Utils.Event.Remove("device_connected", OnDeviceConnected);
		SteamVR_Utils.Event.Remove("TrackedDeviceRoleChanged", OnTrackedDeviceRoleChanged);
	}

	static string[] labels = { "left", "right" };

	// Hide controllers when the dashboard is up.
	private void OnInputFocus(params object[] args)
	{
		bool hasFocus = (bool)args[0];
		if (hasFocus)
		{
			for (int i = 0; i < objects.Length; i++)
			{
				var obj = objects[i];
				if (obj != null)
				{
					var label = (i < 2) ? labels[i] : (i - 1).ToString();
					ShowObject(obj.transform, "hidden (" + label + ")");
				}
			}
		}
		else
		{
			for (int i = 0; i < objects.Length; i++)
			{
				var obj = objects[i];
				if (obj != null)
				{
					var label = (i < 2) ? labels[i] : (i - 1).ToString();
					HideObject(obj.transform, "hidden (" + label + ")");
				}
			}
		}
	}

	// Reparents to a new object and deactivates that object (this allows
	// us to call SetActive in OnDeviceConnected independently.
	private void HideObject(Transform t, string name)
	{
		var hidden = new GameObject(name).transform;
		hidden.parent = t.parent;
		t.parent = hidden;
		hidden.gameObject.SetActive(false);
	}
	private void ShowObject(Transform t, string name)
	{
		var hidden = t.parent;
		if (hidden.gameObject.name != name)
			return;
		t.parent = hidden.parent;
		Destroy(hidden.gameObject);
	}

	private void SetTrackedDeviceIndex(int objectIndex, uint trackedDeviceIndex)
	{
		// First make sure no one else is already using this index.
		if (trackedDeviceIndex != OpenVR.k_unTrackedDeviceIndexInvalid)
		{
			for (int i = 0; i < objects.Length; i++)
			{
				if (i != objectIndex && indices[i] == trackedDeviceIndex)
				{
					var obj = objects[i];
					if (obj != null)
						obj.SetActive(false);

					indices[i] = OpenVR.k_unTrackedDeviceIndexInvalid;
				}
			}
		}

		// Only set when changed.
		if (trackedDeviceIndex != indices[objectIndex])
		{
			indices[objectIndex] = trackedDeviceIndex;

			var obj = objects[objectIndex];
			if (obj != null)
			{
				if (trackedDeviceIndex == OpenVR.k_unTrackedDeviceIndexInvalid)
					obj.SetActive(false);
				else
				{
					obj.SetActive(true);
					obj.BroadcastMessage("SetDeviceIndex", (int)trackedDeviceIndex, SendMessageOptions.DontRequireReceiver);
				}
			}
		}
	}

	// Keep track of assigned roles.
	private void OnTrackedDeviceRoleChanged(params object[] args)
	{
		Refresh();
	}

	// Keep track of connected controller indices.
	private void OnDeviceConnected(params object[] args)
	{
		var index = (uint)(int)args[0];
		bool changed = this.connected[index];
		this.connected[index] = false;

		var connected = (bool)args[1];
		if (connected)
		{
			var system = OpenVR.System;
			if (system != null && system.GetTrackedDeviceClass(index) == ETrackedDeviceClass.Controller)
			{
				this.connected[index] = true;
				changed = !changed; // if we clear and set the same index, nothing has changed
			}
		}

		if (changed)
			Refresh();
	}

	public void Refresh()
	{
		int objectIndex = 0;

		var system = OpenVR.System;
		if (system != null)
		{
			leftIndex = system.GetTrackedDeviceIndexForControllerRole(ETrackedControllerRole.LeftHand);
			rightIndex = system.GetTrackedDeviceIndexForControllerRole(ETrackedControllerRole.RightHand);
		}

		// If neither role has been assigned yet, try hooking up at least the right controller.
		if (leftIndex == OpenVR.k_unTrackedDeviceIndexInvalid && rightIndex == OpenVR.k_unTrackedDeviceIndexInvalid)
		{
			for (uint deviceIndex = 0; deviceIndex < connected.Length; deviceIndex++)
			{
				if (connected[deviceIndex])
				{
					SetTrackedDeviceIndex(objectIndex++, deviceIndex);
					break;
				}
			}
		}
		else
		{
			SetTrackedDeviceIndex(objectIndex++, (rightIndex < connected.Length && connected[rightIndex]) ? rightIndex : OpenVR.k_unTrackedDeviceIndexInvalid);
			SetTrackedDeviceIndex(objectIndex++, (leftIndex < connected.Length && connected[leftIndex]) ? leftIndex : OpenVR.k_unTrackedDeviceIndexInvalid);

			// Assign out any additional controllers only after both left and right have been assigned.
			if (leftIndex != OpenVR.k_unTrackedDeviceIndexInvalid && rightIndex != OpenVR.k_unTrackedDeviceIndexInvalid)
			{
				for (uint deviceIndex = 0; deviceIndex < connected.Length; deviceIndex++)
				{
					if (objectIndex >= objects.Length)
						break;

					if (!connected[deviceIndex])
						continue;

					if (deviceIndex != leftIndex && deviceIndex != rightIndex)
					{
						SetTrackedDeviceIndex(objectIndex++, deviceIndex);
					}
				}
			}
		}

		// Reset the rest.
		while (objectIndex < objects.Length)
		{
			SetTrackedDeviceIndex(objectIndex++, OpenVR.k_unTrackedDeviceIndexInvalid);
		}
	}
}

