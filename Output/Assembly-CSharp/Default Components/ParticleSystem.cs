using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class ParticleSystem : RagnarComponent
    {
        public ParticleSystem(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.PARTICLE_SYSTEM;
        }

        public extern Emitter[] emitters
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

    }
}
