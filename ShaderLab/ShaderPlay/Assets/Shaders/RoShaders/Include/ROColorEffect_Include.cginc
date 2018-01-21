#ifndef RIMCOLOR_INCLUDED
#define RIMCOLOR_INCLUDED

/////////////////////////////////////////////////////////////////////////////////////////////////////
//黑白灰控制
#ifdef GrayControl
//Property
// [Space]
// _LIANG_FW ("高亮范围", Range(0, 1)) = 1//高亮范围
// _LIANG_COLOR ("亮部颜色", Color) = (1,1,1,1)//亮部颜色
// _LIANG_QD ("亮部强度", Range(0, 2)) = 1.1//亮部强度

// _ZHONG_COLOR ("灰部颜色", Color) = (1,1,1,1)//灰部颜色
// _ZHONG_QD ("灰部强度", Range(0, 2)) = 1//灰部强度
// _AN_FW ("暗部范围", Range(0, 1)) = 0//暗部范围
// _AN_COLOR ("暗部颜色", Color) = (1,1,1,1)//暗部颜色
// _AN_QD ("暗部强度", Range(0, 2)) = 0.9//暗部强度

//需要的属性
 fixed _LIANG_FW;
 float4 _LIANG_COLOR;
 fixed _LIANG_QD;

 float4 _ZHONG_COLOR;
 fixed _ZHONG_QD;

 fixed _AN_FW;
 float4 _AN_COLOR;
 fixed _AN_QD;

//应用黑白灰
inline fixed3 ApplyGrayControl(fixed3 _TP_var){
    fixed node_7028 = dot(_TP_var.rgb,float3(0.3,0.59,0.11));
	fixed _Step_02 = step(_LIANG_FW,_TP_var.rgb);
	fixed _Step_01 = step(node_7028,_AN_FW);
	fixed _Clamp_01 = saturate(((-1 * _Step_02) + _Step_01));
	fixed _Clamp_02 = saturate(((-1 * _Step_01) + _Step_02));
	fixed3 finalColor = ((_AN_COLOR.rgb * _AN_QD * _Clamp_01 * _TP_var.rgb) + 
		(_TP_var.rgb * (1.0 - saturate((_Clamp_01 + _Clamp_02))) * _ZHONG_QD * _ZHONG_COLOR.rgb) +
		(_Clamp_02 * _TP_var.rgb * _LIANG_COLOR.rgb * _LIANG_QD));
	return finalColor;
}
#endif
//黑白灰控制
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
//叠加色
#ifdef OverlayColor
//Property
// [Space]
// _ZT_COLOR ("叠加色", Color) = (1,1,1,1)//叠加色
// _ZT_QD ("叠加色强度", Range(0, 2)) = 1//叠加色强度

//需要的属性
 float4 _ZT_COLOR;
 fixed _ZT_QD;

//应用叠加色
inline fixed3 ApplyOverlay(fixed3 color){
	return color * _ZT_COLOR.rgb * _ZT_QD;
}
#endif
//叠加色
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
//伪光源
#ifdef FakeLight


//Property
// [Space]
// _ZHUGUANG ("主光源颜色", Color) = (0,0,0,0.5)//主光源颜色
// _QD ("主光源强度", Float ) = 0//主光源强度
// _LIANG ("亮部颜色", Color) = (0,0,0,1)//亮部颜色
// _AN ("暗部颜色", Color) = (0,0,0,0.5)//暗部颜色
// _ZF ("主光源范围", Vector) = (1,1,1,0)//主光源范围
// _LF ("亮部范围", Vector) = (1,1,1,0)//亮部范围
// _AF ("暗部范围", Vector) = (1,1,1,0)//暗部范围
// _attenYPower ("主光源衰弱强度", Range(1, 5)) = 2//衰弱强度
// _attenYOffset ("主光源衰弱偏移", Range(-5, 5)) = 0//衰弱偏移

//需要定点函数支持
//#define CUSTOM_V2F float4 posWorld : TEXCOORD3;
//
// v2f CustomVert(a2v v, v2f o){
// 	o.posWorld = mul(unity_ObjectToWorld, v.vertex);
// 	return o;
// }


//需要的属性
 uniform float4 _LightAreaColor;
 uniform float4 _DarkAreaColor;
 uniform float4 _MainLightColor;
 uniform float4 _MainLightRange;
 uniform float4 _DarkLightRange;
 uniform float4 _LightLightRange;
 uniform float _MainLightPower;
 uniform float _attenYPower;
 uniform float _attenYOffset;

//应用叠加色
inline fixed3 ApplyFakeLight(v2f i, fixed3 color){

	fixed4 objPos = mul ( unity_ObjectToWorld, float4(0,0,0,1) );
    i.normalDir = normalize(i.normalDir);
    fixed3 normalection = i.normalDir;
	fixed3 _Texture_var = color;

	fixed3 node_9577 = mul( UNITY_MATRIX_V, fixed4(i.normalDir,0) ).xyz;
    fixed3 node_5724 = saturate(dot((_LightLightRange.rgb*2.0+-1.0),node_9577.rgb));
    fixed3 node_1922 = 0.0;
    fixed3 node_8841 = 1.0;
    fixed3 node_1424 = _Texture_var.rgb +
        (_LightAreaColor.rgb*saturate((node_5724*(_LightLightRange.a + 
        ((node_5724 - node_1922) * (node_8841 - _LightLightRange.a) ) / 
        (node_8841 - node_1922)))))+
            (saturate((saturate(dot(node_9577.rgb,(_DarkLightRange.rgb*2.0+-1.0)))*
            (_DarkLightRange.a + 
            ( (saturate(dot(node_9577.rgb,(_DarkLightRange.rgb*2.0+-1.0))) - node_1922) * 
            (node_8841 - _DarkLightRange.a) ) / 
            (node_8841 - node_1922))))*_DarkAreaColor.rgb)+
            (saturate((_MainLightRange.a + 
            ( (saturate(dot(node_9577.rgb,(_MainLightRange.rgb*2.0+-1.0))) - node_1922) * 
            (node_8841 - _MainLightRange.a) ) / 
            (node_8841 - node_1922)))*
            _MainLightColor.rgb*_MainLightPower*saturate(((((i.posWorld.g-objPos.g)+_attenYOffset)*0.5+0.5)*_attenYPower))
			);
    return node_1424;
}
#endif //FakeLight
//伪光源
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
//对比度
#ifdef ColorContrast

//Property
// [Space]
// _DBD ("对比度", Float ) = 1

//需要的属性
fixed _DBD;

//应用对比度
inline fixed3 ApplyContrast(fixed3 color){
	return (1.0 - (_DBD*(1.0 - color.rgb)));
}


#endif //ColorContrast
//对比度
/////////////////////////////////////////////////////////////////////////////////////////////////////


#endif