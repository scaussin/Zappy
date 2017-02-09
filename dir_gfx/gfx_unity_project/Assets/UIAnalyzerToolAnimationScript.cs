using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIAnalyzerToolAnimationScript : MonoBehaviour {
	public void DesactivePanel()
	{
		this.gameObject.SetActive (false);
		transform.parent.GetComponent<UIAnalyzerToolPanelScript> ().CanSelect = true;
	}

	public void DesactiveInput()
	{
		transform.parent.GetComponent<UIAnalyzerToolPanelScript> ().CanSelect = false;
	}

	public void	ActiveInput()
	{
		transform.parent.GetComponent<UIAnalyzerToolPanelScript> ().CanSelect = true;
	}
}
