using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerManager : MonoBehaviour {

	[Header("List of team names")]
	public List<string>			PlayerTeams;
	public List<PlayerObject>	ConnectedPlayers;
	public List<EggObject>		EggList;

	/// <summary>
	/// Cleans the player manager of their teams and players.
	/// </summary>
	public void CleanPlayerManager()
	{
		PlayerTeams.Clear ();
		ConnectedPlayers.Clear ();
		EggList.Clear ();
	}
}
