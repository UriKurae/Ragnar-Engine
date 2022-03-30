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

        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
        {
            characterSelected = 0;
            ChangeCharacter(characterSelected);
            Debug.Log("Character Changed");
        }
        
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
        {
 		    characterSelected = 1;
            ChangeCharacter(characterSelected); 
            Debug.Log("Character Changed");
        }

        if (Input.GetKey(KeyCode.F3) == KeyState.KEY_DOWN)
        {
            characterSelected = 2;
            ChangeCharacter(characterSelected);
            Debug.Log("Character Changed");
        }

    }
    void ChangeCharacter(int id)
    {
        for (int i = 0; i < players.Length; i++)
        {
            players[i].GetComponent<Player>().SetControled(false);

        }
        players[characterSelected].GetComponent<Player>().SetControled(true);
    }
}
