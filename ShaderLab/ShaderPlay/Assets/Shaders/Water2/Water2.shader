// Shader created with Shader Forge v1.37 
// Shader Forge (c) Neat Corporation / Joachim Holmer - http://www.acegikmo.com/shaderforge/
// Note: Manually altering this data may prevent you from opening it in Shader Forge
/*SF_DATA;ver:1.37;sub:START;pass:START;ps:flbk:,iptp:0,cusa:False,bamd:0,cgin:,lico:0,lgpr:1,limd:1,spmd:1,trmd:0,grmd:0,uamb:True,mssp:True,bkdf:True,hqlp:False,rprd:False,enco:False,rmgx:True,imps:False,rpth:0,vtps:0,hqsc:True,nrmq:1,nrsp:0,vomd:0,spxs:False,tesm:0,olmd:1,culm:0,bsrc:3,bdst:7,dpts:2,wrdp:False,dith:0,atcv:False,rfrpo:True,rfrpn:Refraction,coma:15,ufog:False,aust:True,igpj:True,qofs:0,qpre:3,rntp:2,fgom:False,fgoc:False,fgod:False,fgor:False,fgmd:0,fgcr:0.6989619,fgcg:0.9411765,fgcb:0.9111085,fgca:1,fgde:0.0012,fgrn:50,fgrf:80,stcl:False,stva:128,stmr:255,stmw:255,stcp:6,stps:0,stfa:0,stfz:0,ofsf:0,ofsu:0,f2p0:False,fnsp:False,fnfb:False,fsmp:False;n:type:ShaderForge.SFN_Final,id:3138,x:32982,y:32671,varname:node_3138,prsc:2|diff-6435-OUT,alpha-4427-OUT;n:type:ShaderForge.SFN_Color,id:3856,x:32392,y:33027,ptovrint:False,ptlb:Color,ptin:_Color,varname:_Color,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,c1:1,c2:1,c3:1,c4:1;n:type:ShaderForge.SFN_Tex2d,id:2918,x:32379,y:32822,ptovrint:False,ptlb:MainTex,ptin:_MainTex,varname:_MainTex,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,ntxv:0,isnm:False|UVIN-4332-OUT;n:type:ShaderForge.SFN_Multiply,id:6435,x:32669,y:32810,varname:node_6435,prsc:2|A-2918-RGB,B-3856-RGB,C-7505-OUT;n:type:ShaderForge.SFN_TexCoord,id:7175,x:30607,y:33212,varname:node_7175,prsc:2,uv:0,uaff:False;n:type:ShaderForge.SFN_Panner,id:3948,x:31690,y:32570,varname:node_3948,prsc:2,spu:1,spv:0|UVIN-7175-UVOUT,DIST-2568-OUT;n:type:ShaderForge.SFN_Time,id:1072,x:29918,y:32951,varname:node_1072,prsc:2;n:type:ShaderForge.SFN_Slider,id:1785,x:31074,y:32479,ptovrint:False,ptlb:X_SPEED,ptin:_X_SPEED,varname:_X_SPEED,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-20,cur:0.1,max:20;n:type:ShaderForge.SFN_Multiply,id:2568,x:31401,y:32521,varname:node_2568,prsc:2|A-1785-OUT,B-1072-T;n:type:ShaderForge.SFN_ComponentMask,id:1009,x:31920,y:32641,varname:node_1009,prsc:2,cc1:0,cc2:-1,cc3:-1,cc4:-1|IN-3948-UVOUT;n:type:ShaderForge.SFN_Panner,id:1913,x:31690,y:32823,varname:node_1913,prsc:2,spu:0,spv:1|UVIN-7175-UVOUT,DIST-5829-OUT;n:type:ShaderForge.SFN_Multiply,id:5829,x:31401,y:32702,varname:node_5829,prsc:2|A-1072-T,B-475-OUT;n:type:ShaderForge.SFN_Slider,id:475,x:31026,y:32753,ptovrint:False,ptlb:Y_SPEED,ptin:_Y_SPEED,varname:_Y_SPEED,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-20,cur:0.1,max:20;n:type:ShaderForge.SFN_ComponentMask,id:8787,x:31849,y:32885,varname:node_8787,prsc:2,cc1:1,cc2:-1,cc3:-1,cc4:-1|IN-1913-UVOUT;n:type:ShaderForge.SFN_Append,id:7027,x:32009,y:32842,varname:node_7027,prsc:2|A-1009-OUT,B-8787-OUT;n:type:ShaderForge.SFN_Slider,id:4427,x:32266,y:33236,ptovrint:False,ptlb:TM,ptin:_TM,varname:_TM,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:0.4993823,max:1;n:type:ShaderForge.SFN_Tex2d,id:4648,x:31898,y:33595,ptovrint:False,ptlb:tex2,ptin:_tex2,varname:_tex2,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,tex:16beba1ce842da8439ec22b0c91587fa,ntxv:0,isnm:False|UVIN-1771-OUT;n:type:ShaderForge.SFN_Add,id:4332,x:32135,y:33042,varname:node_4332,prsc:2|A-7027-OUT,B-4365-OUT;n:type:ShaderForge.SFN_Panner,id:256,x:31182,y:33436,varname:node_256,prsc:2,spu:1,spv:0|UVIN-7175-UVOUT,DIST-14-OUT;n:type:ShaderForge.SFN_Panner,id:6716,x:31179,y:33679,varname:node_6716,prsc:2,spu:0,spv:1|UVIN-7175-UVOUT,DIST-8695-OUT;n:type:ShaderForge.SFN_Multiply,id:14,x:30691,y:33462,varname:node_14,prsc:2|A-1072-T,B-2890-OUT;n:type:ShaderForge.SFN_Multiply,id:8695,x:30729,y:33661,varname:node_8695,prsc:2|A-1072-T,B-8581-OUT;n:type:ShaderForge.SFN_Slider,id:2890,x:30270,y:33484,ptovrint:False,ptlb:X,ptin:_X,varname:_X,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-1,cur:0.05,max:1;n:type:ShaderForge.SFN_Slider,id:8581,x:30319,y:33688,ptovrint:False,ptlb:Y,ptin:_Y,varname:_Y,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-1,cur:0.01,max:1;n:type:ShaderForge.SFN_ComponentMask,id:8903,x:31414,y:33467,varname:node_8903,prsc:2,cc1:0,cc2:-1,cc3:-1,cc4:-1|IN-256-UVOUT;n:type:ShaderForge.SFN_ComponentMask,id:8322,x:31385,y:33690,varname:node_8322,prsc:2,cc1:1,cc2:-1,cc3:-1,cc4:-1|IN-6716-UVOUT;n:type:ShaderForge.SFN_Append,id:1771,x:31599,y:33598,varname:node_1771,prsc:2|A-8903-OUT,B-8322-OUT;n:type:ShaderForge.SFN_Multiply,id:4365,x:32208,y:33733,varname:node_4365,prsc:2|A-4648-R,B-8369-OUT,C-5016-R;n:type:ShaderForge.SFN_ValueProperty,id:8369,x:31898,y:33830,ptovrint:False,ptlb:node_8369,ptin:_node_8369,varname:_node_8369,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,v1:1;n:type:ShaderForge.SFN_Panner,id:5518,x:31179,y:33904,varname:node_5518,prsc:2,spu:1,spv:0|UVIN-7175-UVOUT,DIST-5540-OUT;n:type:ShaderForge.SFN_Panner,id:5375,x:31182,y:34178,varname:node_5375,prsc:2,spu:0,spv:1|UVIN-7175-UVOUT,DIST-1060-OUT;n:type:ShaderForge.SFN_Multiply,id:5540,x:30729,y:33903,varname:node_5540,prsc:2|A-1072-T,B-2231-OUT;n:type:ShaderForge.SFN_Multiply,id:1060,x:30729,y:34113,varname:node_1060,prsc:2|A-1072-T,B-1197-OUT;n:type:ShaderForge.SFN_Slider,id:2231,x:30377,y:33920,ptovrint:False,ptlb:X_copy,ptin:_X_copy,varname:_X_copy,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-1,cur:-0.05,max:1;n:type:ShaderForge.SFN_Slider,id:1197,x:30357,y:34122,ptovrint:False,ptlb:Y_copy,ptin:_Y_copy,varname:_Y_copy,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-1,cur:-0.01,max:1;n:type:ShaderForge.SFN_ComponentMask,id:2335,x:31460,y:33927,varname:node_2335,prsc:2,cc1:0,cc2:-1,cc3:-1,cc4:-1|IN-5518-UVOUT;n:type:ShaderForge.SFN_ComponentMask,id:6607,x:31431,y:34150,varname:node_6607,prsc:2,cc1:1,cc2:-1,cc3:-1,cc4:-1|IN-5375-UVOUT;n:type:ShaderForge.SFN_Append,id:967,x:31645,y:34058,varname:node_967,prsc:2|A-2335-OUT,B-6607-OUT;n:type:ShaderForge.SFN_Tex2d,id:5016,x:31898,y:34002,ptovrint:False,ptlb:text3,ptin:_text3,varname:_text3,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,tex:188d4743867c70a4094811f4464147cf,ntxv:0,isnm:False|UVIN-967-OUT;n:type:ShaderForge.SFN_ValueProperty,id:7505,x:32379,y:32711,ptovrint:False,ptlb:QD,ptin:_QD,varname:_QD,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,v1:1;proporder:3856-4427-1785-475-2918-2890-8581-8369-2231-1197-4648-5016-7505;pass:END;sub:END;*/

