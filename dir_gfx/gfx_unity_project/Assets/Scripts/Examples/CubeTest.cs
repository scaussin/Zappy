using UnityEngine;
using UnityEngine.Events;
using System.Collections;

public class CubeTest : MonoBehaviour {
	public UnityEvent OnClickCube;

	// Use this for initialization
	void Awake () {
		OnClickCube = new UnityEvent ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnMouseDown()
	{
		Debug.Log ("cube clicked");
		OnClickCube.Invoke ();
	}
}
