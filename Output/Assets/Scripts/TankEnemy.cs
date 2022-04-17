using System;
using RagnarEngine;

public class TankEnemy : RagnarComponent
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

    // Timers
    public float shootCooldown = 0f;
    float deathTimer = -1f;

    float initialSpeed;

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
        if (!pendingToDelete)
        {
            Patrol();
            if (PerceptionCone())
            {
                agents.speed = initialSpeed * 1.1f;
                Shoot();
            }
            else
            {
                agents.speed = initialSpeed * 0.8f;
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
    }

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife")
        {
            // TURN TO ALERTED MODE (TYPE OF DISTRACTION)
        }
        if (other.gameObject.name == "StunnerShot")
        {
            deathTimer = 2f;
        }
        if (other.gameObject.name == "HunterSeeker")
        {
            deathTimer = 5f;

            // WHEN RUNES FUNCTIONAL
            // STUN (BLIND) 3s
            // EXPLOSION AREA STUN (BLIND)
        }
    }

    public void OnTrigger(Rigidbody other)
    {
        //// Paul ========================================
        if (other.gameObject.name == "Rock")
        {
            // DISTRACTION (ROTATE VISION, NO MOVEMENT TO THE DISTRACTION)
            patrol = false;
            stoppedTime = 5f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);
        }
        if (other.gameObject.name == "Eagle")
        {
            // DISTRACTION (ROTATE VISION, NO MOVEMENT TO THE DISTRACTION)
            patrol = false;
            stoppedTime = 6f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);
        }

        //// Chani =======================================
        if (other.gameObject.name == "SpiceGrenade")
        {
            patrol = false;
            stoppedTime = 5f;
            //agents.CalculatePath(other.gameObject.transform.globalPosition);

            // WHEN RUNES FUNCTIONAL
            // SHIELD DESTROYED
            // STUN (BLIND) 5s
        }


        //// Stilgar =====================================
        if (other.gameObject.name == "SwordSlash")
        {
            deathTimer = 2f;
        }
        if (other.gameObject.name == "Whistle")
        {
            // NEED TO CREATE FUNCTION TO INITIATE STOPPED TIME WHEN ARRIVES TO THE POSITION
            patrol = false;
            stoppedTime = 5f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);
        }
        if (other.gameObject.name == "Trap")
        {
            // STUN (BLIND)
            patrol = false;
            stoppedTime = 5f;
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
}