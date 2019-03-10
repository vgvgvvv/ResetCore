using UnityEngine;

namespace RayTracing
{
    public struct HitInfo
    {
        public Color color;
        public Vector3 hitPosition;
        public float distance;
        public Vector3 normal;
    }
}