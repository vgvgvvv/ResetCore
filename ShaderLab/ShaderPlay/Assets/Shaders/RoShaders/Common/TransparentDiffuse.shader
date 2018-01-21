Shader "Custom/Common/TranspantDiffuse" {
Properties {
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_LightArgs("x:MainColor Scale y:Light Scale z:Unused w: Rim Power",Vector) = (1.0,0.21,0.0,3.0)
}
SubShader {  
	Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
	LOD 300            
	Pass {  
	Name "Basic"
        Tags { "LightMode"="ForwardBase" }
		Blend SrcAlpha OneMinusSrcAlpha
		Cull Back  
		Lighting On

        CGPROGRAM 
		//define
		#define LIGHTON
		#define VERTEXLIGHTON
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;

		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D (_MainTex, i.uv);
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG
	}  
} 
}
