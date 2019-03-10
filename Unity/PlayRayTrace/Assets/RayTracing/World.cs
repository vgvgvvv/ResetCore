using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RayTracing
{
    public class World
    {
        public Camera Camera = new Camera();

        private readonly List<RayTraceRenderer> _renderers = new List<RayTraceRenderer>()
        {
            new Ball(new Vector3(0, 0, 8), 1),
        };

        public Color[] Trace()
        {
            Color[] colors = new Color[Camera.RenderWidth * Camera.RenderHeight];
            var height = Camera.RenderHeight;
            var width = Camera.RenderWidth;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    var heightPer = (float)i / Camera.RenderHeight;
                    var widthPer = (float)j / Camera.RenderWidth;
                    Ray ray = new Ray(Camera.Position,
                        Camera.Position + 
                        Camera.Forward * Camera.FarDistance - 
                        Camera.Right * Camera.Width / 2 +
                        Camera.Right * widthPer * Camera.Width - 
                        Camera.Up * Camera.Height / 2 +
                        Camera.Up * heightPer * Camera.Height - Camera.Position);

                    bool hited = false;
                    foreach (var renderer in _renderers)
                    {
                        renderer.Hit(ray, _renderers, out var hitInfo);
                        colors[j + i * width] = hitInfo.color;
                    }

                    if (!hited)
                    {
                        colors[j + i * width] = DefaultColor(ray);
                    }
                }
            }

            return colors;
        }

        public void DrawGizmos()
        {
            foreach (var renderer in _renderers)
            { 
                Camera.DrawGizmo();
                renderer.DrawGizmo();
            }
        }

        private Color DefaultColor(Ray r)
        {
            Vector3 dir = r.NormalizedDirection;
            float t = 0.5f * (dir.y + 1.0f);
            return (1.0f - t) * new Color(1f, 1f, 1f) + 
                   t * new Color(0.5f, 0.7f, 1.0f);
        }
    }

}
