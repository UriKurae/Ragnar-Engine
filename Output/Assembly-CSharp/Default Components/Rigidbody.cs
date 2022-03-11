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
        public extern void ClearForces();

        public void IgnoreCollision(GameObject other, bool value)
        {
            Rigidbody rb = other.GetComponent<Rigidbody>();
            if (rb != null)
                SetIgnoreCollision(other, value);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private void SetIgnoreCollision(GameObject go, bool value);

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
