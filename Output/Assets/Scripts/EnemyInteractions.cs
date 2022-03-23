using System;
using RagnarEngine;

public class EnemyInteraction : RagnarComponent
{
    // Player tracker
    public GameObject player;

    // States
    public bool canShoot = true;
    private bool pendingToDelete = false;

    // Timers
    public float shootCooldown = 0f;

    public void Start()
    {
        player = GameObject.Find("Player");
    }
    public void Update()
    {
        Shoot();
        if (pendingToDelete)
        {
            InternalCalls.Destroy(gameObject);
            SceneManager.LoadScene("WinScene");
        }
    }

    private void Shoot()
    {
        float xDiff = player.transform.globalPosition.x - gameObject.transform.globalPosition.x;
        float zDiff = player.transform.globalPosition.z - gameObject.transform.globalPosition.z;
        double distance = Math.Sqrt(zDiff * zDiff + xDiff * xDiff);

        if (distance < 12.0f && canShoot)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("Enemy1Shoot");
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

    public void OnCollision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife")
        {
            gameObject.GetComponent<AudioSource>().PlayClip("Enemy1Death");
            pendingToDelete = true;               
        }
    }


}