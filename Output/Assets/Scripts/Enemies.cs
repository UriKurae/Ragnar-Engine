using System;
using RagnarEngine;

public enum EnemyType
{
    BASIC,
    TANK,
    UNDISTRACTABLE,
    AIR
}

public enum EnemyState
{
    IDLE,
    PATROLING,
    CAUTIOUS,
    SEARCHING,
    ANGRY,
    DEATH
}

public class Enemies
{
    // Basic Enemy info
    public string name;
    public string prefabPath;
    public EnemyType type;
    public EnemyState state;

    // Position
    public Vector3 pos;

    // Patroling
    public GameObject[] waypoints;

    // Abilities
    public EnemyAbilities[] abilities;
}