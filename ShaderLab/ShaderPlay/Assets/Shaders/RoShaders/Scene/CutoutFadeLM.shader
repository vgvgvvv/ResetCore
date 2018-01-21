// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Scene/CutoutFadeLM" {
Properties {
_Color ("Main Color", Color) = (1, 1, 1, 1)
	_MainTex ("Main Texture", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_Fade("Fade", Range (0.0, 1.0)) = 0.2
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha OneMinusSrcAlpha
	//AlphaTest Greater .01
	ColorMask RGB
	Cull Off Lighting Off ZWrite Off

	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			sampler2D _MainTex;
			
			struct appdata_t {
				half4 vertex : POSITION;
				half2 texcoord : TEXCOORD0;
			};

			struct v2f {
				half4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
			};
			
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = v.texcoord;
				return o;
			}
			sampler2D _Mask;
			fixed _Fade;
			fixed4 frag (v2f i) : SV_Target
			{				
				fixed4 c = tex2D(_MainTex, i.texcoord);
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				c.a = _Fade*alpha;
				return c;
			}
			ENDCG 
		}
	}	
}
}
