using System;
using RagnarEngine;

public class Eagle : RagnarComponent
{
	public GameObject player;
	public PlayerManager playerManager;
    public bool controled = false;
    Rigidbody goRB;
    public float cooldown;
    public float soundRadius = 6f;

    NavAgent agent;
    public void Start()
	{
        agent = gameObject.GetComponent<NavAgent>();
        controled = true;
        player = GameObject.Find("Player");
        goRB = gameObject.GetComponent<Rigidbody>();
        goRB.SetBodyPosition(player.transform.globalPosition);
        goRB.IgnoreCollision(player, true);
        this.gameObject.GetComponent<Player>().SetControled(true);
        agent.CalculatePath(agent.hitPosition);
        agent.MovePath();
    }
	public void Update()
	{
        if (cooldown > 0 && gameObject != null)
        {
            cooldown -= Time.deltaTime;
            if (cooldown < 0)
            {
                cooldown = 0f;
                InternalCalls.Destroy(gameObject);
            }
        }
    }
    public void OnCollisionEnter(Rigidbody other)
    {
        Rigidbody area = gameObject.CreateComponent<Rigidbody>();
        CreateSphereTrigger(area, soundRadius, gameObject.transform.globalPosition);
        cooldown = 6f;
    }

    private static void CreateSphereTrigger(Rigidbody rb, float radius, Vector3 pos)
    {
        rb.SetCollisionSphere(radius, pos.x, pos.y, pos.z);
        rb.SetAsTrigger();
    }

}