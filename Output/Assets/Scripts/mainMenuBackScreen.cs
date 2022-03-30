using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	public void Start()
	{
		gameObject.GetComponent<AudioSource>().PlayClip("MainMenuBackgroundMusic");
	}
	public void Update()
	{
		// Button start
		PlayButtonAction();
		// Button exit
		ExitButtonAction();
		// Image back
		BackgroundImageAction();
	}
	void PlayButtonAction()
	{
		GameObject start = GameObject.Find("Button Start");
		int a = start.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
		case 0:
			// disabled Mode
			break;
		case 1:
			isFirstS = true;
			// normal Mode
			break;
		case 2:
			// focused mode
			if (isFirstS)
			{
				isFirstS = false;
				//poner sonido
			}
			break;
		case 3:
			// pressed mode
			SceneManager.NextScene();
			//cambiar de escena
			break;
		}
	}
	void ExitButtonAction()
	{
		GameObject exit = GameObject.Find("Button Exit");
		int a = exit.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
		case 0:
			// disabled Mode
			break;
		case 1:
			isFirstE = true;
			// normal Mode
			break;
		case 2:
			// focused mode
			if (isFirstE)
			{
				isFirstE = false;
				//poner sonido
			}
			break;
		case 3:
			// pressed mode
			isFirstE = true;
			SceneManager.Exit();
			//salir del Juego
			break;
		}

	}

	void BackgroundImageAction()
	{
		GameObject image = GameObject.Find("Background");
		image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());		

		imagePos = image.GetComponent<Transform2D>().position2D;
		imagePos.Set(0.0f, -11.5f, 84.5f);
		image.GetComponent<Transform2D>().position2D = imagePos;
	}
}