using System;
using RagnarEngine;

public class EnemyBullet : RagnarComponent
{
	public int vel = 40;
	private bool pendingToDelete = false;

	public GameObject[] players = new GameObject[3];
	public int index = 0;

	public void Start()
	{
		players = GameObject.FindGameObjectsWithTag("Player");

		GameObject enemy = GameObject.Find("Enemy");
		Vector3 pos = enemy.transform.globalPosition;
		pos.y += 1;
		gameObject.transform.localPosition = pos;

		Rigidbody knifeRb = gameObject.GetComponent<Rigidbody>();
		knifeRb.SetBodyPosition(pos);
		knifeRb.IgnoreCollision(enemy, true);

        float xDiff = players[index].transform.globalPosition.x - gameObject.transform.globalPosition.x;
        float zDiff = players[index].transform.globalPosition.z - gameObject.transform.globalPosition.z;
        Vector3 shotDirection = new Vector3(xDiff, pos.y, zDiff);
        knifeRb.linearVelocity = shotDirection.normalized * vel;
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