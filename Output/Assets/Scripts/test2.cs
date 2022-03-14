using System;
using RagnarEngine;

public class test2 : RagnarComponent
{
	public void Start()
    {

    }

	public void Update()
	{
		if (Input.GetKey(KeyCode.M) == KeyState.KEY_DOWN)
		{
			Vector3 pos = gameObject.GetComponent<Transform2D>().position2D;
			gameObject.GetComponent<Transform2D>().position2D = pos;
		}
	}

}