using System;
using RagnarEngine;

public class BasicEnemy : RagnarComponent
{
    public int velocity = 1000;

    public NavAgent agents;
    public GameObject[] waypoints;
    private int destPoint = 0;

    // States
    public bool patrol;
    public bool stopState = false;

    // Timers
    public float stoppedTime = 0f;

    // Player tracker
    public GameObject[] players;
    GameObject SceneAudio;
    private Vector3 offset;
    public int index = 0;

    // States
    public bool canShoot = true;
    public bool pendingToDelete = false;
    public bool controlled = false;

    // Timers
    public float shootCooldown = 0f;
    float deathTimer = -1f;
    float controlledCooldown = 10;

    float initialSpeed;

    bool distracted = false;
    float distractedTimer = -1f;
    bool stunned = false;
    float stunnedTimer = -1f;

    public void Start()
    {
        players = GameObject.FindGameObjectsWithTag("Player");
        SceneAudio = GameObject.Find("AudioLevel1");
        offset = gameObject.GetSizeAABB();

        agents = gameObject.GetComponent<NavAgent>();
        gameObject.GetComponent<Animation>().PlayAnimation("Idle");
        if (waypoints.Length != 0)
        {
            GotoNextPoint();
            patrol = false;
        }

        initialSpeed = agents.speed;
    }

