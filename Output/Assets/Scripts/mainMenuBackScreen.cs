using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 imagePos;
	bool isFirstS = true;
	bool isFirstE = true;
	public void Start()
	{

	}
	void PlayButtonAction()
	{
		GameObject start = GameObject.Find("ButtonStart");
		switch (start.CreateComponent<UIButton>().GetButtonState())
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
				imagePos.Set(0.0f, -100.5f, 84.5f);
				start.GetComponent<Transform2D>().position2D = imagePos;

				//cambiar de escena
				break;
		}
	}
	void ExitButtonAction()
	{
		GameObject exit = GameObject.Find("ButtonExit");
		switch (exit.CreateComponent<UIButton>().GetButtonState())
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

				//salir del Juego
				break;
		}

	}

	void BackgroundImageAction()
	{
		GameObject Image = GameObject.Find("BackgroundImage");
		Image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());
		

		imagePos = Image.GetComponent<Transform2D>().position2D;
		imagePos.Set(0.0f, -11.5f, 84.5f);
		Image.GetComponent<Transform2D>().position2D = imagePos;
	}
	public void Update()
	{
        //button start

        //PlayButtonAction();
        ////button exit
        //ExitButtonAction();
        //image back
        BackgroundImageAction();


	}

}