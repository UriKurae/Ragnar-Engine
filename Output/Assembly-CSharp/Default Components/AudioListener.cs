using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class AudioListener : RagnarComponent
    {
        public AudioListener()
        {
            type = ComponentType.AUDIO_LISTENER;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void TestListener();
    }
}
