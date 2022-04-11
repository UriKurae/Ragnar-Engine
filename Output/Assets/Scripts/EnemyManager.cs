using System;
using RagnarEngine;

public class EnemyManager : RagnarComponent
{
	public Enemies[] enemies = new Enemies[1];
	public void Start()
	{
		enemies[0] = new Enemies
		{
			name = "Ground Enemy",
			prefabPath = "Enemy",
			type = Type.Ground,
		};
		foreach (Enemies c in enemies)
		{
			InternalCalls.InstancePrefab(c.prefabPath);
		}
	}
	public void Update()
	{
		
	}

}