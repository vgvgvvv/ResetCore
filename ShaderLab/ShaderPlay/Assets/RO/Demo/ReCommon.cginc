#ifndef __RE_COMMON__
#define __RE_COMMON__

#include "UnityCG.cginc"

//========================================================
//
// 宏定义
//
//========================================================

#define USE_NORMAL true
#define USE_TNORMAL true


//========================================================
//
// 顶点结构体以及片段结构体
//
//========================================================
struct a2v {
    float4 vertex : POSITION;   //顶点位置
	half4 color : COLOR;        //颜色
	float2 texcoord : TEXCOORD0;//UV

    #if USE_NORMAL
    half3 normal : NORMAL;      //法线
    #endif
    
    #if USE_TNORMAL
    half4 tangent: TANGENT;     //切线
    #endif
    
}

struct v2f {
    float4 pos : SV_POSITION;   //像素位置
	half4 color : COLOR;        //颜色
}

v2f vert(a2v i){
    v2f = o;

}

half4 frag(v2f i){
    
}

#endif