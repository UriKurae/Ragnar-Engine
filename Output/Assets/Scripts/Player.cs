using System;

using RagnarEngine;

public class Player : RagnarComponent
{
	public int velocity = 5;
	public GameObject target = null;

    public void Update()
	{
        Transform tr = gameObject.GetComponent<Transform>();
        Rigidbody rb = gameObject.GetComponent<Rigidbody>();

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

            if (rb.linearVelocity != Vector3.zero)
                rb.linearVelocity = new Vector3(0, 0, 0);

            if (rb.totalForce != Vector3.zero)
                rb.ClearForces();
        }

        
        if (Input.GetKey(REKeyCode.D) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(-1000, 0, 0);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(REKeyCode.A) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(1000, 0, 0);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(REKeyCode.W) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, 1000);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(REKeyCode.S) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, -1000);
            rb.ApplyCentralForce(f);
        }

        if (Input.GetKey(REKeyCode.SPACE) == KeyState.KEY_DOWN)
        {
            Vector3 pos = gameObject.transform.globalPosition;
            pos.y += 1;
            GameObject bullet = InternalCalls.Create3DObject("Bullet", (int)PrimitiveType.CUBE, pos);
            bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

            Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
            bulletRb.IgnoreCollision(gameObject, true);
            bulletRb.ApplyCentralForce(gameObject.transform.forward * 1000);

            AudioSource audio = gameObject.GetComponent<AudioSource>();
            audio.PlayClip("Shot");
        }
    }
}

