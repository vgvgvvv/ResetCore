// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

#ifndef COMMONBASIC_INCLUDED
#define COMMONBASIC_INCLUDED 
#include "CommonHead_Include.cginc"
#ifdef LIGHTON
fixed4 _LightColor0 = fixed4(1,1,1,1);
#endif //LIGHTON
#ifdef UIRIM
fixed4 _UIRimMask;
#endif
v2f vert(a2v v) {  
	v2f o = (v2f)0;  
	  
	o.pos = UnityObjectToClipPos(v.vertex);
	o.uv = v.texcoord;

#ifdef UV2
	o.uv1 = v.texcoord1;
#endif//UV2
#ifdef SKINTEX
#endif//SKINTEX
#ifndef NONORMAL
	//1.calc normal
	o.normal = normalize(mul((float3x3)unity_ObjectToWorld, SCALED_NORMAL));
#ifdef MATCAP
	half2 capCoord;
	capCoord.x = dot(UNITY_MATRIX_IT_MV[0].xyz, v.normal);
	capCoord.y = dot(UNITY_MATRIX_IT_MV[1].xyz, v.normal);
	o.cap = capCoord * 0.5 + 0.5;
#endif
#endif
#ifdef LIGHTON	
	//2.calc view
#if defined(RIMLIGHT)|defined(REFLECT)|defined(METALREFLCUBE)|defined(GLASS)|defined(GLASSCUBE)
	o.viewDir = WorldSpaceViewDir( v.vertex );
#ifdef UIRIM
	o.viewDir.xy *=_UIRimMask.xy;
	o.viewDir.z -=_UIRimMask.z;
#endif//UIRIM
	o.viewDir = normalize(o.viewDir);
#endif //RIMLIGHT|BLINNPHONG

	//3. vertex lighting
	o.vertexLighting = fixed3(0,0,0);
	//3.1 SHLight
#ifdef SHLIGHTON
	o.vertexLighting = saturate(ShadeSH9 (normalize(float4(o.normal, 1.0))));
#endif	//SHLIGHTON
	//3.1 vertex light 
#ifdef VERTEXLIGHTON
	fixed3 ambientLighting = UNITY_LIGHTMODEL_AMBIENT.rgb;
	fixed3 diffuseReflection = _LightColor0.rgb * max(0.001, dot(o.normal, _WorldSpaceLightPos0.xyz))*2;
	o.vertexLighting += ambientLighting + diffuseReflection;
	//4. blinn phong or cube
#if defined(REFLECT)|defined(METALREFLCUBE)|defined(GLASSCUBE)
	o.refluv = reflect(-o.viewDir, o.normal);
	//o.refluv = mul(UNITY_MATRIX_MV, float4(o.refluv,0)).rgb;
	//#else METALREFLCUBE
#endif //REFLECT
#endif //VERTEXLIGHTON  
#else

//#ifdef METALREFLCUBE
//	o.refluv = reflect(-o.viewDir, o.normal);
//	o.refluv = mul(UNITY_MATRIX_MV, float4(o.refluv,0)).rgb;
//#endif

#if defined(GLASS)|defined(GLASSCUBE)
	o.viewDir = WorldSpaceViewDir( v.vertex );
#endif //GLASS 
#endif //LIGHTON

	return o;  
} 


#ifdef RIMLIGHT
fixed4 _RimColor;
#endif

#ifdef BLINNPHONG
fixed4 _SpecColor;
#endif //BLINNPHONG

#ifdef MATCAP
sampler2D _MatCap;
#endif

#ifdef METALREFL
 fixed4 _CubeColor; 
//sampler2D _Mask;
#endif

#ifdef GLASS
//sampler2D _Mask;
#endif //GLASS 

#ifdef METALREFLCUBE
samplerCUBE _Cube; 
fixed4 _CubeColor; 
//sampler2D _Mask;
#endif

#ifdef GLASSCUBE
samplerCUBE _Cube; 
#endif //GLASSCUBE

