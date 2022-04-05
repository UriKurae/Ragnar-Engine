using System;
using RagnarEngine;
public enum State
{
    NONE,
    ABILITY_1,
    ABILITY_2,
    POSTCAST,
    DEATH
}

public class Characters
{
    // Basic Character info
    public string name;
    public string prefabPath;
    public State state;

    // Abilities
    public Abilities[] abilities;
}