using System;
using RagnarEngine;

public class PlayerManager : RagnarComponent
{
    public GameObject[] players;
    public int characterSelected = 0;

    public Characters[] characters = new Characters[3];
    public Characters playableCharacter;

    GameObject[] area = null;
    public bool drawnArea = false;
    bool crouched = false;

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
            prefabPath = "Player",
            state = State.NONE,
            abilities = new Abilities[4]
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
            abilities = new Abilities[4]
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
            name = "Spice Bomb",
            prefabPath = "Spice Granade",
            prefabArea = "BackStab Area",
            charges = -1,
            cooldown = 0f
        }; // Spice Bomb
        characters[1].abilities[3] = new Abilities
        {
            name = "Hunter Seeker",
            prefabPath = "BackStab",
            prefabArea = "BackStab Area",
            charges = -1,
            cooldown = 0f
        }; // Hunter Seeker

        // Player 3
        characters[2] = new Characters
        {
            name = "Stilgar",
            prefabPath = "Player_3",
            state = State.NONE,
            abilities = new Abilities[4]
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
        if (Input.GetKey(KeyCode.LSHIFT) == KeyState.KEY_DOWN)
        {
            crouched = !crouched;
        }

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
        if (Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN)
        {
            SpawnArea((int)State.ABILITY_1);
        }

        // LETRA S --> HABILIDAD 2 DE TODOS LOS PJS
        if (Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN)
        {
            SpawnArea((int)State.ABILITY_2);
        }

        // LETRA D --> HABILIDAD 3 DE TODOS LOS PJS
        if (Input.GetKey(KeyCode.D) == KeyState.KEY_DOWN)
        {
            SpawnArea((int)State.ABILITY_3);
        }

        // LETRA F --> HABILIDAD 4 DE TODOS LOS PJS
        if (Input.GetKey(KeyCode.F) == KeyState.KEY_DOWN)
        {
            SpawnArea((int)State.ABILITY_4);
        }

        // Si el estado no es NONE, significa que la habilidad está lista para ser casteada, y entrará en esta función.
        if (playableCharacter.state != State.NONE) CastOrCancel();
    }

    private void SpawnArea(int ability)
    {
        // Comprobador de cargas de habilidad. Si entra aquí, significa que la habilidad no tiene cargas
        if (playableCharacter.abilities[ability - 1].charges == 0)
        {
            playableCharacter.state = State.NONE;
        }
        // Entra aquí si la habilidad tiene cargas o las cargas son -1 (Habilidad infinita (Solo cooldown)). Cambia el estado del player al de la habilidad que haya marcado.
        else if (!playableCharacter.abilities[ability - 1].onCooldown)
        {
            playableCharacter.state = (State)ability;

            // Dibujado del área de rango.
            if (!drawnArea)
            {
                drawnArea = true;
                InternalCalls.InstancePrefab(playableCharacter.abilities[ability - 1].prefabArea);
                area = GameObject.FindGameObjectsWithTag("AbilityRange");
                players[characterSelected].AddChild(area[0]);
                area[0].transform.localPosition = new Vector3(0, area[0].transform.localPosition.y, 0);
            }

            players[characterSelected].GetComponent<Player>().SetState(ability);
        }
        // Si la habilidad está en cooldown y tiene cargas, entrará aquí y pondrá el state del player en NONE.
        else
        {
            Debug.Log("Ability on Cooldown! You have" + (playableCharacter.abilities[ability - 1].cooldown - playableCharacter.abilities[ability - 1].counter) + "seconds left to use it again!");
            playableCharacter.state = State.NONE;
        }
    }

    private void CastOrCancel()
    {
        if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
        {
            if (crouched)
            {
                players[characterSelected].GetComponent<Animation>().PlayAnimation("CrouchShoot");
            }
            else
            {
                players[characterSelected].GetComponent<Animation>().PlayAnimation("Shoot");
            }

            switch(playableCharacter.state)
            {
                case State.ABILITY_1:
                    {
                        if (playableCharacter == characters[0])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONTHROWINGKNIFETHROW");
                        }
                        else if (playableCharacter == characters[1])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONCRYSKNIFESTAB");
                        }
                        else if (playableCharacter == characters[2])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONSWORDHIT");
                        }
                        break;
                    }
                case State.ABILITY_2:
                    {
                        if (playableCharacter == characters[0])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("THROWROCK");
                        }
                        else if (playableCharacter == characters[1])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONCAMOUFLAGEACTIVATE");
                        }
                        else if (playableCharacter == characters[2])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONSTUNNERSHOT");
                        }
                        break;
                    }
                case State.ABILITY_3:
                    {
                        if (playableCharacter == characters[0])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("THROWROCK");
                        }
                        else if (playableCharacter == characters[1])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONCAMOUFLAGEACTIVATE");
                        }
                        else if (playableCharacter == characters[2])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONSTUNNERSHOT");
                        }
                        break;
                    }
                case State.ABILITY_4:
                    {
                        if (playableCharacter == characters[0])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("THROWROCK");
                        }
                        else if (playableCharacter == characters[1])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONCAMOUFLAGEACTIVATE");
                        }
                        else if (playableCharacter == characters[2])
                        {
                            players[characterSelected].GetComponent<AudioSource>().PlayClip("WEAPONSTUNNERSHOT");
                        }
                        break;
                    }
                default:
                    break;

            }

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

            for (int i = 0; i < area.Length; i++)
            {
				gameObject.EraseChild(area[i]);
                InternalCalls.Destroy(area[i]);
            }
            area = null;
            drawnArea = false;
        }
        // Se cancela el estado de la habilidad para que el área de rango deje de mostrarse.
        if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_DOWN)
        {
            playableCharacter.state = State.NONE;
            players[characterSelected].GetComponent<Player>().SetState((int)State.NONE);

            for (int i = 0; i < area.Length; i++)
			{
				gameObject.EraseChild(area[i]);
				InternalCalls.Destroy(area[i]);
            }
            area = null;
            drawnArea = false;
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


