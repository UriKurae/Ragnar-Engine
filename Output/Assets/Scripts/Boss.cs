using System;
using RagnarEngine;

public class Boss : RagnarComponent
{
	public enum BossState
	{
		PHASE1,
		PHASE2,
		PHASE3,
		PHASE4
	};

	public Material material;
	public BossState state;
	int hits;
	public void Start()
	{
		material = gameObject.GetComponent<Material>();
		state = BossState.PHASE1;
	}
	public void Update()
	{
		if (Input.GetKey(KeyCode.N) == KeyState.KEY_DOWN)
		{
			hits++;
			CheckState();
		}
	}

	private void CheckState()
    {
		switch(hits)
        {
			case 2:
				state = BossState.PHASE2;
				material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
				break;
			case 4:
				state = BossState.PHASE3;
				material.SetTexturePath("Assets/Resources/white.png");
				break;
			case 6:
				state = BossState.PHASE4;
				material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
				break;
		}
    }
}