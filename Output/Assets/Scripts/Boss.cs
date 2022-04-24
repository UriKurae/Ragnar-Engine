using System;
using RagnarEngine;

public class Boss : RagnarComponent
{
	public struct BarrelSpawnLocation
	{
		public BarrelSpawnLocation(int num, Vector3 location, bool destroyed)
		{
			barrelNum = num;
			spawnLocation = location;
			isDestroyed = destroyed;
		}

		public bool isDestroyed;
		public int barrelNum;
		public Vector3 spawnLocation;
	}
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
	
	// NavMesh behaviour
	NavAgent agent;
	float calculatePathCooldown = 0.0f;

	// Percetion variables
	private Vector3 offset;
	public int index = 0;

	// Arrays for the players
	GameObject[] players = new GameObject[3];

	// Phase 3 mechanics

	GameObject[] barrels = new GameObject[3];
	public BarrelSpawnLocation[] barrelLocations = new BarrelSpawnLocation[3];
	public int barrelCount = 0;
	public int stunnedHits = 0;
	float barrelCooldown = 0.0f;
	//bool shieldInmunity = false;
	bool phase3Location = false;

	//private int destPoint = 0;

	// Phase 4 mechanics
	int throwedRocks = 0;
	float hitGroundCooldown = 10.0f;
	float tiredCooldown = 0.0f;
	bool rocksAvailable = false;
	bool throwingRocks = false;
	bool tired = false;
	GameObject nextRock;
	float sweepAttackCooldown = 0.0f;

	//GameObject[] waypoints;
	bool bossStop = false;
	float countDown = 5.0f;

	public void Start()
	{
		material = gameObject.GetComponent<Material>();
		state = BossState.PHASE1;

		offset = gameObject.GetSizeAABB();

		//waypoints = GameObject.FindGameObjectsWithTag("BossWaypoints");
		rigidbody = gameObject.GetComponent<Rigidbody>();

		agent = gameObject.GetComponent<NavAgent>();

		barrelLocations[0] = new BarrelSpawnLocation(0, new Vector3(8.0f, 1.5f, 0.0f), true);
		barrelLocations[1] = new BarrelSpawnLocation(1, new Vector3(12.0f, 1.5f, 2.0f), true);
		barrelLocations[2] = new BarrelSpawnLocation(2, new Vector3(12.0f, 1.5f, 5.0f), true);
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
			Phase2();
		}
		else if (state == BossState.PHASE3)
        {
			Phase3();
		}
		else if (state == BossState.PHASE4)
        {
			if (tired == false && hitGroundCooldown <= 0.0f && rocksAvailable == false)
			{
                GenerateRocks();
				hitGroundCooldown = 10.0f;
				throwingRocks = true;
			}
			else if (!throwingRocks && hitGroundCooldown > 0.0f && !tired)
			{
				FollowPlayer();
				hitGroundCooldown -= Time.deltaTime;
			}
			else if (tired == false && rocksAvailable == true)
			{
				ThrowRock();
				throwedRocks++;
				rocksAvailable = false;
			}
			else if (tired == false && throwedRocks < 5 && rocksAvailable == false)
			{
				if (nextRock == null)
				{
					for (int i = 0; i < 5; ++i)
					{
						string rockPrefab = "Rock" + (i + 1);
						nextRock = GameObject.Find(rockPrefab);
						if (nextRock != null)
							break;
					}
				}
				else
				{
					agent.MoveTo(nextRock.transform.localPosition);
				}
			}
			else
			{
				tired = true;
				throwingRocks = false;
				tiredCooldown -= Time.deltaTime;
				if (tiredCooldown <= 0.0f) tired = false;
			}

			SweepAttack();
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
				players = GameObject.FindGameObjectsWithTag("Player");
				rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 0.5f;
                material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
                GotoNextPoint();
                break;
			case BossState.PHASE3:
				rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 0.75f;
				material.SetTexturePath("Assets/Resources/white.png");
				barrelCooldown = 0.0f;
				GenerateBarrels();
				break;
			case BossState.PHASE4:
				//if (!shieldInmunity)
				//{
					rigidbody.linearVelocity = GameObject.Find("Player").GetComponent<Rigidbody>().linearVelocity * 1.2f;
					material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
				//}
				//else state--;
				break;
			default:
				state--;
				break;
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
	public void GenerateRocks()
    {
		for (int i = 0; i < 5; ++i)
		{
			string rockPrefab = "Rock" + (i + 1);
			GameObject rock = GameObject.Find(rockPrefab);
			if (rock != null)
				InternalCalls.Destroy(rock);
		}
		Vector3 bossPos = gameObject.transform.localPosition;
        for (int i = 0; i < 5; ++i)
        {
			string rockPrefab = "Rock" + (i + 1);
			InternalCalls.InstancePrefab(rockPrefab);
			GameObject rock = GameObject.Find(rockPrefab);

			if (i % 2 == 0) rock.GetComponent<Rigidbody>().SetBodyPosition(new Vector3(bossPos.x, 25.0f, bossPos.z + i));
			else rock.GetComponent<Rigidbody>().SetBodyPosition(new Vector3(bossPos.x + i, 25.0f, bossPos.z));
		}
    }

	// Boss behaviour for the phases

	// Phase 2
	public void Phase2()
    {
        // this instakills
        if (PerceptionCone(90))
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
            {
               agent.MoveTo(jumpTo);
            }
        }

        Patrol();
    }
	
