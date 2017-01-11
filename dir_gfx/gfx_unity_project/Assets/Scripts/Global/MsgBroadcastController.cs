﻿using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Net.Sockets;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Message broadcast controller. Receive, parse and send message to corresponding objects.
/// </summary>
public class MsgBroadcastController : MonoBehaviour
{
	public WorldManager		WorldManager;

	/// <summary>
	/// The ConnectionManager fires this event when it receives a msg, then put it in CurrentReceivedMsg.
	/// </summary>
	public UnityEvent		OnMsgReception;
	public string			CurrentReceivedMsg;
	public bool				HasMsgToSend;
	public string			MsgToSend;

	void Awake()
	{
		WorldManager = GameObject.Find("World").gameObject.GetComponent<WorldManager> ();
		OnMsgReception = new UnityEvent ();
	}

	// Use this for initialization
	void Start()
	{
		OnMsgReception.AddListener (OnMsgReceptionAction);
	}

	// Update is called once per frame
	void Update()
	{
		
	}

	public void OnMsgReceptionAction()
	{
		Debug.Log ("Broadcast received: [" + CurrentReceivedMsg + "]");

	}

	public void SendMsg(string msg)
	{
		CurrentReceivedMsg = msg;
		HasMsgToSend = true;
		// The message will be sent at the corresponding time,
		// decided by the select() calls.
	}
}
