using UnityEngine;
using System.Collections;

public class WorldSettings : MonoBehaviour
{
	[Header("World Settings datas")]
	public float	BlockSize = 1.0f;
	public float	BlockSpacing = 0.2f;
	public bool		InstantTimeMode = false;
	public Vector3	BoardZeroPoint;
	public float	PlayerMaxOffset_x = 1.0f;
	public float	PlayerMaxOffset_z = 1.0f;

	[Header("Server received datas (read only)")]
	public float	TimeUnit = 1.0f;
	public int		WorldX;
	public int		WorldY;
}
