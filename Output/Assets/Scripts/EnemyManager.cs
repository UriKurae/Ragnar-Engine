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
			type = Type.BASIC,
			state = EnemyState.IDLE
		};

		foreach (Enemies c in enemies)
		{
			InternalCalls.InstancePrefab(c.prefabPath);
		}
	}
	public void Update()
	{
		// PATROLING
		// La idea es que dentro de la clase propia de Enemies haya un array con los waypoints. Si la lenght es 0 o es null (que lo será por defecto), significa que el enemigo es estático.
		
		// ACCIONES
		// La parte que debería ser la más sencilla, si los enemigos detectan players, que actuen de acuerdo al estado en el que se encuentran.
		
		// CONTROL DE MUERTES
		// Idea: en el mismo instante que el gameobject se destruye, se debe quitar del array de enemies, para que se cree un array auxiliar que se rellene con los datos de todos y se devuelva este array
		// al de enemies pero sin los muertos. Así evitamos problemas de updates de GO inexistentes.
		// IMPORTANTE: Esto irá arriba del todo para hacer la comprobación cada vez que haya un callback :3

	}

}