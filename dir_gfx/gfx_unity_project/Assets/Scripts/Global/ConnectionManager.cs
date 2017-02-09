using UnityEngine;
using UnityEngine.Events;
using System.Collections;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Text.RegularExpressions;

public class ConnectionManager : MonoBehaviour
{
	/// <summary>
	/// The only client socket. Is connected to the server with hostname and port.
	/// </summary>
    public Socket              	 		ClientSocket;

	/// <summary>
	/// Event invoked when the client is connected to the server and authentified as zappy-gfx connection.
	/// </summary>
	//public UnityEvent					OnAuthentificationDone;

	/// <summary>
	/// Invoked when the client is connected to the server, not yet authentified. (server may not be the correct one) 
	/// </summary>
	public UnityEvent					OnConnectionWithServer;

    /// <summary>
	/// Invoked when the client could not connect to the server. Will allow the GameController to reput the menu in front.
	/// </summary>
    public UnityEvent                   OnConnectionFailed;

    // public
    [HideInInspector]
	public int							ServerPort;
	public bool							IsConnected = false;
    public bool                 		IsAuthenticated = false;
	public bool							IsAuthMsgSend = false;

	// private
	public int							msg_size = 2048;
	public CircularBuffer				buffer_recv = new CircularBuffer();
	public CircularBuffer				buffer_send = new CircularBuffer();

	//private byte[]					buffer = new byte[4096];
	private string						retString;

	void Awake()
	{
		
	}

	// Use this for initialization
	void Start()
	{
		
	}
	
	/// <summary>
	/// Update this instance. 
	/// All received messages are sent to the MsgBroadcastController.
	/// </summary>
	void Update()
	{
	    if (IsConnected)
        {
            // Use the SelectWrite enumeration to obtain Socket status.
			if (ClientSocket.Poll(100, SelectMode.SelectRead))
            {
				// Check if server cut the connection.
				if (ClientSocket.Available == 0)
				{
					Debug.Log ("Client Disconnected from server");
					GameManager.instance.GameController.OnServerShutdown ();
					IsConnected = false;
					return ;
				}
				ReadMsg ();
            }
			else if (ClientSocket.Poll(100, SelectMode.SelectWrite))
			{
				SendMsg ();
			}
			else if (ClientSocket.Poll(100, SelectMode.SelectError))
            {
                Debug.Log("This Socket has an error.");
            }
        }
	}

	// to send msg to server, use ConnectionManager.buffer_send.pushMsg(string);
	// the function down there will send at the appropriated moment.
	private void SendMsg()
	{
		int		sizeToSend;
		byte[]	bytesToSend;

		if ((sizeToSend = buffer_send.getLen()) > 0)
		{
			bytesToSend = buffer_send.ExtractBufferBytes ();
			ClientSocket.Send(bytesToSend, 0, sizeToSend, SocketFlags.None);
			Debug.Log ("Sending: [" + System.Text.Encoding.UTF8.GetString(bytesToSend) + "]");
		}
	}

	private void ReadMsg()
	{
		int					ret;
		byte[]				buffer = new byte[msg_size];

		ret = ClientSocket.Receive(buffer, 0, msg_size, SocketFlags.None);
		Debug.Log ("Received: [" + System.Text.Encoding.UTF8.GetString (buffer) + "]");
		buffer_recv.pushBytes (buffer, ret);

		return ;
	}

	/// <summary>
	/// Connects to server. Will set ConnectionManager's ClientSocket to the corresponding server
	/// Every Call to ReadMsg() and SendMsg() will use the ClientSocket set in this method.
	/// Also send event calls on Connection with server, and authentification success with server.
	/// </summary>
	/// <param name="port">Port.</param>
	/// <param name="hostname">Hostname.</param>
	public void		ConnectToServer()
	{
        // Get server info from GM.
        string port = GameManager.instance.Port;
        string hostname = GameManager.instance.Hostname;

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
            // IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = IPAddress.Parse(hostname);
            IPEndPoint remoteEP = new IPEndPoint(ipAddress, ServerPort);

            // Create a TCP/IP  socket.
            ClientSocket = new Socket(AddressFamily.InterNetwork,
            SocketType.Stream, ProtocolType.Tcp);

            ClientSocket.Connect(remoteEP);
            if (!ClientSocket.Connected)
            {
                Debug.Log("Unable to connect to host");
                IsConnected = false;
                OnConnectionFailed.Invoke();
            }
			ClientSocket.ReceiveTimeout = 1000;

            Debug.Log("Socket connected to " + ClientSocket.RemoteEndPoint.ToString());
			OnConnectionWithServer.Invoke();
			IsConnected = true;
        }
        catch (System.Exception e)
        {
            IsConnected = false;
            OnConnectionFailed.Invoke();
            Debug.Log("Error" + e.ToString());
        }


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