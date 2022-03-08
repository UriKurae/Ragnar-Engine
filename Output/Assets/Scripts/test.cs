using System;
using RagnarEngine;

public class test : RagnarComponent
{
	public int prueba = 1;
	public int prueba2 = 5;

	public void Update()
	{
		Transform tr = gameObject.GetComponent<Transform>();
		
		if(Input.GetKey(REKeyCode.B) == KeyState.KEY_DOWN)
			tr.localPosition = new Vector3(tr.localPosition.x + 1, 10, 10);


		if (Input.GetKey(REKeyCode.P) == KeyState.KEY_DOWN)
			gameObject.Instantiate("New Instance", new Vector3(-5, 0, 0), Quaternion.identity);



		Debug.Log("Script Update Methoddd"); // This Works
	}

}




