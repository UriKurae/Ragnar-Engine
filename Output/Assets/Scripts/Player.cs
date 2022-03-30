using System;
using RagnarEngine;

public class Player : RagnarComponent
{
    public int velocity = 1000;
    public GameObject target = null;
    public float force = 100;
    public float rockSoundRadius = 4f;
    private bool pendingToDelete = false;
    private bool paused = false;

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
        if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
        {
            agent.CalculatePath(agent.hitPosition);
            gameObject.GetComponent<Animation>().PlayAnimation("Walk");
            gameObject.GetComponent<AudioSource>().PlayClip("footSteps");
        }

        if (agent.MovePath())
        {
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");
            gameObject.GetComponent<AudioSource>().StopCurrentClip();
        }

        ///////// SOUNDS /////////
        // Movement Sound
        /* if (Input.GetKey(KeyCode.W) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN
             || Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN || Input.GetKey(KeyCode.D) == KeyState.KEY_DOWN)
         {
             gameObject.GetComponent<AudioSource>().PlayClip("FootSteps");
             gameObject.GetComponent<Animation>().PlayAnimation("Walk");
         }*/

        // Reload Sound
        if (Input.GetKey(KeyCode.R) == KeyState.KEY_DOWN)
        {
            gameObject.GetComponent<AudioSource>().PlayClip("Reload");
        }

        // Shoot sound
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
        {
            gameObject.GetComponent<Animation>().PlayAnimation("Shoot");
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
        /*if (Input.GetKey(KeyCode.W) == KeyState.KEY_REPEAT)
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
        }*/

        // Crouch
        if (Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
        {
            gameObject.GetComponent<Animation>().PlayAnimation("Crouch");
            rb.SetHeight(0.6f); // 0.6 = 60%
        }
        if (Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_UP)
        {
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");
            rb.SetHeight(1); // 1 = 100% = Reset
        }
        if (pendingToDelete && gameObject.GetComponent<Animation>().HasFinished())
        {
            SceneManager.LoadScene("LoseScene");
            InternalCalls.Destroy(gameObject);
        }

        if (Input.GetKey(KeyCode.ESCAPE) == KeyState.KEY_DOWN)
        {
            paused = !paused;
            
            if (paused)
                Time.timeScale = 1.0f;
            else
                Time.timeScale = 0.0f;

            // Pause menu
        }
    }

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "EnemyBullet")
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("PlayerDeath");
            gameObject.GetComponent<Animation>().PlayAnimation("Death");
            pendingToDelete = true;
            // AÑADIR AQUÍ EL CAMBIO DE ESCENA A GAME OVER
        }
    }
}




