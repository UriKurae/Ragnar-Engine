using System;
using RagnarEngine;

[System.Serializable]
public class EnemyData
{
    public EnemyType type;
    public EnemyState state;
    public GameObject spawnPoint;

    public EnemyData(Enemies enemy)
    {
        spawnPoint = enemy.spawnPoint;
        state = enemy.state;
        type = enemy.type;
    }

}