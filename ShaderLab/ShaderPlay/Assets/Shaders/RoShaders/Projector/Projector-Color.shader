// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Projector' with 'unity_Projector'
// Upgrade NOTE: replaced '_ProjectorClip' with 'unity_ProjectorClip'

Shader "Custom/Projector/Color" 
{
	Properties {
		_Color("Tint Color", Color) = (1,1,1,1)
		_Args("x:Falloff y:Amplify",Vector) = (1,1,0,0)
		_ShadowTex("Cookie", 2D) = "gray" {}
		
		[Enum(UnityEngine.Rendering.BlendMode)] _SrcBlend ("Src Blend Mode", Float) = 1
        [Enum(UnityEngine.Rendering.BlendMode)] _DstBlend ("Dst Blend Mode", Float) = 1
	}

	Subshader {
		Tags{ "Queue" = "Transparent" }
		Pass
		{
			ZWrite Off
			ColorMask RGB
			Blend [_SrcBlend] [_DstBlend]
			Cull back
			Fog{ Mode Off }
			//Blend SrcAlpha OneMinusSrcAlpha

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"

			struct v2f {
				float4 uvShadow : TEXCOORD0;
				float4 pos : SV_POSITION;
			};

			float4x4 unity_Projector;
			float4x4 unity_ProjectorClip;

			v2f vert(float4 vertex : POSITION)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(vertex);
				o.uvShadow = mul(unity_Projector, vertex);
				return o;
			}

			sampler2D _ShadowTex;
			fixed4 _Color;
			half4 _Args;
			fixed4 frag(v2f i) : SV_Target
			{
				// Apply alpha mask
				fixed4 texCookie = tex2Dproj(_ShadowTex, UNITY_PROJ_COORD(i.uvShadow));
				fixed4 outColor = _Color * texCookie.rgba;
				// Attenuation
				float depth = i.uvShadow.z; // [-1 (near), 1 (far)]
				return outColor * clamp(1.0 - abs(depth) + _Args.x, 0.0, 1.0)*_Args.y;
			}
			ENDCG
		}
	}
}
