using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class WorldManager : MonoBehaviour {
	public WorldBoardSpawner	WorldBoardSpawner;
	public ItemSpawner			ItemSpawner;
	public ActorSpawner			ActorSpawner;
	public PlayerController		PlayerController;

	// Use this for initialization
	void Awake () {
		WorldBoardSpawner = transform.Find ("Board").GetComponent<WorldBoardSpawner> ();
		ItemSpawner = transform.Find ("Board").GetComponent<ItemSpawner> ();
		ActorSpawner = transform.Find ("Actors").GetComponent<ActorSpawner> ();
		PlayerController = transform.Find ("Actors").GetComponent<PlayerController> ();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
