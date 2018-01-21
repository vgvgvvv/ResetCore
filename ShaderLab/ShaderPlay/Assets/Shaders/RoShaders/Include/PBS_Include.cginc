// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

#ifndef PBS_INCLUDED
#define PBS_INCLUDED

#define UNITY_PI	3.14159265359f

//-------------------------------------------------------------------------------------
// Default BRDF to use:
#if !defined (UNITY_BRDF_PBS) // allow to explicitly override BRDF in custom shader
	// still add safe net for low shader models, otherwise we might end up with shaders failing to compile
	#if SHADER_TARGET < 30
		#define UNITY_BRDF_PBS BRDF3_Unity_PBS
	#elif UNITY_PBS_USE_BRDF3
		#define UNITY_BRDF_PBS BRDF3_Unity_PBS
	#elif UNITY_PBS_USE_BRDF2
		#define UNITY_BRDF_PBS BRDF2_Unity_PBS
	#elif UNITY_PBS_USE_BRDF1
		#define UNITY_BRDF_PBS BRDF1_Unity_PBS
	#elif defined(SHADER_TARGET_SURFACE_ANALYSIS)
		// we do preprocess pass during shader analysis and we dont actually care about brdf as we need only inputs/outputs
		#define UNITY_BRDF_PBS BRDF1_Unity_PBS
	#else
		#error something broke in auto-choosing BRDF
	#endif
#endif

half4  custom_ColorSpaceDielectricSpec;

struct LightInfo
{
	half3 color;
	half3 dir;
	half  ndotl;
};

inline half Pow4 (half x)
{
	return x*x*x*x;
}

inline half2 Pow4 (half2 x)
{
	return x*x*x*x;
}

inline half3 Pow4 (half3 x)
{
	return x*x*x*x;
}

inline half4 Pow4 (half4 x)
{
	return x*x*x*x;
}

inline half3 Unity_SafeNormalize(half3 inVec)
{
	half dp3 = max(0.001f, dot(inVec, inVec));
	return inVec * rsqrt(dp3);
}

inline half DotClamped (half3 a, half3 b)
{
	#if (SHADER_TARGET < 30)
		return saturate(dot(a, b));
	#else
		return max(0.0h, dot(a, b));
	#endif
}

inline half BlinnTerm (half3 normal, half3 halfDir)
{
	return DotClamped (normal, halfDir);
}

inline half RoughnessToSpecPower (half roughness)
{
#if UNITY_GLOSS_MATCHES_MARMOSET_TOOLBAG2
	// from https://s3.amazonaws.com/docs.knaldtech.com/knald/1.0.0/lys_power_drops.html
	half n = 10.0 / log2((1-roughness)*0.968 + 0.03);
#if defined(SHADER_API_PS3) || defined(SHADER_API_GLES) || defined(SHADER_API_GLES3)
	// Prevent fp16 overflow when running on platforms where half is actually in use.
	n = max(n,-255.9370);  //i.e. less than sqrt(65504)
#endif
	return n * n;

	// NOTE: another approximate approach to match Marmoset gloss curve is to
	// multiply roughness by 0.7599 in the code below (makes SpecPower range 4..N instead of 1..N)
#else
	half m = max(1e-4f, roughness * roughness);			// m is the true academic roughness.
	
	half n = (2.0 / (m*m)) - 2.0;						// https://dl.dropboxusercontent.com/u/55891920/papers/mm_brdf.pdf
	n = max(n, 1e-4f);									// prevent possible cases of pow(0,0), which could happen when roughness is 1.0 and NdotH is zero
	return n;
#endif
}

// Ref: http://jcgt.org/published/0003/02/03/paper.pdf
inline half SmithJointGGXVisibilityTerm (half NdotL, half NdotV, half roughness)
{
#if 0
	// Original formulation:
	//	lambda_v	= (-1 + sqrt(a2 * (1 - NdotL2) / NdotL2 + 1)) * 0.5f;
	//	lambda_l	= (-1 + sqrt(a2 * (1 - NdotV2) / NdotV2 + 1)) * 0.5f;
	//	G			= 1 / (1 + lambda_v + lambda_l);

	// Reorder code to be more optimal
	half a		= roughness * roughness; // from unity roughness to true roughness
	half a2		= a * a;

	half lambdaV = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
	half lambdaL = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);

	// Unity BRDF code expect already simplified data by (NdotL * NdotV)
	// return (2.0f * NdotL * NdotV) / (lambda_v + lambda_l + 1e-5f);
	return 2.0f / (lambdaV + lambdaL + 1e-5f);
