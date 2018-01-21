Shader "Custom/Scene/CustomShadowA" {
	Properties{

	}		
		SubShader{
		Tags{ "RenderType" = "Opaque" }
		Pass
		{
			Name "ShadowCasterA"

			Fog{ Mode Off }
			ZWrite On ZTest LEqual Cull Off
			Offset 1, 1
			ColorMask A
			CGPROGRAM
			#include "UnityCG.cginc"
			//include
			//#include "../Include/Shadow_Include.cginc"
			#pragma vertex vertCast
			#pragma fragment fragCast
			#pragma multi_compile_shadowcaster

			struct appdata {
				float4 vertex : POSITION;
				half2 texcoord : TEXCOORD0;
			};
			struct v2fCast {
				V2F_SHADOW_CASTER;
				float2  uv : TEXCOORD1;
			};

			v2fCast vertCast(appdata v)
			{
				v2fCast o;
				TRANSFER_SHADOW_CASTER(o)
				o.uv = v.texcoord;
				return o;
			}
			fixed4 fragCast(v2fCast i) : SV_Target
			{
				SHADOW_CASTER_FRAGMENT(i)
			}
			ENDCG
		}
	}
}
