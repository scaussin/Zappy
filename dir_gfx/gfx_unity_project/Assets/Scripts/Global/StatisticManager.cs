using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Statistic manager. Class that component access for storing datas about the game.
/// </summary>
public class StatisticManager : MonoBehaviour
{
	[Header("Statistics for the current game:")]
	public int		NbOfConnections = 0;
	public int		IncantationCount = 0;
	public int		PickUpNb = 0;
	public int		DropNb = 0;
	public int		DeathNumber = 0;
	public int		MoveActionNb = 0;
	public int		BroadcastNb = 0;
	public int		EggCount = 0;

	// to reset the stats.
	public void		CleanStats()
	{
		NbOfConnections = 0;
		IncantationCount = 0;
		PickUpNb = 0;
		DropNb = 0;
		DeathNumber = 0;
		MoveActionNb = 0;
		BroadcastNb = 0;
		EggCount = 0;
	}
}
