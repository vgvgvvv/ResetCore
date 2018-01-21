using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ExecuteInEditMode]
    [ImageEffectShader("Hidden/radialBlur")]
    public class RadialBlur : ImageEffectBase
    {
        public float blurStrength = 6.0f;
        public float blurWidth = 0.7f;

        new void Start()
        {
            if (!SystemInfo.supportsRenderTextures)
            {
                enabled = false;
                return;
            }
        }

        void OnRenderImage(RenderTexture source, RenderTexture dest)
        {
            // Create the accumulation texture
            //if (accumTexture == null || accumTexture.width != source.width || accumTexture.height != source.height)
            //{
            //    UnityEngine.Object.Destroy(accumTexture);
            //    accumTexture = new RenderTexture(source.width, source.height, 0);
            //    accumTexture.hideFlags = HideFlags.HideAndDontSave;
            //    Graphics.Blit(source, accumTexture);
            //}

            material.SetTexture("_MainTex", source);
            material.SetFloat("_BlurStrength", blurStrength);
            material.SetFloat("_BlurWidth", blurWidth);
            material.SetFloat("_iHeight", 1);
            material.SetFloat("_iWidth", 1);
            //accumTexture.MarkRestoreExpected();

            // Graphics.Blit(source, accumTexture, material);
            // Graphics.Blit(accumTexture, dest);

            Graphics.Blit(source, dest, material);
        }
    }
}
