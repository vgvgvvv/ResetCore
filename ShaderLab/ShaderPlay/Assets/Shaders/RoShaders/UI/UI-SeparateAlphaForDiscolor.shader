Shader "Custom/UI/SeparateAlphaForDiscolor"
{
	Properties
	{
		_MainTex("Base (RGB)", 2D) = "white" { }
		_AlphaTex("AlphaTex", 2D) = "white"{}
		_StencilComp("Stencil Comparison", Float) = 8
		_Stencil("Stencil ID", Float) = 0
		_StencilOp("Stencil Operation", Float) = 0
		_StencilWriteMask("Stencil Write Mask", Float) = 255
		_StencilReadMask("Stencil Read Mask", Float) = 255
		_ColorMask("Color Mask", Float) = 15
	}

	SubShader
	{
		Tags
		{
			"Queue" = "Transparent"
			"IgnoreProjector" = "True"
			"RenderType" = "Transparent"
		}

		Stencil
		{
			Ref[_Stencil]
			Comp[_StencilComp]
			Pass[_StencilOp]
			ReadMask[_StencilReadMask]
			WriteMask[_StencilWriteMask]
		}
		ColorMask[_ColorMask]

		Lighting Off
		ZWrite Off
		Blend SrcAlpha OneMinusSrcAlpha
		Cull Off
		AlphaTest Off
		Fog{ Mode Off }
		Offset -1, -1
		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			sampler2D _MainTex;
			sampler2D _AlphaTex;

			struct v2f
			{
				float4  pos : SV_POSITION;
				half2  uv : TEXCOORD0;
				half4 color :COLOR;
			};

			v2f vert(appdata_full v)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(v.vertex);
				o.uv = v.texcoord;
				o.color = v.color;
				return o;
			}

			half4 frag(v2f i) : COLOR
			{
				half4 texcol = tex2D(_MainTex, i.uv);
				half4 result = texcol;
				half grey = Luminance(texcol.rgb);
				half colMask = sign(i.color.r + i.color.g + i.color.b);
				result.rgb = half3(grey, grey, grey);
				result.a = tex2D(_AlphaTex, i.uv).a*i.color.a;
				return result;
			}

			ENDCG
		}
	}
}
