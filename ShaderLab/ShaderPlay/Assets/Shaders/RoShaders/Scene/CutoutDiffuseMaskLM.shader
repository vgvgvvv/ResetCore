// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: commented out 'float4 unity_LightmapST', a built-in variable
// Upgrade NOTE: commented out 'sampler2D unity_Lightmap', a built-in variable
// Upgrade NOTE: replaced tex2D unity_Lightmap with UNITY_SAMPLE_TEX2D


Shader "Custom/Scene/CutoutDiffuseMaskLM" {
Properties {
	_MainTex ("Base (RGB) Trans (A)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
}
SubShader {  
	Tags { "Queue"="AlphaTest" "IgnoreProjector"="True" "RenderType"="TransparentCutout"  }
	LOD 300    

	Pass {
			Tags { "LightMode" = "Vertex" }
			CGPROGRAM
			#pragma multi_compile_fog 
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"
			sampler2D _MainTex;
			sampler2D _Mask;
			struct appdata_t {
				float4 vertex : POSITION;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f {
				half4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
				UNITY_FOG_COORDS(2)
			};
			
			half4 _MainTex_ST;
			
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				UNITY_TRANSFER_FOG(o, o.vertex);
				return o;
			}
			half _Cutoff;
			fixed4 frag (v2f i) : SV_Target
			{				
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				clip(alpha -_Cutoff);
				fixed4 col = tex2D(_MainTex, i.texcoord);			
				col.a = alpha;
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
			}
			ENDCG 
		}
	Pass {  
	Tags { "LightMode" = "VertexLMRGBM" }
		CGPROGRAM
			
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_fog 

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
				UNITY_FOG_COORDS(2)
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
				UNITY_TRANSFER_FOG(o, o.vertex);
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				clip(alpha -_Cutoff);
				fixed4 col = tex2D(_MainTex, i.texcoord);		
				col.rgb *= DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, i.uv2.xy)).rgb;		
				col.a = alpha+0.01;
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
			}
		ENDCG
	}	        


	Pass {  
	Tags { "LightMode" = "VertexLM" }
		CGPROGRAM
			
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_fog 

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
				UNITY_FOG_COORDS(2)
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
				UNITY_TRANSFER_FOG(o, o.vertex);
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				clip(alpha -_Cutoff);
				fixed4 col = tex2D(_MainTex, i.texcoord);		
				col.rgb *= DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, i.uv2.xy)).rgb;		
				col.a = alpha+0.01;
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col;
			}
		ENDCG
	}	      
} 
}