#else
    // Approximation of the above formulation (simplify the sqrt, not mathematically correct but close enough)
	half a = roughness * roughness;
	half lambdaV = NdotL * (NdotV * (1 - a) + a);
	half lambdaL = NdotV * (NdotL * (1 - a) + a);
	return 2.0f / (lambdaV + lambdaL + 1e-5f);	// This function is not intended to be running on Mobile,
												// therefore epsilon is smaller than can be represented by half
#endif
}

inline half GGXTerm (half NdotH, half roughness)
{
	half a = roughness * roughness;
	half a2 = a * a;
	half d = NdotH * NdotH * (a2 - 1.f) + 1.f;
	return a2 / (UNITY_PI * d * d + 1e-7f); // This function is not intended to be running on Mobile,
											// therefore epsilon is smaller than what can be represented by half
}

// Generic Smith-Schlick visibility term
inline half SmithVisibilityTerm (half NdotL, half NdotV, half k)
{
	half gL = NdotL * (1-k) + k;
	half gV = NdotV * (1-k) + k;
	return 1.0 / (gL * gV + 1e-5f); // This function is not intended to be running on Mobile,
									// therefore epsilon is smaller than can be represented by half
}

// Smith-Schlick derived for Beckmann
inline half SmithBeckmannVisibilityTerm (half NdotL, half NdotV, half roughness)
{
	half c = 0.797884560802865h; // c = sqrt(2 / Pi)
	half k = roughness * roughness * c;
	return SmithVisibilityTerm (NdotL, NdotV, k);
}

// BlinnPhong normalized as normal distribution function (NDF)
// for use in micro-facet model: spec=D*G*F
// eq. 19 in https://dl.dropboxusercontent.com/u/55891920/papers/mm_brdf.pdf
inline half NDFBlinnPhongNormalizedTerm (half NdotH, half n)
{
	// norm = (n+2)/(2*pi)
	half normTerm = (n + 2.0) * (0.5/UNITY_PI);

	half specTerm = pow (NdotH, n);
	return specTerm * normTerm;
}

//-------------------------------------------------------------------------------------

// Note: BRDF entry points use oneMinusRoughness (aka "smoothness") and oneMinusReflectivity for optimization
// purposes, mostly for DX9 SM2.0 level. Most of the math is being done on these (1-x) values, and that saves
// a few precious ALU slots.


