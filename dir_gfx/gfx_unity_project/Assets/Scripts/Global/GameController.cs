using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Game controller. This class control the game stages.
/// This is where the events will be fired to put the game in motion
/// or where the differents manager will be accessed.
/// It's the transition box !
/// </summary>
public class GameController : MonoBehaviour {
	[Header("references to other controller")]
	GameObject			CameraViewControl;

	// Player states.
	public bool			InMainMenu;
	public bool			SelectingServerInfos;
	public bool			IsWorldSpawned;
	public bool			InGame;

	void Awake()
	{
		CameraViewControl = GameObject.Find ("CameraRoot");
	}

	// Use this for initialization
	void Start () {
		GameManager.instance.MainMenuController.OnServerInfoSelected.AddListener (OnServerInfoEntered);
        GameManager.instance.ConnectionManager.OnConnectionFailed.AddListener(OnConnectionFailedAction);
        //GameManager.instance.ConnectionManager.OnAuthentificationDone.AddListener (OnGfxAuthentifiedAction);
		GameManager.instance.MsgBroadcastController.OnWorldSizeReceived.AddListener (OnWorldSizeReceivedAction);

        // Starting state for the player:
        ActivateMainMenuInput();
    }

	// Update is called once per frame
	void Update () {
		
	}

    /// <summary>
    /// Transition facilitating method. Puts the MainMenu up, and allows the entering of input.
    /// </summary>
    public void ActivateMainMenuInput()
    {
        GameManager.instance.MainMenuController.gameObject.transform.Find("MainPanel").gameObject.SetActive(true);
        InMainMenu = true;
        SelectingServerInfos = true;
        GameManager.instance.MainMenuController.CanEnterInput = true;
        InGame = false;
        DisablePlayerCameraControl();
    }

	/// <summary>
    /// Used when MainMenuController fires the event when the user enters an address and a port and presses enter.
    /// </summary>
	public void		OnServerInfoEntered()
	{
        // desactivate menu.
        GameManager.instance.MainMenuController.CanEnterInput = false;
        GameManager.instance.MainMenuController.gameObject.transform.Find ("MainPanel").gameObject.SetActive (false);
		InMainMenu = false;
		SelectingServerInfos = false;
        GameManager.instance.ConnectionManager.ConnectToServer ();
	}

    /// <summary>
    /// Used when ConnectionManager fires the event when the connection did not work. Gets the menu back up.
    /// </summary>
    public void     OnConnectionFailedAction()
    {
        ActivateMainMenuInput();
        GameManager.instance.MainMenuController.ResponseText.color = Color.red;
        GameManager.instance.MainMenuController.ResponseText.text = "- Failed to connect to server -";
		DisablePlayerCameraControl ();
    }

    /// <summary>
    /// Called when connection is successful and the client is authentified. Starts spawning the world blocks.
    /// </summary>
	public void		OnWorldSizeReceivedAction()
	{
        CameraViewControl.GetComponent<CameraViewControl>().gameObject.SetActive(true);
        GameManager.instance.WorldManager.WorldBoardSpawner.SpawnBlocks ();
		EnablePlayerCameraControl ();
		IsWorldSpawned = true;
	}

	public void		SetWorldBlockRessources(string msg)
	{
		GameManager.instance.WorldManager.ItemSpawner.SetBlockRessources (msg);
	}

	/// <summary>
    /// Disable user's camera control. We don't want the user to be able to move at certain moments.
    /// </summary>
	public void		DisablePlayerCameraControl()
	{
		CameraViewControl.GetComponent<CameraViewControl> ().enabled = false;
	}

    /// <summary>
    /// Enable user's camera control. When called, the user can now move the camera with the corresponding keys.
    /// </summary>
	public void		EnablePlayerCameraControl()
	{
		CameraViewControl.GetComponent<CameraViewControl> ().enabled = true;
	}

	/// <summary>
	/// Called by BroadcastManager, because it will also make UI calls.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void OnNewPlayerConnection(string msg)
	{
		if (IsWorldSpawned) // we dont want player to spawn on empty ground.
		{
			GameManager.instance.WorldManager.ActorSpawner.SpawnNewPlayer (msg);
		}
		else
		{
			Debug.LogError ("Tried to spawn player on empty world?");
		}
	}
}
