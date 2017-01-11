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
	GameObject			CameraViewControl;

	// Player states.
	public bool			InMainMenu;
	public bool			SelectingServerInfos;
	public bool			InGame;

	void Awake()
	{
		CameraViewControl = GameObject.Find ("CameraRoot");
	}

	// Use this for initialization
	void Start () {
		GameManager.instance.MainMenuController.OnServerInfoSelected.AddListener (OnServerInfoEntered);
		GameManager.instance.ConnectionManager.OnAuthentificationDone.AddListener (OnGfxAuthentifiedAction);

		// Starting state for the player:
		InMainMenu = true;
		SelectingServerInfos = true;
		InGame = false;
		DisablePlayerCameraControl ();
	}

	// Update is called once per frame
	void Update () {
		
	}

	// hostname and port selected, lets try to connect to the server ...
	public void		OnServerInfoEntered()
	{
		// desactivate menu.
		GameManager.instance.MainMenuController.gameObject.transform.Find ("MainPanel").gameObject.SetActive (false);
		InMainMenu = false;
		SelectingServerInfos = false;
		GameManager.instance.ConnectionManager.ConnectToServer (GameManager.instance.Hostname, GameManager.instance.Port);
	}


	public void		OnGfxAuthentifiedAction()
	{
		GameManager.instance.WorldManager.WorldBoardSpawner.SpawnBlocks ();
	}

	// Camera control.
	public void		DisablePlayerCameraControl()
	{
		CameraViewControl.GetComponent<CameraViewControl> ().enabled = false;
	}

	public void		EnablePlayerCameraControl()
	{
		CameraViewControl.GetComponent<CameraViewControl> ().enabled = true;
	}
}
