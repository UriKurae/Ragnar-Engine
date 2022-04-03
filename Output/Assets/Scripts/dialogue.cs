using System;
using RagnarEngine;

public class dialogue : RagnarComponent
{
	UIText toxt;
	public void Start()
	{
		toxt = gameObject.GetComponent<UIText>();
	}
	public void Update()
	{
		if (Input.GetKey(KeyCode.K) == KeyState.KEY_UP)
		{
			Dialogue.LoadDialogueFile("Test");
			Dialogue.StartDialogueById(0);
			toxt.text = Dialogue.GetDialogueLine();
		}

		if (Input.GetKey(KeyCode.J) == KeyState.KEY_UP)
		{
			Dialogue.StartDialogueById(1);
			toxt.text = Dialogue.GetDialogueLine();
		}

		if (Input.GetKey(KeyCode.L) == KeyState.KEY_UP)
		{
			Dialogue.NextLine();
			toxt.text = Dialogue.GetDialogueLine();
		}
	}

}