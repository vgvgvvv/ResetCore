// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Projector' with 'unity_Projector'

Shader "Custom/Projector/RectAdd" {
Properties {
		_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
		_OutlineColor ("Outline Color", Color) = (1,1,1,1)
		_Arg("x:color edge y:color center z:outline width w: outline scale",Vector) = (2.5,0.5,0.03,10)
	}
Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha One
	//ColorMask RGBA
	Cull Off Lighting Off ZWrite Off Fog{ Mode Off }
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			fixed4 _TintColor;
			fixed4 _OutlineColor;
			half4 _Arg;

			struct appdata_t {
				half4 vertex : POSITION;
				half2 texcoord  : TEXCOORD0;			
			};

			struct v2f {
				half4 vertex : POSITION;	
				half2 uv : TEXCOORD0;
			};
			
			float4x4 unity_Projector;

			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				half4 shadowUV = mul(unity_Projector, v.vertex);
				o.uv = shadowUV.xy - half2(0.5,0.5);
				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{				
				half xDis = saturate(0.5 - abs(i.uv.x));
				half yDis = saturate(0.5 - abs(i.uv.y));
				half rect = xDis*yDis;
				half mask = sign(rect);
				half cull = (saturate(_Arg.z - xDis)+ saturate(_Arg.z - yDis))*_Arg.w;
				xDis = 0.5 -xDis;
				half dis = _Arg.y+_Arg.x*xDis*xDis;				
				fixed4 color = _TintColor;
				color.a *=dis;
				
				return (color+ cull*_OutlineColor)*mask;
			}
			ENDCG 
		}
	}	
}
}