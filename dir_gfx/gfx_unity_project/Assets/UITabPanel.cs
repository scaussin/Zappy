using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


/// <summary>
/// User interface tab panel. Mirror to UITeamOverviewPanelController.
/// Except this one focuses on connected player and is only visible when
/// tab btn is pressed;
/// </summary>
public class UITabPanel : MonoBehaviour {
	[Header("server sending delay")]
	public float					SecondsBetweenServerCalls = 0.8f;

	public GameObject				PlayerTabPrefab;
	public GameObject				PlayerTabsContainer;

	public List<GameObject>			PlayerTabs;

	private List<string>			playerTeamsRef;
	private List<PlayerObject>		playerObjsRef;

	private int						playerCount;
	private int						i;

	void Awake()
	{
		playerTeamsRef = GameManager.instance.PlayerManager.PlayerTeams;
		playerObjsRef = GameManager.instance.PlayerManager.ConnectedPlayers;
		PlayerTabsContainer = transform.Find ("PlayerTabsContainer").gameObject;
		PlayerTabPrefab = Resources.Load ("Prefabs/UI/PlayerTabPanel") as GameObject;
		playerCount = 0;
	}

	void OnEnable()
	{
		StartCoroutine ("UpdatePlayerTabRoutine");
	}

	void OnDisable()
	{
		StopCoroutine ("UpdatePlayerTabRoutine");
	}

	public IEnumerator	UpdatePlayerTabRoutine()
	{
		for (;;)
		{
			playerCount = playerObjsRef.Count;
			AskForAllPlayersDatas ();
			// destroy all, then respawn;
			DestroyTabPanels();
			CreateUIPlayerTabPanels ();
			yield return StartCoroutine(CoroutineUtils.WaitForRealSeconds(SecondsBetweenServerCalls));
		}
	}

	private void CreateUIPlayerTabPanels()
	{
		i = 1;
		foreach (PlayerObject player in playerObjsRef)
		{
			//SetTeamInfosFromName (playerTeamsRef[team_count]);
			SetPlayerTabValues (SpawnPlayerTabPanel (player), player);
		}
	}

	private GameObject SpawnPlayerTabPanel(PlayerObject playerObj)
	{
		GameObject	NewPlayerTabPanel = (GameObject)Instantiate (PlayerTabPrefab, this.transform.position,
			                               Quaternion.identity, PlayerTabsContainer.transform);
		PlayerTabs.Add (NewPlayerTabPanel);
		return NewPlayerTabPanel;
	}

	private void SetPlayerTabValues(GameObject NewPlayerTabPanel, PlayerObject playerObj)
	{
		NewPlayerTabPanel.transform.Find ("PlayerTabPanelValue").GetComponent<Text> ().text = playerObj.AssignedNumber.ToString();
		NewPlayerTabPanel.transform.Find ("TeamNameValue").GetComponent<Text> ().text = playerObj.Team;
		NewPlayerTabPanel.transform.Find ("Position_x_value").GetComponent<Text> ().text = playerObj.X.ToString() + "x";
		NewPlayerTabPanel.transform.Find ("Position_y_value").GetComponent<Text> ().text = playerObj.Y.ToString() + "y";
		NewPlayerTabPanel.transform.Find ("LevelValue").GetComponent<Text> ().text = playerObj.Level.ToString();
		NewPlayerTabPanel.transform.Find ("FoodValue").GetComponent<Text> ().text = playerObj.FoodTimeLeft.ToString();
		NewPlayerTabPanel.transform.Find ("LinemateValue").GetComponent<Text> ().text = playerObj.Linemate.ToString();
		NewPlayerTabPanel.transform.Find ("DeraumereValue").GetComponent<Text> ().text = playerObj.Deraumere.ToString();
		NewPlayerTabPanel.transform.Find ("SiburValue").GetComponent<Text> ().text = playerObj.Sibur.ToString();
		NewPlayerTabPanel.transform.Find ("MendianeValue").GetComponent<Text> ().text = playerObj.Mendiane.ToString();
		NewPlayerTabPanel.transform.Find ("PhirasValue").GetComponent<Text> ().text = playerObj.Phiras.ToString();
		NewPlayerTabPanel.transform.Find ("ThystameValue").GetComponent<Text> ().text = playerObj.Thystame.ToString();
	}

	public void AskForAllPlayersDatas()
	{
		foreach (PlayerObject player in playerObjsRef)
		{
			GameManager.instance.MsgBroadcastController
					.PushMsg ("ppo #" + player.AssignedNumber + "\n");
			GameManager.instance.MsgBroadcastController
					.PushMsg ("plv #" + player.AssignedNumber + "\n");
			GameManager.instance.MsgBroadcastController
					.PushMsg ("pin #" + player.AssignedNumber + "\n");
		}
	}

	public void DestroyTabPanels()
	{
		foreach (GameObject playerTab in PlayerTabs)
		{
			Destroy (playerTab.gameObject);
		}
		PlayerTabs.Clear ();
	}
}
