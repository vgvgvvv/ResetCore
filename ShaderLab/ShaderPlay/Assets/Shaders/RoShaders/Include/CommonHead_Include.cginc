#ifndef COMMONHEAD_INCLUDED
#define COMMONHEAD_INCLUDED
	
struct a2v {  
	float4 vertex : POSITION; 
#ifndef NONORMAL	
	float3 normal : NORMAL;	
#endif
	float2 texcoord : TEXCOORD0;		
#ifdef UV2
	float2 texcoord1 : TEXCOORD1;
#endif
};  

struct v2f {  
	float4 pos : SV_POSITION;  	
#ifndef NONORMAL
	half3 normal: NORMAL;
#endif				  
	half2 uv : TEXCOORD0;
#ifdef UV2
	half2 uv1 : TEXCOORD1;
#endif
 
#ifdef LIGHTON
	fixed3 vertexLighting : TEXCOORD2;
#if defined(RIMLIGHT)|defined(REFLECT)|defined(METALREFLCUBE) |defined(GLASS)|defined(GLASSCUBE)
	half3 viewDir: TEXCOORD3;
#endif//RIMLIGHT|BLINNPHONG 
	
#if defined(REFLECT)|defined(METALREFLCUBE)|defined(GLASSCUBE)
	half3 refluv : TEXCOORD4;
#endif //BLINNPHONG

//#ifdef METALREFLCUBE
//	half3 refluv : TEXCOORD4;
//#endif
#else
#ifdef GLASS
	half3 viewDir: TEXCOORD3;
#endif//GLASS
#endif//LIGHTON

#ifdef SKINTEX
		fixed4 mask0: TEXCOORD5;
#ifdef SKINTEX8
		fixed4 mask1: TEXCOORD6;
#endif//SKINTEX8
#endif//SKINTEX
#ifdef MATCAP
		half2 cap : TEXCOORD7;
#endif
}; 

half4 _LightArgs;
#if defined(CUTOUT)|defined(METALREFLCUTOUT)
half _Cutoff;
#endif //CUTOUT
#ifdef MASK
sampler2D _Mask;
#endif // MASK
#endif //COMMONHEAD_INCLUDED