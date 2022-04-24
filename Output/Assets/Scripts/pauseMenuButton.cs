using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	string actualOption = "Screen";
	Vector3 pos;
	Vector3 bounds;
	bool isOptions = false;
	bool isSowing = false;
	bool isFirstS = true;
	bool isFirstE = true;
	bool isFirstO = true;

	
	//////////////PAUSE//////////////
	GameObject Image;
	GameObject Resume;
	GameObject MainM;
	GameObject Opt;
	//////////////OPTIONS//////////////

	bool isFirstOBackB = true;
	bool isFirstOScreenB = true;
	bool isFirstOSoundB = true;
	bool isFirstOControlsB = true;

	GameObject optionsBack;
	GameObject optionsBackImage;
	GameObject optionsBackButton;
	GameObject optionsTransCuad;
	GameObject optionsTitle;
	GameObject optionsScreenButton;
	GameObject optionsScreenImage;
	GameObject optionsSoundButton;
	GameObject optionsSoundImage;
	GameObject optionsControlsButton;
	GameObject optionsControlsImage;
	//////////////AUDIO//////////////
	GameObject SceneAudio;
	float currVolume = 0.0f;

	//////////////GAME//////////////
	GameObject selectedPlayer;
	bool isFirstA1 = true;
	bool isFirstA2 = true;
	bool isFirstA3 = true;
	bool isFirstA4 = true;
	bool isFirstA5 = true;
	GameObject CharacterPhotoBord;
	GameObject AbilityBord;
	GameObject Ability1;
	GameObject Ability2;
	GameObject Ability3;
	GameObject Ability4;
	GameObject Ability5;

	GameObject UICharPhoto;
	GameObject UIAbilityArray;
	GameObject UICharBor1;
	GameObject UICharBor2;
	GameObject UICharBor3;
	GameObject UISelector;
	GameObject UICharacterName;

	GameObject UIPaulImage;
	GameObject UIChaniImage;
	GameObject UIStilgarImage;
	public void Start()
	{
		pos = new Vector3(0.0f, 0.0f, 0.0f);
		bounds = new Vector3(0.0f, 0.0f, 0.0f);
		//////////////AUDIO//////////////
		SceneAudio = GameObject.Find("AudioLevel1");
		SceneAudio.GetComponent<AudioSource>().PlayClip("LEVEL1BGMUSIC");

		//////////////PAUSE//////////////
		Image = GameObject.Find("Background");
		Resume = GameObject.Find("Button Resume");
		MainM = GameObject.Find("Button MainMenu"); 
		Opt = GameObject.Find("Button Options");
		
		ImageHide();

		ResumeButtonHide();
		MainMenuButtonHide();
		OptionsButtonHide();
		//////////////OPTIONS//////////////
		



        optionsBack = GameObject.Find("optionsBack");
        optionsBackImage = GameObject.Find("optionsBackImage");
        optionsBackButton = GameObject.Find("optionsBackButton");
		optionsTransCuad = GameObject.Find("optionsTransCuad");
		optionsTitle = GameObject.Find("optionsTitle");
		optionsScreenButton = GameObject.Find("optionsScreenButton");
		optionsScreenImage = GameObject.Find("optionsScreenImage");
		optionsSoundButton = GameObject.Find("optionsSoundButton");
		optionsSoundImage = GameObject.Find("optionsSoundImage");
		optionsControlsButton = GameObject.Find("optionsControlsButton");
		optionsControlsImage = GameObject.Find("optionsControlsImage");



		

		OptionsBackHide();
        //////////////GAME//////////////
        CharacterPhotoBord = GameObject.Find("Char");
		Ability1 = GameObject.Find("ab1");
		Ability2 = GameObject.Find("ab2");
		Ability3 = GameObject.Find("ab3");
		Ability4 = GameObject.Find("ab4");
		Ability5 = GameObject.Find("ab5");

		AbilityBord = GameObject.Find("AbilImage");

		UICharPhoto = GameObject.Find("UICharPhoto");

		UICharacterName = GameObject.Find("UICharacterName");

		UIAbilityArray = GameObject.Find("UIAbilityArray"); 
		UICharBor1 = GameObject.Find("UICharBor1"); 
		UICharBor2 = GameObject.Find("UICharBor2"); 
		UICharBor3 = GameObject.Find("UICharBor3"); 
		UISelector = GameObject.Find("UISelector");
		UIPaulImage = GameObject.Find("UIPaulImage");
		UIChaniImage = GameObject.Find("UIChaniImage");
		UIStilgarImage = GameObject.Find("UIStilgarImage");
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
		GameObject[] players = GameObject.FindGameObjectsWithTag("Player");
		selectedPlayer = players[GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characterSelected];

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
			SelectedOptionShow();
			OptionsAllButtonsShow();
			
		}
        else
        {
			OptionsBackHide();
			OptionsAllButtonsHide();
			SelectedOptionHide();

		}
    }
	void SelectedOptionShow()
    {
		if (actualOption != "Screen")
		{

		}
		else if (actualOption != "Controls")
		{

		}
		else if (actualOption != "Sound")
		{

		}
		else if (actualOption != "Graphics")
		{

		}

		if (actualOption == "Screen")
		{
			optionsScreenButton.GetComponent<UIButton>().SetButtonState(2);

		}
		else if (actualOption == "Controls")
		{
			optionsControlsButton.GetComponent<UIButton>().SetButtonState(2);

		}
		else if (actualOption == "Sound")
		{
			optionsSoundButton.GetComponent<UIButton>().SetButtonState(2);

		}

	}
	void SelectedOptionHide()
	{
		if (actualOption == "Screen")
		{
			
		}
		else if (actualOption == "Controls")
		{
			
		}
		else if (actualOption == "Sound")
		{
			
		}

	}
	void OptionsBackShow()
    {
		float X = (InternalCalls.GetRegionGame().x / 2);
		float Y = (InternalCalls.GetRegionGame().y / 2);
		pos.Set(0.0f, 0.0f, -10.400f);
		optionsBack.GetComponent<Transform2D>().position2D = pos;
		optionsBack.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());
		optionsBack.isActive = true;
		//////////

		pos.Set(X - 600, 0.0f, -10.400f);
		optionsTransCuad.GetComponent<Transform2D>().position2D = pos;

		bounds.Set(X, (InternalCalls.GetRegionGame().y-300), -10.400f);
		optionsTransCuad.GetComponent<Transform2D>().SetSize(bounds);
		optionsTransCuad.isActive = true;
		//////////

		pos.Set(X - (InternalCalls.GetRegionGame().x / 3), Y - 100, -10.400f);
		optionsTitle.GetComponent<Transform2D>().position2D = pos;
		optionsTitle.isActive = true;


	}
	void OptionsBackHide()
	{
		optionsBack.isActive = false;

		//////////
		optionsTransCuad.isActive = false;

		//////////
		optionsTitle.isActive = false;
	
	}
	void OptionsAllButtonsShow()
    {
		optionsBackButton.isActive = true;
		optionsBackImage.isActive = true;

		optionsScreenButton.isActive = true;
		optionsScreenImage.isActive = true;

		optionsSoundButton.isActive = true;
		optionsSoundImage.isActive = true;

		optionsControlsButton.isActive = true;
		optionsControlsImage.isActive = true;

		////////////////////////////////////////////////
		/// CONTROLS BUTTON
		////////////////////////////////////////////////

		int a = optionsControlsButton.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstOControlsB )
				{
					
					isFirstOControlsB = false;
					pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 380, -10.4f);
					optionsControlsButton.GetComponent<Transform2D>().position2D = pos;
					bounds.Set(275, 58.5f, 0);
					optionsControlsButton.GetComponent<Transform2D>().SetSize(bounds);

					pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 380, -10.4f);
					optionsControlsImage.GetComponent<Transform2D>().position2D = pos;
					bounds.Set(273.5f, 48, 0);
					optionsControlsImage.GetComponent<Transform2D>().SetSize(bounds);
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstOControlsB)
				{					
					isFirstOControlsB = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}
				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 380, -10.4f);
				optionsControlsButton.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(275 + 40, 58.5f, 0);
				optionsControlsButton.GetComponent<Transform2D>().SetSize(bounds);

				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 380, -10.4f);
				optionsControlsImage.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(273.5f + 40, 48, 0);
				optionsControlsImage.GetComponent<Transform2D>().SetSize(bounds);
				break;
			case 3:
				// pressed mode
				actualOption = "Controls";

				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}
		////////////////////////////////////////////////
		/// SOUND BUTTON
		////////////////////////////////////////////////

		a = optionsSoundButton.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstOSoundB )
				{
					
					isFirstOSoundB = false;
					pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 280, -10.4f);
					optionsSoundButton.GetComponent<Transform2D>().position2D = pos;
					bounds.Set(275, 58.5f, 0);
					optionsSoundButton.GetComponent<Transform2D>().SetSize(bounds);

					pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 280, -10.4f);
					optionsSoundImage.GetComponent<Transform2D>().position2D = pos;
					bounds.Set(273.5f, 48, 0);
					optionsSoundImage.GetComponent<Transform2D>().SetSize(bounds);
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstOSoundB)
				{					
					isFirstOSoundB = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}
				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 280, -10.4f);
				optionsSoundButton.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(275 + 40, 58.5f, 0);
				optionsSoundButton.GetComponent<Transform2D>().SetSize(bounds);

				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 280, -10.4f);
				optionsSoundImage.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(273.5f + 40, 48, 0);
				optionsSoundImage.GetComponent<Transform2D>().SetSize(bounds);
				break;
			case 3:
				// pressed mode
				actualOption = "Sound";

				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}

		////////////////////////////////////////////////
		/// SCREEN BUTTON
		////////////////////////////////////////////////

		a = optionsScreenButton.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstOScreenB)
				{
					
					isFirstOScreenB = false;
					pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 180, -10.4f);
					optionsScreenButton.GetComponent<Transform2D>().position2D = pos;
                    bounds.Set(275, 58.5f, 0);
                    optionsScreenButton.GetComponent<Transform2D>().SetSize(bounds);

                    pos.Set(-257, (InternalCalls.GetRegionGame().y / 2) - 180, -10.4f);
					optionsScreenImage.GetComponent<Transform2D>().position2D = pos;
                    bounds.Set(273.5f, 48, 0);
					optionsScreenImage.GetComponent<Transform2D>().SetSize(bounds);
                }
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstOScreenB)
				{				
					isFirstOScreenB = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}
				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 180, -10.4f);
				optionsScreenButton.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(275 + 40, 58.5f, 0);
				optionsScreenButton.GetComponent<Transform2D>().SetSize(bounds);

				pos.Set(-277, (InternalCalls.GetRegionGame().y / 2) - 180, -10.4f);
				optionsScreenImage.GetComponent<Transform2D>().position2D = pos;
				bounds.Set(273.5f + 40, 48, 0);
				optionsScreenImage.GetComponent<Transform2D>().SetSize(bounds);
				break;
			case 3:
				// pressed mode
				actualOption = "Screen";

				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}


		////////////////////////////////////////////////
		/// BACK BUTTON
		////////////////////////////////////////////////
		a = optionsBackButton.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstOBackB)
				{

					isFirstOBackB = false;
					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 80, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);

					optionsBackButton.GetComponent<Transform2D>().position2D = pos;


					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 80, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackImage.GetComponent<Transform2D>().position2D = pos;
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstOBackB)
				{
					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 120, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackButton.GetComponent<Transform2D>().position2D = pos;


					pos.Set(-(InternalCalls.GetRegionGame().x / 2) + 120, -(InternalCalls.GetRegionGame().y / 2) + 100, 36.1f);
					optionsBackImage.GetComponent<Transform2D>().position2D = pos;

					isFirstOBackB = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				isSowing = true;
				isOptions = false;


				isFirstOBackB = true;
				isFirstOScreenB = true;
				isFirstOSoundB = true;
				isFirstOControlsB = true;
				
				//Quitar menu de pausa
				SceneAudio.GetComponent<AudioSource>().SetClipVolume(currVolume);
				SceneAudio.GetComponent<AudioSource>().PlayClip("UISELECT");
				break;
		}
	}
	void OptionsAllButtonsHide()
	{
		optionsBackButton.isActive = false;
		optionsBackImage.isActive = false;

		optionsScreenButton.isActive = false;
		optionsScreenImage.isActive = false;

		optionsSoundButton.isActive = false;
		optionsSoundImage.isActive = false;

		optionsControlsButton.isActive = false;
		optionsControlsImage.isActive = false;
	}
	//////////////PAUSE//////////////
	void updateMenu()
    {
		if (isSowing)
		{
			ImageShow();
			
			ResumeButtonShow();
			MainMenuButtonShow();
			OptionsButtonShow();
			if (isSowing == false)
			{
				ImageHide();
				
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
			}
		}

		if (Input.GetKey(KeyCode.ESCAPE) == KeyState.KEY_DOWN && !isOptions)
		{
			if (isSowing)
			{
				isSowing = false;
				ImageHide();
				
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
		//GET POSITION



		if (isSowing||isOptions)
        {
			CharacterPhotoBord.isActive = false;
			AbilityBord.isActive = false;
			UICharPhoto.isActive = false;
			UIAbilityArray.isActive = false;
			UICharBor1.isActive = false;
			UICharBor2.isActive = false;
			UICharBor3.isActive = false;
			UICharacterName.isActive = false;
			UISelector.isActive = false;
			UIPaulImage.isActive = false;
			UIChaniImage.isActive = false;
			UIStilgarImage.isActive = false;
        }
        else
        {
			CharacterPhotoBord.isActive = true;
			AbilityBord.isActive = true;
			UICharPhoto.isActive = true;
			UIAbilityArray.isActive = true;
			UICharBor1.isActive = true;
			UICharBor2.isActive = true;
			UICharBor3.isActive = true;
			UICharacterName.isActive = true;
			UISelector.isActive = true;
			UIPaulImage.isActive = true;
			UIChaniImage.isActive = true;
			UIStilgarImage.isActive = true;
		}
		float y = -(InternalCalls.GetRegionGame().y / 2) + 100.0f; 
		float x = -(InternalCalls.GetRegionGame().x / 2) + 150;

		//UPDATE ABILITYS BUTTONS
		
		UpdateAbilityButton(x,y);

		//UPDATE THE REST ELEMENTS

		pos.Set(x, y+30, -10.400f);
		CharacterPhotoBord.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(214, 214f, 0);
		CharacterPhotoBord.GetComponent<Transform2D>().SetSize(bounds);


		pos.Set(0.0f, y-30, -10.400f);
		AbilityBord.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(650, 90f, 0);
		AbilityBord.GetComponent<Transform2D>().SetSize(bounds);

		
		
		
		
		pos.Set(x, y+35 , -10.400f);
		UICharPhoto.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(160, 160f, 0);
		UICharPhoto.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(0, y, -10.400f);
		UIAbilityArray.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(400, 45, 0);
		UIAbilityArray.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x+150, y, -10.400f);
		UICharBor1.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UICharBor1.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x + 230, y, -10.400f);
		UICharBor2.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UICharBor2.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x + 310, y, -10.400f);
		UICharBor3.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UICharBor3.GetComponent<Transform2D>().SetSize(bounds);

		if (selectedPlayer.name == "Player")
		{
			UICharPhoto.GetComponent<UIImage>().LoadTexture("Assets/Resources/UI/ui_paul_portrait.png");
			pos.Set(x + 150, y+30, -10.400f);
			UICharacterName.GetComponent<UIText>().text = "Paul";
		}
		else if (selectedPlayer.name == "Player_2")
		{
			UICharPhoto.GetComponent<UIImage>().LoadTexture("Assets/Resources/UI/ui_chani_portrait.png");
			pos.Set(x +230, y + 30, -10.400f);
			UICharacterName.GetComponent<UIText>().text = "Chani";
		}
		else if (selectedPlayer.name == "Player_3")
		{
			UICharPhoto.GetComponent<UIImage>().LoadTexture("Assets/Resources/UI/ui_stilgar_portrait.png");
			pos.Set(x + 310, y + 30, -10.400f);
			UICharacterName.GetComponent<UIText>().text = "Stilgar";
		}
		
		UISelector.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(20, 20, 0);
		UISelector.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x+387, y+182, -10.400f);
		UICharacterName.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(214, 214f, 0);
		UICharacterName.GetComponent<Transform2D>().SetSize(bounds);



		pos.Set(x+150 , y, -10.400f);
		UIPaulImage.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UIPaulImage.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x+230, y , -10.400f);
		UIChaniImage.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UIChaniImage.GetComponent<Transform2D>().SetSize(bounds);

		pos.Set(x+310, y , -10.400f);
		UIStilgarImage.GetComponent<Transform2D>().position2D = pos;
		bounds.Set(60, 60, 0);
		UIStilgarImage.GetComponent<Transform2D>().SetSize(bounds);
	}
	void ImageShow()
	{
		Image.isActive = true ;
		pos.Set(0.0f, 0.0f, -10.400f);
		Image.GetComponent<Transform2D>().position2D = pos;
		Image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

	}

	void ImageHide()
	{
		Image.isActive = false;
	}

	void ResumeButtonShow()
	{
		Resume.isActive = true;
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
		Resume.isActive = false;
	
	}

	void MainMenuButtonShow()
	{
		MainM.isActive=true;
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
		MainM.isActive = false;

	}


	void OptionsButtonShow()
	{
		Opt.isActive = true;
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
		Opt.isActive = false;
	}
	void UpdateAbilityButton(float x,float y)
    {
		if (isSowing || isOptions)
		{
			Ability1.isActive = false;
				Ability2.isActive = false;
			Ability3.isActive = false;
			Ability4.isActive = false;
			Ability5.isActive = false;

		}
        else
        {
			Ability1.isActive = true;
			Ability2.isActive = true;
			Ability3.isActive = true;
			Ability4.isActive = true;
			Ability5.isActive = true;
		}
		if (selectedPlayer.name == "Player")//paul
		{
			Ability1.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_throw_stone.png");
			Ability2.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_throwing_knife.png");
			Ability3.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_voice.png");
			Ability4.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_crysknife.png");
			Ability5.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
		}
		else if (selectedPlayer.name == "Player_2")//chani
		{
			Ability1.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_chani_camouflage.png");
			Ability2.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_chani_crysknife.png");
			Ability3.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
			Ability4.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
			Ability5.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
		}
		else if (selectedPlayer.name == "Player_3")//stilgar
		{
			Ability1.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_stilgar_stunner.png");
			Ability2.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_stilgar_sword.png");
			Ability3.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
			Ability4.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
			Ability5.GetComponent<UIButton>().LoadButtonTexture("Assets/Resources/UI/ui_paul_portrait.png");
		}
		////////////////////////////////////////////////
		/// ABILITY1 BUTTON
		////////////////////////////////////////////////
		int a = Ability1.GetComponent<UIButton>().GetButtonState();
		pos.Set(x + 625, y, -10.400f);
		Ability1.GetComponent<Transform2D>().position2D = pos;

		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstA1)
				{
					
					isFirstA1 = false;
					
				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstA1)
				{
					isFirstA1 = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}
				
				break;
			case 3:
				// pressed mode
				
				break;
		}

		////////////////////////////////////////////////
		/// ABILITY2 BUTTON
		////////////////////////////////////////////////

		a = Ability2.GetComponent<UIButton>().GetButtonState();
		pos.Set(x + 725, y, -10.400f);
		Ability2.GetComponent<Transform2D>().position2D = pos;
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstA2)
				{

					isFirstA2 = false;

				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstA2)
				{
					isFirstA2 = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode

				break;
		}

		////////////////////////////////////////////////
		/// ABILITY3 BUTTON
		////////////////////////////////////////////////

		a = Ability3.GetComponent<UIButton>().GetButtonState();
		pos.Set(0, y, -10.400f);
		Ability3.GetComponent<Transform2D>().position2D = pos;
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstA3)
				{

					isFirstA3 = false;

				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstA3)
				{
					isFirstA3 = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode

				break;
		}

		////////////////////////////////////////////////
		/// ABILITY4 BUTTON
		////////////////////////////////////////////////

		a = Ability4.GetComponent<UIButton>().GetButtonState();
		pos.Set(x + 895, y, -10.400f);
		Ability4.GetComponent<Transform2D>().position2D = pos;
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstA4)
				{

					isFirstA4 = false;

				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstA4)
				{
					isFirstA4 = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode

				break;
		}

		////////////////////////////////////////////////
		/// ABILITY5 BUTTON
		////////////////////////////////////////////////

		a = Ability5.GetComponent<UIButton>().GetButtonState();
		pos.Set(x + 980, y, -10.400f);
		Ability5.GetComponent<Transform2D>().position2D = pos;
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				if (isFirstA5)
				{

					isFirstA5 = false;

				}
				// normal Mode
				break;
			case 2:
				// focused mode
				if (!isFirstA5)
				{
					isFirstA5 = true;
					SceneAudio.GetComponent<AudioSource>().PlayClip("UIHOVER");
					//poner sonido
				}

				break;
			case 3:
				// pressed mode

				break;
		}
	}
}

