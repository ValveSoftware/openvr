using UnityEngine;
using System.Collections;

public class SteamVR_Teleporter : MonoBehaviour
{
    public enum TeleportType
    {
        TeleportTypeUseTerrain,
        TeleportTypeUseCollider,
        TeleportTypeUseZeroY
    }

    public bool teleportOnClick = false;
    public TeleportType teleportType = TeleportType.TeleportTypeUseZeroY;
    Transform reference;

	// Use this for initialization
	void Start ()
    {
        Transform eyeCamera = GameObject.FindObjectOfType<SteamVR_Camera>().GetComponent<Transform>();
        // The referece point for the camera is two levels up from the SteamVR_Camera
        reference = eyeCamera.parent.parent;

        if (GetComponent<SteamVR_TrackedController>() == null)
        {
            Debug.LogError("SteamVR_Teleporter must be on a SteamVR_TrackedController");
            return;
        }
        GetComponent<SteamVR_TrackedController>().TriggerClicked += new ClickedEventHandler(DoClick);

        if (teleportType == TeleportType.TeleportTypeUseTerrain)
        {
            // Start the player at the level of the terrain
            reference.position = new Vector3(reference.position.x, Terrain.activeTerrain.SampleHeight(reference.position), reference.position.z);
        }
	}
	
	// Update is called once per frame
	void Update ()
    {
	
	}

    void DoClick(object sender, ClickedEventArgs e)
    {
        if (teleportOnClick)
        {
            // Teleport
            float refY = reference.position.y;

            Plane plane = new Plane(Vector3.up, -refY);
            Ray ray = new Ray(this.transform.position, transform.forward);

            bool hasGroundTarget = false;
            float dist = 0f;
            if (teleportType == TeleportType.TeleportTypeUseCollider)
            {
                RaycastHit hitInfo;
                TerrainCollider tc = Terrain.activeTerrain.GetComponent<TerrainCollider>();
                hasGroundTarget = tc.Raycast(ray, out hitInfo, 1000f);
                dist = hitInfo.distance;
            }
            else if (teleportType == TeleportType.TeleportTypeUseCollider)
            {
                RaycastHit hitInfo;
                Physics.Raycast(ray, out hitInfo);
                dist = hitInfo.distance;
            }
            else
            {
                hasGroundTarget = plane.Raycast(ray, out dist);
            }
            if (hasGroundTarget)
            {
                Vector3 newPos = ray.origin + ray.direction * dist - new Vector3(reference.GetChild(0).localPosition.x, 0f, reference.GetChild(0).localPosition.z);

                reference.position = newPos;
            }
        }
    }
}
