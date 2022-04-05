using System;
using RagnarEngine;

public class EnemyInteraction : RagnarComponent
{
    // Player tracker
    public GameObject player;
    private Vector3 offset;
    // States
    public bool canShoot = true;
    private bool pendingToDelete = false;

    // Timers
    public float shootCooldown = 0f;

    public void Start()
    {
        player = GameObject.Find("Player");
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
        float xDiff = player.transform.globalPosition.x - gameObject.transform.globalPosition.x;
        float zDiff = player.transform.globalPosition.z - gameObject.transform.globalPosition.z;
        double distance = Math.Sqrt(zDiff * zDiff + xDiff * xDiff);

        if (PlayerDetection(8) && canShoot)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1SHOOT");
            canShoot = false;
            shootCooldown = 4f;
            InternalCalls.InstancePrefab("Assets/Prefabs/EnemyBullet.rgprefab");
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
        Vector3 enemyPos = gameObject.transform.globalPosition;
        Vector3 enemyForward = gameObject.transform.forward;
        Vector3 playerPos = player.transform.globalPosition;
        Vector3 distance = playerPos - enemyPos;
        distance.y = 0;
        if (distance.magnitude < radius)
        {
            float angle = gameObject.transform.GetAngleBetween(enemyForward, distance) * Constants.RADTODEG;
            Vector3 initPos = new Vector3(enemyPos.x + enemyForward.x * offset.x, enemyPos.y + offset.y * 0.9f, enemyPos.z + enemyForward.z* offset.z);
            Vector3 endPos = initPos + distance.normalized * radius;
            endPos.y = initPos.y;

            if (angle < 30 && RayCast.HitToTag(initPos, endPos, "Player")) // 30º to right and 30º to left, total 60º
                return true;
        }
        
        return false;
    }

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife")
        {
            gameObject.GetComponent<AudioSource>().PlayClip("ENEMY1DEATH");
            pendingToDelete = true;               
        }
    }


}