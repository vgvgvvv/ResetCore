// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Scene/Skybox Cubed" {
Properties {
	_Tint ("Tint Color", Color) = (.5, .5, .5, .5)
	_Tex ("Cubemap", Cube) = "white" {}
_Rotation("Rotation", Range(0, 360)) = 0
}

SubShader {
	Tags { "Queue"="Background" "RenderType"="Background" }
	Cull Off ZWrite Off Fog { Mode Off }

	Pass {
		
		CGPROGRAM
		#pragma vertex vert
		#pragma fragment frag

		#include "UnityCG.cginc"

		samplerCUBE _Tex;
		fixed4 _Tint;
		float _Rotation;

		float3 RotateAroundYInDegrees(float3 vertex, float degrees)
		{
			float alpha = degrees * 3.141592654 / 180.0;
			float sina, cosa;
			sincos(alpha, sina, cosa);
			float2x2 m = float2x2(cosa, -sina, sina, cosa);
			return float3(mul(m, vertex.xz), vertex.y).xzy;
		}
		
		struct appdata_t {
			float4 vertex : POSITION;
			float3 texcoord : TEXCOORD0;
		};

		struct v2f {
			float4 vertex : SV_POSITION;
			float3 texcoord : TEXCOORD0;
		};

		v2f vert (appdata_t v)
		{
			v2f o;
			float3 rotated = RotateAroundYInDegrees(v.vertex, _Rotation);
			o.vertex = UnityObjectToClipPos(float4(rotated,1));

			o.texcoord = v.texcoord;
			return o;
		}

		fixed4 frag (v2f i) : SV_Target
		{
			fixed4 tex = texCUBE (_Tex, i.texcoord);
			fixed4 col;
			col.rgb = tex.rgb + _Tint.rgb - unity_ColorSpaceGrey;
			col.a = tex.a * _Tint.a;
			return col;
		}
		ENDCG 
	}
} 	


Fallback Off

}
