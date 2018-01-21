Shader "Custom/Scene/WaterAlphaBlend" {
Properties {
	_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	_MainTex ("Particle Texture", 2D) = "white" {}
	_Cap ("cap", 2D) = "" {}
	_waterColor("Water Color",Color)=(1,1,1,1)
		_vector("x:U_Speed  y:V_Speed   z:WarpScale   w:ReflWarpScale",vector)=(1,1,1,0.25)
	//_InvFade ("Soft Particles Factor", Range(0.01,3.0)) = 1.0
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent"  }
	Blend SrcAlpha OneMinusSrcAlpha
	ColorMask RGB
	Lighting Off ZWrite Off
	
	SubShader {
		Pass {
		
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma target 2.0
			#pragma multi_compile_particles
			#pragma multi_compile_fog

			#include "UnityCG.cginc"

			sampler2D _MainTex;
			sampler2D _Cap; 
			fixed4 _TintColor;
			half4 _MainTex_ST;
			half4 _Cap_ST;
			half4 _BumpTex_ST;
			half4 _vector;
			fixed4 _waterColor;

			struct appdata_t {
				float4 vertex : POSITION;
				float3 normal: NORMAL;
				fixed4 color :COLOR;
				float2 texcoord : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};

			struct v2f {
				float4 vertex : SV_POSITION;
				half2 uvBump : TEXCOORD3;
				half2 cap : TEXCOORD2;
				fixed4 color : TEXCOORD5;
				float2 texcoord : TEXCOORD0;
				float2 texcoord1 : TEXCOORD4;
				UNITY_FOG_COORDS(1)
				UNITY_VERTEX_OUTPUT_STEREO
			};
			

			v2f vert (appdata_t v)
			{
				v2f o;
				UNITY_SETUP_INSTANCE_ID(v);
				UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uvBump = TRANSFORM_TEX(v.texcoord,_BumpTex);
	half2 capCoord;
	capCoord.x = dot(UNITY_MATRIX_IT_MV[0].xyz, v.normal);
	capCoord.y = dot(UNITY_MATRIX_IT_MV[1].xyz, v.normal);
	o.cap = capCoord * 0.5 + 0.5;
				o.color = v.color;
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				o.texcoord1 = TRANSFORM_TEX(v.texcoord,_Cap);
				UNITY_TRANSFER_FOG(o,o.vertex);
				return o;
			}

			sampler2D_float _CameraDepthTexture;
			//float _InvFade;
			
			fixed4 frag (v2f i) : SV_Target
			{
		//	half3 refluv=reflect(-i.view,normalize( i.normal));
			fixed4 MainT = tex2D (_MainTex, i.texcoord+_vector.xy*_Time.x);
			fixed4 mc = tex2D (_Cap, i.texcoord1+MainT.xy*_vector.z);
				
				fixed4 col = mc* _waterColor;;
				col.a=i.color.r*(mc.x+mc.y+mc.z)/3;
				UNITY_APPLY_FOG_COLOR(i.fogCoord, col, fixed4(0,0,0,0)); // fog towards black due to our blend mode
				return col;
			}
			ENDCG 
		}
	}	
}
}
