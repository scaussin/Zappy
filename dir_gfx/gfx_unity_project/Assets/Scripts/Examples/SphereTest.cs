using UnityEngine;
using System.Collections;

public class SphereTest : MonoBehaviour {

	// Use this for initialization
	void Start () {
		transform.parent.GetComponent<CubeTest> ().OnClickCube.AddListener (MoveUp);
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void MoveUp()
	{
		Vector3 Pos = this.transform.position;
		Pos.y += 0.4f;
		this.transform.position = Pos;
	}
}
