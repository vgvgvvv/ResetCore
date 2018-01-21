
Shader "Custom/Common/MobileDiffuseColMask" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_Color ("Main Color", Color) = (1,1,1,1)
	_RimColor ("Rim Color", Color) = (0.353, 0.353, 0.353,0.0)
	_LightArgs("x:MainColor Scale y:Light Scale z:Unused w: Rim Power",Vector) = (1.0,0.21,0.0,3.0)
}
SubShader {  
	Tags { "RenderType"="Opaque" }
	LOD 100            
	Pass {  
	Name "Basic"
		Tags { "LightMode"="ForwardBase" }
		LOD 200
		Cull Back
        
		CGPROGRAM 
		//define
		#define LIGHTON
		#define SHLIGHTON
		#define RIMLIGHT
		#define VERTEXLIGHTON

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
			fixed4 c = tex2D (_MainTex, i.uv);
			fixed mask = tex2D (_Mask, i.uv).r;
			c.rgb = (1-mask)*c+mask*_Color*c;
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
