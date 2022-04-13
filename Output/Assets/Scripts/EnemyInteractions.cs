using System;
using RagnarEngine;

public class EnemyInteractions : RagnarComponent
{
    // Player tracker
    public GameObject[] players;
    GameObject SceneAudio;
    private Vector3 offset;
    public int index = 0;

    // States
    public bool canShoot = true;
    private bool pendingToDelete = false;

    // Timers
    public float shootCooldown = 0f;

    public void Start()
    {
        players = GameObject.FindGameObjectsWithTag("Player");
        SceneAudio = GameObject.Find("AudioLevel1");
        offset = gameObject.GetSizeAABB();
    }
    public void Update()
    {
        if(PerceptionCone())
            Shoot();
        if (pendingToDelete)
        {
            InternalCalls.Destroy(gameObject);
        }
    }

    private bool PerceptionCone()
    {
        Vector3 enemyPos = gameObject.transform.globalPosition;
        Vector3 enemyForward = gameObject.transform.forward;
        Vector3 initPos = new Vector3(enemyPos.x + (enemyForward.x * offset.x * 0.6f), enemyPos.y + 0.1f, enemyPos.z + (enemyForward.z * offset.z * 0.6f));

        return RayCast.PerceptionCone(initPos, enemyForward, 60, 16, 8, players, players.Length);
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

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife" || other.gameObject.name == "StunnerShot")
        {
            gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1DEATH");
            pendingToDelete = true;               
        }
    }

    public void OnTrigger(Rigidbody other)
    {
        if(other.gameObject.name == "SwordSlash")
        {
            gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1DEATH");
            pendingToDelete = true;
        }
    }

    public int GetIndex()
    {
        return index;
    }

}