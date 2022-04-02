using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    class UIButton : RagnarComponent
    {
        public UIButton(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.UI_BUTTON;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 UIFunctionButton();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetButtonState();

        
        public extern string text
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
    }
}
