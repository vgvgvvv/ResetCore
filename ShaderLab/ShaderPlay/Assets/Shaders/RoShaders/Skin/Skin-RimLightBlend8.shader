Shader "Custom/Skin/RimlightBlend8" 
{  
    Properties 
	{  
		_Tex0("Texture0", 2D) = "black" {}		
		_Tex1("Texture1", 2D) = "black" {}
		_Tex2("Texture2", 2D) = "black" {}			
		_Tex3("Texture3", 2D) = "black" {}		
		_Tex4("Texture4", 2D) = "black" {}		
		_Tex5("Texture5", 2D) = "black" {}		
		_Tex6("Texture6", 2D) = "black" {}		
		_Tex7("Texture7", 2D) = "black" {}
			
		_HairColor("Hair Color", Color) = (1, 1, 1, 1)
		_EyesColor("Eyes Color", Color) = (1, 1, 1, 1)
		_Color ("Additive Color", Color) = (1, 1, 1, 1)

		//伪光源
		[Space]
		_MainLightColor ("主光源颜色", Color) = (0,0,0,0.5)//主光源颜色
        _MainLightPower ("主光源强度", Float ) = 0//主光源强度
        _LightAreaColor ("亮部颜色", Color) = (0,0,0,1)//亮部颜色
        _DarkAreaColor ("暗部颜色", Color) = (0,0,0,0.5)//暗部颜色
        _MainLightRange ("主光源范围", Vector) = (1,1,1,0)//主光源范围
        _LightLightRange ("亮部范围", Vector) = (1,1,1,0)//亮部范围
        _DarkLightRange ("暗部范围", Vector) = (1,1,1,0)//暗部范围
        _attenYPower ("主光源衰弱强度", Range(1, 5)) = 2//衰弱强度
        _attenYOffset ("主光源衰弱偏移", Range(-5, 5)) = 0//衰弱偏移

		//对比度
		[Space]
		_DBD ("对比度", Float ) = 1

		//光照探针
		// [Space]
		// _GZTZ ("GZTZ", Range(0, 5)) = 0

		//描边
		[Space]
		_OutlineColor ("Outline Color", Color) = (0.25, 0.18, 0.15, 1)
		_OutlineFactor("Factor",range(0,1)) = 0.5
		_Outline("Thick of Outline",range(0,0.1)) = 0.02

		//眼睛
		[Space]
		_IsCloseEyes("Close Eyes",Int)=0
		_EyesOffset("Eyes Offset",Vector)=(0,0,0,0)

		_IsChangeEmotion("Change Emotion",Int) = 0
    }  
    SubShader 
	{  
		Tags { "RenderType" = "Opaque" }
        LOD 200            
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
			#include "../Include/ROOutLine_Include.cginc"
			ENDCG
		}

        Pass 
		{  
			Name "Basic"
			// Tags { "LightMode"="ForwardBase" }  
			Blend SrcAlpha  OneMinusSrcAlpha
            Cull Back
			Lighting On
            CGPROGRAM 

			//vertex&fragment
			#pragma vertex vert
            #pragma fragment frag 

			//预处理
			// #define UNITY_PASS_FORWARDBASE
            // #define SHOULD_SAMPLE_SH ( defined (LIGHTMAP_OFF) && defined(DYNAMICLIGHTMAP_OFF) )

             #include "UnityCG.cginc"
            // #include "AutoLight.cginc"
            // #include "Lighting.cginc"
            // #include "UnityPBSLighting.cginc"
            // #include "UnityStandardBRDF.cginc"

            // #pragma multi_compile_fwdbase_fullshadows
            // #pragma multi_compile LIGHTMAP_OFF LIGHTMAP_ON
            // #pragma multi_compile DIRLIGHTMAP_OFF DIRLIGHTMAP_COMBINED DIRLIGHTMAP_SEPARATE
            // #pragma multi_compile DYNAMICLIGHTMAP_OFF DYNAMICLIGHTMAP_ON
            // #pragma only_renderers d3d9 d3d11 glcore gles metal 
            // #pragma target 3.0

			//自定义结构体
			#define UV2
			#define VERT_TANGENT
			#define CUSTOM_A2V float2 texcoord2 : TEXCOORD2;

			#define FRAG_NONORMAL
			#define CUSTOM_V2F float4 posWorld : TEXCOORD3;\
				float3 normalDir : TEXCOORD4;\
                float3 tangentDir : TEXCOORD5;\
                float3 bitangentDir : TEXCOORD6;

			//head
			#include "../Include/ROCommonHead_Include.cginc"

			#define SKINTEX
			#define SKINTEX8
			#include "../Include/ROSkinBlend_Include.cginc"

			#define GrayControl
			#define FakeLight
			#define ColorContrast
			#include "../Include/ROColorEffect_Include.cginc"

			// float _GZTZ;

			v2f CustomVert(a2v v, v2f o){

                o.uv0 = v.texcoord0;//

                // #ifdef LIGHTMAP_ON
                //     o.ambientOrLightmapUV.xy = v.texcoord1.xy * unity_LightmapST.xy + unity_LightmapST.zw;
                //     o.ambientOrLightmapUV.zw = 0;
                // #elif UNITY_SHOULD_SAMPLE_SH
                // #endif
                // #ifdef DYNAMICLIGHTMAP_ON
                //     o.ambientOrLightmapUV.zw = v.texcoord2.xy * unity_DynamicLightmapST.xy + unity_DynamicLightmapST.zw;
                // #endif

                 o.normalDir = UnityObjectToWorldNormal(v.normal);
                // o.tangentDir = normalize( mul( unity_ObjectToWorld, float4( v.tangent.xyz, 0.0 ) ).xyz );
                // o.bitangentDir = normalize(cross(o.normalDir, o.tangentDir) * v.tangent.w);
                o.posWorld = mul(unity_ObjectToWorld, v.vertex);
                // o.pos = UnityObjectToClipPos( v.vertex );
                // TRANSFER_VERTEX_TO_FRAGMENT(o);
				return o;
			}

			fixed4 _Color;				
			//custom frag fun
			fixed4 CustomFrag(in v2f i)
			{
                i.normalDir = normalize(i.normalDir);
                // float3 viewDirection = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);
                // float3 normalDirection = i.normalDir;
                // float3 viewReflectDirection = reflect( -viewDirection, normalDirection );
                // float3 lightDirection = normalize(_WorldSpaceLightPos0.xyz);
                // float3 lightColor = _LightColor0.rgb;
////// Lighting:
                // float attenuation = LIGHT_ATTENUATION(i);
                // float3 attenColor = attenuation * _LightColor0.xyz;
/////// GI Data:
                // UnityLight light;
                // #ifdef LIGHTMAP_OFF
                //     light.color = lightColor;
                //     light.dir = lightDirection;
                //     light.ndotl = LambertTerm (normalDirection, light.dir);
                // #else
                //     light.color = half3(0.f, 0.f, 0.f);
                //     light.ndotl = 0.0f;
                //     light.dir = half3(0.f, 0.f, 0.f);
                // #endif
                // UnityGIInput d;
                // d.light = light;
                // d.worldPos = i.posWorld.xyz;
                // d.worldViewDir = viewDirection;
                // d.atten = attenuation;
                // #if defined(LIGHTMAP_ON) || defined(DYNAMICLIGHTMAP_ON)
                //     d.ambient = 0;
                //     d.lightmapUV = i.ambientOrLightmapUV;
                // #else
                //     d.ambient = i.ambientOrLightmapUV;
                // #endif
                // Unity_GlossyEnvironmentData ugls_en_data;
                // ugls_en_data.roughness = 1.0 - 0;
                // ugls_en_data.reflUVW = viewReflectDirection;
                // UnityGI gi = UnityGlobalIllumination(d, 1, normalDirection, ugls_en_data );
                // lightDirection = gi.light.dir;
                // lightColor = gi.light.color;
//////// GI Data End

///////Emission Data
				fixed4 c = BlendColor(i);

				//c.rgb = ApplyGrayControl(c.rgb);
				c.rgb = ApplyFakeLight(i, c.rgb);
				c.rgb = ApplyContrast(c.rgb);
				c.rgb *= _Color.rgb;
// ////////EmissionDataEnd

// ///////Diffuse Data
// 				float NdotL = max(0.0,dot( normalDirection, lightDirection ));
//                 float3 directDiffuse = max( 0.0, NdotL) * attenColor;
//                 float3 indirectDiffuse = float3(0,0,0);
//                 indirectDiffuse += gi.indirect.diffuse;

// 				float3 diffuseColor = ((float3(1,1,1)*_GZTZ)*lerp(c,dot(c,float3(0.3,0.59,0.11)),1.0));
//                 float3 diffuse = (directDiffuse + indirectDiffuse) * diffuseColor;
// ///////Diffuse Data End

// 				float3 finalColor = diffuse + c;

				return fixed4(c.rgb, 1);
			}

			#include "UnityCG.cginc"
            #include "../Include/ROLightingBasic_Include.cginc"
			
            ENDCG
		}

	}
}  