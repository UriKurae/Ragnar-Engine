using System;
using RagnarEngine;

public class StunnerShot : RagnarComponent
{
    private float force = 2500;
    private bool pendingToDelete = false;

    public void Start()
    {
        AimMethod();
    }

    private void AimMethod()
    {
        GameObject player = GameObject.Find("Player_3");
        NavAgent agent = player.GetComponent<NavAgent>();

        Vector3 pos = player.transform.globalPosition;
        pos.y += 1;
        gameObject.transform.localPosition = pos;

        Vector3 direction = agent.hitPosition - player.transform.globalPosition;
        direction.y = 0;

        Rigidbody goRB = gameObject.GetComponent<Rigidbody>();
        goRB.SetBodyPosition(pos);
        goRB.IgnoreCollision(player, true);
        goRB.ApplyCentralForce(direction.normalized * force);
    }

    public void Update()
    {
        if (pendingToDelete) InternalCalls.Destroy(gameObject);
    }

    public void OnCollisionEnter(Rigidbody other)
    {
        pendingToDelete = true;
    }
}