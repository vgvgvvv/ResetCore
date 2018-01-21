Shader "Custom/Common/CutoutAdditiveColor" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_Color ("Color", Color) = (0.5, 0.5, 0.5, 0)
		_Power ("Power", Range (0.2, 0.7)) = 0.5
		_Mask ("Mask (A)", 2D) = "white" {}
		_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
	}
	SubShader {  
		Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout"  }
		LOD 300            
		Pass {  
		Name "Basic"
                  
			Cull Back  
			Lighting Off

            CGPROGRAM 
			//define
			#define CUTOUT
			//head
			#include "../Include/CommonHead_Include.cginc"
			//vertex&fragment
			#pragma vertex vert
            #pragma fragment frag 

			sampler2D _MainTex;
			sampler2D _Mask;
			fixed4 _Color;	
			half _Power;

			//custom frag fun
			fixed4 BasicColor(in v2f i)
			{
				fixed4 c = tex2D (_MainTex, i.uv);
				fixed4 a = tex2D(_Mask, i.uv);
				c.rgb = c.rgb + _Color.rgb * _Power;
				c.a = a.r*_Color.a;
				return c;
			}
			//include
            #include "UnityCG.cginc"  
            #include "../Include/CommonBasic_Include.cginc"
            ENDCG	
		}  
	}   
}
