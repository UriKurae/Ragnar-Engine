using System;
using RagnarEngine;

public class Level_2 : RagnarComponent
{
	public Characters[] characters;
	public Enemies[] enemies;
    private Chronometer timer = null;
    public bool runGame = true;
    private UIButton chrono;
    public void Start()
	{
        // Camera Starting Position
        GameObject.Find("cameraController").transform.localPosition = new Vector3(-23.76f, 0f, -199.01f);
        GameObject.Find("UI Counter").GetComponent<Transform2D>().position2D = new Vector3(0, (0.5f * InternalCalls.GetRegionGame().y) - 28, 0);
        chrono = GameObject.Find("UI Counter").GetComponent<UIButton>();
        chrono.SetTextPosition(-26, -4);
        timer = new Chronometer();

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
            pos = new Vector3(21.70f, 0f, -160.8f)
        };
        characters[0].abilities[0] = new Abilities
        {
            name = "BackStab",
            prefabPath = "BackStab_2",
            transformY = 0.2f,
            intensity = 1.250f,
            constant = 0.1f,
            linear = -0.574f,
            quadratic = 0f,
            charges = -1,
            cooldown = 0f
        }; // BackStab
        characters[0].abilities[1] = new Abilities
        {
            name = "The Voice",
            prefabPath = "Voice",
            transformY = 1.32f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.172f,
            quadratic = 0f,
            charges = -1,
            cooldown = 20f
        }; // The Voice
        characters[0].abilities[2] = new Abilities
        {
            name = "Knife Throw",
            prefabPath = "Knife",
            transformY = 1.32f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.172f,
            quadratic = 0f,
            charges = -1,
            cooldown = 25f
        }; // Throwing Knife
        characters[0].abilities[3] = new Abilities
        {
            name = "Rock Throw",
            prefabPath = "Rock",
            transformY = 1.15f,
            intensity = 1.250f,
            constant = 1.129f,
            linear = -0.188f,
            quadratic = 0f,
            charges = -1,
            cooldown = 20f
        }; // Rock/Eagle

        // Player 2
        characters[1] = new Characters
        {
            name = "Chani",
            prefabPath = "Player_2",
            state = State.NONE,
            abilities = new Abilities[4],
            hitPoints = 2,
            pos = new Vector3(29.58f, 0f, -164.22f)
        };
        characters[1].abilities[0] = new Abilities
        {
            name = "Backstab",
            prefabPath = "BackStab",
            transformY = 0.2f,
            intensity = 1.250f,
            constant = 0.1f,
            linear = -0.574f,
            quadratic = 0f,
            charges = -1,
            cooldown = 0f
        }; // BackStab
        characters[1].abilities[1] = new Abilities
        {
            name = "Camouflage",
            prefabPath = "Camouflage",
            transformY = 0.2f,
            intensity = 1.250f,
            constant = 0.1f,
            linear = -0.574f,
            quadratic = 0f,
            charges = -1,
            cooldown = 30f
        }; // Camouflage
        characters[1].abilities[2] = new Abilities
        {
            name = "Hunter Seeker",
            prefabPath = "HunterSeeker",
            transformY = 1.32f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.172f,
            quadratic = 0f,
            charges = 3,
            cooldown = 40f
        }; // Hunter Seeker
        characters[1].abilities[3] = new Abilities
        {
            name = "Spice Grenade",
            prefabPath = "SpiceGrenade",
            transformY = 1.32f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.172f,
            quadratic = 0f,
            charges = -1,
            cooldown = 30f
        }; // Spice Bomb

        // Player 3
        characters[2] = new Characters
        {
            name = "Stilgar",
            prefabPath = "Player_3",
            state = State.NONE,
            abilities = new Abilities[4],
            hitPoints = 4,
            pos = new Vector3(19.31f, 0f, -167.67f)
        };
        characters[2].abilities[0] = new Abilities
        {
            name = "Sword Slash",
            prefabPath = "SwordSlash",
            transformY = 0.2f,
            intensity = 1.250f,
            constant = 0.1f,
            linear = -0.574f,
            quadratic = 0f,
            charges = -1,
            cooldown = 0f
        }; // Sword Slash
        characters[2].abilities[1] = new Abilities
        {
            name = "Stunner",
            prefabPath = "StunnerShot",
            transformY = 1.32f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.172f,
            quadratic = 0f,
            charges = 4,
            cooldown = 5f
        }; // Stunner Shot
        characters[2].abilities[2] = new Abilities
        {
            name = "Trap",
            prefabPath = "Trap",
            transformY = 0.12f,
            intensity = 1.250f,
            constant = 0.100f,
            linear = -0.942f,
            quadratic = 0f,
            charges = 1,
            cooldown = 0f
        }; // Trap
        characters[2].abilities[3] = new Abilities
        {
            name = "Whistle",
            prefabPath = "Whistle",
            transformY = 1.12f,
            intensity = 1.250f,
            constant = 1.232f,
            linear = -0.201f,
            quadratic = 0f,
            charges = -1,
            cooldown = 6f
        }; // Whistle

        // ENEMIES
        enemies = new Enemies[42];

