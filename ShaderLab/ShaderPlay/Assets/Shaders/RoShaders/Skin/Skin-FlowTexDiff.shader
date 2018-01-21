Shader "Custom/Skin/FlowTexDiff" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "black" {}
		_FlowTex ("Flow (RGB)", 2D) = "black" {}

		_Color ("Flow Color", Color) = (0.5, 0.5, 0.5, 1)
		_FlowArg ("x:Time y:Power z:xMul w:yMul", Vector) = (1, 1, 1, 1)

		_Mask("Mask", 2D) = "white" {}
	}
	SubShader {  
	Tags { "RenderType" = "Opaque"}
	LOD 200            
		Pass {  
		Name "Basic"           
			Cull Back  
			Lighting Off

			CGPROGRAM 
			//define
			#define _passTime _Time.y
			//#define _passTime _DebugTime
			//head
			#include "../Include/CommonHead_Include.cginc"
			//vertex&fragment
			#pragma vertex vert
			#pragma fragment frag 

			sampler2D _MainTex;
			sampler2D _FlowTex;
			half4 _Color;  
			half4 _FlowArg;
			sampler2D _Mask;
			//custom frag fun
			fixed4 BasicColor(in v2f i)
			{
				fixed4 center = tex2D (_MainTex, i.uv);
				fixed4 mask = tex2D (_Mask, i.uv);
				float m = (mask.r + mask.g + mask.b) / 3;

				//flow uv
				half passTime = fmod(_passTime,_FlowArg.x);
				passTime /= _FlowArg.x;
				half2 uv = i.uv;
				uv.x += passTime*_FlowArg.z;
				uv.y += passTime*_FlowArg.w;
				fixed4 f = tex2D (_FlowTex, uv);
			
				return fixed4(center.rgb+f*_Color*m,1);
			}
			//include
			#include "UnityCG.cginc"
			#include "../Include/CommonBasic_Include.cginc"
			ENDCG		
		}  
	} 
}
