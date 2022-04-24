using System;
using RagnarEngine;

public class DialogueManager : RagnarComponent
{
	//UIText toxt;
	GameObject box;
	GameObject text;
	GameObject image;
    GameObject name;
	Vector3 pos;
	string auth;
    int authId;
    // 0=Paul / 1=Chani / 2=Rehen Fremen / 3=Rabban / 
    // 4=Soldado Harkonnen / 5=Soldado / 
    // 6=Lady Jessica / 7=Stilgar
    int idDialogue;
	bool firstTime = true;
	bool endDialogue;

	public void Start()
	{
		Dialogue.LoadDialogueFile("");
		box = GameObject.Find("DialogueBox");
		text = GameObject.Find("DialogueText");
		image = GameObject.Find("DialogueAuthImg");
        name = GameObject.Find("DialogueAuthName");
		auth = "";
        authId = -1;
        idDialogue = 0;
		pos = new Vector3(0, 0, 0);
		endDialogue = false;

        Disable();
        
    }
	public void Update()
    {
        if (firstTime)
        {
            pos.Set(-25.0f, text.GetComponent<Transform2D>().position2D.y, text.GetComponent<Transform2D>().position2D.z + 20);
            text.GetComponent<Transform2D>().position2D = pos;
            pos.Set(0.0f, image.GetComponent<Transform2D>().position2D.y, image.GetComponent<Transform2D>().position2D.z + 20);
            image.GetComponent<Transform2D>().position2D = pos;
            pos.Set(-25.0f, name.GetComponent<Transform2D>().position2D.y, image.GetComponent<Transform2D>().position2D.z + 20);
            name.GetComponent<Transform2D>().position2D = pos;
            pos.Set(box.GetComponent<Transform2D>().position2D.x, box.GetComponent<Transform2D>().position2D.y, box.GetComponent<Transform2D>().position2D.z-10);
            box.GetComponent<Transform2D>().position2D = pos;

            firstTime = false;
        }

        if (Input.GetKey(KeyCode.J) == KeyState.KEY_UP){
            StartNewDialogue(0);
        }

        if (Input.GetKey(KeyCode.K) == KeyState.KEY_UP){
            idDialogue++;
            StartNewDialogue(idDialogue);
        }

        if (Input.GetKey(KeyCode.L) == KeyState.KEY_UP){
            NextLine();
        }

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
		{
			text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
            authId = Dialogue.GetDialogueLineAuthorId();
            auth = Dialogue.GetDialogueLineAuthor();
            name.GetComponent<UIText>().text = auth;
            DrawAuthor();
            Debug.Log(authId.ToString());
        }
        else
		{
            // End the current dialogue
            Disable();

		}
	}
	public void StartNewDialogue(int id)
    {
        gameObject.isActive = true;
        endDialogue = false;
        Dialogue.StartDialogueById(id);
        text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
		auth = Dialogue.GetDialogueLineAuthor();
        name.GetComponent<UIText>().text = auth;
        authId = Dialogue.GetDialogueLineAuthorId();
        DrawAuthor();
        Debug.Log(authId.ToString());
    }
    void Disable()
    {
        endDialogue = true;
        gameObject.isActive = false;
    }

}