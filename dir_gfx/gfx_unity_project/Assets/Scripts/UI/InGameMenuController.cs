using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMenuController : MonoBehaviour {
	public GameObject	TimeControlPanel;
	public GameObject	TeamOverviewPanel;
	public GameObject	AnalyzerToolPanel;
	public GameObject	TabPanel;


	// Use this for initialization
	void Awake () {
		TimeControlPanel = transform.Find ("TimeControlPanel").gameObject;
		TeamOverviewPanel = transform.Find ("TeamOverviewPanel").gameObject;
		AnalyzerToolPanel = transform.Find ("AnalyzerToolPanel").gameObject;
		TabPanel = transform.Find ("TabPanel").gameObject;
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
