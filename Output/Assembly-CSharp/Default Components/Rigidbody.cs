using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Rigidbody : RagnarComponent
    {
        //Change type depending of class
        public Rigidbody(UIntPtr ptr) : base(ptr)
        {
            //pointer = ptr;
            type = ComponentType.RIGID_BODY;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ApplyCentralForce(Vector3 force);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ApplyCentralImpulse(Vector3 impulse);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ApplyTorque(Vector3 torque);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ClearForces();

        public void IgnoreCollision(GameObject other, bool value)
        {
            Rigidbody rb = other.GetComponent<Rigidbody>();
            if (rb != null)
                SetIgnoreCollision(rb, value);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private void SetIgnoreCollision(Rigidbody other, bool value);

        public extern Vector3 linearVelocity
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
        public extern Vector3 totalForce
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
    }
}
