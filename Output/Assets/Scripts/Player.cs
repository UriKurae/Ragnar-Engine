using System;

using RagnarEngine;

public class Player : RagnarComponent
{
	public int velocity = 5;
	public GameObject target = null;
    public float force = 100;

    Rigidbody rb;
    MaterialComponent materialComponent;

    public void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        materialComponent = gameObject.GetComponent<MaterialComponent>();
    }

    public void Update()
	{
        if(Input.GetKey(KeyCode.X) == KeyState.KEY_DOWN)
        {
            //GameObject[] gos = GameObject.FindGameObjectsWithTag("Untagged");
            GameObject[] gos = gameObject.childs;

            foreach (GameObject go in gos)
            {
                Debug.Log(go.name);
            }
        }

        if (Input.GetKey(KeyCode.W) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN
			|| Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.D) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
			gameObject.GetComponent<Animation>().PlayAnimation("Walk");
		}

		if (Input.GetKey(KeyCode.R) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Reload");
		}

		if (Input.GetKey(KeyCode.SPACE) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Shot");
		}

		if (Input.GetKey(KeyCode.D) == KeyState.KEY_UP || Input.GetKey(KeyCode.A) == KeyState.KEY_UP
			|| Input.GetKey(KeyCode.W) == KeyState.KEY_UP || Input.GetKey(KeyCode.S) == KeyState.KEY_UP)
		{
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");
			gameObject.GetComponent<AudioSource>().StopCurrentClip();

            if (rb.linearVelocity != Vector3.zero)
                rb.linearVelocity = new Vector3(0, 0, 0);

            if (rb.totalForce != Vector3.zero)
                rb.ClearForces();
        }

        
        if (Input.GetKey(KeyCode.D) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(-1000, 0, 0);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(KeyCode.A) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(1000, 0, 0);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(KeyCode.W) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, 1000);
            rb.ApplyCentralForce(f);
        }

        else if (Input.GetKey(KeyCode.S) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, -1000);
            rb.ApplyCentralForce(f);
        }

        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
        {
            Vector3 pos = gameObject.transform.globalPosition;
            pos.y += 1;
            GameObject bullet = InternalCalls.Create3DObject("Bullet", (int)PrimitiveType.CUBE, pos);
            bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

            Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
            bulletRb.IgnoreCollision(gameObject, true);
            Vector3 vectorDir = new Vector3(gameObject.transform.forward.x, 1, gameObject.transform.forward.z);
            bulletRb.ApplyCentralForce(vectorDir.normalized * force);

            //AudioSource audio = gameObject.GetComponent<AudioSource>();
            //audio.PlayClip("Shot");
        }

        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
        {
            Vector3 pos = gameObject.transform.globalPosition;
            pos.y += 1;
            GameObject bullet = InternalCalls.Create3DObject("Knife", (int)PrimitiveType.CUBE, pos);
            bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

            Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
            bulletRb.IgnoreCollision(gameObject, true);
            bulletRb.ApplyCentralForce(gameObject.transform.forward * 1000);
        }
    }

    public void OnTriggerEnter(Rigidbody other)
    {
        other.gameObject.transform.localPosition = new Vector3(0, 5, 0);
    }
}

