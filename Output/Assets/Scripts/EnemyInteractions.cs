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
        PerceptionCone(8);
        Shoot();
        if (pendingToDelete)
        {
            SceneManager.LoadScene("WinScene");
            InternalCalls.Destroy(gameObject);
        }
    }

    private void PerceptionCone(int radius)
    {
        Vector3 enemyPos = gameObject.transform.globalPosition;
        Vector3 enemyForward = gameObject.transform.forward;
        Vector3 initPos = new Vector3(enemyPos.x + enemyForward.x * offset.x, enemyPos.y, enemyPos.z + enemyForward.z * offset.z);

        RayCast.PerceptionCone(initPos, enemyForward, 60, 10, radius);
    }

    private void Shoot()
    {
        if (PlayerDetection(8)) SceneAudio.GetComponent<AudioSource>().SetState("MUSIC", "LEVEL1_BATTLE");
        else SceneAudio.GetComponent<AudioSource>().SetState("MUSIC", "LEVEL1_BASE");

        if (PlayerDetection(8) && canShoot)
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

    bool PlayerDetection(int radius)
    {
        for (int i = 0; i < players.Length; i++)
        {
            Vector3 enemyPos = gameObject.transform.globalPosition;
            Vector3 enemyForward = gameObject.transform.forward;
            Vector3 playerPos = players[i].transform.globalPosition;
            Vector3 distance = playerPos - enemyPos;
            distance.y = 0;
            if (distance.magnitude < radius)
            {
                float angle = gameObject.transform.GetAngleBetween(enemyForward, distance) * Constants.RADTODEG;
                Vector3 initPos = new Vector3(enemyPos.x + enemyForward.x * offset.x, enemyPos.y + offset.y * 0.9f, enemyPos.z + enemyForward.z * offset.z);
                Vector3 endPos = initPos + distance.normalized * radius;
                endPos.y = initPos.y;

                if (angle < 30 && RayCast.HitToTag(initPos, endPos, "Player")) // 30º to right and 30º to left, total 60º
                {
                    index = i;
                    return true;
                }
            }
        }
        return false;
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
            gameObject.GetComponent<AudioSource>().PlayClip("Enemy1Death");
            pendingToDelete = true;
        }
    }

    public int GetIndex()
    {
        return index;
    }

}