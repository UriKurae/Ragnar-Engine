using System;
using RagnarEngine;

public class dialogue : RagnarComponent
{
	UIText toxt;
	int id;
	public void Start()
	{
		toxt = gameObject.GetComponent<UIText>();
		id = 0;
	}
	public void Update()
	{
		if (Input.GetKey(KeyCode.J) == KeyState.KEY_UP)
		{
			Dialogue.LoadDialogueFile("dialogos_esp");
			Dialogue.StartDialogueById(id);
			toxt.text = Dialogue.GetDialogueLine();
		}

		if (Input.GetKey(KeyCode.K) == KeyState.KEY_UP)
		{
			id++;
			Dialogue.StartDialogueById(id);
			toxt.text = Dialogue.GetDialogueLine();
		}

		if (Input.GetKey(KeyCode.L) == KeyState.KEY_UP)
		{
			Dialogue.NextLine();
			toxt.text = Dialogue.GetDialogueLine();
		}
	}

}