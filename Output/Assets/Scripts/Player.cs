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
    private bool crouched = false;

    Rigidbody rb;
    Material materialComponent;
    NavAgent agent;

    bool controled = false;

    public void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        materialComponent = gameObject.GetComponent<Material>();
        agent = gameObject.GetComponent<NavAgent>();
        gameObject.GetComponent<AudioSource>().PlayClip("Level1BgMusic");
    }

    public void Update()
    {
        if (controled)
        {
            if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
            {
                agent.CalculatePath(agent.hitPosition);
                gameObject.GetComponent<Animation>().PlayAnimation("Walk");
            }

            // Crouch
            if (!crouched && Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
            {
                crouched = true;
                gameObject.GetComponent<Animation>().PlayAnimation("Crouch");
                rb.SetHeight(0.6f); // 0.6 = 60%
            }
            if (crouched && Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
            {
                crouched = false;
                gameObject.GetComponent<Animation>().PlayAnimation("Idle");
                rb.SetHeight(1); // 1 = 100% = Reset
            }
        }

        if (agent.MovePath())
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");

        ///////// SOUNDS /////////
        // Reload Sound
        if (Input.GetKey(KeyCode.R) == KeyState.KEY_DOWN)
        {
            gameObject.GetComponent<AudioSource>().PlayClip("Reload");
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
        
        if (pendingToDelete && gameObject.GetComponent<Animation>().HasFinished())
        {
            InternalCalls.Destroy(gameObject);
            SceneManager.LoadScene("LoseScene");
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
        }
    }

    public void SetControled(bool var)
    {
        controled = var;
    }
}







