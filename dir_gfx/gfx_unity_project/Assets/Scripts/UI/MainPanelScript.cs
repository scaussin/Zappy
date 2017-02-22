using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


/// <summary>
/// Main panel script. Class acting for the menu at the beginning where you can enter hostname and port;
/// </summary>
public class MainPanelScript : MonoBehaviour {

	[Header("Center panel inputs")]
	public InputField   		HostnameInputField;
	public InputField   		PortInputField;
	public Text         		ResponseText;

	// input field variable helper.
	public bool        			isHostnameSet;
	public bool        			isPortSet;
	public bool        			CanEnterInput;

	[Header("Control buttons")]
	public Button				ExitBtn;
	public Button				ExternalCreditsBtn;

	// private vars
	private MainMenuController	MainMenuController;
	private Text				MainMenuHelpText;

	// Use this for initialization
	void Awake ()
	{
		// get main references.
		MainMenuController = transform.parent.GetComponent<MainMenuController> ();
		HostnameInputField = transform
			.Find("CenterAreaPanel").transform
			.Find("CenterAreaContainer").transform
			.Find("ServerInfosContainer").transform
			.Find("InputHostname").GetComponent<InputField>();
		PortInputField = transform
			.Find("CenterAreaPanel").transform
			.Find("CenterAreaContainer").transform
			.Find("ServerInfosContainer").transform
			.Find("InputPort").GetComponent<InputField>();
		ResponseText = transform
			.Find("CenterAreaPanel").transform
			.Find("CenterAreaContainer").transform
			.Find("ServerInfosContainer").transform
			.Find("ResponseText").GetComponent<Text>();
		MainMenuHelpText = transform
			.Find("CenterAreaPanel").transform
			.Find("MainMenuHelpText").GetComponent<Text>();
		isHostnameSet = false;
		isPortSet = false;

		// get btn refs
		ExitBtn = transform.Find ("ExitGameBtn").GetComponent<Button> ();
		ExternalCreditsBtn = transform
			.Find ("FooterPanel").transform.Find ("ExternalCreditsBtn").GetComponent<Button> ();
	}

	void OnEnable()
	{
		HostnameInputField.onEndEdit.AddListener(OnEndEditHostname);
		PortInputField.onEndEdit.AddListener(OnEndEditPort);
		ExitBtn.onClick.AddListener (OnClickExitBtn);
		ExternalCreditsBtn.onClick.AddListener (OnClickExternalCreditsBtn);
	}

	void OnDisable()
	{
		HostnameInputField.onEndEdit.RemoveListener(OnEndEditHostname);
		PortInputField.onEndEdit.RemoveListener(OnEndEditPort);
		ExitBtn.onClick.RemoveListener (OnClickExitBtn);
		ExternalCreditsBtn.onClick.RemoveListener (OnClickExternalCreditsBtn);
		isHostnameSet = false;
		isPortSet = false;
	}
	
	/// <summary>
	/// Linked to the hostname input field's event.
	/// </summary>
	public void OnEndEditHostname(string str)
	{
		isHostnameSet = true;
		if (HostnameInputField.text == "")
		{
			GameManager.instance.Hostname = "localhost";
		}
		else
		{
			GameManager.instance.Hostname = str;
		}
		PortInputField.Select();
	}

	/// <summary>
	/// Linked to the port input field's event.
	/// </summary>
	public void OnEndEditPort(string str)
	{
		isPortSet = true;
		if (PortInputField.text == "")
		{
			GameManager.instance.Port = "4242";
		}
		else
		{
			GameManager.instance.Port = str;
		}
	}

	// Update is called once per frame
	void Update()
	{
		if (CanEnterInput)
		{
			// press enter when nothing is selected -> try connect.
			if ((Input.GetKeyUp(KeyCode.Return) || Input.GetKeyUp(KeyCode.KeypadEnter)
				|| Input.GetKeyUp(KeyCode.Space)))
			{
				if (!isHostnameSet)
				{
					GameManager.instance.Hostname = "localhost";
				}
				if (!isPortSet)
				{
					GameManager.instance.Port = "4242";
				}
				//MainMenuController.OnServerInfoSelected.Invoke();
				GameManager.instance.GameController.OnServerInfoEntered();
			}
			// Press tab when hostname modification ? select port field.
			if (Input.GetKeyUp(KeyCode.Tab))
			{
				if (HostnameInputField.isFocused == true)
				{
					PortInputField.Select ();
				}
				else if (PortInputField.isFocused == true)
				{
					GameManager.instance.GameController.OnServerInfoEntered();
				}
			}
		}
	}

	/// <summary>
	/// Raises the button hover event. Changes the text of the help text.
	/// </summary>
	/// <param name="msg">Message.</param>
	public void OnBtnHover(string msg)
	{
		MainMenuHelpText.text = msg;
	}

	public void DesactivateMainPanelHover()
	{
		HostnameInputField.GetComponent<UIMainMenuBtn> ().enabled =  false;
		PortInputField.GetComponent<UIMainMenuBtn> ().enabled =  false;
		MainMenuHelpText.text = "";
	}

	public void ActivateMainPanelHover()
	{
		HostnameInputField.GetComponent<UIMainMenuBtn> ().enabled =  true;
		PortInputField.GetComponent<UIMainMenuBtn> ().enabled =  true;
		MainMenuHelpText.text = "";
	}

	/// <summary>
	/// Raises the click exit button event. Leaves the game.
	/// </summary>
	public void OnClickExitBtn()
	{
		if (GameManager.instance.GameController.InGame == false)
		{
			Debug.Log ("Exiting game!");
			Application.Quit ();
		}
	}

	public void OnClickExternalCreditsBtn()
	{
		if (GameManager.instance.GameController.InGame == false)
		{
			if (transform.parent.GetComponent<MainMenuController> ().CreditPanelController.CreditUp == false)
				transform.parent.GetComponent<MainMenuController> ().CreditPanelController.ActivateCreditPanel ();
			else
				transform.parent.GetComponent<MainMenuController> ().CreditPanelController.CloseCreditPanel ();
		}
	}
}
