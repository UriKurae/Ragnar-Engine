using System;
using RagnarEngine;

public class Rock : RagnarComponent
{
	public void Start()
	{
		Debug.Log("Start Rock");
		GameObject player = GameObject.Find("Player");
		Vector3 pos = player.transform.globalPosition;
		pos.y += 1;
		gameObject.transform.localPosition = pos;
	}
	public void Update()
	{

	}
	public void OnCollisionEnter(Rigidbody other)
	{
		Debug.Log("OnCollisionEnter");
	}

}