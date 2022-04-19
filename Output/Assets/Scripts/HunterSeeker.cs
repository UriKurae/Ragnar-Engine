using System;
using RagnarEngine;

public class HunterSeeker : RagnarComponent
{
	NavAgent agent;
	public GameObject[] enemies;
	Rigidbody rb;

	public void Start()
	{
		agent = gameObject.GetComponent<NavAgent>();
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		GameObject player = GameObject.Find("Player");
		player.GetComponent<Player>().SetControled(false);
		Vector3 pos = player.transform.globalPosition + new Vector3(0, 1, 0);
		rb = gameObject.GetComponent<Rigidbody>();
		rb.SetBodyPosition(pos);
		rb.IgnoreCollision(player, true);
	}
	public void Update()
	{
		if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
		{
			if (agent.CalculatePath(agent.hitPosition).Length > 0)
			{ }
		}
		if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_UP)
		{
			if (Attack())
			{
				GameObject player = GameObject.Find("Player");
				player.GetComponent<Player>().SetControled(true);

				InternalCalls.Destroy(gameObject);
			}
		}
	}
	public bool Attack()
	{
		for (int i = 0; i < enemies.Length; i++)
		{
			float distance = Vector3.Magnitude(gameObject.transform.globalPosition - enemies[i].transform.globalPosition);
			if (distance <= 2)
			{
				enemies[i].GetComponent<EnemyInteractions>().pendingToDelete = true;
				return true;
			}
		}
		return false;
	}
}