using System;

using RagnarEngine;

public class EnemyPatrol : RagnarComponent
{
    public int velocity = 1000;

    public NavAgent agents;
    public GameObject[] waypoints;
    private int destPoint = 0;

    // States
    public bool patrol;
    public bool stopState = false;

    // Timers
    public float stoppedTime = 0f;

    public void Start()
    {
        agents = gameObject.GetComponent<NavAgent>();
        waypoints = GameObject.FindGameObjectsWithTag("Waypoints");
        gameObject.GetComponent<Animation>().PlayAnimation("Idle");
        GotoNextPoint();
    }

    public void Update()
    {
        if (GameObject.Find("Rock") == null && agents.MovePath())
        {
            stopState = true;
        }

        if (stopState)
        {
            if (stoppedTime >= 0)
            {
                gameObject.GetComponent<AudioSource>().StopCurrentClip("FOOTSTEPS");
                stoppedTime -= Time.deltaTime;
                if (stoppedTime < 0)
                {
                    stoppedTime = 0f;
                    stopState = false;
                    patrol = true;
                    GotoNextPoint();
                }
            }
        }

        if (agents.MovePath() && patrol && !stopState)
        {
            GotoNextPoint();
        }
    }

    void GotoNextPoint()
    {
        gameObject.GetComponent<AudioSource>().PlayClip("FOOTSTEPS");
        gameObject.GetComponent<Animation>().PlayAnimation("Walk");
        agents.CalculatePath(waypoints[destPoint].transform.globalPosition);
        destPoint = (destPoint + 1) % waypoints.Length;
    }
    public void OnTrigger(Rigidbody other)
    {
        if (other.gameObject.name == "Rock")
        {
            patrol = false;
            stoppedTime = 2f;
            agents.CalculatePath(other.gameObject.transform.globalPosition);
        }
    }
}