Shader "Custom/RimLight/CutoutDiffuseMaskRCap" {
Properties {
_Color ("Main Color", Color) = (1, 1, 1, 1)
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_MatCap("Cap", 2D) = "white" {}
	_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
	_RimColor ("Rim Color", Color) = (0.353, 0.353, 0.353,0.0)
	_LightArgs("x:MainColor Scale y:Light Scale z:cap scale w: Rim Power",Vector) = (1.0,0.21,1.2,3.0)
	_UIRimMask("UI Rim Mask",Vector) = (1,1,0,0)
}
SubShader {  
	Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout"  }
	LOD 100            
	Pass {  
	Name "Basic"
                  
		Cull Back  
		Lighting On

        CGPROGRAM 
		//define
		#define CUTOUT
		#define LIGHTON
		//#define SHLIGHTON
		#define RIMLIGHT
		#define UIRIM
		#define VERTEXLIGHTON
		#define MATCAP
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		sampler2D _Mask;

		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D(_MainTex, i.uv);
			fixed4 a = tex2D(_Mask, i.uv);
			c.a = a.r;
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG
	}  

	Pass 
	{
		Name "ShadowCaster"
		Tags { "LightMode" = "ShadowCaster" }
		
		Fog {Mode Off}
		ZWrite On ZTest LEqual Cull Off
		Offset 1, 1

		CGPROGRAM
		#define CUTOUT
		//include
		#include "../Include/Shadow_Include.cginc"
		#pragma vertex vertCast
		#pragma fragment fragCast
		#pragma multi_compile_shadowcaster
		ENDCG 
	}
} 
}
