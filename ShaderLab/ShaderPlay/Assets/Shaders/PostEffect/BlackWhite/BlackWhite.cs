using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    [ImageEffectShader("Hidden/blackWhite")]
    public class BlackWhite : ImageEffectBase
    {

        //
        void OnRenderImage(RenderTexture source, RenderTexture dest)
        {
            if (material == null)
            {
                Graphics.Blit(source, dest);
                return;
            }

            material.SetTexture("_MainTex", source);
            material.SetFloat("_iHeight", 1);
            material.SetFloat("_iWidth", 1);

            Graphics.Blit(source, dest, material);
        }
    }


}
