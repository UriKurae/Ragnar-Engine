using System;
using RagnarEngine;

public class RockBoss : RagnarComponent
{
    private float force = 1500.0f;
    public void Start()
	{
        GameObject boss = GameObject.Find("Boss");
        NavAgent agent = boss.GetComponent<NavAgent>();

        Vector3 pos = boss.transform.globalPosition;
        pos.y += 1;
        gameObject.transform.localPosition = pos;

        Vector3 direction = agent.hitPosition - boss.transform.globalPosition;
        direction.y = 0;

        Rigidbody goRB = gameObject.GetComponent<Rigidbody>();
        goRB.SetBodyPosition(pos);
        goRB.IgnoreCollision(boss, true);
        goRB.ApplyCentralForce(direction.normalized * force);
    }
	
    public void OnCollision (Rigidbody other)
    {
         InternalCalls.Destroy(gameObject);
    }

}