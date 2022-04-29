using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Emitter
    {
        UIntPtr pointer;
        ParticleSystem psOwner;
        public Emitter(UIntPtr ptr, UIntPtr ownerPtr)
        {
            pointer = ptr;
            psOwner = new ParticleSystem(ownerPtr);
        }

        public ParticleSystem owner
        {
            get { return psOwner; }
        }
    }
}
