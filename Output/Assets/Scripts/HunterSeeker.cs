using System;
using RagnarEngine;

public class HunterSeeker : RagnarComponent
{
	NavAgent agent;
	public GameObject[] enemies;
	Rigidbody rb;
	GameObject area;

	public void Start()
	{
		agent = gameObject.GetComponent<NavAgent>();
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		GameObject player = GameObject.Find("Player_2");
		player.GetComponent<Player>().SetControled(false);
		Vector3 pos = player.transform.globalPosition + new Vector3(0, 1, 0);
		rb = gameObject.GetComponent<Rigidbody>();
		rb.SetBodyPosition(pos);
		rb.IgnoreCollision(player, true);
		InternalCalls.InstancePrefab("Backstab Area");
		area = GameObject.Find("Backstab Area");
		gameObject.AddChild(area);
		area.transform.localPosition = new Vector3(0, area.transform.localPosition.y, 0);
	}
	public void Update()
	{
		if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
		{
			agent.CalculatePath(agent.hitPosition);
		}
		agent.MovePath();
		if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_UP)
		{
			if (Attack())
			{
				GameObject player = GameObject.Find("Player_2");
				player.GetComponent<Player>().SetControled(true);
				gameObject.EraseChild(area);
				//InternalCalls.Destroy(area);
				InternalCalls.Destroy(gameObject);
			}
		}
	}
	public bool Attack()
	{
		for (int i = 0; i < enemies.Length; i++)
		{
			float distance = Vector3.Magnitude(gameObject.transform.globalPosition - enemies[i].transform.globalPosition);
			if (distance <= 3.633)
			{
				enemies[i].GetComponent<EnemyInteractions>().pendingToDelete = true;
				return true;
			}
		}
		return false;
	}
}