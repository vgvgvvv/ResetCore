using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Util;

namespace RayTracing
{
    public class Camera : IDrawGizmo
    {
        public int RenderWidth = 200;
        public int RenderHeight = 100;

        public Vector3 Position = Vector3.zero;
        public float Fov = 60;
        public float FarDistance = 2;
        public float Width = 2;
        public float Height = 1;

        public Vector3 Forward = Vector3.forward;
        public Vector3 Right = Vector3.right;
        public Vector3 Up = Vector3.up;

        public void DrawGizmo()
        {
            Gizmos.color = Color.red;
            var lu = Position +
                     Forward * FarDistance -
                     Right * Width / 2 +
                     Right * 0 * Width -
                     Up * Height / 2 +
                     Up * 1 * Height;
            var ld = Position +
                     Forward * FarDistance -
                     Right * Width / 2 +
                     Right * 0 * Width -
                     Up * Height / 2 +
                     Up * 0 * Height;
            var ru = Position +
                     Forward * FarDistance -
                     Right * Width / 2 +
                     Right * 1 * Width -
                     Up * Height / 2 +
                     Up * 1 * Height;
            var rd = Position +
                     Forward * FarDistance -
                     Right * Width / 2 +
                     Right * 1 * Width -
                     Up * Height / 2 +
                     Up * 0 * Height;
            Gizmos.DrawLine(Position, lu);
            Gizmos.DrawLine(Position, ld);
            Gizmos.DrawLine(Position, ru);
            Gizmos.DrawLine(Position, rd);

            Gizmos.DrawLine(ld, lu);
            Gizmos.DrawLine(lu, ru);
            Gizmos.DrawLine(ru, rd);
            Gizmos.DrawLine(rd, ld);

            Gizmos.color = Color.white;
        }
    }

}
