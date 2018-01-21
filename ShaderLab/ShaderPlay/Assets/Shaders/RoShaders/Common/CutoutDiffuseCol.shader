Shader "Custom/Common/CutoutDiffuseCol" {
Properties {
	_Color ("Main Color", Color) = (1,1,1,1)
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
	//[HideInInspector]
	_LightArgs("x:MainColor Scale y:Light Scale z:Unused w: Rim Power",Vector) = (1.0,0.21,0.0,3.0)
}
SubShader {  
	Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout"  }
	LOD 300            
	Pass {  
	Name "Basic"
        Tags { "LightMode"="ForwardBase" }
		Cull Back  
		Lighting On

        CGPROGRAM 
		//define
		#define CUTOUT
		#define LIGHTON
		#define VERTEXLIGHTON
		//#define TESTLIGHTING
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
			return c;
		}
		//include
        #include "UnityCG.cginc"  
        #include "../Include/CommonBasic_Include.cginc"
        ENDCG 	
	}  
} 
}
