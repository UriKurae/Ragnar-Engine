using System;
using RagnarEngine;

public class Camouflage : RagnarComponent
{
	public GameObject player;
	public void Start()
	{
		player = GameObject.Find("Player_2");
	}
	public void Update()
	{
		SetMaterialTransparent();
	}
	private void SetMaterialTransparent()
    {
		player.GetComponent<Material>().SetTexturePath("Assets/Resources/UI/transparent_tex.png");
    }

}