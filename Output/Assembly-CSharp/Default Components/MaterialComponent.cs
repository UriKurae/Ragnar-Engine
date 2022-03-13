using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class MaterialComponent : RagnarComponent
    {
        public MaterialComponent(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.MATERIAL;
        }

        public extern string texture
        {
            //[MethodImplAttribute(MethodImplOptions.InternalCall)]
            //get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
    }

}