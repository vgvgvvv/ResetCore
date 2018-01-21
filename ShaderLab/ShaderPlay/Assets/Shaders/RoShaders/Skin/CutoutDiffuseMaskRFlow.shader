Shader "Custom/Skin/CutoutDiffuseMaskRFlow" {
Properties {
_Color ("Main Color", Color) = (1, 1, 1, 1)
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_Mask ("MaskAlpha (R) MaskFlow (G)", 2D) = "white" {}
	_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
	_LightArgs("x:MainColor Scale y:Light Scale z:Unused w: Rim Power",Vector) = (1.0,0.21,0.0,3.0)

	_FlowTex ("Flow (RGB)", 2D) = "black" {}

	_FlowColor ("Flow Color", Color) = (0.5, 0.5, 0.5, 1)
	_FlowArg ("x:Time y:Power z:xMul w:yMul", Vector) = (1, 1, 1, 1)

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
		#define _passTime _Time.y
		#define UV2
		#define CUTOUT
		#define LIGHTON
		//#define SHLIGHTON
		//#define RIMLIGHT
		//#define UIRIM
		#define VERTEXLIGHTON
		//head
		#include "../Include/CommonHead_Include.cginc"
		//vertex&fragment
		#pragma vertex vert
        #pragma fragment frag 

		sampler2D _MainTex;
		sampler2D _Mask;
		sampler2D _FlowTex;
		half4 _FlowColor;  
		half4 _FlowArg;
		//custom frag fun
		fixed4 BasicColor(in v2f i)
		{
			fixed4 center = tex2D (_MainTex, i.uv);
			fixed4 mask = tex2D(_Mask, i.uv);

			//flow uv
			half passTime = fmod(_passTime,_FlowArg.x);
			passTime /= _FlowArg.x;
			half2 uv = i.uv1;
			uv.x += passTime*_FlowArg.z;
			uv.y += passTime*_FlowArg.w;
			fixed4 f = tex2D (_FlowTex, uv);
			
			return fixed4(center.rgb+f*_FlowColor.rgb*mask.g,mask.r);
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
