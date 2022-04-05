using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	GameObject AudioController;
	GameObject start;
	public void Start()
	{
		AudioController = GameObject.Find("AudioMainMenu");
		AudioController.GetComponent<AudioSource>().PlayClip("MAINMENUBACKGROUNDMUSIC");
		start = GameObject.Find("Button Start");
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
		GameObject image = GameObject.Find("Background");
		image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		imagePos = image.GetComponent<Transform2D>().position2D;
		imagePos.Set(0.0f, 0.0f, -8.5f);
		image.GetComponent<Transform2D>().position2D = imagePos;
	}
}
