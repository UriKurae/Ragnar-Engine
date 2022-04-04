using System;
using RagnarEngine;

public class Camouflage : RagnarComponent
{
	public void Start()
	{

	}
	public void Update()
	{
		ChooseMesh();
	}
	private void ChooseMesh()
    {
		this.gameObject.GetComponent<Material>().texture = "Assets/Resources/UI/transparent_tex.png";
    }

}