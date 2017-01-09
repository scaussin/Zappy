using UnityEngine;
using System.Collections;
using System.Text;
using System.Net;
using System.Net.Sockets;

public class ConnectionManager : MonoBehaviour {
    //public TcpClient			TcpClient;
    //public NetworkStream		ClientStream;
    public Socket               ClientSocket;

	// public
	[HideInInspector]
	public int					ServerPort;
	public bool					IsConnected = false;
    public bool                 IsOnZappyServer = false;

	// private
	private byte[]				buffer = new byte[100];

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	    if (IsConnected)
        {
            // Use the SelectWrite enumeration to obtain Socket status.
            if (ClientSocket.Poll(-1, SelectMode.SelectWrite))
            {
                Debug.Log("This Socket is writable.");
            }
            else if (ClientSocket.Poll(-1, SelectMode.SelectRead))
            {
                Debug.Log("This Socket is readable.");
                string ReceivedMsg = ReadMsg();
                Debug.Log("Receiveid: " + ReceivedMsg);

            }
            else if (ClientSocket.Poll(-1, SelectMode.SelectError))
            {
                Debug.Log("This Socket has an error.");
            }
            else
            {
                Debug.Log("This Socket is waiting ....");
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
            
            Debug.Log("Socket connected to " + ClientSocket.RemoteEndPoint.ToString());

            // -------- Receive BIENVENUE\n ------//
            // Receive the response from the remote device.
            string servAnswer =  ReadMsg();
            Debug.Log("Sever sent:" + servAnswer);

            // TODO : Change startswith by strncmp equivalent.
            if (servAnswer.StartsWith("BIENVENUE\n"))
            {
                Debug.Log("Zappy server recognized");
                Debug.Log("Sending: GRAPHIC*");
                SendMsg("GRAPHIC\n");
                IsConnected = true;
            }
            else
            {
                ClientSocket.Disconnect(true);
                IsConnected = false;
                throw new System.Exception("Bad server?");
            }
        }
        catch (System.Exception e)
        {
            IsConnected = false;
            Debug.Log("Error" + e.ToString());
        }

        /// ----------- ASYNCHRONOUS CONNECTION ---- //
        /*
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
    }
    
    public void SendMsg(string msg)
	{
		byte[] toBytes = Encoding.ASCII.GetBytes(msg);
        ClientSocket.Send(toBytes);
	}

	public string ReadMsg()
	{
        string retString = "";
        while ((ClientSocket.Receive(buffer)) != 0)
        {
            retString = retString + Encoding.ASCII.GetString(buffer);
        }
		return retString;
	}
}
