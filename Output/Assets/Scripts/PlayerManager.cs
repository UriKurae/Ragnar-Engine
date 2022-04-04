using System;
using RagnarEngine;

public class PlayerManager : RagnarComponent
{
    public GameObject[] players;
    int characterSelected = 0;

    public Characters[] characters = new Characters[3];
    public Characters playableCharacter;

    public void Start()
	{
        ///////////////////////////////////////////////////////////////////
        // AVISO
        // Esto de aquí está hardcodeadísimo
        // Pero no hay otra manera de hacerlo
        // Cuando se puedan meter cositas desde inspector
        // Se solucionará este problema ^^

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
            charges = -1,
            cooldown = 25f
        };
        characters[0].abilities[1] = new Abilities
        {
            name = "Rock Throw",
            prefabPath = "Assets/Prefabs/Rock.rgprefab",
            charges = -1,
            cooldown = 20f
        };

        // Player 2
        characters[1] = new Characters
        {
            name = "Chani",
            prefabPath = "Assets/Prefabs/Player_2.rgprefab",
            state = State.NONE,
            abilities = new Abilities[2]
        };
        characters[1].abilities[0] = new Abilities
        {
            name = "Backstab",
            prefabPath = "Assets/Prefabs/BackStab.rgprefab",
            charges = -1,
            cooldown = 0f
        };
        characters[1].abilities[1] = new Abilities
        {
            name = "Camouflage",
            prefabPath = "Assets/Prefabs/Rock.rgprefab",
            charges = -1,
            cooldown = 30f
        };

        // Player 3
        characters[2] = new Characters
        {
            name = "Stilgar",
            prefabPath = "Assets/Prefabs/Player_3.rgprefab",
            state = State.NONE,
            abilities = new Abilities[2]
        };
        characters[2].abilities[0] = new Abilities
        {
            name = "Sword Slash",
            prefabPath = "Assets/Prefabs/SwordSlash.rgprefab",
            charges = -1,
            cooldown = 0f
        };
        characters[2].abilities[1] = new Abilities
        {
            name = "Stunner",
            prefabPath = "Assets/Prefabs/StunnerShot.rgprefab",
            charges = 4,
            cooldown = 5f
        };
        ///////////////////////////////////////////////////////////////////

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

        /*Cambiador de estados para saber qué habilidad estás o no casteando (Básicamente hace que el personaje entre en un estado donde si clickas una tecla
        muestre el rango de habilidad, y entre en un estado de castear o cancelar la habilidad seleccionada (Click derecho cancel/click izquierdo casteo)).
        Aquí debería ir la zona de rango de cada habilidad.*/
        AbilityStateChanger();

        /*Contador de cooldown para cada habilidad
        Funciona en todos los casos con todos los pjs.*/
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
        // LETRA A --> HABILIDAD 1 DE TODOS LOS PJS
        if (Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN || playableCharacter.state == State.ABILITY_1)
        {
            // Comprobador de cargas de habilidad. Si entra aquí, significa que la habilidad no tiene cargas
            if(playableCharacter.abilities[0].charges == 0)
            {
                playableCharacter.state = State.NONE;
            }
            // Entra aquí si la habilidad tiene cargas o las cargas son -1 (Habilidad infinita (Solo cooldown)). Cambia el estado del player al de la habilidad que haya marcado.
            else if (!playableCharacter.abilities[0].onCooldown)
            {
                playableCharacter.state = State.ABILITY_1;
                players[characterSelected].GetComponent<Player>().SetState((int)State.ABILITY_1);
            }
            // Si la habilidad está en cooldown y tiene cargas, entrará aquí y pondrá el state del player en NONE.
            else
            {
                Debug.Log("Ability on Cooldown! You have" + (playableCharacter.abilities[0].cooldown - playableCharacter.abilities[0].counter) + "seconds left to use it again!");
                playableCharacter.state = State.NONE;
            }
        }

        // LETRA S --> HABILIDAD 2 DE TODOS LOS PJS
        if (Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN || playableCharacter.state == State.ABILITY_2)
        {
            if (playableCharacter.abilities[1].charges == 0)
            {
                playableCharacter.state = State.NONE;
            }
            else if (!playableCharacter.abilities[1].onCooldown)
            {
                playableCharacter.state = State.ABILITY_2;
                players[characterSelected].GetComponent<Player>().SetState((int)State.ABILITY_2);
            }
            else
            {
                Debug.Log("Ability on Cooldown! You have" + (playableCharacter.abilities[1].cooldown - playableCharacter.abilities[1].counter) + "seconds left to use it again!");
                playableCharacter.state = State.NONE;

            }
        }

        // LETRA D --> HABILIDAD 3 DE TODOS LOS PJS
        // TODO

        // LETRA F --> HABILIDAD 4 DE TODOS LOS PJS
        // TODO

        // Si el estado no es NONE, significa que la habilidad está lista para ser casteada, y entrará en esta función.
        if (playableCharacter.state != State.NONE) CastOrCancel();
    }

    private void CastOrCancel()
    {
        if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
        {
            // Instancia la habilidad en cuestión. 
            InternalCalls.InstancePrefab(playableCharacter.abilities[(int)playableCharacter.state - 1].prefabPath);

            // Al haberse instanciado una habilidad, comprueba si funciona por cargas. Si lo hace resta una carga a la habilidad.
            if(playableCharacter.abilities[(int)playableCharacter.state - 1].charges != -1 && playableCharacter.abilities[(int)playableCharacter.state - 1].charges != 0)
            {
                playableCharacter.abilities[(int)playableCharacter.state - 1].charges -= 1;
            }

            // Pone la habilidad en cooldown y el player en estado de NONE
            playableCharacter.abilities[(int)playableCharacter.state - 1].onCooldown = true;
            playableCharacter.state = State.NONE;
            // Se cambia el estado a POSTCAST para evitar que se mueva directamente después de castear la habilidad. En el update de los players se cambiará a NONE nuevamente para que se pueda mover (Tras un ciclo de update). 
            players[characterSelected].GetComponent<Player>().SetState((int)State.POSTCAST);
        }
        // Se cancela el estado de la habilidad para que el área de rango deje de mostrarse.
        if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_DOWN)
        {
            playableCharacter.state = State.NONE;
            players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);
        }
    }

    private void PlayerCases()
    {
        switch (players.Length)
        {
            case 4:
                if (Input.GetKey(KeyCode.F4) == KeyState.KEY_DOWN)
                {
                    players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);
                    characterSelected = 3;
                    playableCharacter.state = State.NONE;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 3;
            case 3:
                if (Input.GetKey(KeyCode.F3) == KeyState.KEY_DOWN)
                {
                    players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);
                    characterSelected = 2;
                    playableCharacter.state = State.NONE;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 2;
            case 2:
                if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
                {
                    players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);
                    characterSelected = 1;
                    playableCharacter.state = State.NONE;
                    playableCharacter = characters[characterSelected];
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 1;
            case 1:
                if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
                {
                    players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);
                    characterSelected = 0;
                    playableCharacter.state = State.NONE;
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


