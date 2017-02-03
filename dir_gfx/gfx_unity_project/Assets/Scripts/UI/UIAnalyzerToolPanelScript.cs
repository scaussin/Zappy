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

	void Awake()
	{
		AnalyzerToolContainer = transform.Find ("AnalyzerToolContainer").gameObject;
		AnalyzerDataContainer = AnalyzerToolContainer.transform.Find ("AnalyzerDataContainer").gameObject;
		// set analyzer data references for updating.
		AnalyzerVisual = AnalyzerToolContainer.transform.Find("AnalyzerVisual").gameObject.GetComponent<Image> ();
		NumberValue = AnalyzerDataContainer.transform.Find("NumberValue").gameObject.GetComponent<Text> ();
		Position_x_val = AnalyzerDataContainer.transform.Find("Position_x_val").gameObject.GetComponent<Text> ();
		Position_y_val = AnalyzerDataContainer.transform.Find("Position_x_val").gameObject.GetComponent<Text> ();
		LevelValue = AnalyzerDataContainer.transform.Find("LevelValue").gameObject.GetComponent<Text> ();
		FoodUnitsValue = AnalyzerDataContainer.transform.Find("FoodUnitsValue").gameObject.GetComponent<Text> ();
		LinemateValue = AnalyzerDataContainer.transform.Find("LinemateValue").gameObject.GetComponent<Text> ();
		DeraumereValue = AnalyzerDataContainer.transform.Find("DeraumereValue").gameObject.GetComponent<Text> ();
		SiburValue = AnalyzerDataContainer.transform.Find("SiburValue").gameObject.GetComponent<Text> ();
		MendianeValue = AnalyzerDataContainer.transform.Find("MendianeValue").gameObject.GetComponent<Text> ();
		PhirasValue = AnalyzerDataContainer.transform.Find("PhirasValue").gameObject.GetComponent<Text> ();
		ThystameValue = AnalyzerDataContainer.transform.Find("ThystameValue").gameObject.GetComponent<Text> ();


	}

	// Use this for initialization
	void OnEnable () {
		AnalyzedObject = null;
		// let's fetch the visuals right now.
		PlayerVisual = Resources.Load<Sprite> ("UI/AnalyzerVisuals/analyzer_player");
		BlockVisual = Resources.Load<Sprite> ("UI/AnalyzerVisuals/analyzer_block");
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (Input.GetMouseButtonUp (0))
		{
			ray = Camera.main.ScreenPointToRay (Input.mousePosition);
			if (Physics.Raycast (ray, out hit, 200.0f, AnalyzerRaycastMask))
			{
				if ((colBroadcast = hit.collider.gameObject.GetComponent<ColliderBroadcast> ()))
				{
					Debug.Log ("Clicked on " + colBroadcast.ObjectRoot.name);
					AnalyzedObject = colBroadcast.ObjectRoot;
					UpdatePanelDisplay ();
				}
				else
				{
					AnalyzedObject = null;
				}
			}
			else
			{
				AnalyzedObject = null;
			}
		}
	}

	/// <summary>
	/// Updates the panel display. Will select the good ui display function
	/// according to the type of object analyzed.
	/// </summary>
	void UpdatePanelDisplay()
	{
		if (AnalyzedObject)
		{
			if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Player)
			{
				// the analyzed object is a player. set the UI as such.
				UpdatePlayerAnalyzer();
			}
			else if (colBroadcast.ObjType == ColliderBroadcast.ObjectType.Block)
			{
				// the analyzed object is a ground block. set the UI as such.
				UpdateBlockAnalyzer();
			}
		}
	}

	void UpdatePlayerAnalyzer()
	{
		// set image of the analyzer;
		AnalyzerVisual.sprite = PlayerVisual;
	}

	void UpdateBlockAnalyzer()
	{
		// set image of the analyzer;
		AnalyzerVisual.sprite = BlockVisual;
	}
}
