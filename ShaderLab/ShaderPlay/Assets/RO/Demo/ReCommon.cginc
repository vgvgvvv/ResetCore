#ifndef __RE_COMMON__
#define __RE_COMMON__

#include "UnityCG.cginc"

//========================================================
//
// 顶点结构体以及片段结构体
//
//========================================================
struct a2v {
    float4 vertex : POSITION;   //顶点位置
	half4 color : COLOR;        //颜色
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