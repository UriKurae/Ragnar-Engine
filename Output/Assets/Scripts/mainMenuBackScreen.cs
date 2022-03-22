using System;
using RagnarEngine;

public class mainMenuBackScreen : RagnarComponent
{
	Vector3 pos;
	public void Start()
    {	
		
	}
	
	public void Update()
    {

        gameObject.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());

        pos = gameObject.GetComponent<Transform2D>().position2D;
		pos.Set(0.0f, -11.5f, 84.5f);
		gameObject.GetComponent<Transform2D>().position2D = pos;
	}

}