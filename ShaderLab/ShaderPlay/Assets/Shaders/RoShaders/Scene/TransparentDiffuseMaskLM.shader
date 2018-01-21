// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: commented out 'float4 unity_LightmapST', a built-in variable
// Upgrade NOTE: commented out 'sampler2D unity_Lightmap', a built-in variable
// Upgrade NOTE: replaced tex2D unity_Lightmap with UNITY_SAMPLE_TEX2D


Shader "Custom/Scene/TransparentDiffuseMaskLM" {
Properties {
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
}
SubShader {  
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"  }
	LOD 300    
	
	Blend SrcAlpha OneMinusSrcAlpha
	Pass {  
	Tags { "LightMode" = "VertexLMRGBM" }
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata_t {
				float4 vertex : POSITION;
				float2 texcoord : TEXCOORD0;
				half2 uv2 : TEXCOORD1;
			};

			struct v2f {
				float4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
				half2 uv2 : TEXCOORD1;
			};

			sampler2D _MainTex;
			sampler2D _Mask;
			// sampler2D unity_Lightmap;
			// float4 unity_LightmapST;
			half _Cutoff;
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = v.texcoord;
				o.uv2.xy = v.uv2.xy * unity_LightmapST.xy + unity_LightmapST.zw;
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				fixed4 col = tex2D(_MainTex, i.texcoord);		
				col.rgb *= DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, i.uv2.xy)).rgb;		
				col.a = alpha+0.01;
				
				return col;
			}
		ENDCG
	}	        


	Pass {  
	Tags { "LightMode" = "VertexLM" }
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata_t {
				float4 vertex : POSITION;
				float2 texcoord : TEXCOORD0;
				half2 uv2 : TEXCOORD1;
			};

			struct v2f {
				float4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
				half2 uv2 : TEXCOORD1;
			};

			sampler2D _MainTex;
			sampler2D _Mask;
			// sampler2D unity_Lightmap;
			// float4 unity_LightmapST;
			half _Cutoff;
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = v.texcoord;
				o.uv2.xy = v.uv2.xy * unity_LightmapST.xy + unity_LightmapST.zw;
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				fixed4 col = tex2D(_MainTex, i.texcoord);		
				col.rgb *= DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, i.uv2.xy)).rgb;		
				col.a = alpha+0.01;
				
				return col;
			}
		ENDCG
	}	      
} 
}