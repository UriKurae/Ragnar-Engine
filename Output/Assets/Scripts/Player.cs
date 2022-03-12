using System;
using RagnarEngine;

public class Player : RagnarComponent
{
	public int velocity = 5;
	public GameObject target = null;
	
	public void Update()
	{
		if (Input.GetKey(REKeyCode.W) == KeyState.KEY_DOWN || Input.GetKey(REKeyCode.A) == KeyState.KEY_DOWN
			|| Input.GetKey(REKeyCode.S) == KeyState.KEY_DOWN || Input.GetKey(REKeyCode.D) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
			gameObject.GetComponent<Animation>().PlayAnimation("Walk");
		}

		if (Input.GetKey(REKeyCode.R) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Reload");
		}

		if (Input.GetKey(REKeyCode.SPACE) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Shot");
		}

		if (Input.GetKey(REKeyCode.D) == KeyState.KEY_UP || Input.GetKey(REKeyCode.A) == KeyState.KEY_UP
			|| Input.GetKey(REKeyCode.W) == KeyState.KEY_UP || Input.GetKey(REKeyCode.S) == KeyState.KEY_UP)
		{
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");
			gameObject.GetComponent<AudioSource>().StopCurrentClip();
		}


        Transform tr = gameObject.GetComponent<Transform>();
        //Rigidbody rb = gameObject.GetComponent<Rigidbody>();
        if (Input.GetKey(REKeyCode.D) == KeyState.KEY_REPEAT)
        {
            tr.localPosition = new Vector3(tr.localPosition.x - velocity * Time.deltaTime, tr.localPosition.y, tr.localPosition.z);
            //Vector3 f = new Vector3(10000 * Time.deltaTime, 0, 0);
            //rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(REKeyCode.A) == KeyState.KEY_REPEAT)
        {
            tr.localPosition = new Vector3(tr.localPosition.x + velocity * Time.deltaTime, tr.localPosition.y, tr.localPosition.z);
            //gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
        }

        else if (Input.GetKey(REKeyCode.W) == KeyState.KEY_REPEAT)
        {
            tr.localPosition = new Vector3(tr.localPosition.x, tr.localPosition.y, tr.localPosition.z + velocity * Time.deltaTime);
            //gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
        }

        else if (Input.GetKey(REKeyCode.S) == KeyState.KEY_REPEAT)
        {
            tr.localPosition = new Vector3(tr.localPosition.x, tr.localPosition.y, tr.localPosition.z - velocity * Time.deltaTime);
            //gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
        }

        if (Input.GetKey(REKeyCode.SPACE) == KeyState.KEY_DOWN)
        {
            InternalCalls.Create3DObject("Bullet", (int)PrimitiveType.CUBE, Vector3.zero, Quaternion.identity);
            AudioSource audio = gameObject.GetComponent<AudioSource>();
            audio.PlayClip("Shot");
        }



    }

}
