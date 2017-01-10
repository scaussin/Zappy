using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldBoardSpawner : MonoBehaviour {
	
	public GameObject			ZeroPoint;
	public GameObject			BlockContainer;

	public List<GameObject>		Blocks;


	// Use this for initialization
	void Awake () {
		ZeroPoint = transform.Find("ZeroPoint").gameObject;
		BlockContainer = transform.Find("BlockContainer").gameObject;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void SpawnBlocks()
	{
		Debug.Log ("Spawning World Blocks!!");
	}
}
