  Shader "Custom/Skin/CubeNoMask" {
    Properties {
		
      _MainTex ("Texture", 2D) = "white" {}
	  _Cube ("Cubemap", CUBE) = "" {}

	  _SpecColor  ("Specular Color", Color) = (1, 1, 1,1)      
	  _CubeColor ("Cube Color", Color) = (1, 1, 1,1)

	  _LightArgs("x:MainColor Scale y:Light Scale z:Cube Power w: Rim Power",Vector)= (1.0,1.0,0.7,0.55)
	  
    }

	SubShader {  
	Tags { "RenderType" = "Opaque"  }
	LOD 200            
	Pass {  
	Name "Basic"
        Tags { "LightMode"="ForwardBase" }            
		Cull Back  
		Lighting On

        CGPROGRAM 
		//define
		#define LIGHTON
		#define REFLECT
		#define RIMLIGHT
		#define VERTEXLIGHTON
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		samplerCUBE _Cube;
		fixed4 _CubeColor;
		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 c = tex2D(_MainTex, i.uv);
			half m = (c.r + c.g + c.b) / 3;
			fixed4 reflcol = texCUBE (_Cube, i.refluv)*m;
			c.rgb +=  reflcol.rgb * _LightArgs.z * _CubeColor;
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
