using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{
    GameObject SkillKnife;
    GameObject SkillNade;
    public void Start()
    {
        Debug.Log("Starting Ability Manager");
        SkillKnife = GameObject.Find("SkillKnife");
        SkillNade = GameObject.Find("SkillNade");
    }

    public void Update()
    {
        ///////// ABILITIES /////////
        // Rock Throw
        if ((Input.GetKey(KeyCode.S) == KeyState.KEY_DOWN || SkillNade.GetComponent<UIButton>().GetButtonState() == 3) && GameObject.Find("Rock") == null)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("THROWROCK");
            InternalCalls.InstancePrefab("Assets/Prefabs/Rock.rgprefab");
        }

        // Knife Throw
        if ((Input.GetKey(KeyCode.A) == KeyState.KEY_DOWN || SkillKnife.GetComponent<UIButton>().GetButtonState() == 3) && GameObject.Find("Knife") == null)
        {
            //TODO_AUDIO
            gameObject.GetComponent<AudioSource>().PlayClip("THROWKNIFE");
            InternalCalls.InstancePrefab("Assets/Prefabs/Knife.rgprefab");
        }
        /////////////////////////////
    }
}