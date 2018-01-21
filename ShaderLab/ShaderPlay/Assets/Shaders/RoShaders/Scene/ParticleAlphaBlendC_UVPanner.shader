// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Scene/AlphaBlend_UVPanner" {
Properties {
	_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	_MainTex ("Particle Texture", 2D) = "white" {}
	_MaskTex ("Mask Texture", 2D) = "white" {}
	_PannerPara("X-横向速度 Y-纵向速度",vector)=(1,1,1,1)
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha OneMinusSrcAlpha
	//AlphaTest Greater .01
	ColorMask RGB
	Cull Off Lighting Off ZWrite Off Fog{ Mode Off }

	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			sampler2D _MainTex;
			sampler2D _MaskTex;
			fixed4 _TintColor;
			
			struct appdata_t {
				half4 vertex : POSITION;
				fixed4 color : COLOR;
				half2 texcoord : TEXCOORD0;
			};

			struct v2f {
				half4 vertex : POSITION;
				fixed4 color : TEXCOORD1;
				half2 texcoord : TEXCOORD0;
			};
			
			float4 _MainTex_ST;
			float4 _PannerPara;
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.color = 2 * v.color* _TintColor;
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{				
			half2 uvoffset= fmod( float2(_Time.x*_PannerPara.xy),1);
			fixed4 mask=tex2D(_MaskTex, i.texcoord+ uvoffset);
			fixed4 c=i.color *  tex2D(_MainTex, i.texcoord+ uvoffset);
			c.a=mask;
				return  c;
			}
			ENDCG 
		}
	}	
}
}
