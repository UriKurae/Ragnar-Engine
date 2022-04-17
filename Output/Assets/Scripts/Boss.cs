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
	public Rigidbody rigidbody;
	
	public BossState state;
	NavAgent agent;
	GameObject[] enemies = new GameObject[10];
	GameObject[] players = new GameObject[3];
	public void Start()
	{
		material = gameObject.GetComponent<Material>();
		state = BossState.PHASE1;

		enemies = GameObject.FindGameObjectsWithTag("EnemiesBoss");
		players = GameObject.FindGameObjectsWithTag("Player");
		rigidbody = gameObject.GetComponent<Rigidbody>();

		agent = gameObject.GetComponent<NavAgent>();
	}
	public void Update()
	{
		if (Input.GetKey(KeyCode.N) == KeyState.KEY_DOWN)
		{
			state += 1;
			NextState();
		}

		if (state == BossState.PHASE2)
        {
			Vector3 jumpTo = new Vector3(100.0f, 100.0f, 100.0f);
			Vector3 area = new Vector3(10.0f, 10.0f, 10.0f);
			for (int i = 0; i < players.Length; ++i)
            {
				if (players[i].transform.localPosition.magnitude <= area.magnitude && 
					players[i].transform.localPosition.magnitude < jumpTo.magnitude)
                {
					jumpTo = players[i].transform.localPosition;
				}
            }

			if (jumpTo != new Vector3(100.0f, 100.0f, 100.0f))
				agent.MoveTo(jumpTo);
		}

		if (Input.GetKey(KeyCode.M) == KeyState.KEY_DOWN)
        {
			GenerateEnemies();
        }
	}

	private void NextState()
    {
		switch (state)
		{
			case BossState.PHASE2:
				rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 0.5f;
				material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
				break;
			case BossState.PHASE3:
				rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 0.8f;
				material.SetTexturePath("Assets/Resources/white.png");
				break;
			case BossState.PHASE4:
				rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 1.2f;
				material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
				break;
		}
    }

	public void OnCollisionEnter(Rigidbody other)
	{
		if (other.gameObject.tag == "Backstab")
        {
			if (enemies.Length == 0 && state == BossState.PHASE1)
            {
				state = BossState.PHASE2;
				NextState();
            }
		}
	}

	private void GenerateEnemies()
    {
		InternalCalls.InstancePrefab("Enemy1Boss");
		GameObject enemy1 = GameObject.Find("Enemy1Boss");
		enemy1.GetComponent<Rigidbody>().SetBodyPosition(new Vector3(5.0f, 0.0f, 0.0f));

		InternalCalls.InstancePrefab("Enemy2Boss");
		enemy1 = GameObject.Find("Enemy2Boss");
		enemy1.GetComponent<Rigidbody>().SetBodyPosition(new Vector3(-5.0f, 0.0f, 0.0f));
	}
}