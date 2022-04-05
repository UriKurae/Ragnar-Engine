using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    class UICheckbox : RagnarComponent
    {
        public UICheckbox(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.UI_CHECKBOX;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool GetIsChecked();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetCheckboxState();
    }
}
