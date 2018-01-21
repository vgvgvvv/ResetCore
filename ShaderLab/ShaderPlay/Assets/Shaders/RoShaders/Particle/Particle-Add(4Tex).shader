// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Particles/Additive4Tex"
{
    Properties
    {
        _TintColor0 ("Tint Color0", Color) = (0.5,0.5,0.5,0.5)
		_MainTex0 ("Particle Texture0", 2D) = "black" {}
		_TintColor1 ("Tint Color1", Color) = (0.5,0.5,0.5,0.5)
		_MainTex1 ("Particle Texture1", 2D) = "black" {}
		_TintColor2 ("Tint Color2", Color) = (0.5,0.5,0.5,0.5)
		_MainTex2 ("Particle Texture2", 2D) = "black" {}
		_TintColor3 ("Tint Color3", Color) = (0.5,0.5,0.5,0.5)
		_MainTex3 ("Particle Texture3", 2D) = "black" {}
		_Scale("Tex Scale",vector) = (1,1,1,1)
		_Power("Tex Power",vector) = (1,1,1,1)
    }

    SubShader
    {
        LOD 200

        Tags
        {
            "Queue" = "Transparent"
            "IgnoreProjector" = "True"
            "RenderType" = "Transparent"
        }

        Pass
        {
            Cull Off
            Lighting Off
            ZWrite Off
            AlphaTest Off
            Fog { Mode Off }
            Offset -1, -1
            ColorMask RGB
            Blend SrcAlpha One
        
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

            struct appdata_t
            {
                float4 vertex : POSITION;
                half4 color : COLOR;
                float2 texcoord : TEXCOORD0;
            };

            struct v2f
            {
                float4 vertex : POSITION;
                half4 color : COLOR;
                float2 texcoord : TEXCOORD0;
            };

            v2f vert (appdata_t v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.color = v.color;
                o.texcoord = v.texcoord;
                return o;
            }
			
			sampler2D _MainTex0;
			fixed4 _TintColor0;
			sampler2D _MainTex1;
			fixed4 _TintColor1;
			sampler2D _MainTex2;
			fixed4 _TintColor2;
			sampler2D _MainTex3;
			fixed4 _TintColor3;
			float4 _Scale;
			float4 _Power;
            half4 frag (v2f IN) : COLOR
            {
				float2 uv0 = float2((IN.texcoord.x-0.5)*_Scale.x + 0.5,0.5 -(0.5 - IN.texcoord.y)*_Scale.x);
                half4 col0 = tex2D(_MainTex0, uv0) * IN.color*_TintColor0*_Power.x;
				float2 uv1 = float2((IN.texcoord.x-0.5)*_Scale.y + 0.5,0.5 -(0.5 - IN.texcoord.y)*_Scale.y);
				half4 col1 = tex2D(_MainTex1, uv1) * IN.color*_TintColor1*_Power.y;
				float2 uv2 = float2((IN.texcoord.x-0.5)*_Scale.z + 0.5,0.5 -(0.5 - IN.texcoord.y)*_Scale.z);
				half4 col2 = tex2D(_MainTex2, uv2) * IN.color*_TintColor2*_Power.z;
				float2 uv3 = float2((IN.texcoord.x-0.5)*_Scale.w + 0.5,0.5 -(0.5 - IN.texcoord.y)*_Scale.w);
				half4 col3 = tex2D(_MainTex3, uv3) * IN.color*_TintColor3*_Power.w;
				half4 col = col0*col0.a+col1*col1.a+col2*col2.a+col3*col3.a;
				//col.a = 1;
                return col;
            }
            ENDCG
        }
    }
}