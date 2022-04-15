using System;
using RagnarEngine;

public class Voice : RagnarComponent
{
	public GameObject player;
	public GameObject selectedEnemy;
	public GameObject[] enemies;
	public PlayerManager playerManager;
	public void Start()
	{
		player = GameObject.Find("Player");
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		playerManager = GameObject.Find("PlayerManager").GetComponent<PlayerManager>();
	}
	public void Update()
	{
		Debug.Log(enemies.Length.ToString());
		selectedEnemy = CalculateDistancePlayerEnemies();
		if (selectedEnemy != null)
		{
			Debug.Log("A");
			AddNewEnemyToPlayer();
		}
		InternalCalls.Destroy(gameObject);
	}
	public GameObject CalculateDistancePlayerEnemies()
	{
		for (int i = 0; i < enemies.Length; i++)
		{
			Vector3 enemyPos = enemies[i].transform.globalPosition;
			Vector3 distance = player.transform.globalPosition - enemyPos;
			distance.y = 0;
			if (distance.magnitude <= 5)
			{
				return enemies[i];
			}
		}
		return null;
	}
	public void AddNewEnemyToPlayer()
    {
		Debug.Log("Is Changing");
		//Yo que seh hermanoh, como se hace esta vaina
		//selectedEnemy.AddComponent<Player>(4);
		selectedEnemy.SetActiveComponent(selectedEnemy.GetComponent<EnemyInteractions>(),false);
		playerManager.players[playerManager.characterSelected].GetComponent<Player>().SetControled(false);
		selectedEnemy.GetComponent<Player>().SetControled(true);
		Debug.Log("Is Changed");
	}
}

