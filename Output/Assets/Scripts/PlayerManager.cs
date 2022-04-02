using System;
using RagnarEngine;

public class PlayerManager : RagnarComponent
{
    public GameObject[] players;
    int characterSelected = 0;
    public void Start()
	{
        players = GameObject.FindGameObjectsWithTag("Player");
        ChangeCharacter(characterSelected);
    }
	public void Update()
	{
        switch (players.Length)
        {
            case 4:
                if (Input.GetKey(KeyCode.F4) == KeyState.KEY_DOWN)
                {
                    characterSelected = 3;
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 3;
            case 3:
                if (Input.GetKey(KeyCode.F3) == KeyState.KEY_DOWN)
                {
                    characterSelected = 2;
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 2;
            case 2:
                if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
                {
                    characterSelected = 1;
                    ChangeCharacter(characterSelected);
                    Debug.Log("Character Changed");
                }
                goto case 1;
            case 1:
                if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
                {
                    characterSelected = 0;
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

