using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class NavAgent : RagnarComponent
    {
        NavAgent(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.NAVAGENT;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void CalculatePath(Vector3 destination);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool SetPath(Vector3[] wayPoints);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool MoveTo(Vector3 destination);

        // Returns true if a target is setted
        public extern bool targetSetted
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public extern Vector3 destination
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
    }
}
