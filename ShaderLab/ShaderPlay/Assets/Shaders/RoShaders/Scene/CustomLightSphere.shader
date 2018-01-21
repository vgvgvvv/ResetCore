// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "Custom/Scene/AddLightSphere" {
Properties {
	_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend One One
	//AlphaTest Greater .01
	ColorMask RGB
	 Lighting Off ZWrite Off Fog{ Mode Off }
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			sampler2D _MainTex;
			fixed4 _TintColor;
			
			struct appdata_t {
				float4 vertex : POSITION;
				//fixed4 color : COLOR;
				//float2 texcoord : TEXCOORD0;
				float3 normal:NORMAL;
			};

			struct v2f {
				half4 vertex : SV_POSITION;
			//	fixed4 color : COLOR;
				//half2 texcoord : TEXCOORD0;
				float3 N : TEXCOORD1;
				float3 V : TEXCOORD2;
			};
			

			float customPow(float x) { return x*x*x*x*x; }

			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				float4 WSPosition = mul(unity_ObjectToWorld, v.vertex);
			o.N= mul(unity_ObjectToWorld, float4(v.normal,0)).xyz;
				o.V = _WorldSpaceCameraPos.xyz - WSPosition.xyz;
				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{				
				float NdL = dot(normalize(i.N),normalize(i.V));
				return 2.0f*_TintColor * customPow(NdL);
			}
			ENDCG 
		}
	}	
}
}
