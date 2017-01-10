using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraViewControl : MonoBehaviour {
	// Control settings variables.
	private float		CameraMoveSensitivity;
	private float		CameraRotateSensitivity;

	private KeyCode		Advance;
	private KeyCode		AdvanceAlt;

	private KeyCode		StepBack;
	private KeyCode		StepBackAlt;

	private KeyCode		StrafeLeft;
	private KeyCode		StrafeLeftAlt;

	private KeyCode		StrafeRight;
	private KeyCode		StrafeRightAlt;

	private Vector3		NewPos;

	// camera objects variables.
	public GameObject	CameraOffsetObject;
	public GameObject	CameraAngleObject;
		

	/// <summary>
	/// Updates the camera view control keys. Call this when the keys are changed in game.
	/// </summary>
	public void UpdateCameraViewControlKeys()
	{
		// get control values for private use.
		CameraMoveSensitivity = GameManager.instance.KeyManager.CameraMoveSensitivity;
		CameraRotateSensitivity = GameManager.instance.KeyManager.CameraRotateSensitivity;

		Advance = GameManager.instance.KeyManager.Advance;
		AdvanceAlt = GameManager.instance.KeyManager.AdvanceAlt;

		StepBack = GameManager.instance.KeyManager.StepBack;
		StepBackAlt = GameManager.instance.KeyManager.StepBackAlt;

		StrafeLeft = GameManager.instance.KeyManager.StrafeLeft;
		StrafeLeftAlt = GameManager.instance.KeyManager.StrafeLeftAlt;

		StrafeRight = GameManager.instance.KeyManager.StrafeRight;
		StrafeRightAlt = GameManager.instance.KeyManager.StrafeRightAlt;
	}

	// Use this for initialization
	void Awake ()
	{
		CameraOffsetObject = transform.Find ("CameraOffset").gameObject;
		CameraAngleObject = transform.Find ("CameraOffset").transform.Find("CameraAngle").gameObject;
	}
	
	// Update is called once per frame
	void Update ()
	{
		NewPos = transform.position;

		// Advance / step back.
		if (Input.GetKey (Advance) || Input.GetKey (AdvanceAlt))
		{
			NewPos.z += 0.1f * CameraMoveSensitivity;
		}
		else if (Input.GetKey (StepBack) || Input.GetKey (StepBackAlt))
		{
			NewPos.z -= 0.1f * CameraMoveSensitivity;
		}

		// Strafe right / Strafe left
		if (Input.GetKey (StrafeRight) || Input.GetKey (StrafeRightAlt))
		{
			NewPos.x += 0.1f * CameraMoveSensitivity;
		}
		else if (Input.GetKey (StrafeLeft) || Input.GetKey (StrafeLeftAlt))
		{
			NewPos.x -= 0.1f * CameraMoveSensitivity;
		}
		transform.position = NewPos;
	}
}
