using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	Vector3 pos;
	bool isSowing = false;
	bool firstTime = true;
	
	public void Start()
    {
		
		
	}
	
	public void Update()
	{
		if (firstTime)
		{
			pos = gameObject.GetComponent<Transform2D>().position2D;
			pos.Set(0.0f, 1000.0f, 0.0f);
			gameObject.GetComponent<Transform2D>().position2D = pos;
			firstTime = false;
		}

		if (Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN)
		{
			if (isSowing)
			{
				isSowing = false;
				pos.Set(0.0f, 1000.0f, 0.0f);
				gameObject.GetComponent<Transform2D>().position2D = pos;
			}
			else
			{
				pos.Set(0.0f, 100.0f, 0.0f);
            
				gameObject.GetComponent<Transform2D>().position2D = pos;
				isSowing = true;
				
			}
		}
	}

}