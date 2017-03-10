using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Time control script. Controls the time changing through the UI user inputs.
/// </summary>
public class TimeControlScript : MonoBehaviour {
	public InputField	TimeInputField;
	public Button		LessBtn;
	public Button		PlusBtn;

	private Text		serverTValue;
	private int			timeDisplay;
	private int			timeValueCheck;
	private float		fTimeValueCheck;

	// Use this for initialization
	void Awake () {
		// Set references to UI items.
		TimeInputField = transform.Find ("TimeInputField").GetComponent<InputField> ();
		serverTValue = transform.Find ("ServerTValue").GetComponent<Text> ();
		LessBtn = transform.Find ("TimeLessBtn").GetComponent<Button> ();
		PlusBtn = transform.Find ("TimePlusBtn").GetComponent<Button> ();

		// Set event callback on world receive time unit.
		GameManager.instance.GameController.OnTimeUnitModified.AddListener (UpdateUITimeValue);
		// Set event catch on UI components.
		TimeInputField.onEndEdit.AddListener (ChangeTimeValueTroughUI);
		LessBtn.onClick.AddListener (OnPressTimeLess);
		PlusBtn.onClick.AddListener (OnPressTimePlus);
	}

	void OnEnable()
	{
		
	}

	// Update is called once per frame
	void Update () {
		
	}

	public void UpdateUITimeValue()
	{
		timeDisplay = Mathf.RoundToInt(1.0f / GameManager.instance.WorldSettings.TimeUnit);
		serverTValue.text = GameManager.instance.WorldSettings.TimeUnit.ToString ();
		TimeInputField.text = timeDisplay.ToString();
	}

	public void ChangeTimeValueTroughUI(string val)
	{
		if (float.TryParse(val, out fTimeValueCheck))
		{
			timeValueCheck = Mathf.RoundToInt (float.Parse (val));
			if (timeValueCheck < 0)
			{
				Debug.Log ("Incorrect value to send to server");
			}
			else
			{
				GameManager.instance.MsgBroadcastController.PushMsg ("sst " + val + "\n");
			}
		}
		else
		{
			Debug.Log ("Incorrect value to send to server");
		}
	}

	public void OnPressTimeLess()
	{
		timeDisplay = Mathf.RoundToInt(1.0f / GameManager.instance.WorldSettings.TimeUnit);
		timeDisplay -= 1;
		if (timeDisplay > 0)
		{
			ChangeTimeValueTroughUI (timeDisplay.ToString ());
		}
	}

	public void OnPressTimePlus()
	{
		timeDisplay = Mathf.RoundToInt(1.0f / GameManager.instance.WorldSettings.TimeUnit);
		timeDisplay += 1;
		// boundary check is done by server.
		ChangeTimeValueTroughUI (timeDisplay.ToString ());
	}

	/// <summary>
	/// Desactivates the time control panel. For animation call.
	/// </summary>
	public void DesactivateTimeControlPanel()
	{
		this.gameObject.SetActive (false);
	}
}
