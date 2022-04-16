using System;
using RagnarEngine;

public class EnemyManager : RagnarComponent
{
    public Enemies[] enemies = new Enemies[3];
    public GameObject[] enemyGOs;

    public void Start()
    {
        enemies[0] = new Enemies
        {
            name = "Ground Enemy 1",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(3.59f, 0f, 18.03f),
            waypoints = new GameObject[3]
        };
        enemies[0].waypoints[0] = GameObject.Find("1");
        enemies[0].waypoints[1] = GameObject.Find("2");
        enemies[0].waypoints[2] = GameObject.Find("3");

        enemies[1] = new Enemies
        {
            name = "Ground Enemy 2",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(4.23f, 0f, -6.68f),
            waypoints = new GameObject[3]
        };
        enemies[1].waypoints[0] = GameObject.Find("4");
        enemies[1].waypoints[1] = GameObject.Find("5");
        enemies[1].waypoints[2] = GameObject.Find("6");

        enemies[2] = new Enemies
        {
            name = "Ground Enemy 3",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(-7.52f, 0f, 31.44f),
            waypoints = new GameObject[1]
        };
        enemies[2].waypoints[0] = GameObject.Find("7");

        foreach (Enemies e in enemies)
        {
            InternalCalls.InstancePrefab(e.prefabPath);
        }

        enemyGOs = GameObject.FindGameObjectsWithTag("Enemies");

        for(int i = 0; i < enemyGOs.Length; i++)
        {
            enemyGOs[i].name = enemies[i].name;
            if (enemies[i].waypoints.Length != 0)
            {
                enemyGOs[i].GetComponent<EnemyInteractions>().waypoints = enemies[i].waypoints;
                enemies[i].state = EnemyState.PATROLING;
            }
            enemyGOs[i].GetComponent<Rigidbody>().SetBodyPosition(enemies[i].pos);
        }
    }
    public void Update()
    {
        // CONTROL DE MUERTES
        if(enemyGOs.Length > 0)
        {
            foreach(GameObject go in enemyGOs)
            {
                if(go.GetComponent<EnemyInteractions>().pendingToDelete)
                {
                    GameObject[] aux = new GameObject[enemyGOs.Length-1];
                    Enemies[] aux2 = new Enemies[enemies.Length-1];
                    for (int i = 0, j = 0; i < enemyGOs.Length; i++, j++)
                    {
                        if (enemyGOs[i] == go)
                        {
                            j--;
                        }
                        else
                        {
                            aux[j] = enemyGOs[i];
                            aux2[j] = enemies[i];
                        }
                    }
                    enemyGOs = aux;
                    enemies = aux2;
                    InternalCalls.Destroy(go);
                }
            }
        }
    }

}