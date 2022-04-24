using System;
using RagnarEngine;

public class SwordSlash : RagnarComponent
{
	public float timeAlive = 0f;
	public bool pendingToDelete = false;
	public void Start()
    {
        timeAlive = 2f;
        Rigidbody rb = gameObject.GetComponent<Rigidbody>();
        rb.IgnoreCollision(GameObject.Find("Player_3"), true);
        rb.SetBodyPosition(GameObject.Find("Player_3").transform.globalPosition);

        rb.SetBodyRotation(AngleSlash());
    }

    private Quaternion AngleSlash()
    {
        Vector3 newForward = (GameObject.Find("Player_3").transform.globalPosition - gameObject.transform.globalPosition).normalized;
        double angle = Math.Atan2(newForward.x, newForward.z);
        Quaternion newRot = new Quaternion(0, (float)(1 * Math.Sin(angle / 2)), 0, (float)Math.Cos(angle / 2));

        return newRot;
    }

    public void Update()
	{
		if(timeAlive > 0 && gameObject != null)
        {
			timeAlive -= Time.deltaTime;
			if(timeAlive < 0)
            {
				timeAlive = 0f;
				pendingToDelete = true;
            }
        }
		if (pendingToDelete) InternalCalls.Destroy(gameObject);
	}

}