using System;
using RagnarEngine;

public class Rock : RagnarComponent
{
	private float force = 1050;
	public float soundRadius = 6f;
	private float cooldown = 0f;
	private bool pendingToDelete = false;
	Rigidbody goRB;

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

		goRB = gameObject.GetComponent<Rigidbody>();
		goRB.SetBodyPosition(pos);
		goRB.IgnoreCollision(player, true);
		goRB.ApplyCentralForce(direction.normalized * force);
	}

	public void Update()
	{
		if (cooldown > 0 && gameObject != null)
		{
			cooldown -= Time.deltaTime;
			if (cooldown < 0)
			{
				cooldown = 0f;
				pendingToDelete = true;
			}
		}
		if (pendingToDelete) InternalCalls.Destroy(gameObject);
	}

	public void OnCollisionEnter(Rigidbody other)
	{
		goRB.SetAsStatic();
		Rigidbody area = gameObject.CreateComponent<Rigidbody>();
		CreateSphereTrigger(area, soundRadius, gameObject.transform.globalPosition);
		cooldown = 2f;
	}

	private static void CreateSphereTrigger(Rigidbody rb, float radius, Vector3 pos)
	{
		rb.SetCollisionSphere(radius, pos.x, pos.y, pos.z);
		rb.SetAsTrigger();
	}
}