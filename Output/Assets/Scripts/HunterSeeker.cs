using System;
using RagnarEngine;

public class HunterSeeker : RagnarComponent
{
	NavAgent agent;
	public GameObject[] enemies;
	Rigidbody rb;
	bool waitHit;

	public void Start()
	{
		waitHit = false;

		agent = gameObject.GetComponent<NavAgent>();
		rb = gameObject.GetComponent<Rigidbody>();
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		GameObject player = GameObject.Find("Player");

		player.GetComponent<Player>().controled = false;

		//not working position
		Vector3 pos = player.transform.globalPosition;
		gameObject.transform.localPosition = pos;
		rb.SetBodyPosition(player.transform.globalPosition);
		rb.IgnoreCollision(player, true);
	}
	public void Update()
	{
		//set const height? gameObject.transform.globalPosition.y = 1;
		if (!waitHit && Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
		{
			waitHit = true;
			if (agent.CalculatePath(agent.hitPosition).Length > 0)
			{
				agent.CalculatePath(gameObject.transform.globalPosition);
				agent.CalculatePath(agent.hitPosition);
			}
		}
		if (waitHit && Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_UP)
		{
			if (Attack())
			{
				InternalCalls.Destroy(gameObject);
				waitHit = false;
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