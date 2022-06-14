using System;
using RagnarEngine;

public class DialogueManager : RagnarComponent
{
    //UIText toxt;
    GameObject boxTextBall;
    GameObject boxTextBox;
    private GameObject SceneAudio;
	GameObject text;
	GameObject image;
    GameObject name;
	Vector3 pos;
	string auth;
    int authId;
    // 0=Paul / 1=Chani / 2=Rehen Fremen / 3=Rabban / 
    // 4=Soldado Harkonnen / 5=Soldado / 
    // 6=Lady Jessica / 7=Stilgar
	bool endDialogue;
    bool inDialogue;

    int pressedSkip;

    public GameObject[] triggerColliders;
    public void Start()
	{
		Dialogue.LoadDialogueFile("");
        SceneAudio = GameObject.Find("AudioLevel1");
        boxTextBall = GameObject.Find("DialogueBoxBall");
        boxTextBox = GameObject.Find("DialogueBoxBox");
        text = GameObject.Find("DialogueText");
		image = GameObject.Find("DialogueAuthImg");
        name = GameObject.Find("DialogueAuthName");
		auth = "";
        authId = -1;
        //idDialogue = 0;
		pos = new Vector3(0, 0, 0);
		endDialogue = false;
        inDialogue = false;
		
		pressedSkip = 0;
        DisableDialogue();
        // Hay que actualizar todos los prefabs y hacer que se instancien por codigo
        // en su levelManager por cada nivel

        InitPosDialogueBox();

        if (SaveSystem.fromContinue)
        {
            Debug.Log("Loading dialogues");
            LoadDialogue();
        }
    }

    void InitPosDialogueBox()
    {
        float posY = InternalCalls.GetRegionGame().y, posX;
        posY *= 0.33f;
        posX = 0;
        //boxTextBox
        pos.Set(posX, posY, boxTextBox.GetComponent<Transform2D>().position2D.z - 10);
        boxTextBox.GetComponent<Transform2D>().position2D = pos;
        //boxTextBall
        pos.Set(posX, posY, boxTextBox.GetComponent<Transform2D>().position2D.z - 10);
        boxTextBall.GetComponent<Transform2D>().position2D = pos;

        posX = boxTextBox.GetComponent<Transform2D>().position2D.x;
        //face
        pos.Set(posX, posY - 2, image.GetComponent<Transform2D>().position2D.z + 20);
        image.GetComponent<Transform2D>().position2D = pos;

        posX = InternalCalls.GetRegionGame().x * 0.025f;
        //author
        //pos.Set(posX - 195.0f, boxTextBox.GetComponent<Transform2D>().position2D.y + 60, name.GetComponent<Transform2D>().position2D.z + 20);
        pos.Set(posX, boxTextBox.GetComponent<Transform2D>().position2D.y + 60, name.GetComponent<Transform2D>().position2D.z + 20);
        name.GetComponent<Transform2D>().position2D = pos;
        //posX -= 40.0f;
        //text
        //.Set(posX - 192.0f, boxTextBox.GetComponent<Transform2D>().position2D.y + 10, text.GetComponent<Transform2D>().position2D.z + 20);
        pos.Set(posX, boxTextBox.GetComponent<Transform2D>().position2D.y + 10, text.GetComponent<Transform2D>().position2D.z + 20);
        text.GetComponent<Transform2D>().position2D = pos;
    }

