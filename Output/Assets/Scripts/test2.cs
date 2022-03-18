using System;
using RagnarEngine;

public class test2 : RagnarComponent
{

	public void Start()
	{

	}

	public void Update()
	{

		if (gameObject.GetComponent<UIButton>().GetButtonState() == 3)
		{

			Vector3 pos = gameObject.GetComponent<Transform2D>().position2D;
			pos.Set(0.0f, 1000.0f, 0.0f);

			gameObject.GetComponent<Transform2D>().position2D = pos;
			
		}
	}

}