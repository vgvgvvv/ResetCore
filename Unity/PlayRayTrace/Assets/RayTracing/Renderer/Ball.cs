using System.Collections.Generic;
using UnityEngine;

namespace RayTracing
{
    public class Ball : RayTraceRenderer
    {
        public Vector3 Position = Vector3.zero;

        public float Radius = 2;

        public override Material Material { get; } = new Material()
        {
            color = new Color(0.5f, 0.5f, 0.5f)
        };

        public Ball(Vector3 position, float radius)
        {
            Position = position;
            Radius = radius;
        }

        public override bool Hit(Ray ray, List<RayTraceRenderer> renderers, out HitInfo hitInfo)
        {
            if (IsHit(ray))
            {
                hitInfo = new HitInfo()
                {
                    color = Color.red
                };
                return true;
            }
            else
            {
                hitInfo = new HitInfo();
                return false;
            }
            
        }

        public override void DrawGizmo()
        {
            Gizmos.DrawWireSphere(Position, Radius);
        }

        private bool IsHit(Ray ray)
        {
            Vector3 oc = ray.Origin - Position;
            float a = Vector3.Dot(ray.Direction, ray.Direction);
            float b = 2.0f * Vector3.Dot(oc, ray.Direction);
            float c = Vector3.Dot(oc, oc) - Radius * Radius;
            float discriminant = b * b - 4 * a * c;
            return discriminant > 0;
        }

    }
}