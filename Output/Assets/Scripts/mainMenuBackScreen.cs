using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	float textPositionX;
	float textPositiony;
	public void Start()
	{
		GameObject start = GameObject.Find("Button Start");
		textPositionX= start.GetComponent<UIButton>().GetTextPosition().x;
		textPositiony = start.GetComponent<UIButton>().GetTextPosition().y;
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
				if (isFirstS == false)
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
				
					start.GetComponent<UIButton>().SetTextPosition(-13.6f, -4.6f);
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
		imagePos.Set(0.0f, 0.0f, 35.0f);
		image.GetComponent<Transform2D>().position2D = imagePos;
	}
}