// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'
#ifndef OUTLINE_INCLUDED
#define OUTLINE_INCLUDED

/**
**使用方法

//描边相关
[Space]
_OutlineColor ("描边颜色", Color) = (0.25, 0.18, 0.15, 1)//描边颜色
_Outline("描边宽度",range(0,0.1)) = 0.02//描边宽度
_OutlineFactor("描边宽高比",range(0,1))=0.5//描边宽高比

Pass
{
	Name "Outline"	
	Tags { "RenderType"="Opaque" "Queue"="Geometry"}
	Cull Front
	ZWrite On

	CGPROGRAM
	#pragma vertex vert
	#pragma fragment frag
	#include "UnityCG.cginc"
	#include "../Include/OutLine_Include.cginc"
	ENDCG
}			

*/


#include "CommonHead_Include.cginc"
float _Outline;
float _OutlineFactor;
float4 _OutlineColor;
			
v2f vert(appdata_full v) {
	v2f o;
	float3 dir = normalize(v.vertex.xyz);
	float3 dir2 = v.normal;
	float d = dot(dir,dir2);
	dir=dir * sign(d);
	dir=dir * _OutlineFactor + dir2 * (1 - _OutlineFactor);
	v.vertex.xyz += dir * _Outline;
	o.pos = UnityObjectToClipPos(v.vertex);
	o.normal = normalize(mul((float3x3)unity_ObjectToWorld, SCALED_NORMAL));
	o.uv = v.texcoord;

	return o;
}

float4 frag(v2f i):COLOR
{
	return _OutlineColor; 
}

#endif //OUTLINE_INCLUDED