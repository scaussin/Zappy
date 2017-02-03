using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using UnityEngine;

/// <summary>
/// Player controller. Access and make the players do their biddings,
/// and updates theirs values. Also calls their animations.
/// </summary>
public class PlayerController : MonoBehaviour {
	public GameObject			ActorContainer;
	public List<PlayerObject>	Players;

	// Private tmp variables.
	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	private int					player_nb;
	private int					new_x;
	private int					new_y;
	private int					new_dir;

	// Use this for initialization
	void Start () {
		ActorContainer = transform.Find ("ActorContainer").gameObject;
		Players = GameManager.instance.PlayerManager.ConnectedPlayers;
	}

	/// <summary>
	/// Moves the player. Called by the GameController, this function parse the msg
	/// in variables and search the player it corresponds to, and call is move action
	/// It can be advance, gauche, or droite.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void MovePlayer(string msg)
	{
		rgx = new Regex ("^ppo #(\\d+) (\\d+) (\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			new_x = int.Parse (groups [2].Value);
			new_y = int.Parse (groups [3].Value);
			new_dir = int.Parse (groups [4].Value);
		
			//Debug.Log ("new x: " + new_x + ", new y: " + new_y);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					// Rotation check
					if (player.Dir != new_dir)
					{
						// its a rotation, lets do just that.
						player.GetComponent<PlayerMovement> ().StartRotation(new_x, new_y, new_dir);
						player.Dir = new_dir;
						return;
					}
					// if not, its an advance.
					//				   	 /\__/\
					// Boundary check   (= _ =')
					if ((player.X == (GameManager.instance.WorldSettings.WorldX - 1) && new_x == 0) 
						|| (player.Y == (GameManager.instance.WorldSettings.WorldY - 1) && new_y == 0)
						|| (player.X == 0 && new_x == (GameManager.instance.WorldSettings.WorldX - 1))
						|| (player.Y == 0 && new_y == (GameManager.instance.WorldSettings.WorldY - 1)))
						player.gameObject.GetComponent<PlayerMovement> ().
											Teleport (new_x, new_y, new_dir);
					else
						player.gameObject.GetComponent<PlayerMovement> ().
											StartMovement (new_x, new_y, new_dir);
					// Update player position datas.
					player.X = new_x;
					player.Y = new_y;
					player.Dir = new_dir;
					return ;
				}
			}
		}
	}

	/// <summary>
	/// Kills the player indicated in the message. Also used for disconnection of client;
	/// </summary>
	/// <param name="msg">Message.</param>
	public void KillPlayer(string msg)
	{
		rgx = new Regex ("^pdi #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					// Found player to kill
					Debug.Log ("Killing player #" + player.AssignedNumber);
					GameManager.instance.PlayerManager.ConnectedPlayers.Remove (player);
					Destroy(player.gameObject); // for now, its instant kill.
					return ;
				}
			}
		}
	}

	/// <summary>
	/// Find and sets the player level from the message parsing.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void SetPlayerLevel(string msg)
	{
		rgx = new Regex ("^plv #(\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.Level = int.Parse (groups [2].Value);
					return ;
				}
			}
		}
	}

	/// <summary>
	/// Sets the player inventory from the msg received. Some datas are unused and
	/// this call may be subject to change.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void SetPlayerInventory(string msg)
	{
		rgx = new Regex("^pin #(\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			new_x = int.Parse (groups [2].Value);
			new_y = int.Parse (groups [3].Value);
			string inventory = groups [4].Value + groups [5].Value +
								groups [6].Value + groups [7].Value +
								groups [8].Value + groups [9].Value +
								groups [10].Value;
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.InventoryStr = inventory;
					player.FoodTimeLeft = int.Parse (groups [4].Value);
					player.Linemate = int.Parse (groups [5].Value);
					player.Deraumere = int.Parse (groups [6].Value);
					player.Sibur = int.Parse (groups [7].Value);
					player.Mendiane = int.Parse (groups [8].Value);
					player.Phiras = int.Parse (groups [9].Value);
					player.Thystame = int.Parse (groups [10].Value);
					return ;
				}
			}
		}
	}

	/// <summary>
	/// A player used the expulse cmd. This will push every player that are at the same coord.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void PlayerExpulse(string msg)
	{
		rgx = new Regex ("^pex #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.GetComponent<Animator> ().SetTrigger ("Expulse");
				}
			}
		}
		// Players move by theirselves.
	}

	public void PlayerDropRessource(string msg)
	{
		rgx = new Regex ("^pgt #(\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.GetComponent<Animator> ().SetTrigger ("ArmAction");
					return ;
				}
			}
		}
	}

	public void PlayerTakeRessource(string msg)
	{
		Debug.Log ("TODO: player take ressource");
	}
}
