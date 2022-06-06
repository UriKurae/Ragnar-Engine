using System;
using System.Collections.Generic;
using RagnarEngine;

public class SoundAreaManager : RagnarComponent
{
	public float stablishedTimer = 3f;
	private bool timerDestroy = false;
	private bool pendingDestroy = false;
	private float timer = 0f;

	public void Start()
	{
		Light light = gameObject.childs[0].GetComponent<Light>();

		light.constant = 0.15f;
		light.quadratic = 0f;

		if (gameObject.GetParent().name == SceneManager.currentSceneName)
			timerDestroy = true;
		else
        {
			light.intensity = 0f;
			light.linear = -1f;
		}

	}

    public void Update()
	{
		if (timerDestroy)
		{
			timer += Time.deltaTime;
			if (timer > stablishedTimer)
			{
				gameObject.childs[0].GetComponent<Light>().intensity = 0f;
				pendingDestroy = true;
			}
			else if (pendingDestroy)
				InternalCalls.Destroy(gameObject);
		}
	}

	public void UpdateRadius(float radius)
    {
		gameObject.GetComponent<Rigidbody>().SetRadiusSphere(radius);

		if (radius <= 0)
			gameObject.childs[0].GetComponent<Light>().intensity = 0f;
		else
		{
			gameObject.childs[0].GetComponent<Light>().intensity = 1f;
			gameObject.childs[0].GetComponent<Light>().linear = -1.1f / radius;
		}

	}
}