    public void Update()
    {
        if(gameObject.isActive)
        {
            Vector3 pos = name.GetComponent<Transform2D>().position2D;
            pos.x = -220;
            name.GetComponent<Transform2D>().position2D = pos;
            pos.y -= 50;
            text.GetComponent<Transform2D>().position2D = pos;
        }
        // Next Line
        if (Input.GetKey(KeyCode.SPACE) == KeyState.KEY_UP && gameObject.isActive)
        {
            SceneAudio.GetComponent<AudioSource>().PlayClip("UI_DIALOGUEPASS");
            NextLine();
            pressedSkip = 0;
        }
        //End Dialogue
        if (Input.GetKey(KeyCode.SPACE) == KeyState.KEY_REPEAT)
        {
            pressedSkip++;
            if (pressedSkip >= 30)
            {
                SceneAudio.GetComponent<AudioSource>().PlayClip("UI_DIALOGUEPASS");
                EndDialogue();
                pressedSkip = 0;
            }
        }
        /*
        // Active Dialogue ID = 0 (firt dialogue)
        if (Input.GetKey(KeyCode.J) == KeyState.KEY_UP)
        {
            StartNewDialogue(0);
        }
        // Next Dialogue
        if (Input.GetKey(KeyCode.K) == KeyState.KEY_UP)
        {
            idDialogue++;
            StartNewDialogue(idDialogue);
        }
        //Desactive Dialogue
        if (Input.GetKey(KeyCode.O) == KeyState.KEY_UP)
        {
            DisableDialogue();
        }
        // Continue dialogue
        if (Input.GetKey(KeyCode.I) == KeyState.KEY_UP)
        {
            ContinueDialogue();
        }*/
    }
    private void DrawAuthor()
    {
        // 0=Paul / 1=Chani / 2=Rehen Fremen / 3=Rabban / 
        // 4=Soldado Harkonnen / 5=Soldado / 
        // 6=Lady Jessica / 7=Stilgar
        switch (authId)
        {
            case 0:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_paul.png");
                break;
            case 1:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_chani.png");
                break;
            case 2:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_fremen_hostage.png");
                break;
            case 3:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_rabann.png");
                break;
            case 4:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_soldier_1.png");
                break;
            case 5:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_soldier_2.png");
                break;
            case 6:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_jessica.png");
                break;
            case 7:
                image.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/ui_dialog_stilgar.png");
                break;

            default:
                break;
        }
    }

    void NextLine()
	{
		endDialogue = Dialogue.NextLine();
		if (endDialogue == false)
            UpdateDialogue();
        else
            EndDialogue();
	}

    void EndDialogue()
    {
        endDialogue = true;
        inDialogue = false;
        DisableDialogue();
    }

    void UpdateDialogue() 
    {
        text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
        auth = Dialogue.GetDialogueLineAuthor();
        name.GetComponent<UIText>().text = auth;
        authId = Dialogue.GetDialogueLineAuthorId();
        DrawAuthor();
    }

	public void StartNewDialogue(int id)
    {
        gameObject.isActive = true;
        endDialogue = false;
        inDialogue = true;
        Dialogue.StartDialogueById(id);
        UpdateDialogue();
        //Debug.Log(authId.ToString());
    }

    public void DisableDialogue()
    {
        gameObject.isActive = false;
    }

    public void ContinueDialogue()
    {
        UpdateDialogue();
        gameObject.isActive = true;
    }
    public void ContinueDialogueChangeLenguage()
    {
        Dialogue.LoadDialogueFile("");
        UpdateDialogue();
        gameObject.isActive = true;
    }
    public bool GetInDialogue() { return inDialogue; }

    public bool GetEndDialogue() { return endDialogue; }

    public void SaveDialogue()
    {
        SaveSystem.DeleteDirectoryFiles("Library/SavedGame/Dialogues");
        triggerColliders = GameObject.FindGameObjectsWithTag("DialogueTrigger");
        for (int i = 0; i < triggerColliders.Length; ++i)
        {
            SaveSystem.SaveDialogue(triggerColliders[i].GetComponent<DialogueTrigger>());
        }
    }

    public void LoadDialogue()
    {
        triggerColliders = GameObject.FindGameObjectsWithTag("DialogueTrigger");
        DialogueData data = SaveSystem.LoadDialogue(triggerColliders[0].name);
        if(data != null)
        {
            for (int i = 0; i < triggerColliders.Length; ++i)
            {
                data = SaveSystem.LoadDialogue(triggerColliders[i].name);
                if(data != null)
                    triggerColliders[i].GetComponent<DialogueTrigger>().SetUsed(false);
                else triggerColliders[i].GetComponent<DialogueTrigger>().SetUsed(true);
            }
        }        
    }
}