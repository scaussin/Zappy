using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EscapeMenuController : MonoBehaviour {
	public GameObject	EscapeMenuContainer;
	public Button		LeaveServerBtn;
	public Button		QuitGameBtn;
	public Button		SettingsBtn;
	public Text			EscapeMenuHelpText;

	private KeyCode							escapeKey;
	private KeyCode							escapeKeyAlt;
	private bool							escapeMenuUp;

	// Use this for initialization
	void Awake () {
		EscapeMenuContainer = transform.Find ("EscapeMenuContainer").gameObject;
		LeaveServerBtn = EscapeMenuContainer.transform.Find ("LeaveServerBtn").gameObject.GetComponent<Button> ();
		QuitGameBtn = EscapeMenuContainer.transform.Find ("QuitGameBtn").gameObject.GetComponent<Button> ();
		SettingsBtn = EscapeMenuContainer.transform.Find ("SettingsBtn").gameObject.GetComponent<Button> ();
		EscapeMenuHelpText = EscapeMenuContainer.transform.Find ("EscapeMenuHelpText").gameObject.GetComponent<Text> ();

		escapeKey = GameManager.instance.KeyManager.Escape;
		escapeKeyAlt = GameManager.instance.KeyManager.EscapeAlt;
	}

	void Start()
	{
		LeaveServerBtn.onClick.AddListener (OnClickLeaveServer);
	}


	// Update is called once per frame
	void Update ()
	{
		// Check for the Escape menu panel up or not.
		if ((Input.GetKeyDown (escapeKey) || Input.GetKeyDown (escapeKeyAlt)))
		{
			if (escapeMenuUp == false)
			{
				EscapeMenuContainer.SetActive (true);
				escapeMenuUp = true;
			}
			else
			{
				EscapeMenuContainer.SetActive (false);
				escapeMenuUp = false;
			}
		}
	}

	public void CloseEscapeMenu()
	{
		EscapeMenuContainer.SetActive (false);
		escapeMenuUp = false;
	}

	public void OnClickLeaveServer()
	{
		if (GameManager.instance.GameController.InGame == true)
		{
			// call the shutdown control method by this button.
			// The GameController will handle the cleaning.
			GameManager.instance.GameController.OnServerShutdown ();
			CloseEscapeMenu ();
		}
	}

	public void OnClickQuitGame()
	{
		Application.Quit ();
	}

	public void OnClickSettings()
	{
		// nothing for now. Maybe later.
	}

	/// <summary>
	/// Raises the button hover event. Change the help text. Called by buttons subscript (need IPointerHandler interface).
	/// </summary>
	/// <param name="msg">Message.</param>
	public void OnBtnHover(string msg)
	{
		EscapeMenuHelpText.text = msg;
	}
}
