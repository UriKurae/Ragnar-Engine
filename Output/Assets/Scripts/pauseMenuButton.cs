using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	Vector3 pos;
	bool isSowing = false;
	bool isFirstS = true;
	bool isFirstE = true;
	GameObject Image;
	GameObject Resume;
	GameObject MainM;
	GameObject Opt;
	//GameObject Rect;
	GameObject SceneAudio;

	float currVolume = 0.0f;
	public void Start()
	{
		SceneAudio = GameObject.Find("AudioLevel1");
		//SceneAudio.GetComponent<AudioSource>().PlayClip("LEVEL1BGMUSIC");

		Image = GameObject.Find("Background");

		//Rect = GameObject.Find("UI Rectangle");
		Resume = GameObject.Find("Button Resume");
		MainM = GameObject.Find("Button MainMenu");
		Opt = GameObject.Find("Button Options");

		pos = new Vector3(0.0f, 0.0f, 0.0f);

		ImageHide();
		RectangleHide();
		ResumeButtonHide();
		MainMenuButtonHide();
		OptionsButtonHide();
	}

	public void Update()
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

				ImageShow();
				RectangleShow();
				ResumeButtonShow();
				MainMenuButtonShow();
				OptionsButtonShow();

			}
			else
			{

				currVolume = SceneAudio.GetComponent<AudioSource>().GetClipVolume();
				SceneAudio.GetComponent<AudioSource>().SetClipVolume(15.0f);
				ImageHide();
				RectangleHide();
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
				isSowing = true;

			}
		}
	}

	void ImageShow()
	{
		pos.Set(0.0f, 0.0f, 30.0f);
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
		pos.Set(-850.0f, 30.0f, 36.1f);
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
		pos.Set(-850.0f, -30f, 36.1f);
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
		//pos.Set(-650.5f, 0.0f, 36.1f);
		//Opt.GetComponent<Transform2D>().position2D = pos;

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

