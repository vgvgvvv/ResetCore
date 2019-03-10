using System.Collections.Generic;
using UnityEngine;

namespace RayTracing
{
    public class RayTracer
    {
        public static void Trace(Ray ray, List<RayTraceRenderer> renderers, out HitInfo hitInfo, int level)
        {
            hitInfo = new HitInfo()
            {
                color = Color.black,
                distance = float.MaxValue
            };
            foreach (var renderer in renderers)
            {
                if (renderer.Hit(ray, renderers, out var tempHitInfo))
                {
                    if (tempHitInfo.distance < hitInfo.distance)
                    {
                        hitInfo = tempHitInfo;
                    }
                }
            }
        }
    }
}