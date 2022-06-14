using System;
using RagnarEngine;

public class loseScreen : RagnarComponent
{
	Vector3 Pos;
	GameObject Back;
	GameObject Menu;
	GameObject Retry;
	GameObject MenuImage;
	GameObject RetryImage;
	GameObject AudioManager;

	GameObject[] players;
	GameObject[] enemies;

	bool isFirstM = true;
	bool isFirstR = true;
	public void Start()
	{
		Input.RestoreDefaultCursor();
		players = new GameObject[3];
		players = GameObject.FindGameObjectsWithTag("Player");

		enemies = new GameObject[2];
		enemies = GameObject.FindGameObjectsWithTag("Enemies");

		foreach (var item in enemies)
		{
			item.GetComponent<Animation>().PlayAnimation("Idle");
		}

		foreach (var item in players)
		{
			item.GetComponent<Animation>().PlayAnimation("Idle");
		}

		Back = GameObject.Find("Background");
		Menu = GameObject.Find("ButtonMenu");
		Retry = GameObject.Find("ButtonRetry");
		MenuImage = GameObject.Find("MenuImage");
		RetryImage = GameObject.Find("RetryImage");
		AudioManager = GameObject.Find("AudioLoseScene");
		Pos = new Vector3(0, 0, 0);
        AudioManager.GetComponent<AudioSource>().PlayClip("UI_LOSESCREEN");

		Pos.Set(MenuImage.GetComponent<Transform2D>().position2D.x - 5, MenuImage.GetComponent<Transform2D>().position2D.y, 36.1f);
		MenuImage.GetComponent<Transform2D>().position2D = Pos;

		Pos.Set(RetryImage.GetComponent<Transform2D>().position2D.x - 5, RetryImage.GetComponent<Transform2D>().position2D.y, 36.1f);
		RetryImage.GetComponent<Transform2D>().position2D = Pos;
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
				AudioManager.GetComponent<AudioSource>().PlayClip("UI_HOVER");
			}
			break;
		case 3:
			// pressed mode
			AudioManager.GetComponent<AudioSource>().PlayClip("UI_SELECT");
			Menu.GetComponent<UIButton>().SetButtonAlpha(0.75f);
			SceneManager.LoadScene("MainMenu");
			//cambiar de escena
			break;
		}
	}
	void RetryAction()
	{
		int a = Retry.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
		case 0:
			// disabled Mode
			break;
		case 1:
			if (!isFirstR)
			{
				Pos.Set(Retry.GetComponent<Transform2D>().position2D.x - 20, Retry.GetComponent<Transform2D>().position2D.y, 36.1f);
				Retry.GetComponent<Transform2D>().position2D = Pos;

				Pos.Set(RetryImage.GetComponent<Transform2D>().position2D.x - 20, RetryImage.GetComponent<Transform2D>().position2D.y, 36.1f);
				RetryImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstR = true;
			}
			// normal Mode
			break;
		case 2:
			// focused mode
			if (isFirstR)
			{
				Pos.Set(Retry.GetComponent<Transform2D>().position2D.x + 20, Retry.GetComponent<Transform2D>().position2D.y, 36.1f);
				Retry.GetComponent<Transform2D>().position2D = Pos;

				Pos.Set(RetryImage.GetComponent<Transform2D>().position2D.x + 20, RetryImage.GetComponent<Transform2D>().position2D.y, 36.1f);
				RetryImage.GetComponent<Transform2D>().position2D = Pos;
				isFirstR = false;
				//poner sonido
                AudioManager.GetComponent<AudioSource>().PlayClip("UI_HOVER");
			}
			break;
		case 3:
			// pressed mode
            AudioManager.GetComponent<AudioSource>().PlayClip("UI_SELECT");
			Retry.GetComponent<UIButton>().SetButtonAlpha(0.75f);
			if (SceneManager.lastSceneName == "build")
				SceneManager.LoadScene("build");
			if (SceneManager.lastSceneName == "build2")
				SceneManager.LoadScene("build2");
			if (SceneManager.lastSceneName == "build3")
				SceneManager.LoadScene("build3");
				//cambiar de escena
				break;
		}
	}
	public void Update()
	{
		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());
		MenuAction();
		RetryAction();
	}
}

