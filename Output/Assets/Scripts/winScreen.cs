using System;
using RagnarEngine;

public class winScren : RagnarComponent
{
	Vector3 Pos;
	GameObject Back;
	GameObject Menu;
	GameObject Next;
	GameObject Replay;

	GameObject MenuImage;
	GameObject NextImage;
	GameObject ReplayImage;
	GameObject AudioManager;

	bool isFirstM = true;
	bool isFirstA = true;
	bool isFirstR = true;
	public void Start()
	{
		Back = GameObject.Find("Background");
		Menu = GameObject.Find("ButtonMenu");
		Next = GameObject.Find("ButtonNextLevel");
		Replay = GameObject.Find("ButtonReplay");

		MenuImage = GameObject.Find("Main Menu Image");
		NextImage = GameObject.Find("Next Level Image");
		ReplayImage = GameObject.Find("Replay Image");
		AudioManager = GameObject.Find("AudioWinScene");

		Pos = new Vector3(0, 0, 0);
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
				Menu.GetComponent<UIButton>().SetAlpha(0.75f);
				SceneManager.LoadScene("MainMenu");
				//cambiar de escena
				break;


		}
	}
	void nextAction()
	{
		int a = Next.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstA)
				{
					Pos.Set(Next.GetComponent<Transform2D>().position2D.x + 20, Next.GetComponent<Transform2D>().position2D.y, 36.1f);
					Next.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(NextImage.GetComponent<Transform2D>().position2D.x + 20, NextImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					NextImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstA = true;
				}



				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstA)
				{

					Pos.Set(Next.GetComponent<Transform2D>().position2D.x - 20, Next.GetComponent<Transform2D>().position2D.y, 36.1f);
					Next.GetComponent<Transform2D>().position2D = Pos;

					Pos.Set(NextImage.GetComponent<Transform2D>().position2D.x - 20, NextImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					NextImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstA = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				Next.GetComponent<UIButton>().SetAlpha(0.75f);
				SceneManager.NextScene();
				//cambiar de escena
				break;


		}
	}

	void ReplayAction()
	{
		int a = Replay.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstR)
				{

					Pos.Set(Replay.GetComponent<Transform2D>().position2D.x, Replay.GetComponent<Transform2D>().position2D.y - 20, 0);
					Replay.GetComponent<Transform2D>().position2D = Pos;

					Pos.Set(ReplayImage.GetComponent<Transform2D>().position2D.x, ReplayImage.GetComponent<Transform2D>().position2D.y - 20, 0);
					ReplayImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstR = true;
				}



				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstR)
				{

					Pos.Set(Replay.GetComponent<Transform2D>().position2D.x, Replay.GetComponent<Transform2D>().position2D.y + 20, 0);
					Replay.GetComponent<Transform2D>().position2D = Pos;

					Pos.Set(ReplayImage.GetComponent<Transform2D>().position2D.x, ReplayImage.GetComponent<Transform2D>().position2D.y + 20, 0);
					ReplayImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstR = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				Replay.GetComponent<UIButton>().SetAlpha(0.75f);
				SceneManager.LoadScene("build");
				//cambiar de escena
				break;


		}
	}
	public void Update()
	{
		MenuAction();
		nextAction();
		ReplayAction();
		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

	}

}

