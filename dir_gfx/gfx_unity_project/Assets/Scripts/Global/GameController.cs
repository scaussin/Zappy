using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
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

	// Events. May be useful to stand alone modules.
	public UnityEvent	OnTimeUnitModified;
	public UnityEvent	OnWorldSizeReceived;

	void Awake()
	{
		OnTimeUnitModified = new UnityEvent ();
		CameraViewControl = GameObject.Find ("CameraRoot");
	}

	// Use this for initialization
	void Start () {
		GameManager.instance.MainMenuController.OnServerInfoSelected.AddListener (OnServerInfoEntered);
        GameManager.instance.ConnectionManager.OnConnectionFailed.AddListener(OnConnectionFailedAction);
		GameManager.instance.ConnectionManager.OnConnectionWithServer.AddListener (OnConnectionWithServerAction);

        // Starting state for the player:
        ActivateMainMenuInput();
    }

	// Update is called once per frame
	void Update () {
		
	}

/* *************************************************************************************** 	*
 * 																							*		
 *	Following: Global Control methods														*
 *	These methods either control global game states (such as the start menu) or				*
 *	do action on a lot (or all) of the game elements.										*
 * 																							*
 * ***************************************************************************************	*/
    /// <summary>
    /// Transition facilitating method. Puts the MainMenu up, and allows the entering of input.
    /// </summary>
    public void		ActivateMainMenuInput()
    {
        InMainMenu = true;
        SelectingServerInfos = true;
		GameManager.instance.MainMenuController.ActivateMainPanelInput ();
        InGame = false;
        DisablePlayerCameraControl();
    }

	/// <summary>
    /// Used when MainMenuController fires the event when the user enters an address and a port and presses enter.
    /// </summary>
	public void		OnServerInfoEntered()
	{
        // desactivate menu.
		//GameManager.instance.MainMenuController.DeactivateMainPanelInput ();

		GameManager.instance.MainMenuController.MainPanelScript.CanEnterInput = false;
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
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.color = Color.red;
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.text = "- Failed to connect to server -";
		DisablePlayerCameraControl ();
	}

    /// <summary>
    /// Used when ConnectionManager fires the event when the connection did not work. Gets the menu back up.
    /// </summary>
	public void     OnConnectionWithServerAction()
    {
		GameManager.instance.MainMenuController.gameObject.GetComponent<Animator> ().SetTrigger ("GameStart");
    }

    /// <summary>
    /// Called when connection is successful and the client is authentified. Starts spawning the world blocks.
    /// </summary>
	public void		OnWorldSizeReceivedAction()
	{
        CameraViewControl.GetComponent<CameraViewControl>().gameObject.SetActive(true);
        GameManager.instance.WorldManager.WorldBoardSpawner.SpawnBlocks ();
		OnWorldSizeReceived.Invoke ();
		EnablePlayerCameraControl ();
		IsWorldSpawned = true;
		InGame = true;
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

	public void OnServerMessageReception(string msg)
	{
		Regex rgx = new Regex("^smg (\\w+)$");
		Match match = rgx.Match(msg);
		if (match.Success)
		{
			GroupCollection groups = match.Groups;
			Debug.Log("Server message: " + groups [1].Value);
			if ((groups [1].Value).Equals ("SHUTDOWN"))
			{
				Debug.Log ("RECEIVED SERVER SHUTDOWN");
				OnServerShutdown ();
			}
		}
	}

	/// <summary>
	/// Called when the connection with the server is down or the serveur sent shutdown signal.
	/// </summary>
	public void OnServerShutdown()
	{
		ActivateMainMenuInput();
		GameManager.instance.MainMenuController.gameObject.GetComponent<Animator> ().SetTrigger ("BackToMenu");
		GameManager.instance.WorldManager.WorldBoardSpawner.DeleteWorld ();
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.color = Color.red;
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.text = "- Connection to server lost-";
		DisablePlayerCameraControl ();
	}

	/// <summary>
	/// Called when a team has won, or all players are dead.
	/// </summary>
	public void	OnGameOver(string msg)
	{
		Debug.Log ("Received Game over, but its not yet implemented");
	}

	public void OnUnknownCmdReception(string msg)
	{
		Debug.Log ("Received an unknown command");
		// TODO: think about this, maybe we should ask for some infos
		// 		 from server?
	}

	public void OnBadParameterForCmd(string msg)
	{
		Debug.Log ("Received a bad parameter for a command");
		// TODO: think about this, maybe we should ask for some infos
		// 		 from server?
	}

/* *************************************************************************************** 	*
 * 																							*		
 *	Game events Methods																		*
 *	All these methods may or not call other component to activate.							*
 * 																							*
 * ***************************************************************************************	*/

	public void OnTimeUnitReceived(string msg)
	{
		// At this point, the time has been modified in GameManager.instance.WorldSettings.
		OnTimeUnitModified.Invoke ();
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

	public void OnPlayerMovement(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.MovePlayer (msg);
	}

	// Also handle player disconnection.
	public void OnPlayerDeath(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.KillPlayer (msg);
	}

	public void OnPlayerLevelReception(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.SetPlayerLevel (msg);
	}

	public void OnPlayerInventoryReception(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.SetPlayerInventory (msg);
	}

	public void OnPlayerExpulseReception(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.PlayerExpulse (msg);
	}

	public void OnPlayerBroadcast(string msg)
	{
		Debug.Log ("Received broadcast cmd, but its not yet implemented");
		// TODO: think about the broadcast system. Spawn an object? make a particle effect?
		// 		  need to be defined.
	}

	public void OnPlayerIncantation(string msg)
	{
		Debug.Log ("Received incantation cmd, but its not yet implemented");
		// TODO: think about the incantation system.
	}

	public void OnIncantationEnd(string msg)
	{
		Debug.Log ("Received incantation end, but its not yet implemented");
		// TODO: think about the incantation system.
	}

	public void OnPlayerStartLayEgg(string msg)
	{
		Debug.Log ("Received Player starts lay egg, but its not yet implemented");
	}

	public void OnPlayerDropRessource(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.PlayerDropRessource (msg);
	}

	public void OnPlayerTakeRessource(string msg)
	{
		GameManager.instance.WorldManager.PlayerController.PlayerDropRessource (msg);
	}

	public void OnPlayerLayedEgg(string msg)
	{
		Debug.Log ("Received Player layed egg, but its not yet implemented");
	}

	public void OnEggHatched(string msg)
	{
		Debug.Log ("Received egg hatched, but its not yet implemented");
	}

	public void OnEggPlayerConnection(string msg)
	{
		Debug.Log ("Received egg player connection, but its not yet implemented");
	}

	public void OnHatchedEggDeath(string msg)
	{
		Debug.Log ("Received hatched egg death, but its not yet implemented");
	}
}
