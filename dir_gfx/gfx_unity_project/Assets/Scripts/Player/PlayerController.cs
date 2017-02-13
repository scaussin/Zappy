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

	[Header("Ongoing incantion nb and ids")]
	public int					Incantation_nb = 0;

	// Private tmp variables.
	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	private int					player_nb;
	private int					new_x;
	private int					new_y;
	private int					new_dir;

	// incantation
	private string				players_incanting_str;
	private string[]			players_incanting_strarray;
	private List<int>			players_incanting_nb;
	private int					incantation_x;
	private int					incantation_y;
	private int					incantation_lv;
	private GameObject			incantation_block_obj;
	private int					incantation_result;
	private int					asked_egg_nb;

	// Use this for initialization
	void Start () {
		ActorContainer = transform.Find ("ActorContainer").gameObject;
		Players = GameManager.instance.PlayerManager.ConnectedPlayers;
	}

	/// <summary>
	/// Moves the player. Called by the GameController, this function parse the msg
	/// in variables and search the player it corresponds to, and calls its move action.
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
					if (GameManager.instance.WorldSettings.InstantTimeMode == true ||
						(player.X == (GameManager.instance.WorldSettings.WorldX - 1) && new_x == 0) 
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
		// Expulsed players move by theirselves.
	}

	public void PlayerBroadcast(string msg)
	{
		rgx = new Regex ("^pbc #(\\d+) (\\w+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.GetComponent<Animator> ().SetTrigger ("Broadcasting");
					player.gameObject.transform.Find ("Effects").transform.Find ("BroadcastEffect").gameObject.SetActive(false);
					player.gameObject.transform.Find ("Effects").transform.Find ("BroadcastEffect").gameObject.SetActive(true);
				}
			}
		}
	}

	public void PlayerDropRessource(string msg)
	{
		rgx = new Regex ("^pdr #(\\d+) (\\d+)$");
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

	public void IncantationStart(string msg)
	{
		// "pic X Y L #n #n …\n"
		rgx = new Regex ("^pic (\\d+) (\\d+) (\\d+)((?: #\\d+)+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			incantation_x = int.Parse (groups [1].Value);
			incantation_y = int.Parse (groups [2].Value);
			incantation_lv = int.Parse (groups [3].Value);
			players_incanting_str = groups [4].Value;
			Incantation_nb += 1;

			// parse cmd to get every player incanting and set them in the animation.
			players_incanting_str = players_incanting_str.TrimStart (' ');
			players_incanting_strarray = players_incanting_str.Split (new string[] { " " }, System.StringSplitOptions.None);
			players_incanting_nb = new List<int> ();
			foreach (string player_str_id in players_incanting_strarray)
			{
				Debug.Log ("[" + player_str_id + "]");
				rgx = new Regex ("^#(\\d+)$");
				match = rgx.Match (player_str_id);
				if (match.Success)
				{
					groups = match.Groups;
					players_incanting_nb.Add (int.Parse (groups [1].Value));
				}
			}
			foreach (PlayerObject player in Players)
			{
				if (players_incanting_nb.IndexOf(player.AssignedNumber) != -1)
				{
					player.IsIncanting = true;
					player.IncantationId = Incantation_nb;
					player.GetComponent<Animator> ().SetBool ("IsIncanting", true);
				}
			}
			// set block status datas.
			incantation_block_obj = GameManager.instance.WorldManager.WorldBoardSpawner.
				GameWorldBoard [incantation_y].Row [incantation_x].gameObject;

			// add one ongoing incantation to the world block.
			incantation_block_obj.GetComponent<BlockObject> ().Incantations_id.Add(Incantation_nb);

			// set the ground particle effect ON.
			incantation_block_obj.transform.Find ("Effects").transform.Find ("IncantingSparkEffect").
				gameObject.SetActive (true);
		}
	}

	public void IncantationEnd(string msg)
	{
		// "pie X Y R\n"
		rgx = new Regex ("^pie (\\d+) (\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			incantation_x = int.Parse (groups [1].Value);
			incantation_y = int.Parse (groups [2].Value);
			incantation_result = int.Parse (groups [3].Value);
			incantation_block_obj = GameManager.instance.WorldManager.WorldBoardSpawner.
				GameWorldBoard [incantation_y].Row [incantation_x].gameObject;
			if (incantation_block_obj.GetComponent<BlockObject> ().Incantations_id.Count > 0)
			{
				Debug.Log ("incantation end");
				// we want the first launched incantation, because they finish in a chronological order(first in, first out);
				int cur_incantation = incantation_block_obj.GetComponent<BlockObject> ().Incantations_id [0];

				foreach (PlayerObject player in Players)
				{
					if (player.IsIncanting == true && player.IncantationId == cur_incantation)
					{
						player.IsIncanting = false;
						player.IncantationId = -1;
						player.GetComponent<Animator> ().SetBool ("IsIncanting", false);
						if (incantation_result == 1) {
							// turn off, turn on for particle effect launch.
							player.gameObject.transform.Find ("Effects").transform.Find ("LevelUpEffect").gameObject.SetActive (false);
							player.gameObject.transform.Find ("Effects").transform.Find ("LevelUpEffect").gameObject.SetActive (true);
						}
					}
				}
				// set the ground particle effect OFF.
				incantation_block_obj.transform.Find ("Effects").transform.Find ("IncantingSparkEffect").
					gameObject.SetActive (false);
			}
		}
	}

	public void PlayerStartsLaying(string msg)
	{
		// "pfk #n\n"
		rgx = new Regex ("^pfk #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			player_nb = int.Parse (groups [1].Value);
			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.GetComponent<Animator> ().SetBool ("LayingEgg", true);
					return ;
				}
			}
		}
	}

	public void PlayerFinishEggLaying(string msg)
	{
		int		egg_nb;
		int		egg_x;
		int		egg_y;

		//"enw #e #n X Y\n"
		rgx = new Regex ("^enw #(\\d+) #(\\d+) (\\d+) (\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			egg_nb = int.Parse (groups [1].Value);
			player_nb = int.Parse (groups [2].Value);
			egg_x = int.Parse (groups [3].Value);
			egg_y = int.Parse (groups [4].Value);

			foreach (PlayerObject player in Players)
			{
				if (player.AssignedNumber == player_nb)
				{
					player.GetComponent<Animator> ().SetBool ("LayingEgg", false);
					GameManager.instance.WorldManager.ActorSpawner.SpawnNewEgg (egg_nb, player, egg_x, egg_y);
					return ;
				}
			}
		}
	}

	public void HatchEgg(string msg)
	{
		// "eht #e\n"
		rgx = new Regex ("^eht #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			asked_egg_nb = int.Parse(groups [1].Value);
			foreach (EggObject egg in GameManager.instance.PlayerManager.EggList)
			{
				if (egg.egg_nb == asked_egg_nb)
				{
					egg.gameObject.GetComponent<Animator> ().SetTrigger ("EggHatch");
				}
			}
		}
	}

	public void EggDeath(string msg)
	{
		// "edi #e\n"
		rgx = new Regex ("^edi #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			asked_egg_nb = int.Parse(groups [1].Value);
			foreach (EggObject egg in GameManager.instance.PlayerManager.EggList)
			{
				if (egg.egg_nb == asked_egg_nb)
				{
					egg.gameObject.GetComponent<Animator> ().SetTrigger ("Disappear");
				}
			}
		}
	}

	public void EggPlayerConnection(string msg)
	{
		// "ebo #e\n"
		rgx = new Regex ("^ebo #(\\d+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			asked_egg_nb = int.Parse(groups [1].Value);
			foreach (EggObject egg in GameManager.instance.PlayerManager.EggList)
			{
				if (egg.egg_nb == asked_egg_nb)
				{
					egg.gameObject.GetComponent<Animator> ().SetTrigger ("Disappear");
				}
			}
		}
	}

	/// <summary>
	/// Cleans the map of all connected players. Called when the server connection is lost.
	/// </summary>
	public void CleanMapOfPlayers()
	{
		foreach (PlayerObject player in Players)
		{
			Destroy (player.gameObject);
		}
		Players.Clear ();
	}
}
