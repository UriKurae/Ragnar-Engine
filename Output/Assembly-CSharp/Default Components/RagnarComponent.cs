using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace RagnarEngine
{
    public enum ComponentType
    {
        NONE = -1,
        TRANSFORM,
        MESH_RENDERER,
        MATERIAL,
        CAMERA,
        SCRIPT,
        AUDIO_SOURCE,
        AUDIO_LISTENER,
        AUDIO_REVERB_ZONE,
        LIGHT,
        ANIMATION,
        BONE,
        RIGID_BODY,
    }

    public class RagnarComponent
    {
        public UIntPtr pointer;
        public ComponentType type;

        public static Dictionary<System.Type, ComponentType> componentTable = new Dictionary<Type, ComponentType>
        {
            { typeof(Transform), ComponentType.TRANSFORM },
            { typeof(AudioSource), ComponentType.AUDIO_SOURCE },
            { typeof(Rigidbody), ComponentType.RIGID_BODY},
            { typeof(Animation), ComponentType.ANIMATION},
            { typeof(Camera), ComponentType.CAMERA},
            { typeof(AudioListener), ComponentType.AUDIO_LISTENER},
        };

        public RagnarComponent()
        {
            this.type = ComponentType.SCRIPT;
        }

        public RagnarComponent(UIntPtr ptr)
        {
            pointer = ptr;
            //this.type = ComponentType.SCRIPT;
        }

        public extern GameObject gameObject
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public ComponentType GetComponentType()
        {
            return type;
        }
    }
}
