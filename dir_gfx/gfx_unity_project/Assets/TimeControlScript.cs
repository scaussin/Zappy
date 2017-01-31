using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Time control script. Controls the time changing through the UI user inputs.
/// </summary>
public class TimeControlScript : MonoBehaviour {
	public InputField	TimeInputField;

	private Text		serverTValue;
	private int			timeDisplay;
	private int			timeValueCheck;
	private float		fTimeValueCheck;

	// Use this for initialization
	void Awake () {
		TimeInputField = transform.Find ("TimeInputField").GetComponent<InputField> ();
		GameManager.instance.GameController.OnTimeUnitModified.AddListener (UpdateUITimeValue);
		TimeInputField.onEndEdit.AddListener (ChangeTimeValueTroughUI);
		serverTValue = transform.Find ("ServerTValue").GetComponent<Text> ();
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

	}

	public void OnPressTimePlus()
	{

	}
}
