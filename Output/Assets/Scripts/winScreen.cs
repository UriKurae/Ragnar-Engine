using System;
using RagnarEngine;

public class winScren : RagnarComponent
{
	bool firstTime = true;
	GameObject Back;
	GameObject Menu;

	public void Start()
    {

	}
	
	public void Update()
	{
		if (firstTime)
		{
			Back = GameObject.Find("Background");
			Menu = GameObject.Find("ButtonMenu");

			
			firstTime = false;
		}

		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		if (Menu.GetComponent<UIButton>().GetButtonState() == 3)
        {
			SceneManager.LoadScene("MainMenu");
        }
	}

}

