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
		Debug.Log(enemy.name);

		Vector3 pos = enemy.transform.globalPosition;
		pos.y += 0.5f;
		gameObject.transform.localPosition = pos;

		Rigidbody bulletRb = gameObject.GetComponent<Rigidbody>();
		bulletRb.SetBodyPosition(pos);
		bulletRb.IgnoreCollision(enemy, true);

		Debug.Log(index.ToString());

        float xDiff = players[index].transform.globalPosition.x - gameObject.transform.globalPosition.x;
        float zDiff = players[index].transform.globalPosition.z - gameObject.transform.globalPosition.z;
        Vector3 shotDirection = new Vector3(xDiff, pos.y, zDiff);
        bulletRb.linearVelocity = shotDirection.normalized * vel;
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