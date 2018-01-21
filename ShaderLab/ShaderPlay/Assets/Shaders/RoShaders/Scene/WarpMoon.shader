// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Scene/WarpMoon" {
Properties {
	_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	_MainTex ("Main Texture", 2D) = "white" {}
	_WarpTex ("Warp Texture", 2D) = "white" {}
	_WarpPara("x:扰动速度 y:扰动强度 z:开始距离 w: ----",Vector)= (1,1,1,1)
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha One
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
			sampler2D _WarpTex;
			fixed4 _TintColor;
			
			struct appdata_t {
				float4 vertex : POSITION;
				fixed4 color : COLOR;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f {
				half4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				half2 texcoord : TEXCOORD0;
			};
			
			half4 _MainTex_ST;
			half4 _WarpTex_ST;
			float4 _WarpPara;

			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.color = v.color;
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{				
			float warp=tex2D(_WarpTex, i.texcoord*_WarpTex_ST.xy+float2(-_Time.x,_Time.x*2)*_WarpPara.x+_WarpTex_ST.zw).x-0.5;
			float warp1=tex2D(_WarpTex, i.texcoord*_WarpTex_ST.xy+float2(_Time.x,-_Time.x)*_WarpPara.x+_WarpTex_ST.zw).x-0.5;
				return 2.0f * i.color*_TintColor * tex2D(_MainTex, i.texcoord+float2( warp,0)*warp1*saturate(pow((1-i.texcoord.y), _WarpPara.z))*_WarpPara.y);
				//return min((1-i.texcoord.y)*2,1);
			}
			ENDCG 
		}
	}	
}
}