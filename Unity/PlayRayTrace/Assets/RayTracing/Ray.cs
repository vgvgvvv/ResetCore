using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RayTracing
{
    public struct Ray
    {
        public Vector3 Origin;
        public Vector3 Direction;
        public Vector3 NormalizedDirection;

        public Ray(Vector3 o, Vector3 d)
        {
            Origin = o;
            Direction = d;
            NormalizedDirection = d.normalized;
        }

    }

    public class RayCast
    {
        //3DMath Book p272
        public static bool RayCastSphere(Ray ray, 
            Vector3 spherePos, float radius, out Vector3 position)
        {
            var e = spherePos - ray.Origin;
            var a = Vector3.Project(e, ray.Direction).magnitude;
            var f2 = radius * radius - Vector3.Dot(e, e) + a * a;
            if (f2 < 0)
            {
                position = Vector3.zero;
                return false;
            }

            var f = Mathf.Sqrt(f2);
            var t1 = a - f;
            if (a < f)
            {
                t1 = a + f;
            }

            position = ray.Origin + t1 * ray.NormalizedDirection;


            return true;
        }
    }

}
