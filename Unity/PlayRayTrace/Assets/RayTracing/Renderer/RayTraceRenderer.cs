using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Util;

namespace RayTracing
{
    public abstract class RayTraceRenderer : IDrawGizmo
    {
        public abstract Material Material { get; }
        
        public abstract bool Hit(Ray ray, List<RayTraceRenderer> rendererList, 
            out HitInfo hitInfo);

        public abstract void DrawGizmo();
    }

}
