using System;
using RagnarEngine;

public class winScren : RagnarComponent
{
	GameObject Back;
	GameObject Menu;
	GameObject Next;
	public void Start()
    {
		Back = GameObject.Find("Background");
		Menu = GameObject.Find("ButtonMenu");
		Next = GameObject.Find("ButtonNextLevel");

	}
	
	public void Update()
	{
		

		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		if (Menu.GetComponent<UIButton>().GetButtonState() == 3)
        {
			SceneManager.LoadScene("MainMenu");
        }
		if (Next.GetComponent<UIButton>().GetButtonState() == 3)
		{
			SceneManager.NextScene();
		}
	}

}

