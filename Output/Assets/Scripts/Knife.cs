using System;
using RagnarEngine;

public class Knife : RagnarComponent
{
	private float force = 1500;
	private bool canReload = false;
	private bool pendingToDelete = false;
	
	public void Start()
    {
        AimMethod();
    }

    private void AimMethod()
    {
        GameObject player = GameObject.Find("Player");
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
		if (canReload) ReloadKnife();
		if (pendingToDelete) InternalCalls.Destroy(gameObject);

	}
	private void ReloadKnife()
	{
		GameObject player = GameObject.Find("Player");

		float xDiff = player.transform.globalPosition.x - gameObject.transform.globalPosition.x;
		float zDiff = player.transform.globalPosition.z - gameObject.transform.globalPosition.z;
		double distance = Math.Sqrt(zDiff * zDiff + xDiff * xDiff);

		if (distance < 1.0f)
		{
			pendingToDelete = true;
			canReload = false;
		}
	}

	public void OnCollisionEnter(Rigidbody other)
	{
		canReload = true;
	}

}