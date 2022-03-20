using System;
using RagnarEngine;

public class Abilities : RagnarComponent
{
    public float force = 200;
    public float rockSoundRadius = 6f;
    public bool canThrowKnife = true;
    private GameObject rock;
    private GameObject soundArea;
    private bool createSoundArea = true;
    private float rockCooldown = 0f;

    public void Start()
    {

    }

    public void Update()
    {
        ///////// ABILITIES /////////
        // Rock Throw
        if (Input.GetKey(KeyCode.F1) == KeyState.KEY_DOWN && rockCooldown == 0f)
        {
            ThrowRock();
        }

        // Knife Throw
        if (Input.GetKey(KeyCode.F2) == KeyState.KEY_DOWN && canThrowKnife)
        {
            Debug.Log("Entra en ThrowKnife");
            ThrowKnife();
        }
        /////////////////////////////

        // Rock sound area creation if rock hits the ground
        // When the rockCooldown goes off, the rock and the area dies and the player restores the ability to throw another rock (Cooldown).
        // This shouldn't be here. The right way is to do this with a prefab instantiation.
        SoundAreaCreationAndDestruction();

        // This shouldn't be here. The right way is to do this with a prefab instantiation.
        // This may cause lag if we repeat this process with every bullet or thing we create.
        // Every reload or bullet/ammo restore should work by prefab scripting.
        ReloadKnife();
    }

    private void SoundAreaCreationAndDestruction()
    {
        if (rock != null && rock.transform.globalPosition.y <= 0.56f && createSoundArea)
        {
            createSoundArea = false;
            Vector3 pos = rock.transform.globalPosition;
            soundArea = InternalCalls.CreateGameObject("SoundArea", pos, rock.transform.globalRotation);
            Rigidbody soundRb = soundArea.CreateComponent<Rigidbody>();
            soundRb.IgnoreCollision(gameObject, true);
            Debug.Log(rock.transform.globalPosition.x.ToString());
            Debug.Log(rock.transform.globalPosition.y.ToString());
            Debug.Log(rock.transform.globalPosition.z.ToString());
            CreateSphereTrigger(soundRb, rockSoundRadius, rock.transform.globalPosition);
        }
        if (rock != null && !createSoundArea)
        {
            rockCooldown -= Time.deltaTime;
            if (rockCooldown < 0)
            {
                rockCooldown = 0f;
                InternalCalls.Destroy(rock);
                rock = null;
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
        createSoundArea = true;

        Vector3 pos = gameObject.transform.globalPosition;
        pos.y += 1;
        rock = InternalCalls.Create3DObject("Rock", (int)PrimitiveType.CUBE, pos);
        rock.transform.scale = new Vector3(0.2f, 0.2f, 0.2f);

        Rigidbody rockRb = rock.CreateComponent<Rigidbody>();
        rockRb.IgnoreCollision(gameObject, true);
        Vector3 vectorDir = new Vector3(gameObject.transform.forward.x, 1, gameObject.transform.forward.z);
        rockRb.ApplyCentralForce(vectorDir.normalized * force);

        rockCooldown = 2f;
    }

    // With this method we create an spherical Trigger.
    private static void CreateSphereTrigger(Rigidbody rb, float radius, Vector3 pos)
    {
        Debug.Log(pos.x.ToString());
        Debug.Log(pos.y.ToString());
        Debug.Log(pos.z.ToString());
        rb.SetCollisionSphere(radius, pos.x, pos.y, pos.z);
        rb.SetAsTrigger();
    }

    // With this method we pick up the knife by being close to it
    private void ReloadKnife()
    {
        GameObject knife = GameObject.Find("Knife");
        if (knife != null)
        {
            // Calcula diferencia de posición (vectores) y haz magnitud entonces.
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
}