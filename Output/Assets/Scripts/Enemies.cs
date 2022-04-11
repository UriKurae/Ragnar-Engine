using System;
using RagnarEngine;
public enum Type
{
    BASIC,
    TANK,
    UNDISTRACTABLE,
    AIR
}

public class Enemies
{
    // Basic Enemy info
    public string name;
    public string prefabPath;
    public Type type;
}