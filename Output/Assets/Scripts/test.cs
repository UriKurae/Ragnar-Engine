using System;
using RagnarEngine;

public class test : RagnarComponent
{
	public int prueba = 1;
	public int prueba2 = 5;

	public void Update()
	{
		Transform tr = gameObject.GetComponent<Transform>();

		if (Input.GetKey(REKeyCode.D) == KeyState.KEY_REPEAT)
			tr.localPosition = new Vector3(tr.localPosition.x - 5 * Time.deltaTime, tr.localPosition.y, tr.localPosition.z);

		else if (Input.GetKey(REKeyCode.A) == KeyState.KEY_REPEAT)
			tr.localPosition = new Vector3(tr.localPosition.x + 5 * Time.deltaTime, tr.localPosition.y, tr.localPosition.z);
		
		else if (Input.GetKey(REKeyCode.W) == KeyState.KEY_REPEAT)
			tr.localPosition = new Vector3(tr.localPosition.x, tr.localPosition.y, tr.localPosition.z + 5 * Time.deltaTime);
		
		else if (Input.GetKey(REKeyCode.S) == KeyState.KEY_REPEAT)
			tr.localPosition = new Vector3(tr.localPosition.x, tr.localPosition.y, tr.localPosition.z - 5 * Time.deltaTime);



	}

}




