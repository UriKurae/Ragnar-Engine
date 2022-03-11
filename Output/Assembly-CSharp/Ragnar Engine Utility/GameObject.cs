using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{

    public sealed class GameObject
    {
        public string name;
        public UIntPtr pointer;

        List<RagnarComponent> components;

        public Transform transform;
        public AudioSource audioSource;
        public Rigidbody rigidBody;
        public Animation animation;
        public Camera camera;
        
        public GameObject()
        {
            name = "Empty";
            pointer = UIntPtr.Zero;
            //InternalCalls.CreateGameObject(name, Vector3.zero);
        }
        public GameObject(string _name, UIntPtr ptr, UIntPtr transPTR/*, UIntPtr audioPTR, UIntPtr rbPTR, UIntPtr animPTR, UIntPtr camPTR*/)
        {
            name = _name;
            pointer = ptr;

            transform = new Transform();
            transform.pointer = transPTR;

            components = new List<RagnarComponent>();
            components.Add(transform);

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


        public T GetComponent<T>() where T : RagnarComponent
        {
            //ComponentType type = T.get;
            ComponentType retValue = ComponentType.SCRIPT;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
            {
                retValue = RagnarComponent.componentTable[typeof(T)];
            }
            return TryGetComponent<T>(typeof(T).ToString(), (int)retValue); // TODO: Very temporary. FIND A BETTER WAY TO DO THIS
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T TryGetComponent<T>(string type, int inputType = 0);

        public T CreateComponent<T>() where T : RagnarComponent
        {
            int type = -1;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
                type = (int)RagnarComponent.componentTable[typeof(T)];

            T comp = AddComponent<T>(type);
            if(components.Contains(comp) == false)
                components.Add(comp);
            
            return comp;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T AddComponent<T>(int componentType);

        //public void CreateComponent(ComponentType type)
        //{
        //    AddComponent((int)type - 1); // TODO: Very temporary. FIND A BETTER WAY TO DO THIS
        //}
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern void AddComponent(int componentType);
    }
}