// Main Physically Based BRDF
// Derived from Disney work and based on Torrance-Sparrow micro-facet model
//
//   BRDF = kD / pi + kS * (D * V * F) / 4
//   I = BRDF * NdotL
//
// * NDF (depending on UNITY_BRDF_GGX):
//  a) Normalized BlinnPhong
//  b) GGX
// * Smith for Visiblity term
// * Schlick approximation for Fresnel
half4 BRDF1_Unity_PBS (half3 diffColor, half3 specColor, half oneMinusReflectivity, half oneMinusRoughness,
	half3 normal, half3 viewDir,
	LightInfo light)
{
	half roughness = 1-oneMinusRoughness;
	half3 halfDir = Unity_SafeNormalize (light.dir + viewDir);

	// NdotV should not be negative for visible pixels, but it can happen due to perspective projection and normal mapping
	// In this case normal should be modified to become valid (i.e facing camera) and not cause weird artifacts.
	// but this operation adds few ALU and users may not want it. Alternative is to simply take the abs of NdotV (less correct but works too).
	// Following define allow to control this. Set it to 0 if ALU is critical on your platform.
	// This correction is interesting for GGX with SmithJoint visibility function because artifacts are more visible in this case due to highlight edge of rough surface
	// Edit: Disable this code by default for now as it is not compatible with two sided lighting used in SpeedTree.
	#define UNITY_HANDLE_CORRECTLY_NEGATIVE_NDOTV 0 

#if UNITY_HANDLE_CORRECTLY_NEGATIVE_NDOTV
	// The amount we shift the normal toward the view vector is defined by the dot product.
	// This correction is only applied with SmithJoint visibility function because artifacts are more visible in this case due to highlight edge of rough surface
	half shiftAmount = dot(normal, viewDir);
	normal = shiftAmount < 0.0f ? normal + viewDir * (-shiftAmount + 1e-5f) : normal;
	// A re-normalization should be applied here but as the shift is small we don't do it to save ALU.
	//normal = normalize(normal);

	// As we have modified the normal we need to recalculate the dot product nl.
	// Note that  light.ndotl is a clamped cosine and only the ForwardSimple mode use a specific ndotL with BRDF3
	half nl = DotClamped(normal, light.dir);
#else
	half nl = light.ndotl;
#endif
	half nh = BlinnTerm (normal, halfDir);
	half nv = DotClamped(normal, viewDir);

	half lv = DotClamped (light.dir, viewDir);
	half lh = DotClamped (light.dir, halfDir);

#if UNITY_BRDF_GGX
	half V = SmithJointGGXVisibilityTerm (nl, nv, roughness);
	half D = GGXTerm (nh, roughness);
#else
	half V = SmithBeckmannVisibilityTerm (nl, nv, roughness);
	half D = NDFBlinnPhongNormalizedTerm (nh, RoughnessToSpecPower (roughness));
#endif

	half nlPow5 = Pow5 (1-nl);
	half nvPow5 = Pow5 (1-nv);
	half Fd90 = 0.5 + 2 * lh * lh * roughness;
	half disneyDiffuse = (1 + (Fd90-1) * nlPow5) * (1 + (Fd90-1) * nvPow5);
	
	// HACK: theoretically we should divide by Pi diffuseTerm and not multiply specularTerm!
	// BUT 1) that will make shader look significantly darker than Legacy ones
	// and 2) on engine side "Non-important" lights have to be divided by Pi to in cases when they are injected into ambient SH
	// NOTE: multiplication by Pi is part of single constant together with 1/4 now
	half specularTerm = (V * D) * (UNITY_PI/4); // Torrance-Sparrow model, Fresnel is applied later (for optimization reasons)
	//if (IsGammaSpace())
	//	specularTerm = sqrt(max(1e-4h, specularTerm));
	specularTerm = max(0, specularTerm * nl);


#if defined(_SPECULARHIGHLIGHTS_OFF)
	specularTerm = 0.0;
#endif

	half diffuseTerm = disneyDiffuse * nl;

	// surfaceReduction = Int D(NdotH) * NdotH * Id(NdotL>0) dH = 1/(realRoughness^2+1)
	half realRoughness = roughness*roughness;		// need to square perceptual roughness
	half surfaceReduction;
	if(IsGammaSpace()) surfaceReduction = 1.0-0.28*realRoughness*roughness;		// 1-0.28*x^3 as approximation for (1/(x^4+1))^(1/2.2) on the domain [0;1]
	else surfaceReduction = 1.0 / (realRoughness*realRoughness + 1.0);			// fade \in [0.5;1]
		
	// To provide true Lambert lighting, we need to be able to kill specular completely.
	specularTerm *= any(specColor) ? 1.0 : 0.0;

	half grazingTerm = saturate(oneMinusRoughness + (1-oneMinusReflectivity));
    half3 color =	diffColor * (light.color * diffuseTerm)
                    + specularTerm * light.color * FresnelTerm (specColor, lh)
					+ surfaceReduction * 0.5 * FresnelLerp (specColor, grazingTerm, nv);

	return half4(color, 1);
}

