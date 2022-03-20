using System;
using RagnarEngine;

public class Knife : RagnarComponent
{
	private float force = 1000;
	private bool canReload = false;
	public void Start()
	{
		Debug.Log("Start Knife");
		GameObject player = GameObject.Find("Player");
		Vector3 pos = player.transform.globalPosition;
		pos.y += 1;
		gameObject.transform.localPosition = pos;

		Rigidbody knifeRb = gameObject.GetComponent<Rigidbody>();
		knifeRb.SetBodyPosition(pos);
		knifeRb.IgnoreCollision(player, true);
		knifeRb.ApplyCentralForce(player.transform.forward * force);
	}

	public void Update()
	{
		if (canReload) ReloadKnife();

	}
	private void ReloadKnife()
	{
		GameObject player = GameObject.Find("Player");
		
		float xDiff = player.transform.globalPosition.x - gameObject.transform.globalPosition.x;
		float zDiff = player.transform.globalPosition.z - gameObject.transform.globalPosition.z;
		double distance = Math.Sqrt(zDiff * zDiff + xDiff * xDiff);

		if (distance < 1.0f)
		{
			InternalCalls.Destroy(gameObject);
			canReload = false;
		}
	}

	public void OnCollisionEnter(Rigidbody other)
    {
		canReload = true;
    }

}