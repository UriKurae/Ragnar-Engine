using System;
using RagnarEngine;

public class EnemyBullet : RagnarComponent
{
	public int vel = 100;

	public GameObject[] players;
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

		GameObject obj = RayCast.HitToTag(pos, players[index].transform.globalPosition, "Player");
		if (obj != null)
		{
			Debug.Log(obj.name.ToString());
			obj.GetComponent<Player>().hitPoints -= 1;
		}

		Vector3 diff = players[index].transform.globalPosition - gameObject.transform.globalPosition;
		diff.y = gameObject.transform.globalPosition.y;

		bulletRb.linearVelocity = diff.normalized * vel;
	}
    public void Update()
	{
	}

	public void OnCollision()
	{
		InternalCalls.Destroy(gameObject);
	}
}