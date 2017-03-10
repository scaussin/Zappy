using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Item spawner. Does NOT actually spawn items. it makes the invisible items on the ground be visible or not.
/// </summary>
public class ItemSpawner : MonoBehaviour {
	public bool					IsBoardSpawned = false;
	private WorldBoardSpawner	WorldBoardSpawner;

	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	private BlockObject			CurBlock;

	// Use this for initialization
	void Start () {
		GetComponent<WorldBoardSpawner> ().OnWorldBoardSpawned.AddListener (OnWorldBoardSpawnedAction);
	}

	public void OnWorldBoardSpawnedAction()
	{
		IsBoardSpawned = true;
		WorldBoardSpawner = GetComponent<WorldBoardSpawner> ();
	}

	/// <summary>
	/// Sets the block ressources. Will parse the msg from the server to put the values
	/// into the world block and update the display of those values.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void SetBlockRessources(string msg)
	{
		if (IsBoardSpawned) {
			
			rgx = new Regex ("^bct (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)$");
			match = rgx.Match (msg);
			if (match.Success) {
				
				groups = match.Groups;
				CurBlock = WorldBoardSpawner.GetBlockObject (int.Parse (groups [1].Value), int.Parse (groups [2].Value));
				if (CurBlock) {
					CurBlock.FoodUnits = int.Parse (groups [3].Value);
					CurBlock.LinemateUnits = int.Parse (groups [4].Value);
					CurBlock.DeraumereUnits = int.Parse (groups [5].Value);
					CurBlock.SiburUnits = int.Parse (groups [6].Value);
					CurBlock.MendianeUnits = int.Parse (groups [7].Value);
					CurBlock.PhirasUnits = int.Parse (groups [8].Value);
					CurBlock.ThystameUnits = int.Parse (groups [9].Value);

					CurBlock.UpdateModelDisplay ();
				}
			}
		}
	}
}
