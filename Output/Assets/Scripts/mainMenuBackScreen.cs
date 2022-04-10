using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	bool isFirstC = true;
	GameObject AudioController;
	GameObject start;
	GameObject exit;
	GameObject credits;
	GameObject image ;
	public void Start()
	{
		AudioController = GameObject.Find("AudioMainMenu");
		AudioController.GetComponent<AudioSource>().PlayClip("MUSICPLAY");
		AudioController.GetComponent<AudioSource>().SetState("MUSIC", "MAINMENU");
		start = GameObject.Find("Button Start"); 
		exit = GameObject.Find("Button Exit");
		credits = GameObject.Find("Button Credits");
		image = GameObject.Find("Background");
	}

	public void Update()
	{
		// Button start
		PlayButtonAction();
		// Button exit
		ExitButtonAction();
		// Button credits
		CreditsButtonAction();
		// Image back
		BackgroundImageAction();
	}
	void CreditsButtonAction()
	{
		int a = credits.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstC)
				{
					credits.GetComponent<UIButton>().SetTextPosition(-23.6f, -4.6f);
					isFirstC = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstC)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					credits.GetComponent<UIButton>().SetTextPosition(-13.6f, -4.6f);
					isFirstC = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				credits.GetComponent<UIButton>().SetTextPosition(-0.0f, -4.6f);
				SceneManager.LoadScene("Credits");
				//cambiar de escena
				break;
		}
	}
	void PlayButtonAction()
	{
		int a = start.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstS)
				{
					start.GetComponent<UIButton>().SetTextPosition(-23.6f, -4.6f);
					isFirstS = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstS)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					start.GetComponent<UIButton>().SetTextPosition(-13.6f, -4.6f);
					isFirstS = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				start.GetComponent<UIButton>().SetTextPosition(-0.0f, -4.6f);
				SceneManager.LoadScene("build");
				//cambiar de escena
				break;
		}
	}
	void ExitButtonAction()
	{
		
		int a = exit.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstE)
				{
					exit.GetComponent<UIButton>().SetTextPosition(-23.6f, -4.8f);
					isFirstE = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstE)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					exit.GetComponent<UIButton>().SetTextPosition(-13.6f, -4.8f);
					isFirstE = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				exit.GetComponent<UIButton>().SetTextPosition(-0.0f, -4.8f);
				SceneManager.Exit();
				//salir del Juego
				break;
		}

	}

	void BackgroundImageAction()
	{
		
		image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		imagePos = image.GetComponent<Transform2D>().position2D;
		imagePos.Set(0.0f, 0.0f, -8.5f);
		image.GetComponent<Transform2D>().position2D = imagePos;
	}
}
