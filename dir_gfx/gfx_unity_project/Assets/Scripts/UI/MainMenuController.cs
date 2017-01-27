using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

public class MainMenuController : MonoBehaviour {
    // something happening events.
    /// <summary>
    /// Will be invoked when user press enter with infos entered in the server info inputfields.
    /// </summary>
    public UnityEvent   OnServerInfoSelected;

    // menu references;
    public InputField   HostnameInputField;
    public InputField   PortInputField;
    public Text         ResponseText;
    public GameObject   MainPanel;

    public bool         CanEnterInput;

    // input field variable helper.
    public bool        isHostnameSet;
    public bool        isPortSet;

    // Use this for initialization
    void Awake()
    {
        MainPanel = transform.Find("MainPanel").gameObject;

        HostnameInputField = MainPanel.transform
            .Find("CenterAreaPanel").transform
            .Find("InputContainer").transform
            .Find("InputHostname").GetComponent<InputField>();
        PortInputField = MainPanel.transform
            .Find("CenterAreaPanel").transform
            .Find("InputContainer").transform
            .Find("InputPort").GetComponent<InputField>();
        ResponseText = MainPanel.transform
               .Find("CenterAreaPanel").transform
               .Find("InputContainer").transform
               .Find("ResponseText").GetComponent<Text>();
        isHostnameSet = false;
        isPortSet = false;
    }

    void OnEnable()
    {
        HostnameInputField.onEndEdit.AddListener(OnEndEditHostname);
        PortInputField.onEndEdit.AddListener(OnEndEditPort);
    }

    void OnDisable()
    {
        isHostnameSet = false;
        isPortSet = false;
    }


    /// <summary>
    /// Linked to the hostname input field's event.
    /// </summary>
    public void OnEndEditHostname(string str)
    {
        isHostnameSet = true;
        if (HostnameInputField.text == "")
        {
            GameManager.instance.Hostname = "localhost";
        }
        else
        {
            GameManager.instance.Hostname = str;
        }

        if (isPortSet == true)
        {
            OnServerInfoSelected.Invoke(); // lets go try connection!
        }
        else
        {
            PortInputField.Select();
        }
    }

    /// <summary>
    /// Linked to the port input field's event.
    /// </summary>
    public void OnEndEditPort(string str)
    {
        isPortSet = true;
        if (PortInputField.text == "")
        {
            GameManager.instance.Port = "4242";
        }
        else
        {
            GameManager.instance.Port = str;
        }
        
        if (isHostnameSet == true)
        {
            OnServerInfoSelected.Invoke(); // lets go try connection!
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (CanEnterInput)
        {
            // press enter when nothing is selected -> default settings.
            if ((Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.KeypadEnter)
                || Input.GetKeyDown(KeyCode.Space))
                && HostnameInputField.isFocused == false && PortInputField.isFocused == false)
            {
                if (!isHostnameSet)
                {
                    GameManager.instance.Hostname = "localhost";
                }
                if (!isPortSet)
                {
                    GameManager.instance.Port = "4242";
                }
                OnServerInfoSelected.Invoke();
            }
            // Press tab when hostname modification ? select port field.
            if (Input.GetKeyDown(KeyCode.Tab))
            {
                if (HostnameInputField.isFocused == true)
                {
                    PortInputField.Select();
                }
            }
        }
    }
}
