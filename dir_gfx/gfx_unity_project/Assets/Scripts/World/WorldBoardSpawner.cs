using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class WorldBoardSpawner : MonoBehaviour {

	[Header("Associated objects")]
	public GameObject					BlockPrefab;
	public GameObject					ZeroPoint;
	public GameObject					BlockContainer;


	[System.Serializable]
	public class WorldBoard
	{
		public List<GameObject> Row;
	}

	[Header("Access to each spawned block")]
	public List <WorldBoard> Blocks_col;


	public int							map_size_x;
	public int							map_size_y;

	public UnityEvent					OnWorldBoardSpawned;

	// spawn required variables.
	private int							cur_y;
	private int							cur_x;

	private Vector3						spawn_location;

	private float						x_base_offset;
	private float						z_base_offset;

	// Use this for initialization
	void Awake () {
		ZeroPoint = transform.Find("ZeroPoint").gameObject;
		BlockContainer = transform.Find("BlockContainer").gameObject;
		BlockPrefab = Resources.Load ("Prefabs/World/Block") as GameObject;
		OnWorldBoardSpawned = new UnityEvent ();
	}

	/// <summary>
	/// Allows the access to a block and its class attributes, from its position X and Y of the *server* map
	/// </summary>
	/// <returns>The block object.</returns>
	/// <param name="x">The x coordinate. (server map)</param>
	/// <param name="y">The y coordinate. (server map)</param>
	public BlockObject	GetBlockObject(int x, int y)
	{
		return (Blocks_col[y].Row[x].GetComponent<BlockObject> ());
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
		spawn_location = ZeroPoint.transform.position;

		// ---- Actual block spawn.
		Debug.Log ("Spawning World Blocks!!");
		Blocks_col = new List<WorldBoard> ();
		while (cur_y < map_size_y)
		{
			Blocks_col.Add(new WorldBoard ());
			Blocks_col[cur_y].Row = new List<GameObject> ();
			while (cur_x < map_size_x)
			{
				// location is set through additionning variables -> faster calculation method.
				GameObject new_block = (GameObject)Instantiate (BlockPrefab, spawn_location,
											Quaternion.identity, BlockContainer.transform);
				Blocks_col[cur_y].Row.Add(new_block);
				new_block.isStatic = true;
				cur_x++;
				spawn_location.x += x_base_offset;
				//new_block.GetComponent<Animator> ().enabled = true;
			}
			cur_y++;
			cur_x = 0;
			// reset x positionning.
			spawn_location.x = ZeroPoint.transform.position.x;
			spawn_location.z += z_base_offset;
		}
		OnWorldBoardSpawned.Invoke ();
	}
}
