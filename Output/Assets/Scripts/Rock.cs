using System;
using RagnarEngine;

public class Rock : RagnarComponent
{
	private float force = 400;
	public float soundRadius = 6f;
	private float cooldown = 0f;

	public void Start()
	{
		Debug.Log("Start Rock");
		GameObject player = GameObject.Find("Player");
		Vector3 pos = player.transform.globalPosition;
		pos.y += 1;
		gameObject.transform.localPosition = pos;

		Rigidbody rockRb = gameObject.GetComponent<Rigidbody>();
		rockRb.SetBodyPosition(pos);
		rockRb.IgnoreCollision(player, true);
		Vector3 vectorDir = new Vector3(player.transform.forward.x, 1, player.transform.forward.z);
		rockRb.ApplyCentralForce(vectorDir.normalized * force);
	}

	public void Update()
	{
		if (cooldown > 0 && gameObject != null)
		{
			cooldown -= Time.deltaTime;
			if (cooldown < 0)
			{
				cooldown = 0f;
				InternalCalls.Destroy(gameObject);
			}
		}
	}

	public void OnCollisionEnter(Rigidbody other)
	{
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