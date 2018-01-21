#ifndef SKINBLEND_INCLUDED
#define SKINBLEND_INCLUDED
#include "CommonHead_Include.cginc"
#ifdef SKINTEX
sampler2D _Tex0;
sampler2D _Tex1;
sampler2D _Tex2;
sampler2D _Tex3;
fixed4 _HairColor;
fixed4 _EyesColor;
#ifdef SKINTEX8
sampler2D _Tex4;
sampler2D _Tex5;
sampler2D _Tex6;
sampler2D _Tex7;
#endif	//SKINTEX8
#endif	//SKINTEX

inline half4 ChangeHairColor(sampler2D tex0, sampler2D tex1,half2 uv0, half2 uv1, half2 uvOffset, half4 changeColor)
{
    half4 colorOrg = tex2D(tex0, uv0 - uvOffset);
	half4 colorMask = tex2D(tex1, uv1 - uvOffset);
    return colorOrg * half4((half3(1, 1, 1) - colorMask.rgb), 1) + changeColor * colorMask ;
}

int _IsChangeEmotion;
int _IsCloseEyes;
float4 _EyesOffset;
inline half4 ChangeEyesColor(sampler2D tex0, sampler2D tex1, half2 uv0, half2 uv1, half2 uvOffset, half4 changeColor)
{
    half2 eyesOffset = half2(_EyesOffset.x, _EyesOffset.y);
    half4 colorOrg = tex2D(tex0, uv0 - uvOffset) * ( 1- _IsCloseEyes) + tex2D(tex0, uv0 + eyesOffset) * _IsCloseEyes;
    half4 colorMask = tex2D(tex1, uv1 - uvOffset);
    half4 res = (colorOrg * half4((half3(1, 1, 1) - colorMask.rgb), 1) + changeColor * colorMask) * (1 - _IsCloseEyes) + colorOrg * _IsCloseEyes;
    res.a *= (1 - _IsChangeEmotion);
    return res;
}

inline fixed4 BlendColor(in v2f i)
{
    half2 uvOffset = half2(0, 0);
    return i.uv.x < 1 ? tex2D(_Tex0, i.uv - uvOffset):
        i.uv.x < 2 ? tex2D(_Tex1, i.uv-uvOffset):
	    i.uv.x < 3 ? ChangeEyesColor(_Tex2, _Tex2, i.uv, i.uv1, uvOffset, _EyesColor) :
        i.uv.x < 4 ? tex2D(_Tex3, i.uv-uvOffset):
        i.uv.x < 5 ? tex2D(_Tex4, i.uv-uvOffset):
	    i.uv.x < 6 ? tex2D(_Tex5, i.uv-uvOffset):
        i.uv.x < 7 ? tex2D(_Tex6, i.uv - uvOffset) : ChangeHairColor(_Tex0, _Tex7, i.uv, i.uv, uvOffset, _HairColor);
}

#endif //SKINBLEND_INCLUDED