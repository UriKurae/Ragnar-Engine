using System;
using RagnarEngine;

public class BackStab : RagnarComponent
{
	public GameObject player;
	public Vector3 pos;
	public GameObject[] enemies;
	public GameObject selectedEnemy;
	public bool backstabed;
	public void Start()
	{
		Debug.Log("Start Knife");
		player = GameObject.Find("Player_2");
		pos = player.transform.globalPosition;
		pos.y += 1;
		gameObject.transform.localPosition = pos;
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		selectedEnemy = null;
		backstabed = false;
	}
	public void Update()
	{
		selectedEnemy = CalculateDistancePlayerEnemies();
		if (selectedEnemy != null && backstabed == false)
        {
			backstabed = true;
			Vector3 behind = selectedEnemy.transform.globalPosition - (selectedEnemy.transform.forward * 1);
			behind.y = -0.8f;
			player.GetComponent<Rigidbody>().SetBodyPosition(behind);
			selectedEnemy.GetComponent<BasicEnemy>().pendingToDelete = true;
			selectedEnemy.GetComponent<Animation>().PlayAnimation("Dying");
		}
		InternalCalls.Destroy(gameObject);
  
	}
	public bool Timer() //moltes gracies Isaac
    {
		float time1 = 0.5f;

		if(time1 > 0)
        {
			time1 -= Time.deltaTime;
			return false;
		}
		else
        {
			return true;
		}
    }
	public GameObject CalculateDistancePlayerEnemies()
    {
		for (int i = 0; i < enemies.Length;i++)
        {
			Vector3 enemyPos = enemies[i].transform.globalPosition;
			Vector3 distance = player.transform.globalPosition - enemyPos;
			distance.y = 0;
			if (distance.magnitude <= 3)
			{
				return enemies[i];
            }
		}
		return null;
    }

}