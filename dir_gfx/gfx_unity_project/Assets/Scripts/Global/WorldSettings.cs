using UnityEngine;
using System.Collections;

public class WorldSettings : MonoBehaviour
{
	[Header("World Settings datas")]
	public float	BlockSize = 1.0f;
	public float	BlockSpacing = 0.2f;

	[Header("Server received datas (read only)")]
	public float	TimeUnit = 1.0f;
	public int		WorldX;
	public int		WorldY;
}
