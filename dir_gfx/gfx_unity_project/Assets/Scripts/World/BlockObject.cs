using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Block object. Script available on each instance of a block.
/// </summary>
public class BlockObject : MonoBehaviour
{
	[Header("Block events")]
	public List<int>			Incantations_id;

	[Header("Block datas")]
	public int					x;
	public int					y;

<<<<<<< HEAD
	/*[Header("Block datas")]
	public int					x;
	public int					y;*/

=======
>>>>>>> a66b4d8ee7688ec80a46e663d5052d48f6ddf630
	[Header("Ressources on the case(modified by server)")]
	public int					FoodUnits = 0;
	public int					LinemateUnits  = 0;
	public int					DeraumereUnits = 0;
	public int					SiburUnits = 0;
	public int					MendianeUnits = 0;
	public int					PhirasUnits = 0;
	public int					ThystameUnits = 0;

	[Header ("Links to ressources 3d models")]
	public GameObject			DecorationsObj;
	public GameObject			BlockModelObj;
	public GameObject			RessourcesContainer;
	public GameObject			FoodModelObject;
	public GameObject			LinemateModelObject;
	public GameObject			DeraumereModelObject;
	public GameObject			SiburModelObject;
	public GameObject			MendianeModelObject;
	public GameObject			PhirasModelObject;
	public GameObject			ThystameModelObject;

	private Animator			BlockAnimator;

	private GameObject			GroundSelectedModel;

	// Use this for initialization
	void Awake ()
	{
		BlockAnimator = GetComponent<Animator>	();
		DecorationsObj = transform.Find ("Decorations").gameObject;
		BlockModelObj =  transform.Find ("Model").transform.Find ("Root").transform.Find ("MainBlock").
			transform.Find ("MainBlockModel").gameObject;
		RessourcesContainer = transform.Find ("Ressources").gameObject;
		FoodModelObject = RessourcesContainer.transform.Find ("Food").gameObject;
		LinemateModelObject = RessourcesContainer.transform.Find ("Linemate").gameObject;
		DeraumereModelObject = RessourcesContainer.transform.Find ("Deraumere").gameObject;
		SiburModelObject = RessourcesContainer.transform.Find ("Sibur").gameObject;
		MendianeModelObject = RessourcesContainer.transform.Find ("Mendiane").gameObject;
		PhirasModelObject = RessourcesContainer.transform.Find ("Phiras").gameObject;
		ThystameModelObject = RessourcesContainer.transform.Find ("Thystame").gameObject;

		GroundSelectedModel = transform.Find ("Effects").transform.Find ("GroundSelected").gameObject;
		ToggleSelection (false);
	}

	/// <summary>
	/// Updates the Ressources models display. Will mask if there is no unit on the case;
	/// </summary>
	public void UpdateModelDisplay()
	{
		if (FoodUnits > 0)
			FoodModelObject.SetActive(true);
		else
			FoodModelObject.SetActive(false);
		
		if (LinemateUnits > 0)
			LinemateModelObject.SetActive(true);
		else
			LinemateModelObject.SetActive(false);
		
		if (DeraumereUnits > 0)
			DeraumereModelObject.SetActive(true);
		else
			DeraumereModelObject.SetActive(false);
		
		if (SiburUnits > 0)
			SiburModelObject.SetActive(true);
		else
			SiburModelObject.SetActive(false);
		
		if (MendianeUnits > 0)
			MendianeModelObject.SetActive(true);
		else
			MendianeModelObject.SetActive(false);
		
		if (PhirasUnits > 0)
			PhirasModelObject.SetActive(true);
		else
			PhirasModelObject.SetActive(false);
		
		if (ThystameUnits > 0)
			ThystameModelObject.SetActive(true);
		else
			ThystameModelObject.SetActive(false);
	}

	public void		ToggleSelection(bool b)
	{
		GroundSelectedModel.SetActive (b);
	}

	/// <summary>
	/// Desactivates the animator at the end of appear animation, cause it makes a lot of lag on each block even when idle.;
	/// </summary>
	public void DesactivateAnimator()
	{
		BlockAnimator.enabled = false;
	}
}
