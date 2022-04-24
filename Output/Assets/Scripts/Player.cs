using System;
using RagnarEngine;

public class Player : RagnarComponent
{
    public int hitPoints;
    public float force = 100;
    private bool pendingToDelete = false;
    private bool paused = false;
    private bool crouched = false;
    public bool invisible = false;
    private bool firstTime = false;
    private bool dead = false;

    Rigidbody rb;
    Material materialComponent;
    NavAgent agent;

    public bool controled = false;
    int state = 0;

    public void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        materialComponent = gameObject.GetComponent<Material>();
        agent = gameObject.GetComponent<NavAgent>();
        gameObject.GetComponent<Animation>().PlayAnimation("Idle");
    }

    public void Update()
    {
        if (hitPoints <= 0 && !dead)
        {
            dead = true;
            Die();
        }

        if (controled && hitPoints > 0)
        {
            if (state == (int)State.NONE && Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
            {
                if (agent.CalculatePath(agent.hitPosition).Length > 0)
                    gameObject.GetComponent<Animation>().PlayAnimation("Walk");
               
                if (firstTime)
                {
                    gameObject.GetComponent<AudioSource>().PlayClip("FOOTSTEPS");
                }
                else
                {
                    firstTime = true;
                }
            }

            // Crouch
            if (!crouched && Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
            {
                crouched = true;
                gameObject.GetComponent<Animation>().PlayAnimation("Crouch");
                rb.SetHeight(0.6f); // 0.6 = 60%

                Vector3 maxPoint = gameObject.GetMaxAABB();
                maxPoint.y *= 0.6f;
                gameObject.SetSizeAABB(gameObject.GetMinAABB(), maxPoint);
            }
            if (crouched && Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
            {
                crouched = false;
                gameObject.GetComponent<Animation>().PlayAnimation("Idle");
                rb.SetHeight(1); // 1 = 100% = Reset
            }
        }
        if(state == (int)State.ABILITY_1 || state == (int)State.ABILITY_2 ||state == (int)State.ABILITY_3)
        {
            agent.CalculatePath(new Vector3(gameObject.transform.globalPosition.x, gameObject.transform.globalPosition.y, gameObject.transform.globalPosition.z));
            agent.CalculatePath(agent.hitPosition);
            if (crouched)
            {
                gameObject.GetComponent<Animation>().PlayAnimation("CrouchWalk");
            }
            else
            {
                gameObject.GetComponent<Animation>().PlayAnimation("Walk");
            }
            //gameObject.GetComponent<AudioSource>().PlayClip("FOOTSTEPS");
        }
        if (agent.MovePath())
        {
            gameObject.GetComponent<Animation>().PlayAnimation("Idle");

            gameObject.GetComponent<AudioSource>().StopCurrentClip("FOOTSTEPS");
        }

        ///////// SOUNDS /////////
        // Reload Sound
        if (Input.GetKey(KeyCode.R) == KeyState.KEY_DOWN)
        {
            gameObject.GetComponent<AudioSource>().PlayClip("RELOAD");
        }
        //////////////////////////
        
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
        if(state == (int)State.POSTCAST)
        {
            state = (int)State.NONE;
        }
    }

    private void Die()
    {
        gameObject.GetComponent<AudioSource>().PlayClip("PLAYERDEATH");
        gameObject.GetComponent<Animation>().PlayAnimation("Death");
        pendingToDelete = true;
        if (GameObject.Find("Knife") != null)
        {
            InternalCalls.Destroy(GameObject.Find("Knife"));
        }
    }

    public void OnTrigger(Rigidbody other)
    {
        if (other.gameObject.name == "WinCondition")
            SceneManager.LoadScene("WinScene");
    }

    public void SetControled(bool var)
    {
        controled = var;
    }

    public void SetState(int var)
    {
        state = var;
    }

    public void GetHit(int dmg)
    {
        hitPoints -= dmg;
    }
}







