using UnityEngine;
using System.Collections;

public class KeyManager : MonoBehaviour {
	[Header("Moving Keys")]
	public KeyCode Advance = KeyCode.W;
	public KeyCode AdvanceAlt = KeyCode.Z;

	public KeyCode StepBack = KeyCode.S;
	public KeyCode StepBackAlt = KeyCode.None;

	public KeyCode StrafeLeft = KeyCode.A;
	public KeyCode StrafeLeftAlt = KeyCode.Q;

	public KeyCode StrafeRight = KeyCode.D;
	public KeyCode StrafeRightAlt = KeyCode.None;

}
