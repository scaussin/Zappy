using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameMenuController : MonoBehaviour {
	public GameObject						TimeControlPanel;
	public GameObject						TeamOverviewPanel;
	public UITeamOverviewPanelController	TeamOverviewPanelController;
	public GameObject						AnalyzerToolPanel;
	public GameObject						TabPanel;

	private KeyCode							tabPanelKey;
	private KeyCode							tabPanelKeyAlt;
	private bool							tabPanelUp;

	// Use this for initialization
	void Awake ()
	{
		TimeControlPanel = transform.Find ("TimeControlPanel").gameObject;
		TeamOverviewPanel = transform.Find ("TeamOverviewPanel").gameObject;
		TeamOverviewPanelController = TeamOverviewPanel.GetComponent<UITeamOverviewPanelController> ();
		AnalyzerToolPanel = transform.Find ("AnalyzerToolPanel").gameObject;
		TabPanel = transform.Find ("TabPanel").gameObject;
		tabPanelKey = GameManager.instance.KeyManager.TabPanelKey;
		tabPanelKeyAlt = GameManager.instance.KeyManager.TabPanelKeyAlt;
	}
	
	// Update is called once per frame
	void Update ()
	{
		if ((Input.GetKeyDown (tabPanelKey) || Input.GetKeyDown (tabPanelKeyAlt))
			&& tabPanelUp == false)
		{
			tabPanelUp = true;
			TabPanel.SetActive (true);
		}
		if ((Input.GetKeyUp (tabPanelKey) || Input.GetKeyUp (tabPanelKeyAlt))
		    && tabPanelUp == true)
		{
			tabPanelUp = false;
			TabPanel.SetActive (false);
		}
	}
}
