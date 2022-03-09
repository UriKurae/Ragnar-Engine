using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{ 
    public class Animation : RagnarComponent
    {
        public Animation()
        {
            type = ComponentType.ANIMATION;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void PlayAnimation(string v);

    }
}
