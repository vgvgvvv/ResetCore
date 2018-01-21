using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/Contrast")]
    public class Contrast : ImageEffectBase
    {

        [Range(0, 5f)]
        public float DBD = 1f;


        public void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            material.SetFloat("_DBD", DBD);
            Graphics.Blit(source, destination, material);
        }

    }
}
