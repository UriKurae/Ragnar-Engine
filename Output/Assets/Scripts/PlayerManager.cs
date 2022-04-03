using System;
using RagnarEngine;

public class PlayerManager : RagnarComponent
{
    public GameObject[] players;
    int characterSelected = 0;

    public Characters[] characters = new Characters[1];
    public Characters playableCharacter;

    public void Start()
	{
        // AVISO
        // Esto de aquí está hardcodeadísimo
        // Pero no hay otra manera de hacerlo
        // Cuando se puedan meter cositas desde inspector
        // Se solucionará este problema

        // Player 1
        characters[0] = new Characters
        {
            name = "Paul Atreides",
            prefabPath = "Assets/Prefabs/Player.rgprefab",
            state = State.NONE,
            abilities = new Abilities[2]
        };
        characters[0].abilities[0] = new Abilities
        {
            name = "Knife Throw",
            prefabPath = "Assets/Prefabs/Knife.rgprefab",
            range = 50,
            charges = -1,
            cooldown = 25f
        };
        characters[0].abilities[1] = new Abilities
        {
            name = "Rock Throw",
            prefabPath = "Assets/Prefabs/Rock.rgprefab",
            range = 50,
            charges = -1,
            cooldown = 20f
        };

        // Player 2

        foreach (Characters c in characters)
        {
            InternalCalls.InstancePrefab(c.prefabPath);
        }

        players = GameObject.FindGameObjectsWithTag("Player");
        ChangeCharacter(characterSelected);
        playableCharacter = characters[characterSelected];
    }

	public void Update()
    {
        PlayerCases();

        // Cambiador de estados para saber qué habilidad estás o no casteando (Básicamente hace que el personaje entre en un estado donde si clickas una tecla
        // muestre el rango de habilidad (por implementar), y entre en un estado de castear o cancelar la habilidad seleccionada (Click derecho cancel/click izquierdo casteo)).
        // Aquí debería ir la zona de rango de cada habilidad.
        AbilityStateChanger();

        // Contador de cooldown para cada habilidad
        // Funciona en todos los casos con todos los pjs
        CooldownCounter();
    }

    private void CooldownCounter()
    {
        foreach (Abilities a in playableCharacter.abilities)
        {
            if (a.onCooldown == true)
            {
                a.counter += Time.deltaTime;
                if (a.counter >= a.cooldown)
                {
                    a.onCooldown = false;
                    a.counter = 0f;
                }

            }
        }
    }

    private void AbilityStateChanger()
    {
        if (Input.GetKey(KeyCode.Q) == KeyState.KEY_DOWN || playableCharacter.state == State.ABILITY_1)
        {
            if (!playableCharacter.abilities[0].onCooldown)
            {
                playableCharacter.state = State.ABILITY_1;
            }
            else
            {
                Debug.Log("Ability on Cooldown! You have" + (playableCharacter.abilities[0].cooldown - playableCharacter.abilities[0].counter) + "seconds left to use it again!");
            }
        }
        if (Input.GetKey(KeyCode.W) == KeyState.KEY_DOWN || playableCharacter.state == State.ABILITY_2)
        {
            if (!playableCharacter.abilities[1].onCooldown)
            {
                playableCharacter.state = State.ABILITY_2;
            }
            else
            {
                Debug.Log("Ability on Cooldown! You have" + (playableCharacter.abilities[1].cooldown - playableCharacter.abilities[1].counter) + "seconds left to use it again!");
            }
        }
        CastOrCancel();
    }

    private void CastOrCancel()
    {
        if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_DOWN && playableCharacter.state != State.NONE)
        {
            InternalCalls.InstancePrefab(playableCharacter.abilities[(int)playableCharacter.state - 1].prefabPath);
            playableCharacter.abilities[(int)playableCharacter.state - 1].onCooldown = true;
            playableCharacter.state = State.NONE;
        }
        if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_DOWN && playableCharacter.state != State.NONE)
        {
            playableCharacter.state = State.NONE;
        }
    }

    private void PlayerCases()
    {
        switch (players.Length)
        {
            case 4:
                if (Input.GetKey(KeyCode.F4) == KeyState.KEY_DOWN)
                {
                    characterSelected = 3;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 3;
            case 3:
                if (Input.GetKey(KeyCode.F3) == KeyState.KEY_DOWN)
                {
                    characterSelected = 2;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 2;
            case 2:
                if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
                {
                    characterSelected = 1;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 1;
            case 1:
                if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
                {
                    characterSelected = 0;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                break;
        }
    }

    void ChangeCharacter(int id)
    {
        for (int i = 0; i < players.Length; i++)
        {
            players[i].GetComponent<Player>().SetControled(false);

        }
        players[id].GetComponent<Player>().SetControled(true);
    }
}

