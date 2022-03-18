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

    public void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        materialComponent = gameObject.GetComponent<Material>();
    }

    public void Update()
	{
        if(Input.GetKey(KeyCode.X) == KeyState.KEY_DOWN)
        {
            //GameObject but = GameObject.Find("Button");
            //but.GetComponent<Button>().text = "Testint Text";
        }

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
        ////////////////////////////

        ///////// ABILITIES /////////
        // Rock Throw
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

            // Falta if(OnCollision(bullet, floor))
            GameObject soundArea = InternalCalls.CreateGameObject("SoundArea", bullet.transform.globalPosition, bullet.transform.globalRotation);
            Rigidbody soundRb = soundArea.CreateComponent<Rigidbody>();
            soundRb.IgnoreCollision(gameObject, true);
            CreateSphereTrigger(soundRb, rockSoundRadius);
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN && canThrowKnife)
        {
            canThrowKnife = false;
            Vector3 pos = gameObject.transform.globalPosition;
            pos.y += 1;
            GameObject bullet = InternalCalls.Create3DObject("Knife", (int)PrimitiveType.CUBE, pos);
            bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

            Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
            bulletRb.IgnoreCollision(gameObject, true);
            bulletRb.ApplyCentralForce(gameObject.transform.forward * 1000);
        }
        /////////////////////////////
    }

    // With this method we create an spherical Trigger.
    private static void CreateSphereTrigger(Rigidbody rb, float radius)
    {
        rb.SetCollisionType(CollisionType.SPHERE);
        rb.SetAsStatic();
        rb.SetAsTrigger();
        rb.SetSphereRadius(radius);
    }

    public void OnTriggerEnter(Rigidbody other)
    {
        // Only for testing purposes
        other.gameObject.transform.localPosition = new Vector3(0, 5, 0);
    }
}


