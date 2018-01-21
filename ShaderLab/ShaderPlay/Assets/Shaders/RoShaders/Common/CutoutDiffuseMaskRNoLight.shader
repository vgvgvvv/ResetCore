Shader "Custom/Common/CutoutDiffuseMaskRNoLight" {
Properties {
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_Cutoff ("Alpha cutoff", Range(0,1)) = 0.15
	_Color ("Color", Color) = (1, 1, 1,0.0)
}
SubShader {  
	Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout"  }
	LOD 300            
	Pass {  
	Name "Basic"
                  
		Cull Back  
		Lighting Off
		Blend SrcAlpha OneMinusSrcAlpha
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
		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D(_MainTex, i.uv);
			fixed4 a = tex2D(_Mask, i.uv);
			c.a =a.r*_Color.a;
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG
	}  
} 
}
