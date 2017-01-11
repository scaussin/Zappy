using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

public class MainMenuController : MonoBehaviour {
	// something happening events.
	/// <summary>
	/// Will be invoked when user press enter with infos entered in the server info inputfields.
	/// </summary>
	public UnityEvent	OnServerInfoSelected;

	// menu references;
	public InputField	HostnameInputField;
	public InputField	PortInputField;
	public Text			ResponseText;
	public GameObject	MainPanel;


	// Use this for initialization
	void Awake () {
		MainPanel = transform.Find ("MainPanel").gameObject;

		HostnameInputField = MainPanel.transform
			.Find ("CenterAreaPanel").transform
			.Find("InputContainer").transform
			.Find("InputHostname").GetComponent<InputField> ();
		PortInputField = MainPanel.transform
			.Find ("CenterAreaPanel").transform
			.Find("InputContainer").transform
			.Find("InputPort").GetComponent<InputField> ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
