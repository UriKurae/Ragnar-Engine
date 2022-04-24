using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	Vector3 pos;
	bool isOptions = false;
	bool isSowing = false;
	bool isFirstS = true;
	bool isFirstE = true;
	bool isFirstO = true;

	bool isFirstOBB = true;
	//////////////PAUSE//////////////
	GameObject Image;
	GameObject Resume;
	GameObject MainM;
	GameObject Opt;
	//////////////OPTIONS//////////////
	GameObject optionsBack;
	GameObject optionsBackImage;
	GameObject optionsBackButton;
	//////////////AUDIO//////////////
	GameObject SceneAudio;
	float currVolume = 0.0f;

	//////////////GAME//////////////
	GameObject CharacterBar;
	GameObject AbilityImage;
	GameObject Ability1;
	GameObject Ability2;
    DialogueManager dialogue;

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
        //////////////OPTIONS//////////////

        optionsBack = GameObject.Find("optionsBack");
        optionsBackImage = GameObject.Find("optionsBackImage");
        optionsBackButton = GameObject.Find("optionsBackButton");

        OptionsBackHide();
        //////////////GAME//////////////
        CharacterBar = GameObject.Find("Char");
		Ability1 = GameObject.Find("ab1");
		Ability2 = GameObject.Find("ab2");
		AbilityImage = GameObject.Find("AbilImage");
        dialogue = GameObject.Find("Dialogue").GetComponent<DialogueManager>();


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
		updateOptions();


	}
	//////////////OPTIONS//////////////
	void updateOptions()
    {
        if (isOptions)
        {
			OptionsBackShow();
			OptionsBackButtonShow();
		}
        else
        {
			OptionsBackHide();
			OptionsBackButtonHide();
		}
    }
	void OptionsBackShow()
    {
		pos.Set(0.0f, 0.0f, -10.400f);
		optionsBack.GetComponent<Transform2D>().position2D = pos;
		optionsBack.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());
	}
	void OptionsBackHide()
	{
		pos.Set(0.0f, 2000.0f, 30.0f);
		optionsBack.GetComponent<Transform2D>().position2D = pos;
	}
	void OptionsBackButtonShow()
    {
		
		int a = optionsBackButton.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
                if (isFirstOBB) {
					
					isFirstOBB = false;
					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 80, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					
					optionsBackButton.GetComponent<Transform2D>().position2D = pos;


					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 80, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackImage.GetComponent<Transform2D>().position2D = pos;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstOBB)
				{
					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 120, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackButton.GetComponent<Transform2D>().position2D = pos;


					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 120, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackImage.GetComponent<Transform2D>().position2D = pos;

					isFirstOBB = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				isSowing = true;
				isOptions = false;
				isFirstOBB = true;
				//Quitar menu de pausa
				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}
	}
	void OptionsBackButtonHide()
	{
		pos.Set(0.0f, 2000.0f, 36.1f);
		optionsBackButton.GetComponent<Transform2D>().position2D = pos;
		optionsBackImage.GetComponent<Transform2D>().position2D = pos;
	}
	//////////////PAUSE//////////////
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
                if (dialogue.GetInDialogue()) { dialogue.ContinueDialogue(); }
            }
            else
			{
                if (dialogue.GetInDialogue()) { dialogue.DisableDialogue(); }
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
		pos.Set(x, y, -10.400f);
		CharacterBar.GetComponent<Transform2D>().position2D = pos;

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
		int a = Opt.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				isFirstO = true;

				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstO)
				{
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					isFirstO = false;

					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				isOptions = true;
				isSowing = false;
				//cambiar de escena
				break;
		}
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

