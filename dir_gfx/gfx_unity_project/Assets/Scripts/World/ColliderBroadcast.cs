using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/// <summary>
/// Collider broadcast. Is used to easily give the collider's root object to accessing functions.
/// When you ray cast hit a collider and get this class with GetComponent, you can access the type
/// and the root object of the collider(which can be anywhere in the prefab's hierarchy).
/// </summary>
public class ColliderBroadcast : MonoBehaviour
{
	/// <summary>
	/// Object type. ColliderBroadcast.Objectype <- to access the public enum.
	/// </summary>
	public enum ObjectType
	{
		Player,
		Block
	}

	public ObjectType				ObjType;
	public GameObject				ObjectRoot;

	void Awake ()
	{
		if (ObjType == ObjectType.Player)
		{
			ObjectRoot = transform.parent.transform.parent.gameObject;
		}
		else if (ObjType == ObjectType.Block)
		{
			ObjectRoot = transform.parent.transform.parent.gameObject;
		}
		else
		{
			ObjectRoot = null;	
		}
	}
}
