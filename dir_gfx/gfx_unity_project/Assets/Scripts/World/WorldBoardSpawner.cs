using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class WorldBoardSpawner : MonoBehaviour {

	[Header("Associated objects")]
	public GameObject					BlockPrefab;
	public Vector3						ZeroPoint;
	public GameObject					BlockContainer;

	[Header("Block random mats")]
	public List<Material>				BlockMats;

	[System.Serializable]
	public class WorldBoard
	{
		public List<GameObject> Row;
	}

	[Header("Access to each spawned block")]
	public List <WorldBoard>			GameWorldBoard; // access with GameWorldBoard[y].Row[x]

	public int							map_size_x;
	public int							map_size_y;

	public UnityEvent					OnWorldBoardSpawned;

	// spawn required variables.
	private int							cur_y;
	private int							cur_x;

	private Vector3						spawn_location;
	private Vector3						spawn_rotation;
	private float						spawn_rotation_val;

	private float						x_base_offset;
	private float						z_base_offset;

	// Use this for initialization
	void Awake ()
	{
		ZeroPoint = GameManager.instance.WorldSettings.BoardZeroPoint;
		BlockContainer = transform.Find("BlockContainer").gameObject;
		BlockPrefab = Resources.Load ("Prefabs/World/Block") as GameObject;
		OnWorldBoardSpawned = new UnityEvent ();

		// Get blocks material for randomization
		BlockMats.Add(Resources.Load ("Materials/Blocks/Block_base_1") as Material);
		BlockMats.Add(Resources.Load ("Materials/Blocks/Block_base_2") as Material);
		BlockMats.Add(Resources.Load ("Materials/Blocks/Block_base_3") as Material);
		BlockMats.Add(Resources.Load ("Materials/Blocks/Block_base_4") as Material);
		BlockMats.Add(Resources.Load ("Materials/Blocks/Block_base_5") as Material);
	}

	/// <summary>
	/// Allows the access to a block and its class attributes, from its position X and Y of the *server* map
	/// </summary>
	/// <returns>The block object.</returns>
	/// <param name="x">The x coordinate. (server map)</param>
	/// <param name="y">The y coordinate. (server map)</param>
	public BlockObject	GetBlockObject(int x, int y)
	{
		return (GameWorldBoard[y].Row[x].GetComponent<BlockObject> ());
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
		// An additionnal spacing will be added. (can be modified through editor->GameManager)
		x_base_offset = GameManager.instance.WorldSettings.BlockSize +
							GameManager.instance.WorldSettings.BlockSpacing;
		z_base_offset = GameManager.instance.WorldSettings.BlockSize +
							GameManager.instance.WorldSettings.BlockSpacing;
		spawn_location = ZeroPoint;

		spawn_rotation = this.transform.eulerAngles;

		// ---- Actual block spawn.
		Debug.Log ("Spawning World Blocks!!");
		GameWorldBoard = new List<WorldBoard> ();
		while (cur_y < map_size_y)
		{
			GameWorldBoard.Add(new WorldBoard ());
			GameWorldBoard[cur_y].Row = new List<GameObject> ();
			while (cur_x < map_size_x)
			{
				int rand;
				rand = Random.Range (0, 1);
				if (rand == 1)
					spawn_rotation.y += 90;
				else
					spawn_rotation.y -= 90;
				// location is set through additionning variables -> faster calculation method.
				GameObject new_block = (GameObject)Instantiate (BlockPrefab, spawn_location,
					Quaternion.Euler(spawn_rotation), BlockContainer.transform);
				new_block.GetComponent<BlockObject> ().x = cur_x;
				new_block.GetComponent<BlockObject> ().y = cur_y;

				// 
				new_block.GetComponent<BlockObject> ().BlockModelObj.GetComponent<MeshRenderer> ().material = BlockMats[Random.Range (0, BlockMats.Count)];

				GameWorldBoard[cur_y].Row.Add(new_block);
				new_block.isStatic = true;
				cur_x++;
				spawn_location.x += x_base_offset;
			}
			cur_y++;
			cur_x = 0;
			// reset x positionning.
			spawn_location.x = ZeroPoint.x;
			spawn_location.z += z_base_offset;
		}
		OnWorldBoardSpawned.Invoke ();
	}

	public void DeleteWorld()
	{
		for (int i = 0; i < GameWorldBoard.Count; i++)
		{
			for (int y = 0; y < GameWorldBoard [i].Row.Count; y++)
			{
				Destroy (GameWorldBoard [i].Row[y].gameObject);
			}
		}
		GameWorldBoard.Clear ();
	}
}
