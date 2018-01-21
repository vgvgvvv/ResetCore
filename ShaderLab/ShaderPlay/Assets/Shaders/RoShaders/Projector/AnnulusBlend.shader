// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Projector' with 'unity_Projector'

Shader "Custom/Projector/AnnulusBlend" {
Properties {
		_Color ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
		//_OutlineColor ("Outline Color", Color) = (1,1,1,1)
		_Args("x:annulus width y:annulus radius ",Vector) = (1,0.5,1,0)
		
	}
Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha OneMinusSrcAlpha
	//ColorMask RGBA
	Cull Off Lighting Off ZWrite Off Fog{ Mode Off }
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			fixed4 _Color;
			//fixed4 _OutlineColor;
			half4 _Args;

			struct appdata_t {
				half4 vertex : POSITION;
				half2 texcoord  : TEXCOORD0;			
			};

			struct v2f {
				half4 vertex : POSITION;	
				half2 uv : TEXCOORD0;
			};
			
			float4x4 unity_Projector;

			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				half4 shadowUV = mul(unity_Projector, v.vertex);
				o.uv = shadowUV.xy - half2(0.5,0.5);
				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{				
				half c= 1-floor(saturate( length(i.uv*2/max(_Args.y,0.0001))));
				half c1= 1-floor(saturate( length(i.uv*2/max((_Args.y-_Args.x),0.0001))));
				fixed4 col=_Color;
				col.a=(c-c1)*_Color.a;
				return col;
				//half ss=   min(saturate( length(i.uv*2/_Args.z/2)), 1-saturate( length(i.uv*2/_Args.z/2)));
				//return ss;
			}
			ENDCG 
		}
	}	
}
}