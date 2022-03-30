using System;
using RagnarEngine;

public class loseScreen : RagnarComponent
{
	bool firstTime = true;
	GameObject Back;
	GameObject Menu;
	GameObject Retry;


	public void Start()
    {
		gameObject.GetComponent<AudioSource>().PlayClip("LoseBackgroundMusic");
	}
	
	public void Update()
	{
		if (firstTime)
		{
			Back = GameObject.Find("Background");
			Menu = GameObject.Find("ButtonMenu");
			Retry = GameObject.Find("ButtonRetry");

			firstTime = false;
		}

		Back.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

		if (Menu.GetComponent<UIButton>().GetButtonState() == 3)
        {
			SceneManager.LoadScene("MainMenu");
        }
		if (Retry.GetComponent<UIButton>().GetButtonState() == 3)
		{
			SceneManager.LoadScene("build");
		}
	}
}

