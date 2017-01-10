using UnityEngine;
using System.Collections;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Text.RegularExpressions;

public class ConnectionManager : MonoBehaviour
{
    public Socket              	 ClientSocket;

	// public
	[HideInInspector]
	public int						ServerPort;
	public bool						IsConnected = false;
    public bool                 	IsAuthenticated = false;
	public bool						IsAuthMsgSend = false;

	// private
	private byte[]					buffer = new byte[100];
	private string					retString;
	private MsgBroadcastController	MsgBroadcastController;

	// Use this for initialization
	void Start () {
		MsgBroadcastController = GameManager.instance.MsgBroadcastController;
	}

	void Awake() {
		
	}
	
	/// <summary>
	/// Update this instance. 
	/// All received messages are sent to the MsgBroadcastController,
	/// except for the starting authentification dialog.
	/// </summary>
	void Update () {
	    if (IsConnected)
        {
            // Use the SelectWrite enumeration to obtain Socket status.
			if (ClientSocket.Poll(100, SelectMode.SelectRead))
            {
				// Check if server cut the connection.
				if (ClientSocket.Available == 0)
				{
					Debug.Log ("Client Disconnected from server");
					IsConnected = false;
					return ;
				}

				// Authentification process - read second message after GRAPHIC\n sent;
				if (!IsAuthenticated && IsAuthMsgSend)
				{
					string receivedMsg = ReadMsg ();
					Debug.Log ("Received: [" + receivedMsg + "]");
					AuthReception (receivedMsg); // will confirm the auth.
				}
				else
				{
					// When authentified, msg are transmitted to MsgBroadcastController for parsing.
					MsgBroadcastController.CurrentReceivedMsg = ReadMsg();
					MsgBroadcastController.OnMsgReception.Invoke ();
				}
            }
			else if (ClientSocket.Poll(100, SelectMode.SelectWrite))
			{
				// Authentification process - write part;
				if (!IsAuthenticated && !IsAuthMsgSend) {
					Debug.Log ("Send: GRAPHIC*");
					SendMsg ("GRAPHIC\n");
					IsAuthMsgSend = true;
				}
				else if (MsgBroadcastController.HasMsgToSend)
				{
					SendMsg (MsgBroadcastController.MsgToSend);
					MsgBroadcastController.HasMsgToSend = false;
				}
			}
			else if (ClientSocket.Poll(100, SelectMode.SelectError))
            {
                Debug.Log("This Socket has an error.");
            }
        }
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

            // Connect to a remote device.
            // Establish the remote endpoint for the socket.
            IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = IPAddress.Parse(hostname);
            IPEndPoint remoteEP = new IPEndPoint(ipAddress, ServerPort);

            // Create a TCP/IP  socket.
            ClientSocket = new Socket(AddressFamily.InterNetwork,
            SocketType.Stream, ProtocolType.Tcp);

            ClientSocket.Connect(remoteEP);
            if (!ClientSocket.Connected)
            {
                Debug.Log("Unable to connect to host");
            }
			ClientSocket.ReceiveTimeout = 1000;

            Debug.Log("Socket connected to " + ClientSocket.RemoteEndPoint.ToString());
			if (ClientSocket.Poll(1000, SelectMode.SelectRead))
			{
				string ReceivedMsg = ReadMsg();
				Debug.Log("Receiveid: " + ReceivedMsg);
			}
			IsConnected = true;
        }
        catch (System.Exception e)
        {
            IsConnected = false;
            Debug.Log("Error" + e.ToString());
        }


    }

	private void AuthReception(string receivedMsg)
	{
		// check if server sent world size
		Regex rgx = new Regex("^msz \\d+ \\d+");
		Match match = rgx.Match(receivedMsg);
		if (match.Success) {
			Debug.Log ("Success - Received world size - Gfx authentified");
			IsAuthenticated = true;

			// fill world size settings
			string[] arr = receivedMsg.Split(" "[0]);
			GameManager.instance.WorldSettings.WorldX = int.Parse(arr[1]);
			GameManager.instance.WorldSettings.WorldY = int.Parse(arr[2]);
		}
		else
		{
			Debug.Log ("Failure - Received world size incorrect");
		}
	}
    
	// to send msg to server, use the MsgBroadcastController. Because the msg must be sent
	// according to the select status.
    private void SendMsg(string msg)
	{
		byte[] toBytes = Encoding.ASCII.GetBytes(msg);
        ClientSocket.Send(toBytes);
	}

	private string ReadMsg()
	{
		ClientSocket.Receive(buffer);
		retString = Encoding.ASCII.GetString(buffer);
		return retString;
	}
}

/*
{
	/// ----------- ASYNCHRONOUS CONNECTION ---- //

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
		Debug.Log("Sending: GRAPHIC*");
		SendMsg("GRAPHIC\n");
	}
	else
	{
		TcpClient.EndConnect(result);
		IsConnected = false;
		throw new System.Exception("Bad server?");
	}
	catch (System.Exception e)
	{
		IsConnected = false;
		Debug.Log("Error" + e.ToString());
	}
}

*/