        enemies[0] = new Enemies
        {
            name = "Basic Enemy 1",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[1] = new Enemies
        {
            name = "Basic Enemy 2",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[2] = new Enemies
        {
            name = "Basic Enemy 3",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[3] = new Enemies
        {
            name = "Basic Enemy 4",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[4] = new Enemies
        {
            name = "Basic Enemy 5",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[5] = new Enemies
        {
            name = "Basic Enemy 6",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[6] = new Enemies
        {
            name = "Basic Enemy 7",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[7] = new Enemies
        {
            name = "Basic Enemy 8",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[8] = new Enemies
        {
            name = "Basic Enemy 9",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[9] = new Enemies
        {
            name = "Basic Enemy 10",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[10] = new Enemies
        {
            name = "Basic Enemy 11",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[11] = new Enemies
        {
            name = "Basic Enemy 12",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        //enemies[12] = new Enemies
        //{
        //    name = "Undistractable Enemy 1",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(-69.01f, 0f, -92.53f),
        //};
        //enemies[12].waypoints[0] = GameObject.Find("13");

        enemies[12] = new Enemies
        {
            name = "Basic Enemy 13",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[13] = new Enemies
        {
            name = "Basic Enemy 14",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[14] = new Enemies
        {
            name = "Basic Enemy 15",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[15] = new Enemies
        {
            name = "Basic Enemy 16",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[16] = new Enemies
        {
            name = "Basic Enemy 17",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[17] = new Enemies
        {
            name = "Basic Enemy 18",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[18] = new Enemies
        {
            name = "Basic Enemy 19",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[19] = new Enemies
        {
            name = "Basic Enemy 20",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[20] = new Enemies
        {
            name = "Basic Enemy 21",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[21] = new Enemies
        {
            name = "Basic Enemy 22",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[22] = new Enemies
        {
            name = "Basic Enemy 23",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[23] = new Enemies
        {
            name = "Basic Enemy 24",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        //enemies[25] = new Enemies
        //{
        //    name = "Undistractable Enemy 2",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(-24.83f, 0f, -28.42f),
        //};
        //enemies[25].waypoints[0] = GameObject.Find("26");

        //enemies[24] = new Enemies
        //{
        //    name = "Undistractable Enemy 3",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(33.6f, 0f, 27.78f),
        //};
        //enemies[24].waypoints[0] = GameObject.Find("27");

        //enemies[25] = new Enemies
        //{
        //    name = "Undistractable Enemy 4",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(29.89f, 0f, 27.59f),
        //};
        //enemies[25].waypoints[0] = GameObject.Find("28");

        //enemies[26] = new Enemies
        //{
        //    name = "Undistractable Enemy 5",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(26.07f, 0f, 27.18f),
        //};
        //enemies[26].waypoints[0] = GameObject.Find("29");

        enemies[24] = new Enemies
        {
            name = "Basic Enemy 25",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[25] = new Enemies
        {
            name = "Basic Enemy 26",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[26] = new Enemies
        {
            name = "Basic Enemy 27",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[27] = new Enemies
        {
            name = "Basic Enemy 28",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[28] = new Enemies
        {
            name = "Basic Enemy 29",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[29] = new Enemies
        {
            name = "Basic Enemy 30",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[30] = new Enemies
        {
            name = "Basic Enemy 31",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[31] = new Enemies
        {
            name = "Basic Enemy 32",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[32] = new Enemies
        {
            name = "Basic Enemy 33",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[33] = new Enemies
        {
            name = "Basic Enemy 34",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[34] = new Enemies
        {
            name = "Basic Enemy 35",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        //enemies[38] = new Enemies
        //{
        //    name = "Undistractable Enemy 6",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(33.97f, 0f, 131.15f),
        //};
        //enemies[38].waypoints[0] = GameObject.Find("41");

        //enemies[39] = new Enemies
        //{
        //    name = "Undistractable Enemy 7",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(30.48f, 0f, 132.19f),
        //};
        //enemies[39].waypoints[0] = GameObject.Find("42");

        //enemies[40] = new Enemies
        //{
        //    name = "Undistractable Enemy 8",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = new Vector3(26.43f, 0f, 132.45f),
        //};
        //enemies[40].waypoints[0] = GameObject.Find("43");

        enemies[35] = new Enemies
        {
            name = "Basic Enemy 36",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[36] = new Enemies
        {
            name = "Basic Enemy 37",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[37] = new Enemies
        {
            name = "Basic Enemy 38",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[38] = new Enemies
        {
            name = "Basic Enemy 39",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        enemies[39] = new Enemies
        {
            name = "Basic Enemy 40",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };

        //enemies[46] = new Enemies
        //{
        //    name = "Undistractable Enemy 9",
        //    prefabPath = "Undistractable Enemy",
        //    type = EnemyType.UNDISTRACTABLE,
        //    state = EnemyState.IDLE,
        //    pos = GameObject.Find("49").transform.globalPosition,
        //    waypoints = new GameObject[2]
        //};
        //enemies[46].waypoints[0] = GameObject.Find("49");
        //enemies[46].waypoints[1] = GameObject.Find("50");

        enemies[40] = new Enemies
        {
            name = "Tank Enemy 1",
            prefabPath = "Tank Enemy",
            type = EnemyType.TANK,
            state = EnemyState.IDLE,
        };
        enemies[40].waypoints.Add(GameObject.Find("25"));
        enemies[40].waypoints.Add(GameObject.Find("25")); 
        enemies[40].waypoints.Add(GameObject.Find("25"));
        enemies[40].waypoints.Add(GameObject.Find("25"));

        enemies[41] = new Enemies
        {
            name = "Basic Enemy 41",
            prefabPath = "Basic Enemy",
            type = EnemyType.BASIC,
            state = EnemyState.IDLE,
        };
        enemies[41].waypoints.Add(GameObject.Find("25"));
        enemies[41].waypoints.Add(GameObject.Find("25"));
        ///////////////////////////////////////////////////////////////////

        InternalCalls.InstancePrefab("PlayerManager");
        GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters = characters;
        InternalCalls.InstancePrefab("EnemyManager");
        GameObject.Find("EnemyManager").GetComponent<EnemyManager>().enemies = enemies;
    }
    public void Update()
	{
        if (runGame) timer.Update();
        chrono.text = timer.GetTimeToString();
    }

}