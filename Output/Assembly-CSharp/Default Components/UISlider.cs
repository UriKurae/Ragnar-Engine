using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    class UISlider : RagnarComponent
    {
        public UISlider(UIntPtr ptr) : base(ptr)
        {
            type = ComponentType.UI_SLIDER;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void UIFunctionSlider();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern float GetSliderActualValue();
    }
}
