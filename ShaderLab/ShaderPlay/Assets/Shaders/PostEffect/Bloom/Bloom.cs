using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/Bloom")]
    public class Bloom : ImageEffectBase
    {
        [Range(0, 4)] public int iterations = 3;

        [Range(0.2f, 3.0f)] public float blurSpread = 0.6f;

        [Range(1, 8)] public int downSample = 4;

        [Range(0.0f, 4.0f)] public float luminanceThreshold = 0.6f;

        void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            if (material != null)
            {
                material.SetFloat("_LuminanceThreshold", luminanceThreshold);

                int rtW = source.width / downSample;
                int rtH = source.height / downSample;

                RenderTexture buffer0 = RenderTexture.GetTemporary(rtW, rtH, 0);
                buffer0.filterMode = FilterMode.Bilinear;

                Graphics.Blit(source, buffer0, material, 0);

                for (int i = 0; i < iterations; i++)
                {
                    material.SetFloat("_BlurSize", 1.0f + i * blurSpread);

                    RenderTexture buffer1 = RenderTexture.GetTemporary(rtW, rtH, 0);

                    // Render the vertical pass
                    Graphics.Blit(buffer0, buffer1, material, 1);

                    RenderTexture.ReleaseTemporary(buffer0);
                    buffer0 = buffer1;
                    buffer1 = RenderTexture.GetTemporary(rtW, rtH, 0);

                    // Render the horizontal pass
                    Graphics.Blit(buffer0, buffer1, material, 2);

                    RenderTexture.ReleaseTemporary(buffer0);
                    buffer0 = buffer1;
                }

                material.SetTexture("_Bloom", buffer0);
                Graphics.Blit(source, destination, material, 3);

                RenderTexture.ReleaseTemporary(buffer0);
            }
            else
            {
                Graphics.Blit(source, destination);
            }

        }

    }
}
