// Upgrade NOTE: commented out 'sampler2D unity_Lightmap', a built-in variable
// Upgrade NOTE: replaced tex2D unity_Lightmap with UNITY_SAMPLE_TEX2D

#ifndef LIGHTMAP_INCLUDED
#define LIGHTMAP_INCLUDED

	#include "UnityCG.cginc"  
	#include "../Include/CommonHead_Include.cginc"
	sampler2D _MainTex;
	sampler2D _Mask;
	// sampler2D unity_Lightmap;
	//custom frag fun
	fixed4 BasicColor(in v2f i)
	{
		fixed4 c = tex2D(_MainTex, i.uv);		
#ifdef USELM
		c.rgb *= DecodeLightmap(UNITY_SAMPLE_TEX2D(unity_Lightmap, i.uv2.xy)).rgb;
#endif//USELM

#ifdef CUTOUT
		fixed4 a = tex2D(_Mask, i.uv);
		c.a = a.a;
#endif //CUTOUT
		return c;
	}
	//include
       
    #include "../Include/CommonBasic_Include.cginc"

#endif //LIGHTMAP_INCLUDED