fixed4 frag(v2f i) : COLOR 
{  
	fixed4 _BasicColor = BasicColor(i);
	fixed4 c=_BasicColor;
	 
#if defined(CUTOUT)|defined(METALREFLCUTOUT)
	clip(c.a - _Cutoff);
#endif //CUTOUT 

#ifdef MATCAP
#ifdef METALREFL
#ifdef GLASS 
	fixed4 mask = tex2D(_Mask, i.uv);
	fixed4 mc = tex2D(_MatCap, i.cap);
	fixed4 R = mc*mask.r*_LightArgs.z;
	half rim = pow(1.0 - saturate(dot (i.viewDir, i.normal)),_LightArgs.w);
	c = lerp(c, lerp(R, R*c * 2, mask.g), mask.r);
	c.a = Luminance(R.rgb) + mask.b+rim*_LightArgs.y;
#else
	
	fixed4 mask = tex2D(_Mask, i.uv);
	fixed4 mc = tex2D(_MatCap, i.cap);
	fixed4 R = mc*mask.r*_CubeColor*_LightArgs.z;
#ifdef METALREFLCUTOUT
	c = lerp(c,R, mask.r);
	#else
	c = lerp(c, lerp(R, R*c * 2, mask.g), mask.r);
	#endif
#endif //GLASS
#else 
	fixed4 mc = tex2D(_MatCap, i.cap);
	c = fixed4(c.rgb*mc.rgb*_LightArgs.z, c.a);
#endif //METALREFL
#endif //MATCAP


#ifdef METALREFLCUBE
	fixed4 mask = tex2D(_Mask, i.uv);
	fixed4 mc = texCUBE (_Cube, i.refluv);
	fixed4 R = mc*mask.r*_CubeColor*_LightArgs.z;
#ifdef METALREFLCUTOUT
	c = lerp(c,R, mask.r);
	#else
	c = lerp(c, lerp(R, R*c * 2, mask.g), mask.r);
	#endif
#endif //METALREFLCUBE

#ifdef GLASSCUBE
	fixed4 mask = tex2D(_Mask, i.uv);
	fixed4 mc = texCUBE(_Cube, i.refluv);
	fixed4 R = mc*mask.r*_LightArgs.z;
	half rim = pow(1.0 - saturate(dot (i.viewDir, i.normal)),_LightArgs.w);
	c = lerp(c, lerp(R, R*c * 2, mask.g), mask.r);
	c.a = Luminance(R.rgb) + mask.b+rim*_LightArgs.y;
	//c=float4(i.refluv.xyz,1);
#endif //GLASSCUBE


#ifdef LIGHTON 

#ifdef VERTEXLIGHTON
	c.rgb *= _LightArgs.x + _LightArgs.y*(i.vertexLighting.rgb);
#ifdef TESTLIGHTING
	c.rgb = i.worldNormal.rgb;
#endif //TESTLIGHTING
#else
#ifdef LAMBERT	
	//lighting	
	fixed3 ambientLighting = UNITY_LIGHTMODEL_AMBIENT.rgb;
	fixed3 lightDirection = _WorldSpaceLightPos0.xyz;
	fixed3 diffuseReflection = _LightColor0.rgb * max(0.0, dot(i.normal, _WorldSpaceLightPos0.xyz))*2;
	c.rgb *= _LightArgs.x* + _LightArgs.y*(ambientLighting + diffuseReflection + i.vertexLighting);
#ifdef TESTLIGHTING
	c.rgb = _LightColor0.rgb * 2;
#endif //TESTLIGHTING
#else
#ifdef BLINNPHONG
	float3 lightDirection = _WorldSpaceLightPos0.xyz;
	half3 h = normalize (lightDirection + i.viewDir);	
	fixed diff = max (0, dot (i.worldNormal, lightDirection));
	float nh = max (0, dot (i.worldNormal, h));
	float spec = pow (nh, _LightArgs.x*128.0) * _LightArgs.y;
#ifdef TESTLIGHTING
	c.rgb = (_LightColor0.rgb * diff + _LightColor0.rgb * _SpecColor.rgb * spec) * 2;
#else
	c.rgb = (c.rgb*_LightArgs.w + (1-_LightArgs.w)*c.rgb *_LightColor0.rgb * diff + _LightColor0.rgb * _SpecColor.rgb * spec) * 2;
#endif
#endif //BLINNPHONG

#endif //LAMBERT

#endif //VERTEXLIGHTON

#ifdef RIMLIGHT
	half rim = 1.0 - saturate(dot (i.viewDir, i.normal));
	c.rgb += saturate(_RimColor.rgb * pow (rim, _LightArgs.w));
#endif

#endif //LIGHTON
#ifdef BLEND
fixed a= Alpha(i);
c.a=a;
#endif //BLEND

#ifdef EMISSION
fixed4 e=EmissionColor(i,_BasicColor);
c=c+e;
#endif //EMISSION

	return c;  
} 
#endif //COMMONBASIC_INCLUDED