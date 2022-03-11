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
			gameObject.GetComponent<AudioSource>().StopCurrentClip();
		}


        Transform tr = gameObject.GetComponent<Transform>();
        Rigidbody rb = gameObject.GetComponent<Rigidbody>();
        if (Input.GetKey(REKeyCode.D) == KeyState.KEY_REPEAT)
        {
            rb.linearVelocity += new Vector3(-50 * Time.deltaTime, 0, 0);
        }

        else if (Input.GetKey(REKeyCode.A) == KeyState.KEY_REPEAT)
        {
            //Vector3 f = new Vector3(-10000 * Time.deltaTime, 0, 0);
            //rb.ApplyCentralForce(f);
            rb.linearVelocity += new Vector3(50 * Time.deltaTime, 0, 0);
        }

        else if (Input.GetKey(REKeyCode.W) == KeyState.KEY_REPEAT)
        {
            //Vector3 f = new Vector3(0, 0, 10000 * Time.deltaTime);
            //rb.ApplyCentralForce(f);
            rb.linearVelocity += new Vector3(0, 0, 50 * Time.deltaTime);

        }

        else if (Input.GetKey(REKeyCode.S) == KeyState.KEY_REPEAT)
        {
            //Vector3 f = new Vector3(0, 0, -10000 * Time.deltaTime);
            //rb.ApplyCentralForce(f);
            rb.linearVelocity += new Vector3(0, 0, -50 * Time.deltaTime);
        }

        if (Input.GetKey(REKeyCode.SPACE) == KeyState.KEY_DOWN)
        {
            GameObject bullet = InternalCalls.Create3DObject("Bullet", (int)PrimitiveType.CUBE, gameObject.transform.globalPosition);
            Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
            bulletRb.IgnoreCollision(gameObject, true);
            bulletRb.linearVelocity += new Vector3(0, 1000 * Time.deltaTime, 0);
            AudioSource audio = gameObject.GetComponent<AudioSource>();
            audio.PlayClip("Shot");
        }
    }

}
