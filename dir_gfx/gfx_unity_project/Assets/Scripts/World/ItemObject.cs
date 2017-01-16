using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemObject : MonoBehaviour {
	public enum ObjectType {Food, Linemate, Deraumere, Sibur, Mendiane, Phiras, Thystame};

	public ObjectType		Type;
	public Animator			ItemAnimator;

	void Awake()
	{
		ItemAnimator = GetComponent<Animator> ();
	}

	public void OnAppearAnimationEnd()
	{
		ItemAnimator.enabled = false;
	}
}
