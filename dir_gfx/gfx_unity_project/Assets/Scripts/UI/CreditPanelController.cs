using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CreditPanelController : MonoBehaviour {
	public bool				CreditUp;
	public GameObject		CreditPanelContainer;
	public Button			CloseCreditBtn;

	void Awake()
	{
		CreditPanelContainer = transform.Find ("CreditPanelContainer").gameObject;
		CloseCreditBtn = CreditPanelContainer.transform.Find ("CloseCreditBtn").GetComponent<Button> ();
	}

	void OnEnable()
	{
		CloseCreditBtn.onClick.AddListener (OnClickCloseCreditBtn);
	}

	void OnDisable()
	{
		CloseCreditBtn.onClick.RemoveListener (OnClickCloseCreditBtn);
	}

	public void ActivateCreditPanel()
	{
		CreditPanelContainer.SetActive (true);
		CreditUp = true;
	}

	public void CloseCreditPanel()
	{
		CreditPanelContainer.SetActive (false);
		CreditUp = false;
	}

	public void OnClickCloseCreditBtn()
	{
		CloseCreditPanel ();
	}
}
