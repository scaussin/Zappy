using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraViewControl : MonoBehaviour {
	// Control settings variables.
	private float				CameraMoveSensitivity;
	private float				CameraRotateSensitivity;

	private KeyCode				Advance;
	private KeyCode				AdvanceAlt;

	private KeyCode				StepBack;
	private KeyCode				StepBackAlt;

	private KeyCode				StrafeLeft;
	private KeyCode				StrafeLeftAlt;

	private KeyCode				StrafeRight;
	private KeyCode				StrafeRightAlt;

	// camera objects variables.
	public GameObject			CameraYRotation;
	public GameObject			CameraXRotation;
		
	// Camera rotation
	private Vector3				newAngle;
	private float				AngleXCount = 0.0f;

	// camera zoom
	private Vector3				newpos;
	private float				curAltitude;

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
		CameraYRotation = transform.Find ("CameraYRotation").gameObject;
		CameraXRotation = CameraYRotation.transform.Find ("CameraXRotation").gameObject;
	}

	// Update is called once per frame
	void Update ()
	{
		// Advance / step back.
		if (Input.GetKey (Advance) || Input.GetKey (AdvanceAlt))
		{
			CameraYRotation.transform.Translate (CameraYRotation.transform.forward * CameraMoveSensitivity * Time.fixedDeltaTime, Space.World);
		}
		else if (Input.GetKey (StepBack) || Input.GetKey (StepBackAlt))
		{
			CameraYRotation.transform.Translate (-CameraYRotation.transform.forward * CameraMoveSensitivity * Time.fixedDeltaTime, Space.World);
		}

		// Strafe right / Strafe left
		if (Input.GetKey (StrafeRight) || Input.GetKey (StrafeRightAlt))
		{
			CameraYRotation.transform.Translate (CameraYRotation.transform.right * CameraMoveSensitivity * Time.fixedDeltaTime, Space.World);
		}
		else if (Input.GetKey (StrafeLeft) || Input.GetKey (StrafeLeftAlt))
		{
			CameraYRotation.transform.Translate (-CameraYRotation.transform.right * CameraMoveSensitivity * Time.fixedDeltaTime, Space.World);
		}

		// Camera rotations
		if (Input.GetMouseButton (1))
		{
			// Y rotation -> looking left or right.
			newAngle.x = 0.0f;
			newAngle.y = Input.GetAxis ("Mouse X") * CameraRotateSensitivity;
			newAngle.z = 0.0f;
			CameraYRotation.transform.Rotate (newAngle);

			// X rotation -> looking up or down.
			newAngle.x = -Input.GetAxis ("Mouse Y") * CameraRotateSensitivity;
			newAngle.y = 0.0f;
			newAngle.z = 0.0f;
			// limiting X rotation. I keep count of the movement done each move
			// to limit the up and down rotation.
			AngleXCount += Input.GetAxis ("Mouse Y");
			if (AngleXCount > -20.0f && AngleXCount < 35.0f) // these values look natural enough.
			{
				CameraXRotation.transform.Rotate (newAngle);
			}
			else
			{
				AngleXCount = Mathf.Clamp (AngleXCount, -25.0f, 35.0f);
			}
		}

		// Camera altitude
		newpos = CameraYRotation.transform.position;
		float d = Input.GetAxis("Mouse ScrollWheel");
		if (d > 0f)
		{
			// scroll up
			newpos.y -= 1.0f;
		}
		else if (d < 0f)
		{
			// scroll down
			newpos.y += 1.0f;
		}
		CameraYRotation.transform.position = newpos;
	}
}
