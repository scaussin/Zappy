using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class WorldManager : MonoBehaviour {
	public WorldBoardSpawner	WorldBoardSpawner;
	public ItemSpawner			ItemSpawner;
	public ActorSpawner			ActorSpawner;

	// Use this for initialization
	void Awake () {
		WorldBoardSpawner = transform.Find ("Board").GetComponent<WorldBoardSpawner> ();
		ItemSpawner = transform.Find ("Board").GetComponent<ItemSpawner> ();
		ActorSpawner = transform.Find ("Actors").GetComponent<ActorSpawner> ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
