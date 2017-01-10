using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour {
	public static GameManager		instance = null;

	[Header("Server address")]
	public string					Port;
	public string					Hostname;

	[Header("Data Manager references")] // ==> dictionnaries.
	public ConnectionManager		ConnectionManager;
	public WorldSettings			WorldSettings;
	public KeyManager				KeyManager;



	[Header("Program Control Manager references")] // program actors.
	public GameController			GameController;
	public MsgBroadcastController	MsgBroadcastController;
	public WorldManager				WorldManager;

	void Awake()
	{
		// Singleton GameManager pattern.
		if (instance == null)
		{
			instance = this;
		}
		else if (instance != this)
		{
			Destroy (gameObject);    
		}
		DontDestroyOnLoad(gameObject);

		// Set data manager references.
		ConnectionManager = this.gameObject.GetComponent<ConnectionManager> ();
		WorldSettings = this.gameObject.GetComponent<WorldSettings> ();
		KeyManager = this.gameObject.GetComponent<KeyManager> ();

		// Set program control manager references.
		GameController = GameObject.Find("GameController").GetComponent<GameController> ();
		MsgBroadcastController = GameObject.Find("MsgBroadcastController").gameObject.GetComponent<MsgBroadcastController> ();
		WorldManager = GameObject.Find("World").gameObject.GetComponent<WorldManager> ();
	}

	// Use this for initialization
	void Start () {
		ConnectionManager.ConnectToServer (Port, Hostname);
		if (!ConnectionManager.IsConnected)
		{
			Debug.Log ("Error during connection to server");
		}
		else
		{
			Debug.Log ("Connected to server");
		}

//		GameObject prefab = (GameObject)Resources.Load("Prefabs/Actors/Player") as GameObject;
//		GameObject newplayer = (GameObject)Instantiate(prefab, transform.position, Quaternion.identity);
//		newplayer.transform.parent = GameObject.Find ("World").transform;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
