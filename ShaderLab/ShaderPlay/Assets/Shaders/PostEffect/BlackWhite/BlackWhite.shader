﻿Shader "Hidden/blackWhite" {
	Properties {
		_MainTex ("Input", 2D) = "white" {}
		_BlurStrength ("str", Float) = 0.5
		_BlurWidth ("width", Float) = 0.5
	}

    SubShader {
        Pass {
            ZTest Always Cull Off ZWrite Off
            Fog { Mode off }
       
		CGPROGRAM
		// Upgrade NOTE: excluded shader from DX11, Xbox360, OpenGL ES 2.0 because it uses unsized arrays
		//#pragma exclude_renderers d3d11 xbox360 gles
   
		#pragma vertex vert_img
		#pragma fragment frag
		#pragma fragmentoption ARB_precision_hint_fastest
 
		#include "UnityCG.cginc"
 
		uniform sampler2D _MainTex;
		uniform half _BlurStrength;
		uniform half _BlurWidth;
		uniform half _iWidth;
		uniform half _iHeight;
 
		half4 frag (v2f_img i) : COLOR {
			half4 color = tex2D(_MainTex, i.uv);
       
			float grey = Luminance(color.rgb);
			color.rgb = float3(grey, grey, grey);

			return color;
		}
		ENDCG
        }
    }
}