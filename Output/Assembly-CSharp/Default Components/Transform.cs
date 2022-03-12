using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Transform : RagnarComponent
    {
        //Change type depending of class
        public Transform()
        {
            type = ComponentType.TRANSFORM;
        }

        public extern Vector3 localPosition
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern Vector3 globalPosition
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public extern Quaternion localRotation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern Quaternion globalRotation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public extern Vector3 localScale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern Vector3 globalScale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public extern Vector3 forward
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public extern Vector3 right
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
        public extern Vector3 up
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

    }
}