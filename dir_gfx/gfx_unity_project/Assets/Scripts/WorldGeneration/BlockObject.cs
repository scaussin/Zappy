using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Block object. Script available on each instance of a block.
/// </summary>
public class BlockObject : MonoBehaviour
{
	[Header("Object with points of interests")]
	public GameObject	Points;

	// Use this for initialization
	void Awake () {
		Points = transform.Find ("Points").gameObject;
	}

	/// <summary>
	/// Desactivates the animator at the end of appear animation, cause it makes a lot of lag on each block even when idle.;
	/// </summary>
	public void DesactivateAnimator()
	{
		GetComponent<Animator> ().enabled = false;
	}
}
