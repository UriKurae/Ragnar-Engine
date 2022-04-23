using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Light : RagnarComponent
    {
        public Light(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.LIGHT;
        }
        public extern float intensity
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern Vector3 ambient
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
    }
}
