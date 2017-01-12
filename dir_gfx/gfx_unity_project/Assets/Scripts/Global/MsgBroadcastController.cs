using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Net.Sockets;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Message broadcast controller. Receive, parse and redirect message to corresponding objects.
/// </summary>
public class MsgBroadcastController : MonoBehaviour
{
	public WorldManager			WorldManager;
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
		WorldManager = GameObject.Find("World").gameObject.GetComponent<WorldManager> ();
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

	// Update is called once per frame
	void Update()
	{
		if (IsReceiving)
		{
			CurrentReceivedMsg = ConnectionManager.buffer_recv.getMsg (); // lexing here
			if (CurrentReceivedMsg != null) {
				//Debug.Log ("Msg received: [" + CurrentReceivedMsg + "]");
				//LexMsg ();
				ParseMsg (CurrentReceivedMsg);
			}
		}
	}

	public void LexMsg()
	{
		CurrentReceivedMsg = CurrentReceivedMsg.Trim ();
		LexedMsg = CurrentReceivedMsg.Split ("\n"[0]);
		//LexedMsg = Regex.Split (CurrentReceivedMsg, @"(?<=[\n])");
	}

	public void ParseMsg(string msg)
	{
		Debug.Log ("size : " + msg.Length + ": [" + msg + "]");

		// Check bienvenue.
		rgx = new Regex("^BIENVENUE$");
		match = rgx.Match(CurrentReceivedMsg);
		if (match.Success) {
			Debug.Log ("bienvenue message OK! Zappy server it is");
			PushMsg ("GRAPHIC\n");
		}

		// check server world size msg
		rgx = new Regex("^msz (\\d+) (\\d+)$");
		match = rgx.Match(msg);
		if (match.Success && match.Groups.Count == 3)
		{
			groups = match.Groups;
			GameManager.instance.WorldSettings.WorldX = int.Parse(groups[1].Value);
			GameManager.instance.WorldSettings.WorldY = int.Parse(groups[2].Value);

			Debug.Log ("Success - Received world size");
			OnWorldSizeReceived.Invoke();
		}

		// check server time unit msg
		rgx = new Regex("^sgt ([0-9]*\\.?[0-9]+)$");
		match = rgx.Match(msg);
		if (match.Success && match.Groups.Count == 2)
		{
			groups = match.Groups;
			GameManager.instance.WorldSettings.TimeUnit = float.Parse (groups [1].Value);

			Debug.Log ("Success - Received World Time unit");
			OnWorldTimeUnitReceived.Invoke();
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
}
