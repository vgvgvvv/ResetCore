// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Projector' with 'unity_Projector'

Shader "Custom/Projector/FanAdd" {
Properties {
		_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
		_OutlineColor ("Outline Color", Color) = (1,1,1,1)
		_Arg("x:color transition y:color scale z:outline width w: outline scale",Vector) = (1.1,1,0.1,1)
		_Angle("Angle Control ",Float) = 0
	}
Category {
	Tags { "Queue"="Transparent" "RenderType"="Transparent" }
	Blend SrcAlpha One
	//ColorMask RGBA
	Cull Off Lighting Off ZWrite Off Fog{ Mode Off }
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			fixed4 _TintColor;
			fixed4 _OutlineColor;
			half4 _Arg;
			half _Angle;

			struct appdata_t {
				half4 vertex : POSITION;
				half2 texcoord  : TEXCOORD0;			
			};

			struct v2f {
				half4 vertex : SV_POSITION;			
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
				//circle outline
				half circle = saturate((0.25 - ( i.uv.x* i.uv.x+ i.uv.y* i.uv.y))*4);
				//circle mask & fan mask
				half mask = sign(circle*(normalize(i.uv).y - _Angle));
				//distance to center,_Arg.x make distance >0
				half dis = _Arg.y*(_Arg.x-circle);
				//outline
				half cull = saturate(_Arg.z-circle)*_Arg.w;
				fixed4 color = _TintColor;
				color.a *=dis;
				return (color+ cull*_OutlineColor)*mask;
			}
			ENDCG 
		}
	}	
}
}