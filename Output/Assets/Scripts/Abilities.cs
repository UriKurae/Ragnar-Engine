using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{
    
    public bool canThrowKnife = true;

    public void Start()
    {
       
    }

    public void Update()
    {
        ///////// ABILITIES /////////
        // Rock Throw
        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN && GameObject.Find("Rock") == null)
        {
            ThrowRock();
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN && canThrowKnife)
        {
            ThrowKnife();
        }
        /////////////////////////////

        

        ReloadKnife();
    }

    private void ThrowKnife()
    {
        canThrowKnife = false;
        Vector3 pos = gameObject.transform.globalPosition;
        pos.y += 1;
        pos.z += 1;
        GameObject knife = InternalCalls.Create3DObject("Knife", (int)PrimitiveType.CUBE, pos);
        knife.tag = "Knife";
        knife.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

        Rigidbody knifeRb = knife.CreateComponent<Rigidbody>();
        knifeRb.IgnoreCollision(gameObject, true);
        knifeRb.ApplyCentralForce(gameObject.transform.forward * 1000);
    }

    private void ThrowRock()
    {
        InternalCalls.InstancePrefab("Assets/Prefabs/Rock.rgprefab");
    }

    // With this method we pick up the knife by being close to it
    private void ReloadKnife()
    {
        GameObject knife = GameObject.Find("Knife");
        if (knife != null)
        {
            float xDiff = knife.transform.globalPosition.x - gameObject.transform.globalPosition.x;
            float zDiff = knife.transform.globalPosition.z - gameObject.transform.globalPosition.z;
            double distance = Math.Sqrt(zDiff * zDiff + xDiff * xDiff);

            if (distance < 3) Debug.Log(distance.ToString());
            if (distance < 0.6f)
            {
                InternalCalls.Destroy(knife);
                knife = null;
                canThrowKnife = true;
            }
        }
    }
    public void OnTriggerEnter(Rigidbody other)
    {
        Debug.Log("OnTriggerEnter");
    }
    public void OnTrigger(Rigidbody other)
    {
        Debug.Log("OnTrigger");
    }
    public void OnCollisionEnter(Rigidbody other)
    {
        Debug.Log("OnCollisionEnter");
    }
    public void OnCollision(Rigidbody other)
    {
        Debug.Log("OnCollision");
    }
}