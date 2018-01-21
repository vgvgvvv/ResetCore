using UnityEngine;
using System.Collections;
using ResetCore.ShaderPlay;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/Rgb")]
    public class TestPostEffect : ImageEffectBase
    {

        [Range(0, 0.05f)]
        public float force = 0.005f;
        public Vector2 direction = new Vector2(1, 1);


        public void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            if (material == null)
            {
                Graphics.Blit(source, destination);
                return;
            }
        
            material.SetFloat("_Force", force);
            material.SetVector("_Direction", direction);
            Graphics.Blit(source, destination, material);
        }
    }

}
