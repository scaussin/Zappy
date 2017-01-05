using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour {
	public static GameManager	instance = null;

	[Header("Server address")]
	public string				Port;
	public string				Hostname;

	[Header("Manager references")]
	public ConnectionManager	ConnectionManager;
	public WorldSettings		WorldSettings;


	//Awake is always called before any Start functions
	void Awake()
	{
		//Check if instance already exists
		if (instance == null)
		{
			instance = this;
		}
		//If instance already exists and it's not this:
		else if (instance != this)
		{
			Destroy (gameObject);    
		}
		//Sets this to not be destroyed when reloading scene
		DontDestroyOnLoad(gameObject);
	}

	// Use this for initialization
	void Start () {
		ConnectionManager = this.gameObject.GetComponent<ConnectionManager> ();
		WorldSettings = this.gameObject.GetComponent<WorldSettings> ();

		ConnectionManager.ConnectToServer (Port, Hostname);
		if (!ConnectionManager.IsConnected)
		{
			Debug.Log ("Error during connection to server");
		}
		else
		{
			Debug.Log ("Connected to server");
		}

		GameObject prefab = (GameObject)Resources.Load("Prefabs/Actors/Player") as GameObject;
		GameObject newplayer = (GameObject)Instantiate(prefab, transform.position, Quaternion.identity);
		newplayer.transform.parent = GameObject.Find ("World").transform;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
