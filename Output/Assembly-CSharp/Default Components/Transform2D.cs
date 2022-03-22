using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    class Transform2D : RagnarComponent
    {
        public Transform2D(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.TRANFORM2D;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void UIFunctionTransform2D();

        public extern Vector3 position2D
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetSize();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void SetSize(Vector3 bounds);
    }
}
