using System;
using RagnarEngine;

[System.Serializable]
public class LevelData
{
    public float timer;
    public float[] posCam;
    public bool[] abilities;

    public LevelData(float t, Vector3 pos, bool[] ret)
    {
        timer = t;
        posCam = new float[3];
        posCam[0] = pos.x;
        posCam[1] = pos.y;
        posCam[2] = pos.z;
        abilities = ret;
    }
}