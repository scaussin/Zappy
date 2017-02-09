using UnityEngine;
using System.Collections;

public class KeyManager : MonoBehaviour {
	[Header("Control Sensitivities")]
	public float		CameraMoveSensitivity = 1.0f;
	public float		CameraRotateSensitivity = 1.0f;


	[Header("Moving Keys")]
	public KeyCode		Advance = KeyCode.W;
	public KeyCode		AdvanceAlt = KeyCode.Z;

	public KeyCode		StepBack = KeyCode.S;
	public KeyCode		StepBackAlt = KeyCode.None;

	public KeyCode		StrafeLeft = KeyCode.A;
	public KeyCode		StrafeLeftAlt = KeyCode.Q;

	public KeyCode		StrafeRight = KeyCode.D;
	public KeyCode		StrafeRightAlt = KeyCode.None;

	[Header("UI Keys")]
	public KeyCode		Enter;
	public KeyCode		EnterAlt;

	public KeyCode		Escape;
	public KeyCode		EscapeAlt;

	public KeyCode		TabPanelKey;
	public KeyCode		TabPanelKeyAlt;

}
