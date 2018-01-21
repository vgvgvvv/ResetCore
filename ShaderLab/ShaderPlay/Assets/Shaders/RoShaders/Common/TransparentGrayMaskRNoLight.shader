// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'


Shader "Custom/Common/TransparentGrayMaskRNoLight" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_Mask ("Mask (A)", 2D) = "white" {}
	//_GrayMask("Gray Mask",Vector) = (0,0,0.33,0.33)
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
				//fixed gray : TEXCOORD1;
			};

			sampler2D _MainTex;
			sampler2D _Mask;
			//fixed4 _GrayMask;
			v2f vert (appdata_t v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.texcoord = v.texcoord;
				//fixed2 inside = step(_GrayMask.xy, o.texcoord.xy) * step(o.texcoord.xy, _GrayMask.zw);
				//o.gray = inside.x * inside.y;
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{
				fixed4 col = tex2D(_MainTex, i.texcoord);
				fixed grey = Luminance(col.rgb);
				//col.rgb = col.rgb*(1-i.gray)+i.gray*fixed3(grey, grey, grey);
				col.rgb = fixed3(grey, grey, grey);
				fixed alpha = tex2D (_Mask, i.texcoord).r;
				col.a = alpha+0.01;
				return col;
			}
		ENDCG
	}
}

}
