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

	// optimization references
	private PlayerController	PlayerControllerScript;

	void Awake()
	{
		OnTimeUnitModified = new UnityEvent ();
		CameraViewControl = GameObject.Find ("CameraRoot");

	}

	// Use this for initialization
	void Start () {
		PlayerControllerScript = GameManager.instance.WorldManager.PlayerController;
		//GameManager.instance.MainMenuController.OnServerInfoSelected.AddListener (OnServerInfoEntered);
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
		GameManager.instance.MainMenuController.MainPanelScript.CanEnterInput = false;
		GameManager.instance.MainMenuController.DeactivateMainPanelInputs ();
		InMainMenu = false;
		SelectingServerInfos = false;
		GameManager.instance.MainMenuController.MainPanelScript.isHostnameSet = false;
		GameManager.instance.MainMenuController.MainPanelScript.isPortSet = false;
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
		GameManager.instance.MainMenuController.MainPanelScript.isHostnameSet = false;
		GameManager.instance.MainMenuController.MainPanelScript.isPortSet = false;
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
	///  -----------> equals ON GAME START 
    /// </summary>
	public void		OnWorldSizeReceivedAction()
	{
        CameraViewControl.GetComponent<CameraViewControl>().gameObject.SetActive(true);
        GameManager.instance.WorldManager.WorldBoardSpawner.SpawnBlocks ();
		OnWorldSizeReceived.Invoke ();
		EnablePlayerCameraControl ();
		IsWorldSpawned = true;
		InGame = true;

		// activate analyzer tool;
		GameManager.instance.MainMenuController.InGameMenuController.
		AnalyzerToolPanel.
		GetComponent<UIAnalyzerToolPanelScript> ().CanSelect = true;
		// active time control tool
		GameManager.instance.MainMenuController.InGameMenuController.
		TimeControlPanel.SetActive (true);
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
		// ----- Visual cleaning 
		ActivateMainMenuInput();
		// deactivate analyzer tool;
		GameManager.instance.MainMenuController.InGameMenuController.
			AnalyzerToolPanel.
			GetComponent<UIAnalyzerToolPanelScript> ().CanSelect = false;
		GameManager.instance.MainMenuController.InGameMenuController.
		AnalyzerToolPanel.GetComponent<UIAnalyzerToolPanelScript> ().DesactivateAnalyzerTool ();

		// hide and desactivate ui time control.
		GameManager.instance.MainMenuController.InGameMenuController.HideTimeControlMenu();
		// set the menu animation in motion.
		GameManager.instance.MainMenuController.gameObject.GetComponent<Animator> ().SetTrigger ("BackToMenu");

		// ----- Data cleaning.
		InGame = false;
		GameManager.instance.ConnectionManager.buffer_send.ResetBuffer ();
		GameManager.instance.WorldManager.PlayerController.CleanMapOfPlayers ();
		GameManager.instance.PlayerManager.CleanPlayerManager ();
		GameManager.instance.WorldManager.WorldBoardSpawner.DeleteWorld ();

		// set Main menu UI message.
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.color = Color.red;
		GameManager.instance.MainMenuController.MainPanelScript.ResponseText.text = "- Connection to server lost-";
		DisablePlayerCameraControl ();
	}

	/// <summary>
	/// Called when a team has won.
	/// </summary>
	public void	OnGameOver(string msg)
	{
		GameManager.instance.MainMenuController.GameOverMenuController.ActivateGameOver (msg);
	}

	public void OnUnknownCmdReception(string msg)
	{
		Debug.LogError ("Server received an unknown command");
	}

	public void OnBadParameterForCmd(string msg)
	{
		Debug.LogError ("Server received a bad parameter for a command");
	}

/* *************************************************************************************** 	*
 * 																							*		
 *	Game events Methods																		*
 *	All these methods may or not call other components to activate.							*
 * 																							*
 * ***************************************************************************************	*/

	/// <summary>
	/// Turns the off all animators. Optimization method for high time scale use.
	/// </summary>
	public void TurnOffAllAnimator()
	{
		foreach (PlayerObject player in GameManager.instance.PlayerManager.ConnectedPlayers) {
			player.gameObject.GetComponent<Animator> ().enabled = false;
		}
	}

	public void TurnOnAllAnimator()
	{
		foreach (PlayerObject player in GameManager.instance.PlayerManager.ConnectedPlayers) {
			player.gameObject.GetComponent<Animator> ().enabled = true;
		}
	}

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
			GameManager.instance.StatisticManager.NbOfConnections += 1;
		}
		else
		{
			Debug.LogError ("Tried to spawn player on empty world?");
		}
	}

	public void OnPlayerMovement(string msg)
	{
		PlayerControllerScript.MovePlayer (msg);
		GameManager.instance.StatisticManager.MoveActionNb += 1;
	}

	// Also handle player disconnection.
	public void OnPlayerDeath(string msg)
	{
		PlayerControllerScript.KillPlayer (msg);
		GameManager.instance.StatisticManager.DeathNumber += 1;
	}

	public void OnPlayerLevelReception(string msg)
	{
		PlayerControllerScript.SetPlayerLevel (msg);
	}

	public void OnPlayerInventoryReception(string msg)
	{
		PlayerControllerScript.SetPlayerInventory (msg);
	}

	public void OnPlayerExpulseReception(string msg)
	{
		PlayerControllerScript.PlayerExpulse (msg);
	}

	public void OnPlayerBroadcast(string msg)
	{
		PlayerControllerScript.PlayerBroadcast (msg);
		GameManager.instance.StatisticManager.BroadcastNb += 1;
	}

	public void OnPlayerIncantation(string msg)
	{
		PlayerControllerScript.IncantationStart (msg);
		GameManager.instance.StatisticManager.IncantationCount += 1;
	}

	public void OnIncantationEnd(string msg)
	{
		PlayerControllerScript.IncantationEnd (msg);
	}

	public void OnPlayerDropRessource(string msg)
	{
		PlayerControllerScript.PlayerDropRessource (msg);
		GameManager.instance.StatisticManager.DropNb += 1;
	}

	public void OnPlayerTakeRessource(string msg)
	{
		PlayerControllerScript.PlayerTakeRessource (msg);
		GameManager.instance.StatisticManager.PickUpNb += 1;
	}

	public void OnPlayerStartLayEgg(string msg)
	{
		PlayerControllerScript.PlayerStartsLaying (msg);
	}

	public void OnPlayerLayedEgg(string msg)
	{
		PlayerControllerScript.PlayerFinishEggLaying (msg);
		GameManager.instance.StatisticManager.EggCount += 1;
	}

	public void OnEggHatched(string msg)
	{
		PlayerControllerScript.HatchEgg (msg);
	}

	public void OnEggPlayerConnection(string msg)
	{
		PlayerControllerScript.EggPlayerConnection (msg);
	}

	public void OnHatchedEggDeath(string msg)
	{
		PlayerControllerScript.EggDeath (msg);
	}
}