Shader "Shader Forge/U_Y_AP_1" {
    Properties {
        _Color ("Color", Color) = (1,1,1,1)
        _TM ("TM", Range(0, 1)) = 0.4993823
        _X_SPEED ("X_SPEED", Range(-20, 20)) = 0.1
        _Y_SPEED ("Y_SPEED", Range(-20, 20)) = 0.1
        _MainTex ("MainTex", 2D) = "white" {}
        _X ("X", Range(-1, 1)) = 0.05
        _Y ("Y", Range(-1, 1)) = 0.01
        _node_8369 ("node_8369", Float ) = 1
        _X_copy ("X_copy", Range(-1, 1)) = -0.05
        _Y_copy ("Y_copy", Range(-1, 1)) = -0.01
        _tex2 ("tex2", 2D) = "white" {}
        _text3 ("text3", 2D) = "white" {}
        _QD ("QD", Float ) = 1
        [HideInInspector]_Cutoff ("Alpha cutoff", Range(0,1)) = 0.5
    }
    SubShader {
        Tags {
            "IgnoreProjector"="True"
            "Queue"="Transparent"
            "RenderType"="Transparent"
        }
        Pass {
            Name "FORWARD"
            Tags {
                "LightMode"="ForwardBase"
            }
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #define UNITY_PASS_FORWARDBASE
            #define SHOULD_SAMPLE_SH ( defined (LIGHTMAP_OFF) && defined(DYNAMICLIGHTMAP_OFF) )
            #include "UnityCG.cginc"
            #include "Lighting.cginc"
            #include "UnityPBSLighting.cginc"
            #include "UnityStandardBRDF.cginc"
            #pragma multi_compile_fwdbase
            #pragma multi_compile LIGHTMAP_OFF LIGHTMAP_ON
            #pragma multi_compile DIRLIGHTMAP_OFF DIRLIGHTMAP_COMBINED DIRLIGHTMAP_SEPARATE
            #pragma multi_compile DYNAMICLIGHTMAP_OFF DYNAMICLIGHTMAP_ON
            #pragma only_renderers d3d9 d3d11 glcore gles 
            #pragma target 3.0
            uniform float4 _TimeEditor;
            uniform float4 _Color;
            uniform sampler2D _MainTex; uniform float4 _MainTex_ST;
            uniform float _X_SPEED;
            uniform float _Y_SPEED;
            uniform float _TM;
            uniform sampler2D _tex2; uniform float4 _tex2_ST;
            uniform float _X;
            uniform float _Y;
            uniform float _node_8369;
            uniform float _X_copy;
            uniform float _Y_copy;
            uniform sampler2D _text3; uniform float4 _text3_ST;
            uniform float _QD;
            struct VertexInput {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float4 tangent : TANGENT;
                float2 texcoord0 : TEXCOORD0;
                float2 texcoord1 : TEXCOORD1;
                float2 texcoord2 : TEXCOORD2;
            };
            struct VertexOutput {
                float4 pos : SV_POSITION;
                float2 uv0 : TEXCOORD0;
                float2 uv1 : TEXCOORD1;
                float2 uv2 : TEXCOORD2;
                float4 posWorld : TEXCOORD3;
                float3 normalDir : TEXCOORD4;
                float3 tangentDir : TEXCOORD5;
                float3 bitangentDir : TEXCOORD6;
                #if defined(LIGHTMAP_ON) || defined(UNITY_SHOULD_SAMPLE_SH)
                    float4 ambientOrLightmapUV : TEXCOORD7;
                #endif
            };
            VertexOutput vert (VertexInput v) {
                VertexOutput o = (VertexOutput)0;
                o.uv0 = v.texcoord0;
                o.uv1 = v.texcoord1;
                o.uv2 = v.texcoord2;
                #ifdef LIGHTMAP_ON
                    o.ambientOrLightmapUV.xy = v.texcoord1.xy * unity_LightmapST.xy + unity_LightmapST.zw;
                    o.ambientOrLightmapUV.zw = 0;
                #elif UNITY_SHOULD_SAMPLE_SH
                #endif
                #ifdef DYNAMICLIGHTMAP_ON
                    o.ambientOrLightmapUV.zw = v.texcoord2.xy * unity_DynamicLightmapST.xy + unity_DynamicLightmapST.zw;
                #endif
                o.normalDir = UnityObjectToWorldNormal(v.normal);
                o.tangentDir = normalize( mul( unity_ObjectToWorld, float4( v.tangent.xyz, 0.0 ) ).xyz );
                o.bitangentDir = normalize(cross(o.normalDir, o.tangentDir) * v.tangent.w);
                o.posWorld = mul(unity_ObjectToWorld, v.vertex);
                float3 lightColor = _LightColor0.rgb;
                o.pos = UnityObjectToClipPos( v.vertex );
                return o;
            }
            float4 frag(VertexOutput i) : COLOR {
                i.normalDir = normalize(i.normalDir);
                float3x3 tangentTransform = float3x3( i.tangentDir, i.bitangentDir, i.normalDir);
                float3 viewDirection = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);
                float3 normalDirection = i.normalDir;
                float3 viewReflectDirection = reflect( -viewDirection, normalDirection );
                float3 lightDirection = normalize(_WorldSpaceLightPos0.xyz);
                float3 lightColor = _LightColor0.rgb;
////// Lighting:
                float attenuation = 1;
                float3 attenColor = attenuation * _LightColor0.xyz;
/////// GI Data:
                UnityLight light;
                #ifdef LIGHTMAP_OFF
                    light.color = lightColor;
                    light.dir = lightDirection;
                    light.ndotl = LambertTerm (normalDirection, light.dir);
                #else
                    light.color = half3(0.f, 0.f, 0.f);
                    light.ndotl = 0.0f;
                    light.dir = half3(0.f, 0.f, 0.f);
                #endif
                UnityGIInput d;
                d.light = light;
                d.worldPos = i.posWorld.xyz;
                d.worldViewDir = viewDirection;
                d.atten = attenuation;
                #if defined(LIGHTMAP_ON) || defined(DYNAMICLIGHTMAP_ON)
                    d.ambient = 0;
                    d.lightmapUV = i.ambientOrLightmapUV;
                #else
                    d.ambient = i.ambientOrLightmapUV;
                #endif
                Unity_GlossyEnvironmentData ugls_en_data;
                ugls_en_data.roughness = 1.0 - 0;
                ugls_en_data.reflUVW = viewReflectDirection;
                UnityGI gi = UnityGlobalIllumination(d, 1, normalDirection, ugls_en_data );
                lightDirection = gi.light.dir;
                lightColor = gi.light.color;
/////// Diffuse:
                float NdotL = max(0.0,dot( normalDirection, lightDirection ));
                float3 directDiffuse = max( 0.0, NdotL) * attenColor;
                float3 indirectDiffuse = float3(0,0,0);
                indirectDiffuse += gi.indirect.diffuse;
                float4 node_1072 = _Time + _TimeEditor;
                float2 node_1771 = float2((i.uv0+(node_1072.g*_X)*float2(1,0)).r,(i.uv0+(node_1072.g*_Y)*float2(0,1)).g);
                float4 _tex2_var = tex2D(_tex2,TRANSFORM_TEX(node_1771, _tex2));
                float2 node_967 = float2((i.uv0+(node_1072.g*_X_copy)*float2(1,0)).r,(i.uv0+(node_1072.g*_Y_copy)*float2(0,1)).g);
                float4 _text3_var = tex2D(_text3,TRANSFORM_TEX(node_967, _text3));
                float2 node_4332 = (float2((i.uv0+(_X_SPEED*node_1072.g)*float2(1,0)).r,(i.uv0+(node_1072.g*_Y_SPEED)*float2(0,1)).g)+(_tex2_var.r*_node_8369*_text3_var.r));
                float4 _MainTex_var = tex2D(_MainTex,TRANSFORM_TEX(node_4332, _MainTex));
                float3 diffuseColor = (_MainTex_var.rgb*_Color.rgb*_QD);
                float3 diffuse = (directDiffuse + indirectDiffuse) * diffuseColor;
/// Final Color:
                float3 finalColor = diffuse;
                return fixed4(finalColor,_TM);
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
    CustomEditor "ShaderForgeMaterialInspector"
}
