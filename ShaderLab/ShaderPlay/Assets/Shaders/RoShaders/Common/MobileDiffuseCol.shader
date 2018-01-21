// Simplified Diffuse shader. Differences from regular Diffuse one:
// - no Main Color
// - fully supports only 1 directional light. Other lights can affect it, but it will be per-vertex/SH.

Shader "Custom/Common/MobileDiffuseCol" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_Color ("Main Color", Color) = (1,1,1,1)
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
		#define VERTEXLIGHTON
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		fixed4 _Color;

		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D (_MainTex, i.uv)*_Color;
			c.a = 1;
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG    
	}  
} 
}
