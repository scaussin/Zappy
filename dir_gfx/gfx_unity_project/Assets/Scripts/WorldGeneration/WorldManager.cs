using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class WorldManager : MonoBehaviour {
	public WorldBoardSpawner	WorldBoardSpawner;
	public ItemSpawner			ItemSpawner;

	// Use this for initialization
	void Awake () {
		WorldBoardSpawner = transform.Find ("Board").GetComponent<WorldBoardSpawner> ();
		ItemSpawner = transform.Find ("Board").GetComponent<ItemSpawner> ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
