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

	public UnityEvent			OnWorldSizeReceived;

	public UnityEvent			OnWorldTimeUnitReceived;

	public string				CurrentReceivedMsg;
	public bool					HasMsgToSend;
	public string				MsgToSend;

	private string[]			LexedMsg;
	private Regex				rgx;
	private Match				match;
	private GroupCollection		groups;

	public bool					IsReceiving;


	void Awake()
	{
		ConnectionManager = GameManager.instance.ConnectionManager;

		OnWorldSizeReceived =  new UnityEvent ();
		OnWorldTimeUnitReceived = new UnityEvent ();
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
		Debug.Log ("size : " + msg.Length + ": [" + msg + "]");

		// We dont want to execute every regex parsing method when we found the one we needed.
		// put the most likely receivable msg at the top.
		if (CatchBienvenue (msg) ||
			CatchWorldSize (msg) ||
			CatchServerTimeUnit (msg) ||
			CatchCaseContent (msg) ||
			CatchTeamName (msg) ||
			CatchPlayerConnection(msg) ||
			CatchPlayerMovement(msg))
		{
			return;
		}



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
			OnWorldSizeReceived.Invoke(); // call back to GameController, cause the timing is undetermined.
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
			groups = match.Groups;
			GameManager.instance.WorldSettings.TimeUnit = float.Parse (groups [1].Value);
			Time.timeScale *= (float) (1.0f / GameManager.instance.WorldSettings.TimeUnit);
			Debug.Log ("Success - Received World Time unit");
			OnWorldTimeUnitReceived.Invoke();
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
		rgx = new Regex("^tna (\\w+)$");
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
		rgx = new Regex("^pnw \\d+ \\d+ \\d+ \\d+ \\d+ \\w+$");
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
		rgx = new Regex("^ppo \\d+ \\d+ \\d+ \\d+$");
		match = rgx.Match(msg);
		if (match.Success)
		{
			// Use the game controller for action cause it may call other things.
			GameManager.instance.GameController.OnPlayerMovement(msg);
			return (true);
		}
		return (false);
	}


}
