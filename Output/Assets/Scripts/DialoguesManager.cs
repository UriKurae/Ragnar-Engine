using System;
using RagnarEngine;

public class DialogueManager : RagnarComponent
{
	//UIText toxt;
	GameObject text;
	GameObject paul;
	GameObject chani;
	Vector3 Pos;
	string auth;
	//bool dialogEnd = false;
	int id;
	bool firstTime = true;
	bool endDialogue;
	public void Start()
	{
		//toxt = gameObject.GetComponent<UIText>();
		text = GameObject.Find("DialogueText");
		paul = GameObject.Find("DialoguePaul");
		chani = GameObject.Find("DialogueChani");
		auth = "";
		id = 0;
		Pos = new Vector3(0, 0, 0);
		endDialogue = false;
	}
	public void Update()
	{
		if(firstTime)
        {
			Pos.Set(text.GetComponent<Transform2D>().position2D.x - 105.0f, text.GetComponent<Transform2D>().position2D.y - 10, text.GetComponent<Transform2D>().position2D.z);
			text.GetComponent<Transform2D>().position2D = Pos;
			firstTime = false;
        }
		
		
		
		if (Input.GetKey(KeyCode.J) == KeyState.KEY_UP)
		{
			Dialogue.LoadDialogueFile("dialogos_esp");
			Dialogue.StartDialogueById(id);
			text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
			//toxt.text = Dialogue.GetDialogueLine();
			auth = Dialogue.GetDialogueLineAuthor();
		}

		if (Input.GetKey(KeyCode.K) == KeyState.KEY_UP)
		{
			id++;
			Dialogue.StartDialogueById(id);
			text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
			auth = Dialogue.GetDialogueLineAuthor();
		}

		if (Input.GetKey(KeyCode.L) == KeyState.KEY_UP)
		{
			endDialogue = Dialogue.NextLine();
			if (endDialogue == false){
				text.GetComponent<UIText>().text = Dialogue.GetDialogueLine();
				auth = Dialogue.GetDialogueLineAuthor();
			}
			else{
				Debug.Log("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
			}
		}


		if (auth == "Paul")
		{
			paul.GetComponent<UIButton>().SetAlpha(1.0f);
			chani.GetComponent<UIButton>().SetAlpha(0.0f);
		}
		else if (auth == "Chani")
		{
			paul.GetComponent<UIButton>().SetAlpha(0.0f);
			chani.GetComponent<UIButton>().SetAlpha(1.0f);
		}
		else
		{
			paul.GetComponent<UIButton>().SetAlpha(0.0f);
			chani.GetComponent<UIButton>().SetAlpha(0.0f);
		}
	}
}