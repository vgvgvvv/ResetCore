// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Simplified Additive Particle shader. Differences from regular Additive Particle one:
// - no Tint color
// - no Smooth particle support
// - no AlphaTest
// - no ColorMask

Shader "Custom/Particles/AdditiveColorRGB" {
	Properties{
		_MainTex("Particle Texture", 2D) = "white" {}
		_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	}

	Category{
			Tags{ "Queue" = "Transparent" "IgnoreProjector" = "True" "RenderType" = "Transparent" }
			Blend SrcAlpha One
			//AlphaTest Greater .1
			Cull Off Lighting Off ZWrite Off Fog{ Mode Off }

			SubShader{			
				Pass{

					CGPROGRAM
#pragma vertex vert
#pragma fragment frag

#include "UnityCG.cginc"

					sampler2D _MainTex;
					fixed4 _TintColor;
					struct appdata_t {
						half4 vertex : POSITION;
						fixed4 color : COLOR;
						half2 texcoord : TEXCOORD0;
					};

					struct v2f {
						half4 vertex : SV_POSITION;
						fixed4 color : COLOR;
						half2 texcoord : TEXCOORD0;
					};

					float4 _MainTex_ST;
					v2f vert(appdata_t v)
					{
						v2f o;
						o.vertex = UnityObjectToClipPos(v.vertex);
						o.color = v.color;
						o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
						return o;
					}

					fixed4 frag(v2f i) : SV_Target
					{
						fixed4 color = i.color * tex2D(_MainTex, i.texcoord)* _TintColor;
						color.a = (color.r+color.g+color.b)*2;
						return color;//fixed4(color.a,color.a,color.a,1);
					}
						ENDCG
				}
			}
		}
}