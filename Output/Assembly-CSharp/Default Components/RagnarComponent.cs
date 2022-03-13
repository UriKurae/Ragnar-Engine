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
        UI_BUTTON,
        UI_IMAGE,
        UI_CHECKBOX,
        UI_SLIDER,
        UI_INPUTBOX,
        UI_CANVAS,
        TRANFORM2D,
        UI_TEXT,
    }

    public class RagnarComponent
    {
        public UIntPtr pointer;
        public ComponentType type;

        public static Dictionary<System.Type, ComponentType> componentTable = new Dictionary<Type, ComponentType>
        {
            { typeof(Transform), ComponentType.TRANSFORM },
            { typeof(MaterialComponent), ComponentType.MATERIAL},
            { typeof(AudioSource), ComponentType.AUDIO_SOURCE },
            { typeof(Rigidbody), ComponentType.RIGID_BODY},
            { typeof(Animation), ComponentType.ANIMATION},
            { typeof(Camera), ComponentType.CAMERA},
            { typeof(AudioListener), ComponentType.AUDIO_LISTENER},
            { typeof(Transform2D), ComponentType.TRANFORM2D},
            { typeof(UIButton), ComponentType.UI_BUTTON},
            { typeof(UICheckbox), ComponentType.UI_CHECKBOX},
            { typeof(UISlider), ComponentType.UI_SLIDER},
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
