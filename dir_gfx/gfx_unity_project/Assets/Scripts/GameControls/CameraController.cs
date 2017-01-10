using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {
	public CameraViewControl CameraViewControl;


	void Awake()
	{
		CameraViewControl = this.GetComponent<CameraViewControl> ();
	}

	// Use this for initialization
	void Start ()
	{
		CameraViewControl.GetComponent<CameraViewControl> ().UpdateCameraViewControlKeys ();
	}
	
	// Update is called once per frame
	void Update ()
	{
		
	}
}
