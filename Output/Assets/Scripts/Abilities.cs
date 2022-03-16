using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{
    public float force = 100;
    public float rockSoundRadius = 4f;
    public void Update()
	{
        ///////// ABILITIES /////////
        // Rock Throw
        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN)
        {
            ThrowRock();
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN)
        {
            ThrowKnife();
        }
        /////////////////////////////
    }

    private void ThrowKnife()
    {
        Vector3 pos = gameObject.transform.globalPosition;
        pos.y += 1;
        GameObject bullet = InternalCalls.Create3DObject("Knife", (int)PrimitiveType.CUBE, pos);
        bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

        Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
        bulletRb.IgnoreCollision(gameObject, true);
        bulletRb.ApplyCentralForce(gameObject.transform.forward * 1000);
    }

    private void ThrowRock()
    {
        Vector3 pos = gameObject.transform.globalPosition;
        pos.y += 1;
        GameObject bullet = InternalCalls.Create3DObject("Bullet", (int)PrimitiveType.CUBE, pos);
        bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

        Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
        bulletRb.IgnoreCollision(gameObject, true);
        Vector3 vectorDir = new Vector3(gameObject.transform.forward.x, 1, gameObject.transform.forward.z);
        bulletRb.ApplyCentralForce(vectorDir.normalized * force);

        // Falta if(OnCollision(bullet, floor))
        GameObject soundArea = InternalCalls.CreateGameObject("SoundArea", bullet.transform.globalPosition, bullet.transform.globalRotation);
        Rigidbody soundRb = soundArea.CreateComponent<Rigidbody>();
        soundRb.IgnoreCollision(gameObject, true);
        CreateSphereTrigger(soundRb, rockSoundRadius);
    }

    // With this method we create an spherical Trigger.
    private static void CreateSphereTrigger(Rigidbody rb, float radius)
    {
        rb.SetCollisionType(CollisionType.SPHERE);
        rb.SetAsStatic();
        rb.SetAsTrigger();
        rb.SetSphereRadius(radius);
    }
}