// Based on Minimalist CookTorrance BRDF
// Implementation is slightly different from original derivation: http://www.thetenthplanet.de/archives/255
//
// * BlinnPhong as NDF
// * Modified Kelemen and Szirmay-​Kalos for Visibility term
// * Fresnel approximated with 1/LdotH
half4 BRDF2_Unity_PBS (half3 diffColor, half3 specColor, half oneMinusReflectivity, half oneMinusRoughness,
	half3 normal, half3 viewDir,
	LightInfo light)
{
	half3 halfDir = Unity_SafeNormalize (light.dir + viewDir);

	half nl = light.ndotl;
	half nh = BlinnTerm (normal, halfDir);
	half nv = DotClamped (normal, viewDir);
	half lh = DotClamped (light.dir, halfDir);

	half roughness = 1-oneMinusRoughness;
	half specularPower = RoughnessToSpecPower (roughness);
	// Modified with approximate Visibility function that takes roughness into account
	// Original ((n+1)*N.H^n) / (8*Pi * L.H^3) didn't take into account roughness 
	// and produced extremely bright specular at grazing angles

	// HACK: theoretically we should divide by Pi diffuseTerm and not multiply specularTerm!
	// BUT 1) that will make shader look significantly darker than Legacy ones
	// and 2) on engine side "Non-important" lights have to be divided by Pi to in cases when they are injected into ambient SH
	// NOTE: multiplication by Pi is cancelled with Pi in denominator

	half invV = lh * lh * oneMinusRoughness + roughness * roughness; // approx ModifiedKelemenVisibilityTerm(lh, 1-oneMinusRoughness);
	half invF = lh;
	half specular = ((specularPower + 1) * pow (nh, specularPower)) / (8 * invV * invF + 1e-4h);
	//if (IsGammaSpace())
	//	specular = sqrt(max(1e-4h, specular));

	// surfaceReduction = Int D(NdotH) * NdotH * Id(NdotL>0) dH = 1/(realRoughness^2+1)
	half realRoughness = roughness*roughness;		// need to square perceptual roughness
	
	// 1-0.28*x^3 as approximation for (1/(x^4+1))^(1/2.2) on the domain [0;1]
	// 1-x^3*(0.6-0.08*x)   approximation for 1/(x^4+1)
	//half surfaceReduction = IsGammaSpace() ? 0.28 : (0.6-0.08*roughness);
	half surfaceReduction = (0.6-0.08*roughness);
	surfaceReduction = 1.0 - realRoughness*roughness*surfaceReduction;
	
	// Prevent FP16 overflow on mobiles
#if SHADER_API_GLES || SHADER_API_GLES3
	specular = clamp(specular, 0.0, 100.0);
#endif

#if defined(_SPECULARHIGHLIGHTS_OFF)
	specular = 0.0;
#endif

	half grazingTerm = saturate(oneMinusRoughness + (1-oneMinusReflectivity));
    half3 color =	(diffColor + specular * specColor) * light.color * nl    				
					+ surfaceReduction * 0.5 * FresnelLerpFast (specColor, grazingTerm, nv);

	return half4(color, 1);
}

sampler2D unity_NHxRoughness;
half3 BRDF3_Direct(half3 diffColor, half3 specColor, half rlPow4, half oneMinusRoughness)
{
	half LUT_RANGE = 16.0; // must match range in NHxRoughness() function in GeneratedTextures.cpp
	// Lookup texture to save instructions
	half specular = tex2D(unity_NHxRoughness, half2(rlPow4, 1-oneMinusRoughness)).a * LUT_RANGE;
#if defined(_SPECULARHIGHLIGHTS_OFF)
	specular = 0.0;
#endif

	return diffColor + specular * specColor;
}

// Old school, not microfacet based Modified Normalized Blinn-Phong BRDF
// Implementation uses Lookup texture for performance
//
// * Normalized BlinnPhong in RDF form
// * Implicit Visibility term
// * No Fresnel term
//
// TODO: specular is too weak in Linear rendering mode
half4 BRDF3_Unity_PBS (half3 diffColor, half3 specColor, half oneMinusReflectivity, half oneMinusRoughness,
	half3 normal, half3 viewDir,
	LightInfo light)
{
	half3 reflDir = reflect (viewDir, normal);

	half nl = light.ndotl;
	half nv = DotClamped (normal, viewDir);

	// Vectorize Pow4 to save instructions
	half2 rlPow4AndFresnelTerm = Pow4 (half2(dot(reflDir, light.dir), 1-nv));  // use R.L instead of N.H to save couple of instructions
	half rlPow4 = rlPow4AndFresnelTerm.x; // power exponent must match kHorizontalWarpExp in NHxRoughness() function in GeneratedTextures.cpp
	//half fresnelTerm = rlPow4AndFresnelTerm.y;

	//half grazingTerm = saturate(oneMinusRoughness + (1-oneMinusReflectivity));

	half3 color = BRDF3_Direct(diffColor, specColor, rlPow4, oneMinusRoughness);
	color *= light.color * nl;
	//color += BRDF3_Indirect(diffColor, specColor, gi, grazingTerm, fresnelTerm);

	return half4(color, 1);
}

