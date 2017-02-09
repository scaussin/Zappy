using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIAnalyzerToolPanelScript : MonoBehaviour
{
	[Header("The clickables layers")]
	public LayerMask	AnalyzerRaycastMask;

	[Header("The currently Analyzed Object")]
	public GameObject	AnalyzedObject;

	[Header("Bool For input blocking")]
	public bool					CanSelect;
	[Header("Server communication")]
	public float				SecondsBetweenServerCalls = 0.8f;

	[Header("Analyzer data references")]
	// quick ref accesses.
	public GameObject			AnalyzerToolContainer;
	public GameObject			AnalyzerDataContainer;

	// data refs.
	public Image				AnalyzerVisual;
	public Text					NumberValue;
	public Text					Position_x_val;
	public Text					Position_y_val;
	public Text					LevelValue;
	public Text					FoodUnitsValue;
	public Text					LinemateValue;
	public Text					DeraumereValue;
	public Text					SiburValue;
	public Text					MendianeValue;
	public Text					PhirasValue;
	public Text					ThystameValue;

	[Header("Stocked content references")]
	public Sprite				PlayerVisual;
	public Sprite				BlockVisual;

	// Used for calculations.
	private Ray					ray;
	private RaycastHit			hit;
	private ColliderBroadcast	colBroadcast;
	private ColliderBroadcast	previousColBroadcast;

	// private update references
	private PlayerObject		analyzedPlayerScript;
	private BlockObject			analyzedBlockScript;

	void Awake()
	{
		AnalyzerToolContainer = transform.Find ("AnalyzerToolContainer").gameObject;
		AnalyzerDataContainer = AnalyzerToolContainer.transform.Find ("AnalyzerDataContainer").gameObject;
		// set analyzer data references for updating.
		AnalyzerVisual = AnalyzerToolContainer.transform.Find("AnalyzerPicturePanel").transform.Find("AnalyzerVisual").gameObject.GetComponent<Image> ();
		NumberValue = AnalyzerDataContainer.transform.Find("NumberValue").gameObject.GetComponent<Text> ();
		Position_x_val = AnalyzerDataContainer.transform.Find("Position_x_val").gameObject.GetComponent<Text> ();
		Position_y_val = AnalyzerDataContainer.transform.Find("Position_y_val").gameObject.GetComponent<Text> ();
		LevelValue = AnalyzerDataContainer.transform.Find("LevelValue").gameObject.GetComponent<Text> ();
		FoodUnitsValue = AnalyzerDataContainer.transform.Find("FoodUnitsValue").gameObject.GetComponent<Text> ();
		LinemateValue = AnalyzerDataContainer.transform.Find("LinemateValue").gameObject.GetComponent<Text> ();
		DeraumereValue = AnalyzerDataContainer.transform.Find("DeraumereValue").gameObject.GetComponent<Text> ();
		SiburValue = AnalyzerDataContainer.transform.Find("SiburValue").gameObject.GetComponent<Text> ();
		MendianeValue = AnalyzerDataContainer.transform.Find("MendianeValue").gameObject.GetComponent<Text> ();
		PhirasValue = AnalyzerDataContainer.transform.Find("PhirasValue").gameObject.GetComponent<Text> ();
		ThystameValue = AnalyzerDataContainer.transform.Find("ThystameValue").gameObject.GetComponent<Text> ();
	}
		
	void OnEnable () {
		CanSelect = false;
		AnalyzedObject = null;
		// let's fetch the visuals right now.
		PlayerVisual = Resources.Load<Sprite> ("UI/AnalyzerVisuals/analyzer_player");
		BlockVisual = Resources.Load<Sprite> ("UI/AnalyzerVisuals/analyzer_block");
		StartCoroutine ("AnalyzerCommunicationDisplayRoutine");
	}

	void OnDisable()
	{
		StopCoroutine("AnalyzerCommunicationDisplayRoutine");
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (CanSelect && Input.GetMouseButtonUp (0))
		{
			// ray cast for click on screen.
			ray = Camera.main.ScreenPointToRay (Input.mousePosition);
			if (Physics.Raycast (ray, out hit, 200.0f, AnalyzerRaycastMask))
			{
				previousColBroadcast = colBroadcast;
				if ((colBroadcast = hit.collider.gameObject.GetComponent<ColliderBroadcast> ()))
				{
					// something touched.
					Debug.Log ("Clicked on " + colBroadcast.ObjectRoot.name);
					// we only update if it is a new object.
					if (AnalyzedObject != colBroadcast.ObjectRoot)
					{
						DisengageSelectionDisplay ();
						AnalyzedObject = colBroadcast.ObjectRoot;
						EngagePanelDisplay ();
					}
				}
				else
				{
					// clicked on something not in my colliding matrix, make ui disappear.
					previousColBroadcast = colBroadcast;
					DisengageSelectionDisplay ();
					AnalyzerToolContainer.GetComponent<Animator> ().SetTrigger ("Disappear");
					AnalyzedObject = null;
				}
			}
			else
			{
				// clicked on nothing.
				previousColBroadcast = colBroadcast;
				DisengageSelectionDisplay ();
				AnalyzerToolContainer.GetComponent<Animator> ().SetTrigger ("Disappear");
				AnalyzedObject = null;
			}
		}
	}
		
	/// <summary>
	/// Updates the panel display. Will select the good ui display function
	/// according to the type of object analyzed.
	/// </summary>
	void EngagePanelDisplay()
	{
		if (AnalyzedObject)
		{
			AnalyzerToolContainer.SetActive (true);
			if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Player)
			{
				// the analyzed object is a player. set the UI as such.
				analyzedPlayerScript = AnalyzedObject.GetComponent<PlayerObject> ();
				analyzedPlayerScript.ToggleSelection (true);
				// send cmds to serv for data update
				PlayerAnalyzeServerUpdate();
				// Delayed function call, set the UI display.
				Invoke ("UpdatePlayerAnalyzerDisplay", 0.1f);
			}
			else if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Block)
			{
				// the analyzed object is a ground block. set the UI as such.
				analyzedBlockScript = AnalyzedObject.GetComponent<BlockObject> ();
				analyzedBlockScript.ToggleSelection (true);
				// send cmds to serv for data update
				BlockAnalyzeServerUpdate();

				// Delayed function call, set the UI display.
				Invoke ("UpdateBlockAnalyzerDisplay", 0.1f);
			}
		}
	}

	/// <summary>
	/// Disengages the selection display. Needed in case something is already selected.
	/// </summary>
	void DisengageSelectionDisplay()
	{
		if (AnalyzedObject && previousColBroadcast)
		{
			if (previousColBroadcast.ObjType == ColliderBroadcast.ObjectType.Player)
			{
				// todo: disengage player selection visuals etc.
				AnalyzedObject.GetComponent<PlayerObject>().ToggleSelection(false);
			}
			else if (previousColBroadcast.ObjType == ColliderBroadcast.ObjectType.Block)
			{
				AnalyzedObject.GetComponent<BlockObject> ().ToggleSelection (false);
			}
		}
	}


	///
	///		------ UI UPDATES ------
	///

	/// <summary>
	/// Updates the player analyzer display, meaning UI update.
	/// </summary>
	void UpdatePlayerAnalyzerDisplay()
	{
		// set image of the analyzer;
		AnalyzerVisual.sprite = PlayerVisual;
		// set ui texts.
		NumberValue.text = analyzedPlayerScript.AssignedNumber.ToString();
		Position_x_val.text = analyzedPlayerScript.X.ToString() + "x";
		Position_y_val.text = analyzedPlayerScript.Y.ToString() + "y";
		LevelValue.text = analyzedPlayerScript.Level.ToString();
		FoodUnitsValue.text = analyzedPlayerScript.FoodTimeLeft.ToString();
		LinemateValue.text = analyzedPlayerScript.Linemate.ToString();
		DeraumereValue.text = analyzedPlayerScript.Deraumere.ToString();
		SiburValue.text = analyzedPlayerScript.Sibur.ToString();
		PhirasValue.text = analyzedPlayerScript.Phiras.ToString();
		MendianeValue.text = analyzedPlayerScript.Mendiane.ToString();
		ThystameValue.text = analyzedPlayerScript.Thystame.ToString();
	}


	void UpdateBlockAnalyzerDisplay()
	{
		// set image of the analyzer;
		AnalyzerVisual.sprite = BlockVisual;
		// set ui texts.
		NumberValue.text = "-";
		Position_x_val.text = analyzedBlockScript.x.ToString() + "x";
		Position_y_val.text = analyzedBlockScript.y.ToString() + "y";
		LevelValue.text = "-";
		FoodUnitsValue.text = analyzedBlockScript.FoodUnits.ToString();
		LinemateValue.text = analyzedBlockScript.LinemateUnits.ToString();
		DeraumereValue.text = analyzedBlockScript.DeraumereUnits.ToString();
		SiburValue.text = analyzedBlockScript.SiburUnits.ToString();
		PhirasValue.text = analyzedBlockScript.PhirasUnits.ToString();
		MendianeValue.text = analyzedBlockScript.MendianeUnits.ToString();
		ThystameValue.text = analyzedBlockScript.ThystameUnits.ToString();
	}




	///
	///		------ Communication with server ------
	///

	/// <summary>
	/// Routine that will ask the server for update every n seconds.
	/// </summary>
	/// <returns>The analyzer communication display routine.</returns>
	IEnumerator	AnalyzerCommunicationDisplayRoutine()
	{
		for (;;)
		{
			if (AnalyzedObject)
			{
				if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Player)
				{
					PlayerAnalyzeServerUpdate ();
					UpdatePlayerAnalyzerDisplay ();
				}
				else if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Block)
				{
					BlockAnalyzeServerUpdate ();
					UpdateBlockAnalyzerDisplay ();
				}
			}
			yield return StartCoroutine(CoroutineUtils.WaitForRealSeconds(SecondsBetweenServerCalls));
		}
	}



	/// <summary>
	/// Sends msg to serv to update players datas.
	/// </summary>
	void PlayerAnalyzeServerUpdate()
	{
		GameManager.instance.MsgBroadcastController
			.PushMsg ("ppo #" + analyzedPlayerScript.AssignedNumber + "\n");
		GameManager.instance.MsgBroadcastController
			.PushMsg ("plv #" + analyzedPlayerScript.AssignedNumber + "\n");
		GameManager.instance.MsgBroadcastController
			.PushMsg ("pin #" + analyzedPlayerScript.AssignedNumber + "\n");
	}

	/// <summary>
	/// Sends msg to serv to update block datas.
	/// </summary>
	void BlockAnalyzeServerUpdate()
	{
		GameManager.instance.MsgBroadcastController
			.PushMsg ("bct " + analyzedBlockScript.x + " " + analyzedBlockScript.x + "\n");
	}
}
