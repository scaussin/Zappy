using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UIEscapeMenuBtn : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
	public string		HelpMessage; // please set this in Unity editor.

	public void OnPointerEnter (PointerEventData eventData) 
	{
		transform.parent.transform.parent.GetComponent<EscapeMenuController> ().OnBtnHover (HelpMessage);
	}

	public void OnPointerExit (PointerEventData eventData)
	{
		transform.parent.transform.parent.GetComponent<EscapeMenuController> ().OnBtnHover ("");
	}
}
