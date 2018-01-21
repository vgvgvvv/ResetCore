using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/Rgb")]
    public class RGBSepration : ImageEffectBase
    {
        [Range(0, 0.05f)]
        public float force = 0.005f;
        public Vector2 direction = new Vector2(1, 1);


        public void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            material.SetFloat("_Force", force);
            material.SetVector("_Direction", direction);
            Graphics.Blit(source, destination, material);
        }
    }


}
