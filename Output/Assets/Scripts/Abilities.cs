using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{

    public void Start()
    {
        Debug.Log("Starting Ability Manager");
    }

    public void Update()
    {
        ///////// ABILITIES /////////
        // Rock Throw
        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN && GameObject.Find("Rock") == null)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("ThrowRock");
            InternalCalls.InstancePrefab("Assets/Prefabs/Rock.rgprefab");
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN && GameObject.Find("Knife") == null)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("ThrowKnife");
            InternalCalls.InstancePrefab("Assets/Prefabs/Knife.rgprefab");
        }
        /////////////////////////////
    }
}