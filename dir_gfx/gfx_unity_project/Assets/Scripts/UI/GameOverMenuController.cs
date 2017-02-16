using System.Collections;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameOverMenuController : MonoBehaviour
{
	public GameObject				GameOverPanelContainer;
	public bool						GameOverMenuUp;
	public GameObject				GameOverDatasContainer;

	private KeyCode					enterKey;
	private KeyCode					enterKeyAlt;
	private List<PlayerObject>		playerObjsRef;
	private int						nbOfPlayers = 0;
	private Regex					rgx;
	private Match					match;
	private GroupCollection			groups;

	// Use this for initialization
	void Awake ()
	{
		GameOverPanelContainer = transform.Find ("GameOverPanelContainer").gameObject;
		GameOverDatasContainer = GameOverPanelContainer.transform.Find ("GameOverDatasContainer").gameObject;
		playerObjsRef = GameManager.instance.PlayerManager.ConnectedPlayers;
		enterKey = GameManager.instance.KeyManager.Enter;
		enterKeyAlt = GameManager.instance.KeyManager.EnterAlt;
	}

	/// <summary>
	/// Activates the game over. A game has been finished, we put the menu up and
	/// wait for an input.
	/// </summary>
	public void ActivateGameOver(string msg)
	{
		// Parse and get value from message.
		rgx = new Regex("^seg (\\w+)$");
		match = rgx.Match (msg);
		if (match.Success)
		{
			groups = match.Groups;
			string winningTeam = groups [1].Value;

			GameOverMenuUp = true;
			GameOverPanelContainer.SetActive (true);
			SetGameOverMenuDatas (winningTeam);
		}
		else
		{
			Debug.LogError ("regex problem for victory");
		}
	}

	private void SetGameOverMenuDatas(string winningTeam)
	{
		// Winning team stats
		GameOverDatasContainer.transform.Find ("WinningTeamValue").GetComponent<Text> ().text = winningTeam;
		GameOverDatasContainer.transform.Find ("TeamNbValue").GetComponent<Text> ().text =
			GameManager.instance.PlayerManager.PlayerTeams.IndexOf(winningTeam).ToString();

		// Get nb of players for team;
		foreach (PlayerObject player in playerObjsRef)
		{
			if (player.Team.Equals (winningTeam))
			{
				nbOfPlayers += 1;
			}
		}
		GameOverDatasContainer.transform.Find ("NbOfUnitsValue").GetComponent<Text> ().text = nbOfPlayers.ToString();

		// Global stats
		GameOverDatasContainer.transform.Find ("NbOfConnectionsValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.NbOfConnections.ToString();
		GameOverDatasContainer.transform.Find ("NbOfDeathsValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.DeathNumber.ToString();
		GameOverDatasContainer.transform.Find ("NbOfIncantationValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.IncantationCount.ToString();
		GameOverDatasContainer.transform.Find ("NbOfPickUpsValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.PickUpNb.ToString();
		GameOverDatasContainer.transform.Find ("NbOfDropsValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.DropNb.ToString();
		GameOverDatasContainer.transform.Find ("NbOfBroadcastValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.BroadcastNb.ToString();
		GameOverDatasContainer.transform.Find ("NbOfEggSpawnedValue").GetComponent<Text> ().text =
			GameManager.instance.StatisticManager.EggCount.ToString();
	}

	void Update()
	{
		if (GameOverMenuUp && (Input.GetKeyUp(enterKey) || Input.GetKeyUp(enterKeyAlt)))
		{
			GameOverMenuUp = false;
			GameOverPanelContainer.SetActive (false);
			GameManager.instance.GameController.OnServerShutdown();
		}
	}
}
