Shader "Custom/Common/AdditiveColor" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_Color ("Color", Color) = (0.5, 0.5, 0.5, 0)
		_Power ("Power", Range (0.2, 0.7)) = 0.5
	}
	SubShader {  
	Tags { "RenderType" = "Opaque" }  
    LOD 300            
    Pass {  
	Name "Basic"
                  
            Cull Back  
			Lighting Off

            CGPROGRAM 
			//define
			//head
			#include "../Include/CommonHead_Include.cginc"
			//vertex&fragment
			#pragma vertex vert
            #pragma fragment frag 

			sampler2D _MainTex;
			fixed4 _Color;	
			half _Power;

			//custom frag fun
			fixed4 BasicColor(in v2f i)
			{
				fixed4 c = tex2D (_MainTex, i.uv);
				c.rgb = c.rgb + _Color * _Power;
				c.a = 1;
				return c;
			}
			//include
            #include "UnityCG.cginc"  
            #include "../Include/CommonBasic_Include.cginc"
            ENDCG 
        }  
    } 
}
