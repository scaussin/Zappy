using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using UnityEngine;

/// <summary>
/// Actor spawner. "Actor" because it spawns not only players, but also eggs for example.
/// </summary>
public class ActorSpawner : MonoBehaviour
{
	[Header("Associated Objects")]
	public GameObject		ActorContainer;
	public GameObject		PlayerPrefab;
	public GameObject		EggPrefab;
	private Vector3			BoardZeroPoint;

	private Vector3			SpawnLocation;
	private PlayerObject	NewPlayerScriptRef;
	private EggObject		NewEggScriptRef;
	private float			offset_x;
	private float			offset_z;

	// Use this for initialization
	void Awake ()
	{
		ActorContainer = transform.Find ("ActorContainer").gameObject;
		PlayerPrefab = Resources.Load ("Prefabs/Actors/Player") as GameObject;
		EggPrefab = Resources.Load ("Prefabs/Actors/Egg") as GameObject;
		BoardZeroPoint = GameManager.instance.WorldSettings.BoardZeroPoint;
	}
	
	public void SpawnNewPlayer(string msg)
	{
		// "pnw #n X Y O L N\n"
		Regex rgx = new Regex ("^pnw #(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) ([\\w ]+)$");
		Match match = rgx.Match (msg);
		GroupCollection groups = match.Groups;

		// ---- Preparing spawn position variables.
		SpawnLocation = BoardZeroPoint;
		offset_x = Random.Range (0.0f, GameManager.instance.WorldSettings.PlayerMaxOffset_x);
		offset_z = Random.Range (0.0f, GameManager.instance.WorldSettings.PlayerMaxOffset_z);

		SpawnLocation.x += (GameManager.instance.WorldSettings.BlockSize +
			GameManager.instance.WorldSettings.BlockSpacing) * (int.Parse (groups [2].Value)) + offset_x;
		SpawnLocation.z += (GameManager.instance.WorldSettings.BlockSize +
			GameManager.instance.WorldSettings.BlockSpacing) * (int.Parse (groups [3].Value)) + offset_z;

		// ---- Actual player spawn.
		GameObject newPlayer = (GameObject)Instantiate (PlayerPrefab, SpawnLocation,
			Quaternion.identity, ActorContainer.transform);

		// ---- Set script datas.
		NewPlayerScriptRef = newPlayer.GetComponent<PlayerObject> ();
		NewPlayerScriptRef.AssignedNumber = int.Parse(groups[1].Value);
		NewPlayerScriptRef.X = int.Parse (groups [2].Value);
		NewPlayerScriptRef.Y = int.Parse (groups [3].Value);
		NewPlayerScriptRef.Dir = int.Parse (groups [4].Value);
		NewPlayerScriptRef.Level = int.Parse (groups [5].Value);
		NewPlayerScriptRef.Team = groups [6].Value;
		newPlayer.GetComponent<PlayerMovement> ().Offset_x = offset_x;
		newPlayer.GetComponent<PlayerMovement> ().Offset_z = offset_z;

		// Player Added to list of players.
		GameManager.instance.PlayerManager.ConnectedPlayers.Add(newPlayer.GetComponent<PlayerObject> ());
	}

	public void SpawnNewEgg(int egg_nb, PlayerObject player, int egg_x, int egg_y)
	{
		Debug.Log ("Laying new egg !");
		SpawnLocation = BoardZeroPoint;
		SpawnLocation.x += (GameManager.instance.WorldSettings.BlockSize +
			GameManager.instance.WorldSettings.BlockSpacing) * egg_x;
		SpawnLocation.z += (GameManager.instance.WorldSettings.BlockSize +
			GameManager.instance.WorldSettings.BlockSpacing) * egg_y;

		// ---- Actual egg spawn.
		GameObject newEgg = (GameObject)Instantiate (EggPrefab, SpawnLocation,
			Quaternion.identity, ActorContainer.transform);

		// ---- Set script datas.
		NewEggScriptRef = newEgg.GetComponent<EggObject> ();
		NewEggScriptRef.egg_nb = egg_nb;
		NewEggScriptRef.pos_x = egg_x;
		NewEggScriptRef.pos_y = egg_y;

		// Egg Added to list of Eggs.
		GameManager.instance.PlayerManager.EggList.Add(newEgg.GetComponent<EggObject> ());
	}
}
