using UnityEngine;
using System.Collections;

public class PlayerObject : MonoBehaviour {
	[Header("Player Status flags")]
	public bool			IsIncanting;
	public int			IncantationId = -1; // -1 == incantation not started.

	[Header("Player Datas")]
	public int			AssignedNumber; // #n
	public int			X; // X
	public int			Y; // Y
	public int			Dir; // O
	public int			Level; // L
	public string		Team; // N

	[Header("Player Inventory")]
	public string	InventoryStr;
	public int		FoodTimeLeft;
	public int		Linemate;
	public int		Deraumere;
	public int		Sibur;
	public int		Mendiane;
	public int		Phiras;
	public int		Thystame;

	[Header("Player points of interest")]
//	public GameObject	ModelContainer;
//	public GameObject	HeadModelContainer;
//	public GameObject	BodyModelContainer;
	public GameObject	CameraPoint;
	public GameObject	PlayerSelectedObject;



	// Use this for initialization. called just after it is spawned. Its datas are set.
	void Start ()
	{
		CameraPoint = transform.Find ("Points").transform.Find ("CameraPoint").gameObject;
//		ModelContainer = transform.Find ("Model").gameObject;
//		HeadModelContainer = ModelContainer.transform.Find ("Root").transform.Find ("Head").transform.Find ("HeadModel").gameObject;
//		BodyModelContainer = ModelContainer.transform.Find ("Root").transform.Find ("Body").transform.Find ("BodyModel").gameObject;
		GetComponent<PlayerMovement> ().UpdateOrientation ();
		PlayerSelectedObject = transform.Find ("Effects").transform.Find ("PlayerSelected").gameObject;
	}

	public void ToggleSelection(bool b)
	{
		PlayerSelectedObject.SetActive (b);
	}
}
