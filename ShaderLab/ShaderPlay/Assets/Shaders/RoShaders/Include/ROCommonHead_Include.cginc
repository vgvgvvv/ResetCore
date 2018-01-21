#ifndef COMMONHEAD_INCLUDED
#define COMMONHEAD_INCLUDED
	
struct a2v {  
	float4 vertex : POSITION; 
#ifndef VERT_NONORMAL	
	float3 normal : NORMAL;	
#endif

#ifdef VERT_TANGENT
	float4 tangent : TANGENT;	
#endif		

//UV
	float2 texcoord0 : TEXCOORD0;		
#ifdef UV2
	float2 texcoord1 : TEXCOORD1;
#endif

#ifdef CUSTOM_A2V
	CUSTOM_A2V
#endif

};  

struct v2f {  

	float4 pos : SV_POSITION;  	
#ifndef FRAG_NONORMAL
	half3 normal: NORMAL;
#endif

#ifdef FRAG_TANGENT
	float4 tangent : TANGENT;	
#endif	
	 
//UV
	half2 uv0 : TEXCOORD0;
#ifdef UV2
	half2 uv1 : TEXCOORD1;
#endif

#ifdef CUSTOM_V2F
	CUSTOM_V2F
#endif

#ifdef SKINTEX
		fixed4 mask0: TEXCOORD5;
#ifdef SKINTEX8
		fixed4 mask1: TEXCOORD6;
#endif//SKINTEX8
#endif//SKINTEX

#ifdef LIGHTING_MAPPING
 LIGHTING_COORDS(7,8)
 #if defined(LIGHTMAP_ON) || defined(UNITY_SHOULD_SAMPLE_SH)
     float4 ambientOrLightmapUV : TEXCOORD9;
 #endif
 #endif

}; 



#endif //ROCOMMONHEAD_INCLUDED