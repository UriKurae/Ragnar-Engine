using System;
using RagnarEngine;
public enum Type
{
    BASIC,
    TANK,
    UNDISTRACTABLE,
    AIR
}

public enum EnemyState
{
    IDLE,
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
    public Type type;
    public EnemyState state;

    // Abilities
    public EnemyAbilities[] abilities;
}