using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	GameObject AudioController;
	public void Start()
	{
		AudioController = GameObject.Find("AudioMainMenu");
		AudioController.GetComponent<AudioSource>().PlayClip("MAINMENUBACKGROUNDMUSIC");
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
				AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
				isFirstS = false;
				//poner sonido
			}
			break;
		case 3:
			// pressed mode
			AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
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
				//poner sonido
				AudioController.GetComponent<AudioSource>().PlayClip("UIHOVER");
				isFirstE = false;
			}
			break;
		case 3:
			// pressed mode
			isFirstE = true;
			AudioController.GetComponent<AudioSource>().PlayClip("UISELECT");
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