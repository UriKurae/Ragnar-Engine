using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{

    public sealed class GameObject
    {
        public string name;
        public UIntPtr pointer;
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
        public GameObject(string _name, UIntPtr ptr, UIntPtr transPTR, UIntPtr audioPTR, UIntPtr rbPTR, UIntPtr animPTR, UIntPtr camPTR)
        {
            name = _name;
            pointer = ptr;

            transform = new Transform();
            transform.pointer = transPTR;

            audioSource = new AudioSource();
            audioSource.pointer = audioPTR;

            rigidBody = new Rigidbody();
            rigidBody.pointer = rbPTR;
            
            animation = new Animation();
            animation.pointer = animPTR;

            camera = new Camera();
            camera.pointer = camPTR;
        }


        public T GetComponent<T>() where T : RagnarComponent
        {
            //ComponentType type = T.get;
            ComponentType retValue = ComponentType.SCRIPT;
            if (RagnarComponent.componentTable.ContainsKey(typeof(T)))
            {
                retValue = RagnarComponent.componentTable[typeof(T)];
            }
            return TryGetComponent<T>(typeof(T).ToString(), (int)retValue - 1); // TODO: Very temporary. FIND A BETTER WAY TO DO THIS
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T TryGetComponent<T>(string type, int inputType = 0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void AddComponent(int componentType);
    }
}
