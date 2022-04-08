using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	Vector3 pos;
	bool isSowing = false;
	bool isFirstS = true;
	bool isFirstE = true;

	//////////////PAUSE//////////////
	GameObject Image;
	GameObject Resume;
	GameObject MainM;
	GameObject Opt;

	//////////////AUDIO//////////////
	GameObject SceneAudio;
	float currVolume = 0.0f;

	//////////////GAME//////////////
	GameObject Character;
	GameObject AbilityImage;
	GameObject Ability1;
	GameObject Ability2;
	
	public void Start()
	{
		pos = new Vector3(0.0f, 0.0f, 0.0f);

		//////////////AUDIO//////////////
		SceneAudio = GameObject.Find("AudioLevel1");
		SceneAudio.GetComponent<AudioSource>().PlayClip("LEVEL1BGMUSIC");

		//////////////PAUSE//////////////
		Image = GameObject.Find("Background");
		Resume = GameObject.Find("Button Resume");
		MainM = GameObject.Find("Button MainMenu");
		Opt = GameObject.Find("Button Options");

		ImageHide();
		RectangleHide();
		ResumeButtonHide();
		MainMenuButtonHide();
		OptionsButtonHide();

		//////////////GAME//////////////
		Character = GameObject.Find("Char");
		Ability1 = GameObject.Find("ab1");
		Ability2 = GameObject.Find("ab2");
		AbilityImage = GameObject.Find("AbilImage");

		
	}
	
	public void Update()
	{
		if (Input.GetKey(KeyCode.I) == KeyState.KEY_DOWN)
		{
			SceneManager.LoadScene("WinScene");

		}
		if (Input.GetKey(KeyCode.G) == KeyState.KEY_DOWN)
		{
			SceneManager.LoadScene("LoseScene");
		}


		SetAllPositions();
		updateMenu();
		

	}
	void updateMenu()
    {
		if (isSowing)
		{
			ImageShow();
			RectangleShow();
			ResumeButtonShow();
			MainMenuButtonShow();
			OptionsButtonShow();
			if (isSowing == false)
			{
				ImageHide();
				RectangleHide();
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
			}
		}

		if (Input.GetKey(KeyCode.ESCAPE) == KeyState.KEY_DOWN)
		{
			if (isSowing)
			{
				isSowing = false;
				ImageHide();
				RectangleHide();
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
			}
			else
			{
				currVolume = SceneAudio.GetComponent<AudioSource>().GetClipVolume();
				SceneAudio.GetComponent<AudioSource>().SetClipVolume(15.0f);
				isSowing = true;
				
			}
		}
	}
	void SetAllPositions()
	{
		float sum = 100.0f;

		if (isSowing)
        {
			sum = 2000;
        }
		float y = -(InternalCalls.GetRegionGame().y / 2) + sum;
		float x = -(InternalCalls.GetRegionGame().x / 2) + 150;

		pos.Set(-136.0f, y + 30, -10.400f);
		Ability1.GetComponent<Transform2D>().position2D = pos;

		pos.Set(-69.0f, y+30, -10.400f);
		Ability2.GetComponent<Transform2D>().position2D = pos;

		////////////////
		///
		pos.Set(x, y, -10.400f);
		Character.GetComponent<Transform2D>().position2D = pos;

		pos.Set(0.0f, y, -10.400f);
		AbilityImage.GetComponent<Transform2D>().position2D = pos;

		//pos.Set(0.0f, y, -10.400f);
		//Ability3.GetComponent<Transform2D>().position2D = pos;
		//pos.Set(69.0f, y, -10.400f);
		//Ability4.GetComponent<Transform2D>().position2D = pos;
		//pos.Set(136.0f, y, -10.400f);
		//Ability5.GetComponent<Transform2D>().position2D = pos;
	}
	void ImageShow()
	{
		pos.Set(0.0f, 0.0f, -10.400f);
		Image.GetComponent<Transform2D>().position2D = pos;
		Image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

	}

	void ImageHide()
	{
		pos.Set(0.0f, 2000.0f, 30.0f);
		Image.GetComponent<Transform2D>().position2D = pos;
	}

	void ResumeButtonShow()
	{

		pos.Set(-850.0f, 0.0f, 36.1f);
		Resume.GetComponent<Transform2D>().position2D = pos;
		int a = Resume.GetComponent<UIButton>().GetButtonState();
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
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				isSowing = false;
				//Quitar menu de pausa
				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}
	}
	void ResumeButtonHide()
	{
		pos.Set(0.0f, 2000.0f, 36.1f);
		Resume.GetComponent<Transform2D>().position2D = pos;
	}

	void MainMenuButtonShow()
	{

		pos.Set(-850.0f, 50.0f, 36.1f);
		MainM.GetComponent<Transform2D>().position2D = pos;
		int a = MainM.GetComponent<UIButton>().GetButtonState();
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
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					isFirstE = false;

					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				SceneManager.LoadScene("MainMenu");
				//cambiar de escena
				break;
		}
	}
	void MainMenuButtonHide()
	{
		pos.Set(0.0f, 2000.0f, 36.1f);
		MainM.GetComponent<Transform2D>().position2D = pos;
	}


	void OptionsButtonShow()
	{
        pos.Set(-850.0f, -50.0f, 36.1f);
        Opt.GetComponent<Transform2D>().position2D = pos;

        // Options button logic
    }
	void OptionsButtonHide()
	{
		pos.Set(0.0f, 2000.0f, 36.1f);
		Opt.GetComponent<Transform2D>().position2D = pos;
	}
	void RectangleShow()
	{
		//pos.Set(-326.0f, 7.5f, 36.1f);
		//Rect.GetComponent<Transform2D>().position2D = pos;
	}
	void RectangleHide()
	{
		//pos.Set(0.0f, 2000.0f, 36.1f);
		//Rect.GetComponent<Transform2D>().position2D = pos;
	}
}