	// Phase 3
	public void Phase3()
    {
		if (phase3Location == false)
		{
			// Move to new location as soon as phase 3 starts
			Vector3 destination = new Vector3(1.0f, 1.0f, 1.0f);

			agent.MoveTo(destination);

			if (this.gameObject.transform.localPosition == destination) phase3Location = true;
			
			phase3Location = true;
			//shieldInmunity = true;
		}
		else
        {
			ExplodeBarrels();
			GenerateBarrels();
			if (barrelCount < 3) barrelCooldown -= Time.deltaTime;
		}

		if (phase3Location) FollowPlayer();
	}
	
	// Phase 4
	public void Phase4()
    {
		int asd = 0;
		asd++;
	}

	private void ThrowRock()
    {
		InternalCalls.InstancePrefab("RockBoss");
    }
	private void GenerateBarrels()
    {
		if (barrelCooldown <= 0.0f && barrelCount < 3)
		{
            for (int i = 0; i < 3; ++i)
            {
				if (barrelLocations[i].isDestroyed)
				{
					string barrelName = "Barrel" + (i + 1);
					// Need to know the name of the prefab
					InternalCalls.InstancePrefab(barrelName);
					GameObject barrel = GameObject.Find(barrelName);
					barrel.GetComponent<Rigidbody>().SetBodyPosition(barrelLocations[i].spawnLocation);
					barrelLocations[i].isDestroyed = false;
					barrelCount++;
				}
			}
			barrelCooldown = 15.0f;
		}
	}

	private void ExplodeBarrels()
    {
		if (stunnedHits == 3)
        {

        }
		else if (stunnedHits == 1)
		{
			material.SetTexturePath("Assets/Resources/UI/mainMenuScreen.png");
		}
	}

	private bool PerceptionCone(int angleDegrees)
	{
        Vector3 bossPos = gameObject.transform.globalPosition;
        Vector3 bossForward = gameObject.transform.forward;
        Vector3 initPos = new Vector3(bossPos.x + (bossForward.x * offset.x * 0.6f), bossPos.y + 0.1f, bossPos.z + (bossForward.z * offset.z * 0.6f));

        index = RayCast.PerceptionCone(initPos, bossForward, angleDegrees, 16, 8, players, players.Length);
        return (index == -1) ? false : true;
    }

	public void GotoNextPoint()
	{
        //gameObject.GetComponent<AudioSource>().PlayClip("FOOTSTEPS");
        //gameObject.GetComponent<Animation>().PlayAnimation("Walk");

        //Debug.Log(waypoints[destPoint].transform.globalPosition.ToString());
        //agent.CalculatePath(waypoints[destPoint].transform.globalPosition);
        //destPoint = (destPoint + 1) % waypoints.Length;
    }

	private void Patrol()
    {
		if (!bossStop && agent.MovePath())
        {
			Debug.Log("Stopped");
			bossStop = true;
        }

		if (bossStop)
        {
			countDown -= Time.deltaTime;
			if (countDown <= 0.0f)
            {
				bossStop = false;
				countDown = 5.0f;
				GotoNextPoint();
            }
        }
	}

	private void FollowPlayer()
	{
		if (calculatePathCooldown <= 0.0f)
        {
			agent.CalculatePath(players[0].transform.localPosition);
			calculatePathCooldown = 3.0f;
		}

		agent.MovePath();
		calculatePathCooldown -= Time.deltaTime;
	}
	public void SweepAttack()
    {
		for (int i = 0; i < players.Length; ++i)
		{
			if ((players[i].transform.localPosition.magnitude - gameObject.transform.localPosition.magnitude) <= 2.0f)
			{
				// Play sweep attack animation

				// Play sweep attack sound

				// Hit player, lower his HP

				// Reset Cooldown
				sweepAttackCooldown = 5.0f;
			}
		}
   

		sweepAttackCooldown -= Time.deltaTime;
    }
	public void OnCollision(Rigidbody other)
	{
		if (other.gameObject.tag == "Backstab")
		{
			state++;
			NextState();
		}
		else if(other.gameObject.tag == "Rocks" && rocksAvailable == false)
        {
			rocksAvailable = true;
			InternalCalls.Destroy(other.gameObject);
			nextRock = null;
        }
	}
}