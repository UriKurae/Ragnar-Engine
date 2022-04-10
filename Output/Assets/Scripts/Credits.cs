using System;
using RagnarEngine;

public class Credits : RagnarComponent
{
	Vector3 Pos;
	GameObject Back;
	GameObject Menu;
	GameObject Text;
	GameObject MenuImage;
	GameObject AudioManager;
	float textCounter = 0;
	float newDelta=0;
	float velocity = 10;
	bool isFirstM = true;
	public void Start()
	{
		Back = GameObject.Find("Background");
		Menu = GameObject.Find("ButtonMenu");


		MenuImage = GameObject.Find("Main Menu Image");
		Text = GameObject.Find("Names List");
		AudioManager = GameObject.Find("AudioWinMenu");
		
		Pos = new Vector3(0, 0, 0);

		Pos.Set(0, 0, 36.1f);
		Text.GetComponent<Transform2D>().position2D = Pos;
	}
	public void Update()
	{
		newDelta = Time.deltaTime;

		BackgroundImageAction();

		MenuAction();

		textAction();
	}
	void textAction()
    {
		if (textCounter >= 5)
        {			
			float vel = newDelta * velocity;
			Pos.Set(0, Text.GetComponent<Transform2D>().position2D.y + vel, 36.1f);
			Text.GetComponent<Transform2D>().position2D = Pos;
        }
        else
        {
			textCounter += newDelta;
		}
		
	}
	void BackgroundImageAction()
	{

		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		Pos = Back.GetComponent<Transform2D>().position2D;
		Pos.Set(0.0f, 0.0f, 36.0f);
		Back.GetComponent<Transform2D>().position2D = Pos;
	}
	void MenuAction()
	{
		int a = Menu.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstM)
				{
					Pos.Set(Menu.GetComponent<Transform2D>().position2D.x - 20, Menu.GetComponent<Transform2D>().position2D.y, 36.1f);
					Menu.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(MenuImage.GetComponent<Transform2D>().position2D.x - 20, MenuImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					MenuImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstM = true;
				}



				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstM)
				{
					Pos.Set(Menu.GetComponent<Transform2D>().position2D.x + 20, Menu.GetComponent<Transform2D>().position2D.y, 36.1f);
					Menu.GetComponent<Transform2D>().position2D = Pos;

					Pos.Set(MenuImage.GetComponent<Transform2D>().position2D.x + 20, MenuImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					MenuImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstM = false;
					//poner sonido
					AudioManager.GetComponent<AudioSource>().PlayClip("UIHOVER");
				}
				break;
			case 3:
				// pressed mode
				AudioManager.GetComponent<AudioSource>().PlayClip("UISELECT");
				Menu.GetComponent<UIButton>().SetButtonAlpha(0.75f);
				SceneManager.LoadScene("MainMenu");
				//cambiar de escena
				break;


		}
	}
}

