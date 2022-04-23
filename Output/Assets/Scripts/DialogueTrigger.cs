using System;
using RagnarEngine;

public class DialogueTrigger : RagnarComponent
{
    GameObject manager;
    public int dialogueId;
    bool isUsed;
	public void Start()
	{
        manager = GameObject.Find("Dialogue");
        isUsed = false;
    }
    public void Update()
	{

	}

    public void ActiveDialogue()
    {
        if (!isUsed)
        {
            manager.GetComponent<DialogueManager>().StartNewDialogue(dialogueId);
            isUsed = true;
        }
    }


}