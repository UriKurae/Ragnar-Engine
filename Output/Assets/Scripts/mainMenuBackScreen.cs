using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 Pos;
	bool isFirstS = true;
	bool isFirstE = true;
	bool isFirstCre = true;
	bool isFirstCont = true;
	bool isFirstOp = true;
	GameObject AudioController;
	GameObject continueB;
	GameObject start;
	GameObject options;
	GameObject exit;
	GameObject credits;

	GameObject continueImage;
	GameObject startImage;
	GameObject optionsImage;
	GameObject exitImage;
	GameObject creditsImage;

	GameObject back ;
	public void Start()
	{
		AudioController = GameObject.Find("AudioMainMenu");
		AudioController.GetComponent<AudioSource>().PlayClip("MUSICPLAY");
		AudioController.GetComponent<AudioSource>().SetState("MUSIC", "MAINMENU");
		continueB = GameObject.Find("Button Continue");
		start = GameObject.Find("Button Start");
		options = GameObject.Find("Button Options");
		exit = GameObject.Find("Button Exit");
		credits = GameObject.Find("Button Credits");



		continueImage = GameObject.Find("Image Continue");
		startImage = GameObject.Find("Image Start");
		optionsImage = GameObject.Find("Image Options");
		exitImage = GameObject.Find("Image Exit");
		creditsImage = GameObject.Find("Image Credits");



		back = GameObject.Find("Background");
	}

	public void Update()
	{
		// Button start
		PlayButtonAction();
		// Button exit
		ExitButtonAction();
		// Button credits
		CreditsButtonAction();
		// Button options
		ContinueButtonAction();
		// Button options
		OptionsButtonAction();
		// Image back
		BackgroundImageAction();
	}
    void ContinueButtonAction()
    {
		int a = continueB.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstCont)
				{
					Pos.Set(continueB.GetComponent<Transform2D>().position2D.x - 20, continueB.GetComponent<Transform2D>().position2D.y, 36.1f);
					continueB.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(continueImage.GetComponent<Transform2D>().position2D.x - 20, continueImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					continueImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstCont = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstCont)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					Pos.Set(continueB.GetComponent<Transform2D>().position2D.x + 20, continueB.GetComponent<Transform2D>().position2D.y, 36.1f);
					continueB.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(continueImage.GetComponent<Transform2D>().position2D.x + 20, continueImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					continueImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstCont = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				
				
				//cambiar de escena
				break;
		}
	}
	void OptionsButtonAction()
    {
		int a = options.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (!isFirstOp)
				{
					Pos.Set(options.GetComponent<Transform2D>().position2D.x - 20, options.GetComponent<Transform2D>().position2D.y, 36.1f);
					options.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(optionsImage.GetComponent<Transform2D>().position2D.x - 20, optionsImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					optionsImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstOp = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstOp)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					Pos.Set(options.GetComponent<Transform2D>().position2D.x + 20, options.GetComponent<Transform2D>().position2D.y, 36.1f);
					options.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(optionsImage.GetComponent<Transform2D>().position2D.x + 20, optionsImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					optionsImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstOp = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				

				//cambiar de escena
				break;
		}
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
				if (!isFirstCre)
				{
					Pos.Set(credits.GetComponent<Transform2D>().position2D.x - 20, credits.GetComponent<Transform2D>().position2D.y, 36.1f);
					credits.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(creditsImage.GetComponent<Transform2D>().position2D.x - 20, creditsImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					creditsImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstCre = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstCre)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					Pos.Set(credits.GetComponent<Transform2D>().position2D.x + 20, credits.GetComponent<Transform2D>().position2D.y, 36.1f);
					credits.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(creditsImage.GetComponent<Transform2D>().position2D.x + 20, creditsImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					creditsImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstCre = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				
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
					Pos.Set(start.GetComponent<Transform2D>().position2D.x - 20, start.GetComponent<Transform2D>().position2D.y, 36.1f);
					start.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(startImage.GetComponent<Transform2D>().position2D.x - 20, startImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					startImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstS = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstS)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					Pos.Set(start.GetComponent<Transform2D>().position2D.x + 20, start.GetComponent<Transform2D>().position2D.y, 36.1f);
					start.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(startImage.GetComponent<Transform2D>().position2D.x + 20, startImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					startImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstS = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				
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
					Pos.Set(exit.GetComponent<Transform2D>().position2D.x - 20, exit.GetComponent<Transform2D>().position2D.y, 36.1f);
					exit.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(exitImage.GetComponent<Transform2D>().position2D.x - 20, exitImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					exitImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstE = true;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstE)
				{
					AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
					Pos.Set(exit.GetComponent<Transform2D>().position2D.x + 20, exit.GetComponent<Transform2D>().position2D.y, 36.1f);
					exit.GetComponent<Transform2D>().position2D = Pos;


					Pos.Set(exitImage.GetComponent<Transform2D>().position2D.x + 20, exitImage.GetComponent<Transform2D>().position2D.y, 36.1f);
					exitImage.GetComponent<Transform2D>().position2D = Pos;
					isFirstE = false;
					//poner sonido
				}
				break;
			case 3:
				// pressed mode
				AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
				
				SceneManager.Exit();
				//salir del Juego
				break;
		}

	}

	void BackgroundImageAction()
	{

		back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		Pos = back.GetComponent<Transform2D>().position2D;
		Pos.Set(0.0f, 0.0f, -8.5f);
		back.GetComponent<Transform2D>().position2D =Pos;
	}
}