inline half3 PreMultiplyAlpha (half3 diffColor, half alpha, half oneMinusReflectivity, out half outModifiedAlpha)
{
	#if defined(_ALPHAPREMULTIPLY_ON)
		// NOTE: shader relies on pre-multiply alpha-blend (_SrcBlend = One, _DstBlend = OneMinusSrcAlpha)

		// Transparency 'removes' from Diffuse component
 		diffColor *= alpha;
 		
 		#if (SHADER_TARGET < 30)
 			// SM2.0: instruction count limitation
 			// Instead will sacrifice part of physically based transparency where amount Reflectivity is affecting Transparency
 			// SM2.0: uses unmodified alpha
 			outModifiedAlpha = alpha;
 		#else
	 		// Reflectivity 'removes' from the rest of components, including Transparency
	 		// outAlpha = 1-(1-alpha)*(1-reflectivity) = 1-(oneMinusReflectivity - alpha*oneMinusReflectivity) =
	 		//          = 1-oneMinusReflectivity + alpha*oneMinusReflectivity
	 		outModifiedAlpha = 1-oneMinusReflectivity + alpha*oneMinusReflectivity;
 		#endif
 	#else
 		outModifiedAlpha = alpha;
 	#endif
 	return diffColor;
}

inline half OneMinusReflectivityFromMetallic(half metallic)
{
	// We'll need oneMinusReflectivity, so
	//   1-reflectivity = 1-lerp(dielectricSpec, 1, metallic) = lerp(1-dielectricSpec, 0, metallic)
	// store (1-dielectricSpec) in custom_ColorSpaceDielectricSpec.a, then
	//	 1-reflectivity = lerp(alpha, 0, metallic) = alpha + metallic*(0 - alpha) = 
	//                  = alpha - metallic * alpha
	half oneMinusDielectricSpec = custom_ColorSpaceDielectricSpec.a;
	return oneMinusDielectricSpec - metallic * oneMinusDielectricSpec;
}
	

inline half3 DiffuseAndSpecularFromMetallic (half3 albedo, half metallic, out half3 specColor, out half oneMinusReflectivity)
{
	specColor = lerp (custom_ColorSpaceDielectricSpec.rgb, albedo, metallic);
	oneMinusReflectivity = OneMinusReflectivityFromMetallic(metallic);
	return albedo * oneMinusReflectivity;
}

half _Metallic;
half _Smoothness;
inline half4 LightingStandard (v2f i,half4 c,LightInfo light)
{
	half oneMinusReflectivity;
	half3 specColor;
	c.rgb = DiffuseAndSpecularFromMetallic (c.rgb, _Metallic, /*out*/ specColor, /*out*/ oneMinusReflectivity);

	// shader relies on pre-multiply alpha-blend (_SrcBlend = One, _DstBlend = OneMinusSrcAlpha)
	// this is necessary to handle transparency in physically correct way - only diffuse component gets affected by alpha
	half outputAlpha = 1;
	c.rgb = PreMultiplyAlpha (c.rgb, c.a, oneMinusReflectivity, /*out*/ outputAlpha);

	c = UNITY_BRDF_PBS (c.rgb, specColor, oneMinusReflectivity, _Smoothness, i.normal, i.viewDir, light);
	c.a = outputAlpha;
	return c;
}


v2f vert(a2v v) {  
	v2f o = (v2f)0;  
	  
	o.pos = UnityObjectToClipPos(v.vertex);
	o.uv = v.texcoord;	
#ifdef SKINTEX
	SkinUVMask(o);
#endif//SKINTEX

#ifdef LIGHTON
	//1.calc normal
	o.normal = normalize(mul((float3x3)unity_ObjectToWorld, SCALED_NORMAL));	
	
	//2.calc view
	o.viewDir = WorldSpaceViewDir( v.vertex );
	o.viewDir = normalize(o.viewDir);

	//3. vertex lighting
	o.vertexLighting = fixed3(0,0,0);
	//3.1 SHLight
#ifdef SHLIGHTON
	o.vertexLighting = saturate(ShadeSH9 (normalize(float4(o.normal, 1.0))));
#endif	//SHLIGHTON

#endif //LIGHTON	
	return o;  
} 

#ifdef CUTOUT
half _Cutoff;
#endif //CUTOUT

fixed4 frag(v2f i) : COLOR 
{  
	fixed4 c = BasicColor(i);

#ifdef CUTOUT
	clip(c.a - _Cutoff);
#endif //CUTOUT

	
#ifdef LIGHTON

	//lighting	
	LightInfo light;
	light.color = half3(1,1,1);
	light.dir = normalize(_WorldSpaceLightPos0.xyz);
	light.ndotl = dot(i.normal, light.dir);
	LightingStandard(i,c,light);

#ifdef RIMLIGHT
	half rim = 1.0 - saturate(dot (i.viewDir, i.normal));
	c.rgb += saturate(_RimColor.rgb * pow (rim, _LightArgs.w));
#endif

#endif //LIGHTON

	return c;  
} 
#endif //PBS_INCLUDED