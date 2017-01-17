using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using UnityEngine;

/// <summary>
/// Player controller. Access and make the players do their biddings,
/// and updates theirs values.
/// </summary>
public class PlayerController : MonoBehaviour {
	public GameObject			ActorContainer;
	public List<PlayerObject>	Players;

	// Private tmp variables.
	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	private int					new_x;
	private int					new_y;
	private int					new_dir;

	// Use this for initialization
	void Start () {
		ActorContainer = transform.Find ("ActorContainer").gameObject;
	}

	/// <summary>
	/// Moves the player. Called by the GameController, this function parse the msg
	/// in variables and search the player it corresponds to, in order to
	/// execute the required action. 
	/// </summary>
	/// <param name="msg">Message.</param>
	public void MovePlayer(string msg)
	{
		Debug.Log ("Player moves");
		rgx = new Regex ("^ppo (\\d+) (\\d+) (\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			new_x = int.Parse (groups [2].Value);
			new_y = int.Parse (groups [3].Value);
			new_dir = int.Parse (groups [4].Value);

			Debug.Log ("new x: " + new_x + ", new y: " + new_y);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == int.Parse (groups [1].Value))
				{
					// found player that will move.

					//				   	 /\__/\
					// Boundary check 	(= _ =')
					if ((player.X == (GameManager.instance.WorldSettings.WorldX - 1) && new_x == 0) 
						|| (player.Y == (GameManager.instance.WorldSettings.WorldY - 1) && new_y == 0)
						|| (player.X == 0 && new_x == (GameManager.instance.WorldSettings.WorldX - 1))
						|| (player.Y == 0 && new_y == (GameManager.instance.WorldSettings.WorldY - 1)))
						player.gameObject.GetComponent<PlayerMovement> ().
											Teleport (new_x, new_y, new_dir);
					else
						player.gameObject.GetComponent<PlayerMovement> ().
											StartMovement (new_x, new_y, new_dir);
					// update player position.
					player.X = new_x;
					player.Y = new_y;
					player.Dir = new_dir;
					return ;
				}
			}
		}
	}
}
