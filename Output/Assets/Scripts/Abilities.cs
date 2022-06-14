using System;
using RagnarEngine;

public class Abilities
{
    public string name;
    public string prefabPath;

    // Range Area Parameters
    public float transformY;
    public float intensity;
    public float constant;
    public float linear;
    public float quadratic;

    // -1 is infinite charges
    public int charges;

    // Cooldown stuff
    public float cooldown;
    public bool onCooldown = false;
    public float counter = 0;
}