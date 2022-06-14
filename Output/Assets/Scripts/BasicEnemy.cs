using System;
using System.Collections.Generic;
using RagnarEngine;

public class BasicEnemy : RagnarComponent
{

    // Components

    private Animation animationComponent;
    private Rigidbody rb;
    private AudioSource audioComponent;

    /////////////
    public int velocity = 1000;

    public NavAgent agents;
    public List<GameObject> waypoints;
    private int destPoint = 0;
    public EnemyState state;
    public EnemyType enemyType;

    public Vector3 initialPos;
    public Quaternion initialRot;

    // States
    public bool patrol;
    public bool stopState = false;
    public bool backstab = false;

    // Timers
    public float stoppedTime = 0f;

    // Player tracker
    public GameObject[] players;
    public GameObject[] colliders;
    GameObject SceneAudio;
    private Vector3 offset;
    public int index = 0;

    // States
    public bool canShoot = true;
    public bool pendingToDelete = false;
    public bool controlled = false;
    public bool returning = false;
    public bool enterDistract = true;
    public bool enterStunner = true;

    // Timers
    public float shootCooldown = 0f;
    public bool isDying = false;
    public float controlledCooldown = 10;

    float initialSpeed;

    bool distracted = false;
    public float distractedTimer = -1f;
    bool stunned = false;
    public float stunnedTimer = -1f;

    float coneTimer = 0.0f;
    int coneMaxTime = 1;

    // Cone
    public bool coneRotate = true;
    private bool toRight = true;
    private float angleOffset = 0;
    private int radius = 23;
    private int angle = 60;

    GameObject[] childs;
    public ParticleSystem stunPartSys;

    public bool canLookOut = false;
    int retardedFrames;

    GameObject buffCounter;
    float buffTemp;
    public GameObject circle;
    GameObject pointCharacter;
    Light pointerLight;
    public Light abilityLight;

    GameObject timerSlider;

    GameObject Ability1Bg;
    GameObject Ability2Bg;
    GameObject Ability3Bg;
    GameObject Ability4Bg;

