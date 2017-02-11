using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Net.Sockets;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Message broadcast controller. Receive, parse and redirect messages.
/// Will rarely directly access Controller component, such as WorldManager. Instead, it will call
/// GameController, because the GameController may need to insert others calls,
/// such as UI calls alongside the reception of the msg.
/// </summary>
public class MsgBroadcastController : MonoBehaviour
{
	public ConnectionManager	ConnectionManager;

	public string				CurrentReceivedMsg;

	private string[]			LexedMsg;
	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	public bool					IsReceiving;


	void Awake()
	{
		ConnectionManager = GameManager.instance.ConnectionManager;

		//OnWorldSizeReceived =  new UnityEvent ();
		//OnWorldTimeUnitReceived = new UnityEvent ();
	}
		

	// Use this for initialization
	void Start()
	{
		GameManager.instance.ConnectionManager.OnConnectionWithServer.AddListener (OnServerConnectionAction);
		//OnMsgReception.AddListener (OnMsgReceptionAction);
	}

	public void OnServerConnectionAction()
	{
		IsReceiving = true;
	}

/* *************************************************************************************** 	*
 * 																							*		
 *  The method that will catch every new cmd from the reception buffer, and the one to be	*
 *	used to send data to the server.														*
 * 																							*
 *  ***************************************************************************************	*/

	void FixedUpdate()
	{
		if (IsReceiving) // must not receive if is not connected to server.
		{
			while ((CurrentReceivedMsg = ConnectionManager.buffer_recv.getMsg ()) != null) // lexing here
			{
				//Debug.Log ("Msg received: [" + CurrentReceivedMsg + "]");
				ParseMsg (CurrentReceivedMsg);
			}
		}
	}

	/// <summary>
	/// Use this when you want to send a msg to the server. The connection Manager will do the sending.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void PushMsg(string msg)
	{
		ConnectionManager.buffer_send.pushMsg (msg);
		// The message will be sent at the corresponding time,
		// decided by the select() calls.
	}

/* *************************************************************************************** 	*
 * 																							*		
 *  The main parsing method -> the box that will call other small parsers.					*
 * 																							*
 *  ***************************************************************************************	*/

	/// <summary>
	/// Parses the message. Main parsing function.
	/// Makes extensive use of regexes and regex capturing groups.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void ParseMsg(string msg)
	{
		Debug.Log ("Parsing: [" + msg + "]");

		// We dont want to execute every regex parsing method when we found the one we needed.
		// The most likely receivable msg should at the top.
		// Optimization pass: we check the first three chars before checking the rest.
		if (CatchBienvenue (msg))
			return;
		if (msg.StartsWith ("msz") && CatchWorldSize (msg))
			return;
		else if (msg.StartsWith ("sgt") && CatchServerTimeUnit (msg))
			return;
		else if (msg.StartsWith ("bct") && CatchCaseContent (msg))
			return;
		else if (msg.StartsWith ("tna") && CatchTeamName (msg))
			return;
		else if (msg.StartsWith ("pnw") && CatchPlayerConnection (msg))
			return;
		else if (msg.StartsWith ("ppo") && CatchPlayerMovement (msg))
			return;
		else if (msg.StartsWith ("pdi") && CatchPlayerDeath (msg))
			return;
		else if (msg.StartsWith ("smg") && CatchServerMessage (msg))
			return;
		else if (msg.StartsWith ("plv") && CatchPlayerLevel (msg))
			return;
		else if (msg.StartsWith ("pin") && CatchPlayerInventory (msg))
			return;
		else if (msg.StartsWith ("pex") && CatchPlayerExpulse (msg))
			return;
		else if (msg.StartsWith ("pbc") && CatchPlayerBroadcast (msg))
			return;
		else if (msg.StartsWith ("pic") && CatchPlayerIncantation (msg))
			return;
		else if (msg.StartsWith ("pie") && CatchIncantationEnd (msg))
			return;
		else if (msg.StartsWith ("pfk") && CatchPlayerStartLayEgg (msg))
			return;
		else if (msg.StartsWith ("pdr") && CatchPlayerDropRessource (msg))
			return;
		else if (msg.StartsWith ("pgt") && CatchPlayerTakeRessource (msg))
			return;
		else if (msg.StartsWith ("enw") && CatchPlayerEggLayed (msg))
			return;
		else if (msg.StartsWith ("eht") && CatchEggHatched (msg))
			return;
		else if (msg.StartsWith ("ebo") && CatchEggPlayerConnection (msg))
			return;
		else if (msg.StartsWith ("edi") && CatchHatchedEggDied (msg))
			return;
		else if (msg.StartsWith ("seg") && CatchGameOver (msg))
			return;
		else if (msg.StartsWith ("suc") && CatchUnknownCmd (msg))
			return;
		else if (msg.StartsWith ("sbp") && CatchBadParameterForCmd (msg))
			return;
	}

/* *************************************************************************************** 	*
 * 																							*		
 *  Following: parsing methods for each event sent by the server.							*
 * 																							*
 *  ***************************************************************************************	*/

