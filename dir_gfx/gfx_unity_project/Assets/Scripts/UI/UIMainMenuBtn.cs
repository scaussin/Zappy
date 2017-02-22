using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UIMainMenuBtn : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
	public string					HelpMessage; // please set this in Unity editor.
	private MainPanelScript			MainPanelScript;

	void Start()
	{
		MainPanelScript = transform.parent.transform.parent.transform.parent.transform.parent
			.GetComponent<MainPanelScript> ();
	}

	public void OnPointerEnter (PointerEventData eventData) 
	{
		MainPanelScript.OnBtnHover (HelpMessage);
	}

	public void OnPointerExit (PointerEventData eventData)
	{
		MainPanelScript.OnBtnHover ("");
	}
}
