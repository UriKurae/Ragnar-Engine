using System;
using RagnarEngine;

public class Eagle : RagnarComponent
{
	public GameObject player;
	public PlayerManager playerManager;
    public bool controled = false;

    NavAgent agent;
    public void Start()
	{
        agent = gameObject.GetComponent<NavAgent>();
        controled = true;
        player = GameObject.Find("Player");
        Rigidbody goRB = gameObject.GetComponent<Rigidbody>();
        goRB.SetBodyPosition(player.transform.globalPosition);
        goRB.IgnoreCollision(player, true);
        player.GetComponent<Player>().SetControled(false);
        this.gameObject.GetComponent<Player>().SetControled(true);
    }
	public void Update()
	{
        if (controled)
        {
            Debug.Log("Controlled");
            if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_UP)
            {
                agent.CalculatePath(agent.hitPosition);

            }
            agent.MovePath();
        }
    }

}