using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/Saturation")]
    public class Saturation : ImageEffectBase
    {
        [Range(0, 1)]
        public float force = 0;
        public Color color = Color.white;

        public void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            material.SetFloat("_Force", force);
            material.SetColor("_Color", color);
            Graphics.Blit(source, destination, material);
        }
    }
}
