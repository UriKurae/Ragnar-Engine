using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public sealed class GameObject
    {
        private string goName;
        private UIntPtr pointer;

        List<RagnarComponent> components;

        private Transform trans;
        //public AudioSource audioSource;
        //public Rigidbody rigidBody;
        //public Animation animation;
        //public Camera camera;

        public GameObject()
        {
            goName = "Empty";
            pointer = UIntPtr.Zero;
        }
        public GameObject(string name, UIntPtr ptr, UIntPtr transPTR/*, UIntPtr audioPTR, UIntPtr rbPTR, UIntPtr animPTR, UIntPtr camPTR*/)
        {
            goName = name;
            pointer = ptr;

            trans = new Transform
            {
                pointer = transPTR
            };

            components = new List<RagnarComponent>();
            components.Add(trans);

            //audioSource = new AudioSource();
            //audioSource.pointer = audioPTR;
            //
            //rigidBody = new Rigidbody();
            //rigidBody.pointer = rbPTR;
            //
            //animation = new Animation();
            //animation.pointer = animPTR;
            //
            //camera = new Camera();
            //camera.pointer = camPTR;
        }

        public T[] GetComponents<T>() where T : RagnarComponent
        {
            ComponentType type = ComponentType.SCRIPT;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
            {
                type = RagnarComponent.componentTable[typeof(T)];
            }
            return TryGetComponents<T>(typeof(T).ToString(), (int)type);
        }

        public T GetComponent<T>() where T : RagnarComponent
        {
            ComponentType type = ComponentType.SCRIPT;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
            {
                type = RagnarComponent.componentTable[typeof(T)];
            }
            return TryGetComponent<T>(typeof(T).ToString(), (int)type);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T TryGetComponent<T>(string type, int inputType = 0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T[] TryGetComponents<T>(string type, int inputType = 0);

        public T CreateComponent<T>() where T : RagnarComponent
        {
            int type = -1;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
                type = (int)RagnarComponent.componentTable[typeof(T)];

            T comp = AddComponent<T>(type);
            if (components.Contains(comp) == false)
                components.Add(comp);

            return comp;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T AddComponent<T>(int componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern public GameObject Find(string name);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //static extern public GameObject FindGameObjectWithTag(string tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern public GameObject[] FindGameObjectsWithTag(string tag);

        public extern GameObject[] childs
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public Transform transform
        {
            get { return trans; }
        }

        public extern string tag
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern string name
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern bool isActive
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Vector3 GetSizeAABB();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Vector3 GetMinAABB();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Vector3 GetMaxAABB();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public void SetSizeAABB(Vector3 min, Vector3 max);
    }
}
