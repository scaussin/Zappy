using UnityEngine;
using System.Collections;

public class PlayerObject : MonoBehaviour {
	[Header("Player Datas")]
	public int			AssignedNumber; // #n
	public int			X; // X
	public int			Y; // Y
	public int			Dir; // O
	public int			Level; // L
	public string		Team; // N

	public string		Inventory;

	[Header("Player points of interest")]
	public GameObject	ModelContainer;
	public GameObject	CameraPoint;

	// Private setters variables.
	private Vector3		ObjectEulerAngle;

	// Use this for initialization
	void Start () {
		CameraPoint = transform.Find ("Points").transform.Find ("CameraPoint").gameObject;
		ModelContainer = transform.Find ("Model").gameObject;
		SetOrientation ();
	}

	/// <summary>
	/// Sets the orientation. Orientation (N:1, E:2, S:3, O:4)
	/// For the moment, the rotation is in euler (unrecommended);
	/// </summary>
	public void	SetOrientation()
	{
		ObjectEulerAngle = transform.localEulerAngles;
		if (Dir == 1)
		{
			ObjectEulerAngle.y = -90;
		}
		else if (Dir == 2)
		{
			ObjectEulerAngle.y = 0;
		}
		else if (Dir == 3)
		{
			ObjectEulerAngle.y = 90;
		}
		else if (Dir == 4)
		{
			ObjectEulerAngle.y = 180;
		}
		transform.localEulerAngles = ObjectEulerAngle;
	}
}
