Shader "Custom/RimLight/DiffuseCap" {
    Properties {
      _MainTex ("Texture", 2D) = "white" {}
	  _MatCap("Cap", 2D) = "white" {}
      _RimColor ("Rim Color", Color) = (0.353, 0.353, 0.353,0.0)
      _LightArgs("x:MainColor Scale y:Light Scale z:cap scale w: Rim Power",Vector) = (1.0,0.21,1.2,3.0)
	  _UIRimMask("UI Rim Mask",Vector) = (1,1,0,0)
    }
	SubShader {  
	Tags { "RenderType" = "Opaque"}
	LOD 200            
	Pass {  
	Name "Basic"
     Tags { "LightMode"="ForwardBase" }               
		Cull Back  
		Lighting On

        CGPROGRAM 
		//define
		#define LIGHTON
		#define RIMLIGHT
		#define UIRIM
		#define SHLIGHTON
		#define VERTEXLIGHTON
		#define MATCAP
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D(_MainTex, i.uv);
			c.a = 1;
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
		//include
		#include "../Include/Shadow_Include.cginc"
		#pragma vertex vertCast
		#pragma fragment fragCast
		#pragma multi_compile_shadowcaster
		ENDCG 
	}
} 
}
