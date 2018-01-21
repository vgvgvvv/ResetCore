
Shader "Custom/Common/TransparentDiffuseMaskRCube" {
Properties {
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Cube("Cubemap", CUBE) = "" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_LightArgs("x:MainColor Scale y:Light Scale z:Unused w: Rim Power",Vector) = (1.0,0.21,0.0,3.0)
}
SubShader {  
	Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
	LOD 100            
	Pass {  
	Name "Basic"

        Tags { "LightMode"="ForwardBase" }
		Blend SrcAlpha OneMinusSrcAlpha
		Cull Back  
		Lighting On   
		ZWrite On

        CGPROGRAM 
		//define
		#define LIGHTON
		//#define SHLIGHTON
		//#define RIMLIGHT
#define REFLECT
		#define VERTEXLIGHTON
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		sampler2D _Mask;
		samplerCUBE _Cube;
		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D(_MainTex, i.uv);
			fixed4 a = tex2D(_Mask, i.uv);
			fixed4 reflcol = texCUBE(_Cube, i.refluv);
			c.rgb += reflcol.rgb * _LightArgs.z;
			c.a = a.r;
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG
	}  
} 
}
