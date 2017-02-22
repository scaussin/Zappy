using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

/// <summary>
/// Main menu controller. The main bridge for the GameController to access the UI of the game.
/// Here, we will have access to starting menu, game menu, end menu .... etc.
/// </summary>
public class MainMenuController : MonoBehaviour {
	/// <summary>
	/// The on time unit modified event, for when the user changes the time in the UI.
	/// </summary>
	public UnityEvent   			OnTimeUnitModified;

    // menu references;

	// global transitionning tool.
	public GameObject				BlackBackground;

	// starting menu
    public GameObject   			MainPanel;
	public MainPanelScript			MainPanelScript;
	// ingame menu
	public GameObject				InGameMenuPanel;
	public InGameMenuController		InGameMenuController;

	// escape menu
	public GameObject				EscapeMenuPanel;
	public EscapeMenuController		EscapeMenuController;

	// GameOver menu
	public GameObject				GameOverMenuPanel;
	public GameOverMenuController	GameOverMenuController;

	// Credit Menu
	public GameObject				CreditPanel;
	public CreditPanelController	CreditPanelController;

    // Use this for initialization
    void Awake()
    {
		// Set references to objects.
		BlackBackground = transform.Find ("BlackBackground").gameObject;
		MainPanel = transform.Find("MainPanel").gameObject;
		MainPanelScript = MainPanel.GetComponent<MainPanelScript> ();
		InGameMenuPanel = transform.Find ("InGameMenuPanel").gameObject;
		InGameMenuController = InGameMenuPanel.GetComponent<InGameMenuController> ();
		EscapeMenuPanel = transform.Find ("EscapeMenuPanel").gameObject;
		EscapeMenuController = EscapeMenuPanel.GetComponent<EscapeMenuController> ();
		GameOverMenuPanel = transform.Find ("GameOverMenuPanel").gameObject;
		GameOverMenuController = GameOverMenuPanel.GetComponent<GameOverMenuController> ();
		CreditPanel = transform.Find ("CreditPanel").gameObject;
		CreditPanelController = CreditPanel.GetComponent<CreditPanelController> ();

		// Clean initatialization ( for when we press play )
		MainPanel.SetActive(true);
		BlackBackground.SetActive (true);
		InGameMenuPanel.SetActive (true);
		EscapeMenuPanel.SetActive (true);
		GameOverMenuPanel.SetActive (true);
		InGameMenuController.TabPanel.SetActive (false);
    }

	/// <summary>
	/// Activates the main panel input for entering server host and port. Used by the GameController.
	/// </summary>
	public void ActivateMainPanelInput()
	{
		MainPanelScript.ActivateMainPanelHover();
		MainPanelScript.HostnameInputField.enabled = true;
		MainPanelScript.PortInputField.enabled = true;
		MainPanelScript.CanEnterInput = true;
		BlackBackground.SetActive (true);
	}

	/// <summary>
	/// Deactivates the main panel input. Used in animation when server connection successful.
	/// </summary>
	public void DeactivateMainPanelInputs()
	{
		MainPanelScript.DesactivateMainPanelHover ();
		MainPanelScript.HostnameInputField.enabled = false;
		MainPanelScript.HostnameInputField.text = "";
		MainPanelScript.PortInputField.enabled = false;
		MainPanelScript.PortInputField.text = "";
		MainPanelScript.CanEnterInput = false;
	}

	/// <summary>
	/// Deactivates the black background object. Used in animation after smooth transitionning.
	/// </summary>
	public void DeactivateBlackBackgroundObject()
	{
		BlackBackground.SetActive (false);
	}

	/// <summary>
	/// Activate the black background object. used in animation when server connection cut.
	/// </summary>
	public void ActivateBlackBackgroundObject()
	{
		BlackBackground.SetActive (true);
	}



   
}
