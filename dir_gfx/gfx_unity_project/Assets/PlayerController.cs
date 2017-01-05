using UnityEngine;
using System.Collections;

public class PlayerController : MonoBehaviour {
	[Header("Player points of interest")]
	public GameObject CameraPoint;
	// Use this for initialization
	void Start () {
		CameraPoint = transform.Find ("Points").transform.Find ("CameraPoint").gameObject;
	}
	
	// Update is called once per frame
	void Update () {

	}
}