    public void Update()
    {

        if (!controlled)
        {
            if (!pendingToDelete && deathTimer == -1)
            {
                if (!stunned)
                {
                    if (!distracted)
                    {
                        Patrol();
                    }
                    if (PerceptionCone())
                    {
                        agents.speed = initialSpeed * 1.2f;
                        Shoot();
                    }
                    else
                    {
                        agents.speed = initialSpeed;
                    }
                }
            }

            if (deathTimer >= 0)
            {
                deathTimer -= Time.deltaTime;
                if (deathTimer < 0)
                {
                    gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1DEATH");
                    deathTimer = -1f;
                    pendingToDelete = true;
                }
            }

            if (stunnedTimer >= 0)
            {
                stunnedTimer -= Time.deltaTime;
                if (stunnedTimer < 0)
                {
                    stunned = false;
                    stunnedTimer = -1f;
                }
            }

            if (distractedTimer >= 0)
            {
                distractedTimer -= Time.deltaTime;
                if (distractedTimer < 0)
                {
                    distracted = false;
                    distractedTimer = -1f;
                }
            }
        }
        else
        {
            Vector3 dis = this.gameObject.transform.globalPosition - players[0].transform.globalPosition;
            if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
            {
                agents.CalculatePath(agents.hitPosition);

            }
            if (dis.magnitude > 10)
            {
                agents.CalculatePath(agents.hitPosition - dis);
            }
            agents.MovePath();
            //Poner que puede hacer el enemigo

            //
            controlledCooldown -= Time.deltaTime;
            if (controlledCooldown < 0)
            {
                controlledCooldown = 0f;
                controlled = false;
                players[0].GetComponent<Player>().SetControled(true);
                agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
            }
            
        }

    }

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife")
        {
            deathTimer = 4f;
            gameObject.GetComponent<Animation>().PlayAnimation("Dying");

            // WHEN RUNES FUNCTIONAL
            // deathTimer = 0f;
        }
        if (other.gameObject.name == "StunnerShot")
        {
            deathTimer = 2f;
            gameObject.GetComponent<Animation>().PlayAnimation("Dying");
        }
        if (other.gameObject.name == "HunterSeeker")
        {
            deathTimer = 5f;
            gameObject.GetComponent<Animation>().PlayAnimation("Dying");

            // WHEN RUNES FUNCTIONAL
            // EXPLOSION AREA
        }
    }

    public void OnTrigger(Rigidbody other)
    {
        //// Paul ========================================

        if (other.gameObject.name == "Rock")
        {
            // DISTRACTION (ROTATE VISION, NO MOVEMENT TO THE DISTRACTION)

            distracted = true;
            distractedTimer = 5f;
            //stoppedTime = 5f;
            Distraction(other.gameObject.transform.globalPosition);
            //agents.CalculatePath(other.gameObject.transform.globalPosition);
        }
        if (other.gameObject.name == "Eagle")
        {
            // DISTRACTION (ROTATE VISION, NO MOVEMENT TO THE DISTRACTION)
            patrol = false;
            stoppedTime = 6f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);
        }

        //// Chani =======================================
        if (other.gameObject.name == "Spice Grenade")
        {
            // STUN (BLIND)
            Stun(5f);

            //patrol = false;
            //stoppedTime = 5f;
            //agents.CalculatePath(other.gameObject.transform.globalPosition);
        }


        //// Stilgar =====================================
        if (other.gameObject.name == "SwordSlash")
        {
            deathTimer = 2f;
            gameObject.GetComponent<Animation>().PlayAnimation("Dying");
        }
        if (other.gameObject.name == "Whistle")
        {
            // NEED TO CREATE FUNCTION TO INITIATE STOPPED TIME WHEN ARRIVES TO THE POSITION
            patrol = false;
            stoppedTime = 5f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);

            // WHEN RUNES FUNCTIONAL
            // STUN (BLIND) 3s
        }
        if (other.gameObject.name == "Trap")
        {
            // STUN (BLIND)
            Stun(5f);

            //patrol = false;
            //stoppedTime = 5f;
            //agents.CalculatePath(other.gameObject.transform.globalPosition);
        }
    }

    private bool PerceptionCone()
    {
        Vector3 enemyPos = gameObject.transform.globalPosition;
        Vector3 enemyForward = gameObject.transform.forward;
        Vector3 initPos = new Vector3(enemyPos.x + (enemyForward.x * offset.x * 0.6f), enemyPos.y + 0.1f, enemyPos.z + (enemyForward.z * offset.z * 0.6f));

        index = RayCast.PerceptionCone(initPos, enemyForward, 60, 16, 8, players, players.Length);
        return (index == -1) ? false : true;
    }
    public void SetControled(bool flag)
    {
        controlled = flag;
        if (flag) controlledCooldown = 10;
    }

    private void Shoot()
    {
        SceneAudio.GetComponent<AudioSource>().SetState("MUSIC", "LEVEL1_BATTLE");

        if (canShoot)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1SHOOT");
            canShoot = false;
            shootCooldown = 4f;
            InternalCalls.InstancePrefab("EnemyBullet");
            GameObject.Find("EnemyBullet").GetComponent<EnemyBullet>().enemy = gameObject;
            GameObject.Find("EnemyBullet").GetComponent<EnemyBullet>().index = index;
        }

        if (!canShoot)
        {
            if (shootCooldown >= 0)
            {
                shootCooldown -= Time.deltaTime;
                if (shootCooldown < 0)
                {
                    shootCooldown = 0f;
                    canShoot = true;
                }
            }
        }
    }

    public int GetIndex()
    {
        return index;
    }

    public void SetPendingToDelete()
    {
        pendingToDelete = true;
    }

    public void GotoNextPoint()
    {
        gameObject.GetComponent<AudioSource>().PlayClip("FOOTSTEPS");
        gameObject.GetComponent<Animation>().PlayAnimation("Walk");
        agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
        destPoint = (destPoint + 1) % waypoints.Length;
    }

    public void Patrol()
    {
        if (GameObject.Find("Rock") == null && agents.MovePath())
        {
            stopState = true;
        }

        if (stopState)
        {
            if (stoppedTime >= 0)
            {
                gameObject.GetComponent<AudioSource>().StopCurrentClip("FOOTSTEPS");
                stoppedTime -= Time.deltaTime;
                if (stoppedTime < 0)
                {
                    stoppedTime = 0f;
                    stopState = false;
                    if (waypoints.Length != 0)
                    {
                        patrol = true;
                        GotoNextPoint();
                    }
                }
            }
        }

        if (agents.MovePath() && waypoints.Length != 0 && patrol && !stopState)
        {
            GotoNextPoint();
        }
    }

    public void Distraction(Vector3 distractionItem)
    {
        Vector3 forward = gameObject.transform.forward;
        Vector3 newForward = (distractionItem - gameObject.transform.globalPosition).normalized;

        float dot = (forward.x * newForward.x) + (forward.y * newForward.y) + (forward.z * newForward.z);
        float mag = forward.magnitude * newForward.magnitude;
        float angleInRadians = (float)Math.Acos(dot / mag);

        Quaternion newRot = Quaternion.RotateAroundAxis(gameObject.transform.up, angleInRadians);

        gameObject.GetComponent<Rigidbody>().SetBodyRotation(newRot);

        //gameObject.transform.localRotation = newRot;

        gameObject.GetComponent<Animation>().PlayAnimation("Idle");
    }

    public void Stun(float timeStunned)
    {
        stunned = true;
        stunnedTimer = timeStunned;
        gameObject.GetComponent<Animation>().PlayAnimation("Idle");
    }
}