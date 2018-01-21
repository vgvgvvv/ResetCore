#ifndef PBSHEAD_INCLUDED
#define PBSHEAD_INCLUDED
	
struct a2v {  
	float4 vertex : POSITION; 	
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
};  

struct v2f {  
	float4 pos : POSITION;  					
	half2 uv : TEXCOORD0;
	half3 normal;
#ifdef LIGHTON	
	fixed3 vertexLighting : TEXCOORD2;
	half3 viewDir: TEXCOORD3;	
#endif//LIGHTON

#ifdef SKINTEX
		fixed4 mask0: TEXCOORD5;
#ifdef SKINTEX8
		fixed4 mask1: TEXCOORD6;
#endif//SKINTEX8
#endif//SKINTEX

}; 

#endif //PBSHEAD_INCLUDED