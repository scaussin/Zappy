using UnityEngine;
using System.Collections;
using System.Text;
using System.Net;
using System.Net.Sockets;

public class ConnectionManager : MonoBehaviour {
	public TcpClient			TcpClient;
	public NetworkStream		ClientStream;

	// public
	[HideInInspector]
	public int					ServerPort;
	public bool					IsConnected = false;

	// private
	private byte[]				buffer = new byte[100];

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	/// <summary>
	/// Connects to server. Will set ConnectionManager's ClientStream to the corresponding server
	/// Every Call to ReadMsg() and SendMsg() will use the ClientStream set in this method. 
	/// </summary>
	/// <param name="port">Port.</param>
	/// <param name="hostname">Hostname.</param>
	public void		ConnectToServer(string port, string hostname)
	{
		try
		{
			// -------------- Set the connection datas.
			if (hostname.Equals("localhost"))
			{
				hostname = "127.0.0.1";
			}
			ServerPort = System.Int32.Parse(port); // convert string port to int port

			// --------------- Init Connection
			TcpClient = new TcpClient();
			Debug.Log("Connecting...");
			
			System.IAsyncResult result = TcpClient.BeginConnect(hostname, ServerPort, null, null); // actual connection to server.
			bool success = result.AsyncWaitHandle.WaitOne(System.TimeSpan.FromSeconds(1)); // setting timeout for connection.
			if (!success)
			{
				throw new System.Exception("Failed to connect.");
			}

			// -> Server connection successful
			Debug.Log("Connected to " + hostname + " on port " + ServerPort);
			IsConnected = true;
			ClientStream = TcpClient.GetStream(); // socket equivalent.

			// --------------- Server first dialog
			string serverAnswer = ReadMsg();
			Debug.Log("Server Answer: size = " + serverAnswer.Length + (serverAnswer + "a"));
			// TODO : Change startswith by strncmp equivalent.
			if (serverAnswer.StartsWith("BIENVENUE\n"))
			{
				Debug.Log("Zappy server recognized");
				Debug.Log("Sending: gfx*");
				SendMsg("gfx\n");
			}
			else
			{
				TcpClient.EndConnect(result);
				IsConnected = false;
				throw new System.Exception("Bad server?");
			}
		}
		catch(System.Exception e)
		{
			IsConnected = false;
			Debug.Log("Error" + e.ToString());
		}
	}

	public void SendMsg(string msg)
	{
		byte[] toBytes = Encoding.ASCII.GetBytes(msg);

		if (ClientStream.CanWrite)
		{
			ClientStream.Write(toBytes, 0, msg.Length);
		}
	}

	public string ReadMsg()
	{
		if (ClientStream.CanRead)
		{
			ClientStream.Read(buffer, 0, 100);
		}
		string retString = Encoding.ASCII.GetString(buffer);
		return retString;
	}
}
