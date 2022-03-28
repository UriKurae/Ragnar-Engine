using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    internal class UIImage : RagnarComponent
    {
        public UIImage(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.UI_IMAGE;
        }
        

    }
}
