// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

//Shader "Custom/Common/TransparentDiffuseMaskNoLight" {
//Properties {
//	_MainTex ("Base (RGB)", 2D) = "white" {}
//	_Mask ("Mask (A)", 2D) = "white" {}
//}
//SubShader {  
//	Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
//	LOD 300            
//	Pass {  
//	Name "Basic"                  
//		Blend One OneMinusSrcAlpha
//		LOD 200
//		Cull Back 
//		Lighting Off

//        CGPROGRAM 
//		//define
//		//head
//		#include "../Include/CommonHead_Include.cginc"
//		//vertex&fragment
//		#pragma vertex vert
//        #pragma fragment frag 

//		sampler2D _MainTex;
//		sampler2D _Mask;
//		//custom frag fun
//		fixed4 BasicColor(in v2f i)
//		{
//			fixed4 c = tex2D (_MainTex, i.uv);
//			fixed4 a = tex2D (_Mask, i.uv);
//			c.a = a.a+0.01;
//			return a;
//		}
//		//include
//        #include "UnityCG.cginc"  
//        #include "../Include/CommonBasic_Include.cginc"
//        ENDCG
//	}  
//} 
//}
// Unlit alpha-blended shader.
// - no lighting
// - no lightmap support
// - no per-material color

Shader "Custom/Common/TransparentDiffuseMaskRNoLight" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
}

SubShader {
	Tags {"Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"}
	LOD 100
	
	ZWrite Off
	Blend SrcAlpha OneMinusSrcAlpha 
	
	Pass {  
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata_t {
				float4 vertex : POSITION;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f {
				float4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
			};

			sampler2D _MainTex;
			sampler2D _Mask;
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = v.texcoord;
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed4 col = tex2D(_MainTex, i.texcoord);
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				col.a = alpha+0.01;
				return col;
			}
		ENDCG
	}
}

}
