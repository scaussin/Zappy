using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Game controller. This class control the game stages.
/// This is where the events will be fired to put the game in motion.
/// </summary>
public class GameController : MonoBehaviour {
	public UnityEvent OnGfxAuthentified;


	// Use this for initialization
	void Start () {
		OnGfxAuthentified = new UnityEvent ();
		OnGfxAuthentified.AddListener (OnGfxAuthentifiedAction);
	}

	void OnDestroy()
	{
		OnGfxAuthentified.RemoveAllListeners ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void		OnGfxAuthentifiedAction()
	{

	}
}
