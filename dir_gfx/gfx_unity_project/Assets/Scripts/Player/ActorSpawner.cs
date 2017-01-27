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
	public GameObject		BoardZeroPoint;

	private Vector3			SpawnLocation;

	// Use this for initialization
	void Start ()
	{
		ActorContainer = transform.Find ("ActorContainer").gameObject;
		PlayerPrefab = Resources.Load ("Prefabs/Actors/Player") as GameObject;
		BoardZeroPoint = GameManager.instance.WorldManager.WorldBoardSpawner.ZeroPoint;
	}
	
	public void SpawnNewPlayer(string msg)
	{
		// "pnw #n X Y O L N\n"
		Regex rgx = new Regex ("^pnw #(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\w+)$");
		Match match = rgx.Match (msg);
		GroupCollection groups = match.Groups;

		// ---- Preparing spawn position variables.
		SpawnLocation = BoardZeroPoint.transform.position;

		SpawnLocation.x += (GameManager.instance.WorldSettings.BlockSize +
		GameManager.instance.WorldSettings.BlockSpacing) * (int.Parse (groups [2].Value));
		SpawnLocation.z += (GameManager.instance.WorldSettings.BlockSize +
		GameManager.instance.WorldSettings.BlockSpacing) * (int.Parse (groups [3].Value));

		// ---- Actual player spawn.
		GameObject newPlayer = (GameObject)Instantiate (PlayerPrefab, SpawnLocation,
			Quaternion.identity, ActorContainer.transform);

		// ---- Set script datas.
		newPlayer.GetComponent<PlayerObject> ().AssignedNumber = int.Parse(groups[1].Value);
		newPlayer.GetComponent<PlayerObject> ().X = int.Parse (groups [2].Value);
		newPlayer.GetComponent<PlayerObject> ().Y = int.Parse (groups [3].Value);
		newPlayer.GetComponent<PlayerObject> ().Dir = int.Parse (groups [4].Value);
		newPlayer.GetComponent<PlayerObject> ().Level = int.Parse (groups [5].Value);
		newPlayer.GetComponent<PlayerObject> ().Team = groups [6].Value;
		GameManager.instance.PlayerManager.ConnectedPlayers.Add(newPlayer.GetComponent<PlayerObject> ());
		//GetComponent<PlayerController> ().Players.Add(newPlayer.GetComponent<PlayerObject> ());
	}
}
