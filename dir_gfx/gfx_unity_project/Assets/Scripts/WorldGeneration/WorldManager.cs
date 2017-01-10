using UnityEngine;
using System.Collections;

public class WorldManager : MonoBehaviour {
	public WorldBoardSpawner	WorldBoardSpawner;


	// Use this for initialization
	void Awake () {
		WorldBoardSpawner = transform.Find ("Board").GetComponent<WorldBoardSpawner> ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
