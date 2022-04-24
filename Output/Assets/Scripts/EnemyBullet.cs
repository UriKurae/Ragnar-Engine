using System;
using RagnarEngine;

public class EnemyBullet : RagnarComponent
{
	public int vel = 100;
	private bool pendingToDelete = false;

	public GameObject[] players = new GameObject[3];
	public int index = 0;
	public GameObject enemy;

	public void Start()
	{
		players = GameObject.FindGameObjectsWithTag("Player");

		Vector3 pos = enemy.transform.globalPosition;
		pos.y += 0.5f;
		gameObject.transform.localPosition = pos;

		Rigidbody bulletRb = gameObject.GetComponent<Rigidbody>();
		bulletRb.IgnoreCollision(enemy, true);
		bulletRb.SetBodyPosition(pos);

		Vector3 diff = players[index].transform.globalPosition - gameObject.transform.globalPosition;
		diff.y = gameObject.transform.globalPosition.y;

		GameObject obj = RayCast.HitToTag(pos, diff, "Player");
		if (obj != null) obj.GetComponent<Player>().hitPoints -= 1;

		bulletRb.linearVelocity = diff.normalized * vel;
	}
    public void Update()
	{
		if (pendingToDelete) InternalCalls.Destroy(gameObject);
	}

	public void OnCollision()
	{
		pendingToDelete = true;
	}
}