    Vector3 pos;
    Vector3 wPos;
    public void Start()
    {
        // Get all Components
        animationComponent = gameObject.GetComponent<Animation>();
        rb = gameObject.GetComponent<Rigidbody>();

        pos = new Vector3(0.0f, 0.0f, 0.0f);
        wPos = new Vector3(0.0f, 0.0f, 0.0f);

        SceneAudio = GameObject.Find("AudioLevel1");
        offset = gameObject.GetSizeAABB();

        if (state != EnemyState.DEATH)
        {
            animationComponent.PlayAnimation("Idle");
            if (waypoints.Count != 0)
            {
                GotoNextPoint();
                patrol = false;
            }
        }

        initialSpeed = 6;

        childs = gameObject.childs;

        for (int i = 0; i < childs.Length; ++i)
        {
            if (childs[i].name == "StunParticles")
            {
                stunPartSys = childs[i].GetComponent<ParticleSystem>();
                //break;
            }
            else if (childs[i].name == "StabParticles")
            {
                childs[i].GetComponent<ParticleSystem>().Pause();
            }
            else if (childs[i].name == "KnifeParticles")
            {
                childs[i].GetComponent<ParticleSystem>().Pause();
            }
            else if (childs[i].name == "SwordSlashParticles")
            {
                childs[i].GetComponent<ParticleSystem>().Pause();
            }
        }

        stunPartSys.Pause();
        retardedFrames = GameObject.Find("EnemyManager").GetComponent<EnemyManager>().retardedFrames;

        buffCounter = GameObject.Find("UIBVoiceNum");

        pointCharacter = GameObject.Find("PlayerReminder").childs[3];
        pointerLight = pointCharacter.GetComponent<Light>();


        Ability1Bg = GameObject.Find("Ability1Bg");
        Ability2Bg = GameObject.Find("Ability2Bg");
        Ability3Bg = GameObject.Find("Ability3Bg");
        Ability4Bg = GameObject.Find("Ability4Bg");
    }
    public void OnCreation()
    {
        players = GameObject.FindGameObjectsWithTag("Player");
        agents = gameObject.GetComponent<NavAgent>();
        audioComponent = gameObject.GetComponent<AudioSource>();
    }
    public void Update()
    {
        if (state != EnemyState.DEATH && state != EnemyState.IS_DYING)
        {
            if (!controlled)
            {
                if (!pendingToDelete && !isDying && !stunned)
                {
                    if (returning)
                    {
                        agents.CalculatePath(initialPos);
                        if (agents.MovePath())
                        {
                            rb.SetBodyRotation(initialRot);
                            returning = false;
                        }
                    }
                    if (!distracted && waypoints.Count != 0)
                    {
                        Patrol();
                    }
                    if (canLookOut)
                        LookOut(retardedFrames);
                }                

                if (stunnedTimer >= 0)
                {
                    stunnedTimer -= Time.deltaTime;
                    if (stunnedTimer < 0)
                    {
                        stunPartSys.Pause();
                        if (waypoints.Count != 0)
                            animationComponent.PlayAnimation("Walk");
                        else
                            animationComponent.PlayAnimation("Idle");
                        stunned = false;
                        stunnedTimer = -1f;
                        enterStunner = true;
                    }
                }

                if (distractedTimer >= 0)
                {
                    distractedTimer -= Time.deltaTime;
                    
                    if (distractedTimer < 0)
                    {
                        distracted = false;
                        distractedTimer = -1f;
                        enterDistract = true;
                    }
                }
            }
            else
            {
                if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP && !backstab)
                {
                    if (agents.CalculatePath(agents.hitPosition).Length > 0)
                    {
                        pointCharacter.transform.globalPosition = agents.hitPosition;
                        pointerLight.intensity = 10;
                        animationComponent.PlayAnimation("Walk");
                    }
                }
                if (agents.MovePath())
                {
                    pointerLight.intensity = 0;
                    animationComponent.PlayAnimation("Idle");
                }

                if (!backstab && Input.GetKey(KeyCode.Z) == KeyState.KEY_REPEAT)
                {
                    backstab = true;
                    abilityLight.intensity = 1;
                }
                if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP && backstab)
                {
                    InternalCalls.InstancePrefab("BackStabEnemy", gameObject.transform.globalPosition);
                    abilityLight.intensity = 0;
                    backstab = false;
                }
                if (Input.GetMouseClick(MouseButton.RIGHT) == KeyState.KEY_DOWN && backstab)
                {
                    abilityLight.intensity = 0;
                    backstab = false;
                }
                buffTemp = controlledCooldown;
                buffTemp = (float)Math.Round((double)buffTemp, 0);
                buffCounter.GetComponent<UIText>().text = buffTemp.ToString();

                if (timerSlider != null)
                {
                    pos.Set(timerSlider.GetComponent<Transform2D>().position2D.x, timerSlider.GetComponent<Transform2D>().position2D.y, timerSlider.GetComponent<Transform2D>().position2D.z);
                    pos.y += 10;
                    buffCounter.GetComponent<Transform2D>().position2D = pos;
                }

                Ability1Bg.GetComponent<UIImage>().SetImageGeneralColor(255, 0, 0);
                Ability2Bg.GetComponent<UIImage>().SetImageGeneralColor(255, 0, 0);
                Ability3Bg.GetComponent<UIImage>().SetImageGeneralColor(255, 0, 0);
                Ability4Bg.GetComponent<UIImage>().SetImageGeneralColor(255, 0, 0);
                GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility3 = false;
                GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility4 = false;

                if (Input.GetKey(KeyCode.ALPHA1) == KeyState.KEY_DOWN || (Input.GetKey(KeyCode.ALPHA2) == KeyState.KEY_DOWN && players.Length > 1) || (Input.GetKey(KeyCode.ALPHA3) == KeyState.KEY_DOWN && players.Length > 2))
                {
                    pointerLight.intensity = 0;
                    abilityLight.intensity = 0;
                    controlled = false;
                    returning = true;
                    gameObject.EraseChild(circle);
                    buffCounter.GetComponent<UIText>().text = "";
                    GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility3 = true;
                    GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility4 = true;
                }

                controlledCooldown -= Time.deltaTime;
                
                if (controlledCooldown < 0)
                {
                    pointerLight.intensity = 0;
                    abilityLight.intensity = 0;
                    Mathf.dir = 0;
                    controlledCooldown = 0f;
                    buffCounter.GetComponent<UIText>().text = "";
                    GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility3 = true;
                    GameObject.Find("PlayerManager").GetComponent<PlayerManager>().canDoAbility4 = true;
                    controlled = false;
                    gameObject.EraseChild(circle);
                    GameObject.Find("PlayerManager").GetComponent<PlayerManager>().ChangeCharacter(0);
                    if (waypoints.Count != 0) agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
                    else returning = true;
                }
                else if(controlledCooldown < 3)
                {
                    float value = circle.GetComponent<Material>().emissiveColor.x;
                    circle.GetComponent<Material>().emissiveColor = new Vector3(Mathf.PingPongFloat(value, Time.deltaTime * 2, 1, 0.1f, false), 0, 0);
                }

                //Effect PointerCharacter
                if (pointerLight.intensity > 0)
                {
                    pointerLight.linear = Mathf.PingPongFloat(pointerLight.linear, Time.deltaTime / 5, -2.05f, -2.12f, true);
                }
            }
        }
        if (isDying)
        {
            state = EnemyState.IS_DYING;
            stunPartSys.Pause();
            if (animationComponent.HasFinished())
            {
                isDying = false;
                pendingToDelete = true;
            }
        }
    }

    public void OnCollision(Rigidbody other)
    {
        if (state != EnemyState.DEATH && state != EnemyState.IS_DYING)
        {
            //gameObject.GetComponent<AudioSource>().PlayClip("EBASIC_SCREAM");
            if (other.gameObject.name == "Knife")
            {
                if (!isDying)
                {
                    isDying = true;
                    for (int i = 0; i < childs.Length; ++i)
                    {
                        if (childs[i].name == "KnifeParticles")
                        {
                            childs[i].GetComponent<ParticleSystem>().Play();
                            break;
                        }
                    }
                    animationComponent.PlayAnimation("Dying");
                    QuestSystem system = GameObject.Find("Quest System").GetComponent<QuestSystem>();
                    system.hasKilledEnemies = true;
                    system.killWithPaul = true;
                    if (system.camouflageActive)
                        system.enemiesCamouflage++;
                }
            }
            if (other.gameObject.tag == "Player")
            {
                Distraction(other.gameObject.transform.globalPosition);
            }
            if (other.gameObject.name == "StunnerShot")
            {
                if (!isDying)
                {
                    audioComponent.PlayClip("EBASIC_BULLETHIT");
                    isDying = true;
                    for (int i = 0; i < childs.Length; ++i)
                    {
                        if (childs[i].name == "KnifeParticles")
                        {
                            childs[i].GetComponent<ParticleSystem>().Play();
                            break;
                        }
                    }
                    animationComponent.PlayAnimation("Dying");
                    QuestSystem system = GameObject.Find("Quest System").GetComponent<QuestSystem>();
                    system.hasKilledEnemies = true;
                    system.killWithStilgar = true;
                    if (system.camouflageActive)
                        system.enemiesCamouflage++;
                }
            }
            if (other.gameObject.name == "HunterSeeker")
            {
                if (!isDying)
                {
                    isDying = true;
                    for (int i = 0; i < childs.Length; ++i)
                    {
                        if (childs[i].name == "StabParticles")
                        {
                            childs[i].GetComponent<ParticleSystem>().Play();
                            break;
                        }
                    }
                    animationComponent.PlayAnimation("Dying");
                    QuestSystem system = GameObject.Find("Quest System").GetComponent<QuestSystem>();
                    system.hasKilledEnemies = true;
                    system.killWithChani = true;
                    if (system.camouflageActive)
                        system.enemiesCamouflage++;
                }
            }
        }
    }

    public void OnTriggerEnter(Rigidbody other)
    {
        if (state != EnemyState.DEATH && state != EnemyState.IS_DYING)
        {
            //// Paul ========================================
            if (other.gameObject.name == "SoundArea" && !stunned)
            {
                // DISTRACTION (ROTATE VISION, NO MOVEMENT TO THE DISTRACTION)
                distracted = true;
                distractedTimer = 5f;
                
                Distraction(other.gameObject.transform.globalPosition);
                if (enterDistract)
                {
                    timerSlider = InternalCalls.InstancePrefab("TimerP", gameObject.transform.globalPosition);
                    timerSlider.GetComponent<TimerSlider>().getGa(gameObject, distractedTimer, (int)enemyType, "distractedTimer");
                    GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesDistractedStone++;
                    enterDistract = false;
                }
            }

            //// Chani =======================================
            if (other.gameObject.name == "SpiceGrenade")
            {
                // STUN (BLIND)
                audioComponent.PlayClip("EBASIC_SCREAM");
                Stun(5f);
                stunPartSys.Play();
                if (enterStunner)
                {
                    GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesGrenade++;
                    enterStunner = false;
                }
            }


            //// Stilgar =====================================
            if (other.gameObject.name == "SwordSlash")
            {
                audioComponent.PlayClip("WPN_SWORDHIT");
                isDying = true;
                for (int i = 0; i < childs.Length; ++i)
                {
                    if (childs[i].name == "SwordSlashParticles")
                    {
                        childs[i].GetComponent<ParticleSystem>().Play();
                        break;
                    }
                }
                animationComponent.PlayAnimation("Dying");
                QuestSystem system = GameObject.Find("Quest System").GetComponent<QuestSystem>();
                system.hasKilledEnemies = true;
                system.killWithStilgar = true;
                if (system.camouflageActive)
                    system.enemiesCamouflage++;
            }
            if (other.gameObject.name == "Whistle" && !stunned)
            {
                // NEED TO CREATE FUNCTION TO INITIATE STOPPED TIME WHEN ARRIVES TO THE POSITION
                patrol = false;
                stoppedTime = 5f;
                agents.CalculatePath(other.gameObject.transform.globalPosition);
                GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesWhistle++;
            }
            if (other.gameObject.name == "Trap")
            {
                // STUN (BLIND)
                audioComponent.PlayClip("EBASIC_SCREAM");
                Stun(7f);
                GameObject.Find("ElectricParticles").GetComponent<ParticleSystem>().Play();
                stunPartSys.Play();
                GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesTrap++;
            }
        }
    }

    public void LookOut(int frames)
    {
        if (state != EnemyState.DEATH && state != EnemyState.IS_DYING && !controlled && !stunned)
        {
            if ((frames == retardedFrames) ? PerceptionCone() : PlayerIsNear())
            {
                coneTimer += Time.deltaTime * frames;
                if (coneTimer >= coneMaxTime)
                {
                    agents.speed = initialSpeed * 1.2f;
                    Shoot();
                }
            }
            else
            {
                agents.speed = initialSpeed;
                if (coneTimer < 0)
                {
                    coneTimer = 0;
                    if (waypoints.Count != 0)
                        animationComponent.PlayAnimation("Walk");
                    else
                        animationComponent.PlayAnimation("Idle");
                }
                else if (coneTimer > 0) coneTimer -= Time.deltaTime * retardedFrames;
            }
            if (!canShoot && shootCooldown >= 0)
            {
                shootCooldown -= Time.deltaTime * frames;
                if (shootCooldown < 0)
                {
                    shootCooldown = 0f;
                    canShoot = true;
                }
            }
        }        
        canLookOut = false;
    }

    private bool PerceptionCone()
    {
        Vector3 enemyPos = gameObject.transform.globalPosition;
        Vector3 enemyForward = gameObject.transform.forward;
        
        if (coneRotate) enemyForward = RotateVector(enemyForward, 80, 2);
        index = RayCast.PerceptionCone(enemyPos, enemyForward, angle, 10, radius, players, players.Length, "Collider", coneTimer/coneMaxTime);

        if (index != -1 && (players[index].GetComponent<Player>().invisible || players[index].GetComponent<Player>().dead || players[index].GetComponent<Player>().isHidden)) return false;
        return (index == -1) ? false : true;
    }

    public bool PlayerIsNear()
    {
        for (int i = 0; i < players.Length; i++)
        {
            Vector3 vecDir = players[i].transform.globalPosition - gameObject.transform.globalPosition;
            if ((vecDir).magnitude <= radius)
            {
                if (Transform.GetAngleBetween(gameObject.transform.forward, vecDir) <= angle * 0.5f)
                {
                    if (RayCast.HitToTag(gameObject.transform.globalPosition, players[i].transform.globalPosition, "Player") != null)
                    {
                        if (players[i].GetComponent<Player>().invisible || players[i].GetComponent<Player>().dead || players[i].GetComponent<Player>().isHidden)
                            return false;

                        index = i;
                        return true;
                    }
                }                    
            }                
        }
        return false;
    }

    private Vector3 RotateVector(Vector3 vec, int angles, int time)
    {
        float w = (angles / time) / (1 / (Time.deltaTime * retardedFrames)); // Angular Velocity in frames

        if (toRight)
        {
            angleOffset += w;
            Vector3 newVec = Transform.RotateY(vec, angleOffset);
            if (Transform.GetAngleBetween(vec, newVec) > angles * 0.5)
                toRight = false;

            vec = newVec;
        }
        else
        {
            angleOffset -= w;
            Vector3 newVec = Transform.RotateY(vec, angleOffset);
            if (Transform.GetAngleBetween(vec, newVec) > angles * 0.5)
                toRight = true;

            vec = newVec;
        }

        return vec;
    }

    public void SetControled(bool flag)
    {
        controlled = flag;
        if (flag){
            controlledCooldown = 10;
            timerSlider = InternalCalls.InstancePrefab("TimerP", gameObject.transform.globalPosition);
            timerSlider.GetComponent<TimerSlider>().getGa(gameObject, controlledCooldown, (int)enemyType, "controlledCooldown");
        }
        
    }

    private void Shoot()
    {
        audioComponent.SetState("MUSIC", "LEVEL1_BATTLE");

        if (canShoot)
        {
            //TODO_AUDIO
            audioComponent.PlayClip("EBASIC_SHOTGUN");
            animationComponent.PlayAnimation("Shoot");
            canShoot = false;
            shootCooldown = 1f;
            Vector3 pos = gameObject.transform.globalPosition;
            pos.y += 0.5f;
            
            GameObject bullet = InternalCalls.InstancePrefab("EnemyBullet", pos);
            bullet.GetComponent<Rigidbody>().IgnoreCollision(gameObject, true);
            EnemyBullet enemyBullet = bullet.GetComponent<EnemyBullet>();
            enemyBullet.enemy = gameObject;
            enemyBullet.index = index;
            enemyBullet.offset = offset;
        }
    }

    public int GetIndex()
    {
        return index;
    }

    public void SetPendingToDelete()
    {
        pendingToDelete = true;
    }

    public void GotoNextPoint()
    {
        gameObject.GetComponent<AudioSource>().PlayClip("EBASIC_WALKSAND");
        animationComponent.PlayAnimation("Walk");
        agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
        destPoint = (destPoint + 1) % waypoints.Count;
    }

    public void Patrol()
    {
        if (agents.MovePath())
        {
            stopState = true;
        }

        if (stopState)
        {
            if (stoppedTime >= 0)
            {
                audioComponent.StopCurrentClip("EBASIC_WALKSAND");
                stoppedTime -= Time.deltaTime;
                if (stoppedTime < 0)
                {
                    stoppedTime = 0f;
                    stopState = false;
                    if (waypoints.Count != 0)
                    {
                        patrol = true;
                        GotoNextPoint();
                    }
                }
            }
        }

        if (agents.MovePath() && waypoints.Count != 0 && patrol && !stopState)
        {
            GotoNextPoint();
        }
    }

    public void Distraction(Vector3 distractionItem)
    {
        Vector3 newForward = (distractionItem - gameObject.transform.globalPosition).normalized;

        double angle = Math.Atan2(newForward.x, newForward.z);

        Quaternion newRot = new Quaternion(0, (float)(1 * Math.Sin(angle / 2)), 0, (float)Math.Cos(angle / 2));

        rb.SetBodyRotation(newRot);

        animationComponent.PlayAnimation("Idle");
    }

    public void Stun(float timeStunned)
    {
        stunned = true;
        stunnedTimer = timeStunned;
        animationComponent.PlayAnimation("Stun");
        timerSlider = InternalCalls.InstancePrefab("TimerP", gameObject.transform.globalPosition);
        timerSlider.GetComponent<TimerSlider>().getGa(gameObject, stunnedTimer, (int)enemyType, "stunnedTimer");
    }
}