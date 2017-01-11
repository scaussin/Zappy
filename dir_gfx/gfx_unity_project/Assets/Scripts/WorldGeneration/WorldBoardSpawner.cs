using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldBoardSpawner : MonoBehaviour {


	[Header("Associated objects")]
	public GameObject			BlockPrefab;
	public GameObject			ZeroPoint;
	public GameObject			BlockContainer;

	[Header("Access to each spawned block")]
	public List<GameObject>		Blocks;


	public int					map_size_x;
	public int					map_size_y;

	// spawn required variables.
	private int					cur_y;
	private int					cur_x;

	private Vector3				spawn_location;

	private float				x_base_offset;
	private float				z_base_offset;

	// Use this for initialization
	void Awake () {
		ZeroPoint = transform.Find("ZeroPoint").gameObject;
		BlockContainer = transform.Find("BlockContainer").gameObject;
		BlockPrefab = Resources.Load ("Prefabs/World/Block") as GameObject;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	/// <summary>
	/// Spawns the blocks. Called distantly by WorldManager on order from the GameController.
	/// Warning : on our server 2D map, Y advances, on Unity 3D space, Z advances.
	/// </summary>
	public void SpawnBlocks()
	{
		// get those size when the serveur is authentified, not before.
		map_size_x = GameManager.instance.WorldSettings.WorldX;
		map_size_y = GameManager.instance.WorldSettings.WorldY;

		// ---- Preparing spawn position variables.
		// cur_x and cur_y are for running through 2d loop.
		cur_y = 0;
		cur_x = 0;

		// Blocks will be separated by a world distance offset.
		// An additionnal changeable spacing will be added.
		x_base_offset = GameManager.instance.WorldSettings.BlockSize + GameManager.instance.WorldSettings.BlockSpacing;
		z_base_offset = GameManager.instance.WorldSettings.BlockSize + GameManager.instance.WorldSettings.BlockSpacing;



		spawn_location = ZeroPoint.transform.position;

		Debug.Log ("Spawning World Blocks!!");
		// ---- Actual block spawn.
		while (cur_y < map_size_y)
		{
			while (cur_x < map_size_x)
			{
				// location is set through additionning variables -> faster calculation method.
				GameObject new_block = (GameObject)Instantiate (BlockPrefab, spawn_location, Quaternion.identity, BlockContainer.transform);
				new_block.isStatic = true;
				Blocks.Add (new_block);
				cur_x++;
				spawn_location.x += x_base_offset;
			}
			cur_y++;
			cur_x = 0;
			// reset x positionning.
			spawn_location.x = ZeroPoint.transform.position.x;
			spawn_location.z += z_base_offset;
		}



	}
}
