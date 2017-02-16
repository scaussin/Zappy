using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EggObject : MonoBehaviour {
	public int		egg_nb;
	public int		pos_x;
	public int		pos_y;

	/// <summary>
	/// Desactives the object. Used in sortie animation.
	/// </summary>
	public void DesactiveObject()
	{
		gameObject.SetActive (false);
		GameManager.instance.PlayerManager.EggList.Remove (this.GetComponent<EggObject> ());
		GameObject.Destroy (this.gameObject);
	}
}
