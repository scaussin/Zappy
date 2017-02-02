using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UITeamOverviewPanelController : MonoBehaviour {
	// will be spawned in the UI for each team.
	public GameObject				TeamPanelPrefab;
	public GameObject				TeamPanelContainer;

	public List<GameObject>			TeamPanels;

	private List<string>			playerTeamsRef;
	private List<PlayerObject>		playerObjsRef;

	// Team infos to display in panels.
	private int						nbOfPlayers;
	private int						maxLv;
	private int						nbOfPlayersMaxLv;

	private int						i;
	private int						team_count;

	//private GameObject				newTeamPanel;



	void Awake()
	{
		playerTeamsRef = GameManager.instance.PlayerManager.PlayerTeams;
		playerObjsRef = GameManager.instance.PlayerManager.ConnectedPlayers;
		TeamPanelContainer = transform.Find ("TeamPanelContainer").gameObject;
		TeamPanelPrefab = Resources.Load ("Prefabs/UI/TeamPanel") as GameObject;
		team_count = 0;

	}

	void OnEnable()
	{
		StartCoroutine ("UpdateTeamsRoutine");
	}

	void OnDisable()
	{
		StopCoroutine ("UpdateTeamsRoutine");
	}

	/// <summary>
	/// Updates the teams, at intervals, will check if there are new teams in the team array.
	/// Updating might mean destroying then adding again ui elements.
	/// </summary>
	/// <returns>The teams.</returns>
	public IEnumerator	UpdateTeamsRoutine()
	{
		for (;;)
		{
			// do we add panels or update those already placed?
			if (team_count < playerTeamsRef.Count)
			{
				CreateUITeamPanels ();
			}
			else if (playerTeamsRef.Count != 0)
			{
				UpdateUITeamPanelsValues ();
			}
			else
			{
				DestroyPanels();
				team_count = 0;
			}
			yield return new WaitForSeconds(0.5f);
		}
	}

	/// <summary>
	/// Updates the user interface team panels. Spawns the required panels.
	/// </summary>
	private void CreateUITeamPanels()
	{
		//DestroyPanels ();
		i = 1;
		for (;team_count < playerTeamsRef.Count; team_count++)
		{
			SetTeamInfosFromName (playerTeamsRef[team_count]);
			SpawnPanelForTeam (playerTeamsRef[team_count], team_count);
		}
	}

	private void UpdateUITeamPanelsValues()
	{
		i = 0;
		foreach (GameObject teamPanel in TeamPanels)
		{
			SetTeamInfosFromName (playerTeamsRef[i]);
			SetTeamPanelValues (teamPanel, playerTeamsRef[i], i + 1);
			i++;
		}
	}

	/// <summary>
	/// Sets the class's variables from the team name. Better use those var just after this call.
	/// </summary>
	/// <param name="teamName">Team name.</param>
	private void SetTeamInfosFromName(string teamName)
	{
		maxLv = 1;
		nbOfPlayers = 0;
		nbOfPlayersMaxLv = 0;
		// pass 1: get nb of players and max lv for team;
		foreach (PlayerObject player in playerObjsRef)
		{
			if (player.Team.Equals (teamName))
			{
				nbOfPlayers += 1;
				if (player.Level > maxLv)
				{
					maxLv = player.Level;
				}
			}
		}
		// pass 2: get nb of players at max lv for team;
		foreach (PlayerObject player in playerObjsRef)
		{
			if (player.Team.Equals (teamName))
			{
				if (player.Level == maxLv)
				{
					nbOfPlayersMaxLv += 1;
				}
			}
		}
	}

	private	void SpawnPanelForTeam(string teamName, int team_i)
	{
		GameObject newTeamPanel = (GameObject)Instantiate
			(TeamPanelPrefab, this.transform.position, Quaternion.identity, TeamPanelContainer.transform);
		SetTeamPanelValues (newTeamPanel, teamName, team_i);
		TeamPanels.Add (newTeamPanel);
	}

	private void SetTeamPanelValues(GameObject newTeamPanel, string teamName, int team_i)
	{
		newTeamPanel.transform.Find ("TeamName").GetComponent<Text> ().text = teamName;
		newTeamPanel.transform.Find ("TeamPanelLabelValue").GetComponent<Text> ().text = team_i.ToString();
		newTeamPanel.transform.Find ("NbUnitsValue").GetComponent<Text> ().text = nbOfPlayers.ToString();
		newTeamPanel.transform.Find ("MaxLevelValue").GetComponent<Text> ().text = maxLv.ToString();
		newTeamPanel.transform.Find ("NbUnitsMaxValue").GetComponent<Text> ().text = nbOfPlayersMaxLv.ToString();
	}

	/// <summary>
	/// Destroies the panels of the UI. Called on server shutdown.
	/// </summary>
	public void DestroyPanels()
	{
		foreach (GameObject teamPanel in TeamPanels)
		{
			Destroy (teamPanel.gameObject);
		}
		TeamPanels.Clear ();
	}
}
