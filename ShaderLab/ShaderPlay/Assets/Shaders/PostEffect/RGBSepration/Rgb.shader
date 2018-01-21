// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Hidden/Rgb"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_Force("Force", Range(0, 1)) = 0.005
		_Direction("Direction",Vector)=(1,1,0,0)
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
			};

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}
			
			sampler2D _MainTex;
			float _Force;
			float2 _Direction;

			fixed4 frag (v2f i) : SV_Target
			{
				//float2 uv2 = float2((i.uv.x - 0.5)*2, (i.uv.y - 0.5) * 2);
				//float distanceFromCenter = sqrt(uv2.x * uv2.x + uv2.y * uv2.y);
				float force = _Force;
				fixed4 colr = tex2D(_MainTex, i.uv);
				fixed4 colg = tex2D(_MainTex, float2(i.uv.x, i.uv.y) + normalize(_Direction) * force);
				fixed4 colb = tex2D(_MainTex, float2(i.uv.x, i.uv.y) + normalize(_Direction) * force * 2);
				// just invert the colors
				fixed4 col = float4(colr.x, colg.y, colb.z, 1);
				//fixed4 col = float4(uv2.x, uv2.x, uv2.x, 1);
				return col;
			}
			ENDCG
		}
	}
}
