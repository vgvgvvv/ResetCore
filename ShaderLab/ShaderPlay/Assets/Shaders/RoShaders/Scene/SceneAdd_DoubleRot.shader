// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Scene/Add_DoubleRot" {
Properties {
	_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	_MainTex ("Particle Texture", 2D) = "white" {}
	_InvFade ("Soft Particles Factor", Range(0.01,3.0)) = 1.0
	_RotSpeed("X-正向速度 Y-反向速度",vector)=(1,1,1,1)
}

Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha One
	AlphaTest Greater .01
	ColorMask RGB
	Cull Off Lighting Off ZWrite Off Fog { Color (0,0,0,0) }
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_particles

			#include "UnityCG.cginc"

			sampler2D _MainTex;
			fixed4 _TintColor;
			float4 _RotSpeed;
			
			struct appdata_t {
				float4 vertex : POSITION;
				fixed4 color : COLOR;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f {
				float4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				float2 texcoord : TEXCOORD0;
				#ifdef SOFTPARTICLES_ON
				float4 projPos : TEXCOORD1; 
				#endif
			};
			 
			float4 _MainTex_ST;

			float2 RotateUV(float2 UV, float Speed)
			{
				float alpha = Speed*_Time.y * 3.141592654 / 180.0;
				float sina, cosa;
				sincos(alpha, sina, cosa);
				float2x2 m = float2x2(cosa, -sina, sina, cosa);
				return mul(m, UV);
			}

			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				#ifdef SOFTPARTICLES_ON
				o.projPos = ComputeScreenPos (o.vertex);
				COMPUTE_EYEDEPTH(o.projPos.z);
				#endif
				o.color = v.color;
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				return o;
			}

			sampler2D_float _CameraDepthTexture;
			float _InvFade;
			
			fixed4 frag (v2f i) : SV_Target
			{
				//#ifdef SOFTPARTICLES_ON

				//float sceneZ = LinearEyeDepth (SAMPLE_DEPTH_TEXTURE_PROJ(_CameraDepthTexture, UNITY_PROJ_COORD(i.projPos)));
				//float partZ = i.projPos.z;
				//float fade = saturate (_InvFade * (sceneZ-partZ));
				//i.color.a *= fade;
				//#endif
				fixed4 mask=tex2D(_MainTex, RotateUV(i.texcoord.xy - 0.5,_RotSpeed.x*10) + 0.5);
				fixed4 mask1=tex2D(_MainTex, RotateUV(float2(1-i.texcoord.x,i.texcoord.y) - 0.5,_RotSpeed.y*10) + 0.5);
				return 2.0f * i.color * _TintColor *mask*  mask1;
			//return RotateUV(i.texcoord.xy, 100).xyxy;
			}
			ENDCG 
		}
	}	
}
}
