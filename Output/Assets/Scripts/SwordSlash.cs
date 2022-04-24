using System;
using RagnarEngine;

public class SwordSlash : RagnarComponent
{
	public float timeAlive = 0f;
	public bool pendingToDelete = false;
	public void Start()
    {
        timeAlive = 2f;

        Vector3 pos = GameObject.Find("Player").transform.globalPosition;
        pos.y += 1;
        gameObject.transform.localPosition = pos;

        gameObject.transform.forward.Set(GameObject.Find("Player").transform.forward.x, GameObject.Find("Player").transform.forward.y, GameObject.Find("Player").transform.forward.z);

        Rigidbody rb = gameObject.GetComponent<Rigidbody>();
        rb.SetBodyPosition(pos);
        rb.IgnoreCollision(GameObject.Find("Player"), true);
        rb.SetAsTrigger();
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