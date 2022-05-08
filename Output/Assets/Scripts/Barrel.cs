using System;
using RagnarEngine;

public class Barrel : RagnarComponent
{

    GameObject boss;
    public int barrelIndex = 0;

    // Use this for initialization
    public void Start()
    {
        boss = GameObject.Find("Boss");
    }

    // Update is called once per frame
    public void Update()
    {
        if (Input.GetKey(KeyCode.H) == KeyState.KEY_DOWN)
        {
            NotifyBoss();
        }
    }

    public void OnColision(Rigidbody other)
    {
        if (other.gameObject.name == "Knife")
        {
            if (boss.transform.globalPosition.magnitude - gameObject.transform.globalPosition.magnitude < 2.0f)
            {
                boss.GetComponent<Boss>().stunnedHits++;
            }
            boss.GetComponent<Boss>().barrelCount--;
            boss.GetComponent<Boss>().barrelLocations[barrelIndex].isDestroyed = true;
            InternalCalls.Destroy(gameObject);
        }
    }
    void NotifyBoss()
    {
        Debug.Log("Destroying barrel ----------------------------------------------------------------------");
        boss.GetComponent<Boss>().stunnedHits++;
        boss.GetComponent<Boss>().barrelLocations[barrelIndex].isDestroyed = true;
        boss.GetComponent<Boss>().barrelCount--;
        InternalCalls.Destroy(gameObject);
    }
}