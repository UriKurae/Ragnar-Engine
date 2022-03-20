using System;

using RagnarEngine;

public class EnemyPatrol : RagnarComponent
{
	public int velocity = 1000;

	public NavAgent agents;
    public GameObject[] waypoints;
    private int destPoint = 0;

    public void Start()
	{
		agents = gameObject.GetComponent<NavAgent>();
        waypoints = GameObject.FindGameObjectsWithTag("Waypoints");
        GotoNextPoint();
    }
	public void Update()
	{
        if(agents.MovePath())
        {
            GotoNextPoint();
        }
    }
    void GotoNextPoint()
    {
        agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
        destPoint = (destPoint + 1) % waypoints.Length;
    }

}