	/// <summary>
	/// Catchs the BIENVENUE\n message. Sends GRAPHIC\n in response.
	/// </summary>
	/// <returns><c>true</c>, if BIENVENUE\n was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchBienvenue(string msg)
	{
		rgx = new Regex("^BIENVENUE$");
		match = rgx.Match(CurrentReceivedMsg);
		if (match.Success) {
			Debug.Log ("bienvenue message OK! Zappy server it is");
			PushMsg ("GRAPHIC\n");
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the size of the world and puts it in the GameManager->WorldSettings. Format: "msz X Y\n" 
	/// </summary>
	/// <returns><c>true</c>, if world size was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchWorldSize(string msg)
	{
		rgx = new Regex("^msz (\\d+) (\\d+)$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			groups = match.Groups;
			GameManager.instance.WorldSettings.WorldX = int.Parse(groups[1].Value);
			GameManager.instance.WorldSettings.WorldY = int.Parse(groups[2].Value);

			Debug.Log ("Success - Received world size");
			GameManager.instance.GameController.OnWorldSizeReceivedAction(); // call back to GameController, cause the timing is undetermined.
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the server time unit and puts it in the GameManager->WorldSettings. Format: "sgt T\n"
	/// </summary>
	/// <returns><c>true</c>, if server time unit was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchServerTimeUnit(string msg)
	{
		rgx = new Regex("^sgt ([0-9]*\\.?[0-9]+)$"); // floating point number regex.
		match = rgx.Match(msg);
		if (match.Success)
		{
			float calculatedTimescale;
			groups = match.Groups;
			GameManager.instance.WorldSettings.TimeUnit = float.Parse (groups [1].Value);

			// makes a reverse calculation of the time unit, as the reference is 1.
			calculatedTimescale = (float) (1.0f / GameManager.instance.WorldSettings.TimeUnit);
			Debug.Log ("calculated timescale = " + calculatedTimescale.ToString ());
			if (calculatedTimescale > 10.0f)
			{
				GameManager.instance.WorldSettings.InstantTimeMode = false;
				Time.timeScale = 2;
			}
			else if (calculatedTimescale > 30.0f)
			{
				GameManager.instance.WorldSettings.InstantTimeMode = true;
				Time.timeScale = 10;
			}
			else
			{
				GameManager.instance.WorldSettings.InstantTimeMode = false;
				//Time.timeScale = (int)calculatedTimescale;
				Time.timeScale = 1;
			}
			Debug.Log ("Success - Received World Time unit");
			GameManager.instance.GameController.OnTimeUnitReceived(msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the content of one case of the board. Format: "bct X Y q q q q q q q\n"
	/// When catching this, the base empty board has already been spawned in the gfx.
	/// This will put ressources on the case.
	/// </summary>
	/// <returns><c>true</c>, if case content was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchCaseContent(string msg)
	{
		rgx = new Regex("^bct \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			// Use the game controller for action cause it may call other things.
			GameManager.instance.GameController.SetWorldBlockRessources (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the name of the team. Format: "tna N\n"
	/// </summary>
	/// <returns><c>true</c>, if team name was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchTeamName(string msg)
	{
		rgx = new Regex("^tna ([\\w ]+)$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			groups = match.Groups;
			GameManager.instance.PlayerManager.PlayerTeams.Add (groups [1].Value);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player connection. Format: "pnw #n X Y O L N\n"
	/// </summary>
	/// <returns><c>true</c>, if player connection was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchPlayerConnection(string msg)
	{
		rgx = new Regex("^pnw #\\d+ \\d+ \\d+ \\d+ \\d+ [\\w ]+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			// Use the game controller for action cause it may call other things.
			GameManager.instance.GameController.OnNewPlayerConnection(msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player movement. Note that an change of direction or a movement in any
	/// direction is the same msg. Format: "ppo #n X Y O\n"
	/// </summary>
	/// <returns><c>true</c>, if player movement was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool CatchPlayerMovement(string msg)
	{
		rgx = new Regex("^ppo #\\d+ \\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			// Use the game controller for action cause it may call other things.
			GameManager.instance.GameController.OnPlayerMovement(msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player death. Format: "pdi #n\n"
	/// </summary>
	/// <returns><c>true</c>, if player death was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerDeath(string msg)
	{
		rgx = new Regex("^pdi #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			// Use the game controller for action cause it may call other things.
			GameManager.instance.GameController.OnPlayerDeath(msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the server message. Format: "smg M\n"
	/// </summary>
	/// <returns><c>true</c>, if server message was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchServerMessage(string msg)
	{
		rgx = new Regex("^smg \\w+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnServerMessageReception(msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player level. Format: "plv #n L\n"
	/// </summary>
	/// <returns><c>true</c>, if player level was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerLevel(string msg)
	{
		rgx = new Regex("^plv #\\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerLevelReception (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player inventory. Format: "pin #n X Y q q q q q q q\n"
	/// </summary>
	/// <returns><c>true</c>, if player inventory was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerInventory(string msg)
	{
		rgx = new Regex("^pin #\\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerInventoryReception (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player expulse cmd. Format: "pex #n\n"
	/// </summary>
	/// <returns><c>true</c>, if player expulse was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerExpulse(string msg)
	{
		rgx = new Regex("^pex #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerExpulseReception (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player broadcast cmd. Format: "pbc #n M\n"
	/// </summary>
	/// <returns><c>true</c>, if player broadcast was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerBroadcast(string msg)
	{
		rgx = new Regex("^pbc #\\d+ \\w+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerBroadcast (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player incantation. Format: "pic X Y L #n #n …\n"
	/// </summary>
	/// <returns><c>true</c>, if player incantation was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerIncantation(string msg)
	{
		rgx = new Regex("^pic \\d+ \\d+ \\d+(?: #\\d+)+$"); // using non capturing group for var arg.
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerIncantation (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the incantation end. Format: "pie X Y R\n" 
	/// </summary>
	/// <returns><c>true</c>, if incantation end was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchIncantationEnd(string msg)
	{
		rgx = new Regex("^pie \\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnIncantationEnd (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player lay egg. The egg is not yet layed, but the action is engaged.
	/// Format: "pfk #n\n"
	/// </summary>
	/// <returns><c>true</c>, if player lay egg was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerStartLayEgg(string msg)
	{
		rgx = new Regex("^pfk #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerStartLayEgg (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player drop ressource. Format: "pdr #n i\n"
	/// </summary>
	/// <returns><c>true</c>, if player drop ressource was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerDropRessource(string msg)
	{
		rgx = new Regex("^pdr #\\d+ \\d$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerDropRessource (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player take ressource. Format: "pgt #n i\n"
	/// </summary>
	/// <returns><c>true</c>, if player take ressource was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerTakeRessource(string msg)
	{
		rgx = new Regex("^pgt #\\d+ \\d$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerTakeRessource (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the player egg layed. The player finished laying its egg. its not yet hatched.
	/// Format: "enw #e #n X Y\n"
	/// </summary>
	/// <returns><c>true</c>, if player egg layed was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchPlayerEggLayed(string msg)
	{
		rgx = new Regex("^enw #\\d+ #\\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnPlayerLayedEgg (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the egg hatched. Format: "eht #e\n"
	/// </summary>
	/// <returns><c>true</c>, if egg hatched was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchEggHatched(string msg)
	{
		rgx = new Regex("^eht #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnEggHatched (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the egg player connection. Format: "ebo #e\n"
	/// </summary>
	/// <returns><c>true</c>, if egg player connection was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchEggPlayerConnection(string msg)
	{
		rgx = new Regex("^ebo #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnEggPlayerConnection (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the hatched egg's death. Format: "edi #e\n"
	/// </summary>
	/// <returns><c>true</c>, if hatched egg died was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchHatchedEggDied(string msg)
	{
		rgx = new Regex("^edi #\\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnHatchedEggDeath (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the game over. Format: "seg N\n"
	/// </summary>
	/// <returns><c>true</c>, if game over was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchGameOver(string msg)
	{
		rgx = new Regex("^seg \\w+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnGameOver (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the msg for an unknown cmd. Format: "suc\n"
	/// </summary>
	/// <returns><c>true</c>, if unknown cmd was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchUnknownCmd(string msg)
	{
		rgx = new Regex("^suc$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnUnknownCmdReception (msg);
			return (true);
		}
		return (false);
	}

	/// <summary>
	/// Catchs the msg for bad parameter in a cmd. Format: "sbp\n"
	/// </summary>
	/// <returns><c>true</c>, if bad parameter for cmd was caught, <c>false</c> otherwise.</returns>
	/// <param name="msg">Message.</param>
	private bool	CatchBadParameterForCmd(string msg)
	{
		rgx = new Regex("^suc$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			GameManager.instance.GameController.OnBadParameterForCmd (msg);
			return (true);
		}
		return (false);
	}
}
