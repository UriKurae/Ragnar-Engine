using System;
using RagnarEngine;

public class Barrel : RagnarComponent
{

    GameObject boss;

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
        if (other.gameObject.name == "Boss" && gameObject.name == "Barrel1")
        {
            boss.GetComponent<Boss>().stunnedHits++;
            InternalCalls.Destroy(gameObject);
        }
    }
    void NotifyBoss()
    {
        if (gameObject.name == "Barrel1")
        {
            boss.GetComponent<Boss>().stunnedHits++;
            boss.GetComponent<Boss>().barrelLocations[0].isDestroyed = true;
            boss.GetComponent<Boss>().barrelCount--;
            InternalCalls.Destroy(gameObject);
        }
    }


}