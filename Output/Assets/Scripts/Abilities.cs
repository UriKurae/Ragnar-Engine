using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{
    public float force = 200;
    public float rockSoundRadius = 4f;
    public bool canThrowKnife = true;
    private GameObject bullet;
    private GameObject soundArea;
    private bool createSoundArea = true;
    private float timer = 0f;

    public void Start()
    {

    }

    public void Update()
	{
        ///////// ABILITIES /////////
        // Rock Throw
        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN && timer == 0f)
        {
            ThrowRock();
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN && canThrowKnife)
        {
            ThrowKnife();
        }
        /////////////////////////////

        if (bullet != null && bullet.transform.globalPosition.y <= 0.56f && createSoundArea)
        {
            createSoundArea = false;
            soundArea = InternalCalls.CreateGameObject("SoundArea", bullet.transform.globalPosition, bullet.transform.globalRotation);
            Rigidbody soundRb = soundArea.CreateComponent<Rigidbody>();
            soundRb.IgnoreCollision(gameObject, true);
            CreateSphereTrigger(soundRb, rockSoundRadius);
        }
        if(bullet != null && !createSoundArea)
        {
            timer -= Time.deltaTime;
            if(timer < 0)
            {
                timer = 0f;
                InternalCalls.Destroy(bullet);
                bullet = null;
                InternalCalls.Destroy(soundArea);
                soundArea = null;
            }
        }
    }

    private void ThrowKnife()
    {
        canThrowKnife = false;
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
        createSoundArea = true;

        Vector3 pos = gameObject.transform.globalPosition;
        pos.y += 1;
        bullet = InternalCalls.Create3DObject("Rock", (int)PrimitiveType.CUBE, pos);
        bullet.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

        Rigidbody bulletRb = bullet.CreateComponent<Rigidbody>();
        bulletRb.IgnoreCollision(gameObject, true);
        Vector3 vectorDir = new Vector3(gameObject.transform.forward.x, 1, gameObject.transform.forward.z);
        bulletRb.ApplyCentralForce(vectorDir.normalized * force);

        timer = 2f;
    }

    // With this method we create an spherical Trigger.
    private static void CreateSphereTrigger(Rigidbody rb, float radius)
    {
        rb.SetCollisionType(CollisionType.SPHERE);
        rb.SetAsStatic();
        rb.SetAsTrigger();
        rb.SetSphereRadius(radius);
    }

    private void ReloadKnife()
    {
        if (!canThrowKnife) canThrowKnife = true;
    }
}