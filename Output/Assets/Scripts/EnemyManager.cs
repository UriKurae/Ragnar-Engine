using System;
using RagnarEngine;

public class EnemyManager : RagnarComponent
{
    public Enemies[] enemies;
    public GameObject[] enemyGOs;

    public void Start()
    {
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
                //enemyGOs[i].GetComponent<EnemyInteractions>().waypoints = enemies[i].waypoints;
                switch (enemies[i].type)
                {
                    case EnemyType.BASIC:
                        enemyGOs[i].GetComponent<BasicEnemy>().waypoints = enemies[i].waypoints;
                        break;
                    case EnemyType.TANK:
                        enemyGOs[i].GetComponent<TankEnemy>().waypoints = enemies[i].waypoints;
                        break;
                    case EnemyType.UNDISTRACTABLE:
                        enemyGOs[i].GetComponent<UndistractableEnemy>().waypoints = enemies[i].waypoints;
                        break;
                    case EnemyType.AIR:
                        enemyGOs[i].GetComponent<AirEnemy>().waypoints = enemies[i].waypoints;
                        break;
                }

                enemies[i].state = EnemyState.PATROLING;
            }
            enemyGOs[i].GetComponent<Rigidbody>().SetBodyPosition(enemies[i].pos);
        }
    }
    public void Update()
    {
        // Death Control
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