using System;

using RagnarEngine;

public class Player : RagnarComponent
{
	public int velocity = 1000;
	public GameObject target = null;
    public float force = 100;
    public float rockSoundRadius = 4f;
    public bool canThrowKnife = true;

    Rigidbody rb;
    Material materialComponent;
    NavAgent agent;

    public void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        materialComponent = gameObject.GetComponent<Material>();
        agent = gameObject.GetComponent<NavAgent>();
    }

    public void Update()
	{
        if (agent.targetSetted)
            agent.CalculatePath(agent.destination);

        if (agent.MovePath()) { Debug.Log("No es null"); }

        ///////// SOUNDS /////////
        // Movement Sound
        if (Input.GetKey(KeyCode.W) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN
			|| Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.D) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
			gameObject.GetComponent<Animation>().PlayAnimation("Walk");
		}

        // Reload Sound
		if (Input.GetKey(KeyCode.R) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Reload");
		}

        // Shoot sound
		if (Input.GetKey(KeyCode.SPACE) == KeyState.KEY_DOWN)
		{
			gameObject.GetComponent<AudioSource>().PlayClip("Shot");
		}
        //////////////////////////

        ///////// MOVEMENT /////////
        // Idle
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
        // WASD Movement
        if (Input.GetKey(KeyCode.W) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, velocity);
            rb.ApplyCentralForce(f);
        }
        else if (Input.GetKey(KeyCode.A) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(velocity, 0, 0);
            rb.ApplyCentralForce(f);
        }
        else if (Input.GetKey(KeyCode.S) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(0, 0, -velocity);
            rb.ApplyCentralForce(f);
        }
        else if (Input.GetKey(KeyCode.D) == KeyState.KEY_REPEAT)
        {
            Vector3 f = new Vector3(-velocity, 0, 0);
            rb.ApplyCentralForce(f);
        }

        // Crouch
        if (Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
        {
            rb.SetHeight(0.6f); // 0.6 = 60%
        }
        if (Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_UP)
        {
            rb.SetHeight(1); // 1 = 100% = Reset
        }

    }

    public void OnTriggerEnter(Rigidbody other)
    {
        Debug.Log("OnTriggerEnter");
    }
    public void OnTrigger(Rigidbody other)
    {
        Debug.Log("OnTrigger");
    }
    public void OnCollisionEnter(Rigidbody other)
    {
        Debug.Log("OnCollisionEnter");
    }
    public void OnCollision(Rigidbody other)
    {
        Debug.Log("OnCollision");
    }
}



