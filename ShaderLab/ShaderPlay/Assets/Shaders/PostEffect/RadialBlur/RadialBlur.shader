﻿Shader "Hidden/radialBlur" {
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
       
			// some sample positions
			//half samples[10] = half[10](-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08);
       
			//vector to the middle of the screen
			half2 dir = 0.5 * half2(_iHeight,_iWidth) - i.uv;
       
			//distance to center
			half dist = sqrt(dir.x*dir.x + dir.y*dir.y);
       
			//normalize direction
			dir = dir/dist;
       
			//additional samples towards center of screen
			half4 sum = color;
			//for(int n = 3; n < 7; n++)
			//{
			//	sum += tex2D(_MainTex, i.uv + dir * samples[n] * _BlurWidth * _iWidth);
			//}
       
			sum += tex2D(_MainTex, i.uv + dir * -0.02 * _BlurWidth * _iWidth);
			sum += tex2D(_MainTex, i.uv + dir * -0.01 * _BlurWidth * _iWidth);
			sum += tex2D(_MainTex, i.uv + dir * 0.01 * _BlurWidth * _iWidth);
			sum += tex2D(_MainTex, i.uv + dir * 0.02 * _BlurWidth * _iWidth);

			//eleven samples...
			sum *= 1.0/5.0;
       
			//weighten blur depending on distance to screen center
			half t = dist * _BlurStrength / _iWidth;
			t = clamp(t, 0.0, 1.0);
       
			//blend original with blur
			return lerp(color, sum, t);
		}
		ENDCG
        }
    }
}