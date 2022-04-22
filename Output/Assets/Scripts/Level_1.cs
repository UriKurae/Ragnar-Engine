using System;
using RagnarEngine;

public class Level_1 : RagnarComponent
{
    public Characters[] characters;
    public Enemies[] enemies;

    public void Start()
	{
        // PLAYERS
        characters = new Characters[3];
        // Player 1
        characters[0] = new Characters
        {
            name = "Paul Atreides",
            prefabPath = "Player",
            state = State.NONE,
            abilities = new Abilities[4],
            hitPoints = 3,
        };
        characters[0].abilities[0] = new Abilities
        {
            name = "Knife Throw",
            prefabPath = "Knife",
            prefabArea = "Knife Area",
            charges = -1,
            cooldown = 25f
        }; // Throwing Knife
        characters[0].abilities[1] = new Abilities
        {
            name = "Rock Throw",
            prefabPath = "Rock",
            prefabArea = "Rock Area",
            charges = -1,
            cooldown = 20f
        }; // Rock/Eagle
        characters[0].abilities[2] = new Abilities
        {
            name = "The Voice",
            prefabPath = "Voice",
            prefabArea = "Rock Area",
            charges = -1,
            cooldown = 20f
        }; // The Voice
        characters[0].abilities[3] = new Abilities
        {
            name = "BackStab",
            prefabPath = "BackStab_2",
            prefabArea = "BackStab Area",
            charges = -1,
            cooldown = 0f
        }; // BackStab

        // Player 2
        characters[1] = new Characters
        {
            name = "Chani",
            prefabPath = "Player_2",
            state = State.NONE,
            abilities = new Abilities[4],
            hitPoints = 2,
        };
        characters[1].abilities[0] = new Abilities
        {
            name = "Backstab",
            prefabPath = "BackStab",
            prefabArea = "BackStab Area",
            charges = -1,
            cooldown = 0f
        }; // BackStab
        characters[1].abilities[1] = new Abilities
        {
            name = "Camouflage",
            prefabPath = "Camouflage",
            prefabArea = "Backstab Area",
            charges = -1,
            cooldown = 30f
        }; // Camouflage
        characters[1].abilities[2] = new Abilities
        {
            name = "Spice Grenade",
            prefabPath = "Spice Grenade",
            prefabArea = "Rock Area",
            charges = -1,
            cooldown = 0f
        }; // Spice Bomb
        characters[1].abilities[3] = new Abilities
        {
            name = "Hunter Seeker",
            prefabPath = "HunterSeeker",
            prefabArea = "Rock Area",
            charges = -1,
            cooldown = 4f
        }; // Hunter Seeker

        // Player 3
        characters[2] = new Characters
        {
            name = "Stilgar",
            prefabPath = "Player_3",
            state = State.NONE,
            abilities = new Abilities[4],
            hitPoints = 4,
        };
        characters[2].abilities[0] = new Abilities
        {
            name = "Sword Slash",
            prefabPath = "SwordSlash",
            prefabArea = "SwordSlash Area",
            charges = -1,
            cooldown = 0f
        }; // Sword Slash
        characters[2].abilities[1] = new Abilities
        {
            name = "Stunner",
            prefabPath = "StunnerShot",
            prefabArea = "Stunner Area",
            charges = 4,
            cooldown = 5f
        }; // Stunner Shot
        characters[2].abilities[2] = new Abilities
        {
            name = "Whistle",
            prefabPath = "Whistle",
            prefabArea = "Whistle Area",
            charges = -1,
            cooldown = 6f
        }; // Whistle
        characters[2].abilities[3] = new Abilities
        {
            name = "Trap",
            prefabPath = "Trap",
            prefabArea = "Trap Area",
            charges = 1,
            cooldown = 0f
        }; // Trap
        ///////////////////////////////////////////////////////////////////

        // ENEMIES
        enemies = new Enemies[3];

		// Basic Enemy with Patrol
        enemies[0] = new Enemies
        {
            name = "Ground Enemy 1",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(3.59f, 0f, 18.03f),
            waypoints = new GameObject[3]
        };
        enemies[0].waypoints[0] = GameObject.Find("1");
        enemies[0].waypoints[1] = GameObject.Find("2");
        enemies[0].waypoints[2] = GameObject.Find("3");

        enemies[1] = new Enemies
        {
            name = "Ground Enemy 2",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(4.23f, 0f, -6.68f),
            waypoints = new GameObject[3]
        };
        enemies[1].waypoints[0] = GameObject.Find("4");
        enemies[1].waypoints[1] = GameObject.Find("5");
        enemies[1].waypoints[2] = GameObject.Find("6");

        // Basic Enemy without Patrol
        enemies[2] = new Enemies
        {
            name = "Ground Enemy 3",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
            pos = new Vector3(-7.52f, 0f, 31.44f),
            waypoints = new GameObject[1]
        };
        enemies[2].waypoints[0] = GameObject.Find("7");
        /////////////////////////////////////////////////
      
        InternalCalls.InstancePrefab("PlayerManager");
        GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters = characters;
        InternalCalls.InstancePrefab("EnemyManager");
        GameObject.Find("EnemyManager").GetComponent<EnemyManager>().enemies = enemies;
	}
	public void Update()
	{

	}

}