using System;
using RagnarEngine;

public class Abilities
{
    public string name;
    public string prefabPath;
    public string prefabArea;

    // -1 is infinite charges
    public int charges;

    // Cooldown stuff
    public float cooldown;
    public bool onCooldown = false;
    public float counter = 0;
}