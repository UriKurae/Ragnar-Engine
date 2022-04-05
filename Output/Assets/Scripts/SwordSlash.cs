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
		rb.SetBodyPosition(GameObject.Find("Player_3").transform.globalPosition);
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