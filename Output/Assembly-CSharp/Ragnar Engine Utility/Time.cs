﻿using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace RagnarEngine
{
    public class Time
    {
        public static extern float deltaTime
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public static extern float timeScale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
    }
}
