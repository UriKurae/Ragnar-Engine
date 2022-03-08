using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Rigidbody : RagnarComponent
    {
        //Change type depending of class
        public Rigidbody()
        {
            type = ComponentType.RIGID_BODY;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ApplyCentralForce(Vector3 force);
    }
}
