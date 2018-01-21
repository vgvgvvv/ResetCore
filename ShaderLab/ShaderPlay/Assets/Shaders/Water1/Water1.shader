// Shader created with Shader Forge v1.37 
// Shader Forge (c) Neat Corporation / Joachim Holmer - http://www.acegikmo.com/shaderforge/
// Note: Manually altering this data may prevent you from opening it in Shader Forge
/*SF_DATA;ver:1.37;sub:START;pass:START;ps:flbk:,iptp:0,cusa:False,bamd:0,cgin:,lico:1,lgpr:1,limd:0,spmd:1,trmd:0,grmd:0,uamb:True,mssp:True,bkdf:False,hqlp:False,rprd:False,enco:False,rmgx:True,imps:True,rpth:0,vtps:0,hqsc:True,nrmq:1,nrsp:0,vomd:0,spxs:False,tesm:0,olmd:1,culm:2,bsrc:3,bdst:7,dpts:2,wrdp:False,dith:0,atcv:False,rfrpo:True,rfrpn:Refraction,coma:15,ufog:False,aust:True,igpj:True,qofs:0,qpre:3,rntp:2,fgom:False,fgoc:False,fgod:False,fgor:False,fgmd:0,fgcr:0.2346453,fgcg:0.6315774,fgcb:0.9117647,fgca:1,fgde:0.0003,fgrn:-69.21,fgrf:1365,stcl:False,stva:128,stmr:255,stmw:255,stcp:6,stps:0,stfa:0,stfz:0,ofsf:0,ofsu:0,f2p0:False,fnsp:False,fnfb:False,fsmp:False;n:type:ShaderForge.SFN_Final,id:3138,x:33102,y:32645,varname:node_3138,prsc:2|emission-203-OUT,alpha-1699-OUT;n:type:ShaderForge.SFN_ScreenPos,id:1935,x:31328,y:33030,varname:node_1935,prsc:2,sctp:2;n:type:ShaderForge.SFN_Multiply,id:203,x:32494,y:32886,varname:node_203,prsc:2|A-8891-RGB,B-4441-OUT,C-6700-OUT;n:type:ShaderForge.SFN_Color,id:8891,x:32278,y:32766,ptovrint:False,ptlb:AddtiveColor,ptin:_AddtiveColor,varname:_node_8891,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,c1:1,c2:1,c3:1,c4:1;n:type:ShaderForge.SFN_TexCoord,id:1214,x:30521,y:32512,varname:node_1214,prsc:2,uv:0,uaff:False;n:type:ShaderForge.SFN_Panner,id:2039,x:30881,y:32621,varname:node_2039,prsc:2,spu:1,spv:0|UVIN-1214-UVOUT,DIST-3110-OUT;n:type:ShaderForge.SFN_Slider,id:785,x:30424,y:32775,ptovrint:False,ptlb:SPEED,ptin:_SPEED,cmnt:流动速度,varname:_SPEED,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:1.059829,max:2;n:type:ShaderForge.SFN_Time,id:2476,x:30487,y:32907,varname:node_2476,prsc:2;n:type:ShaderForge.SFN_Multiply,id:3110,x:30744,y:32831,varname:node_3110,prsc:2|A-785-OUT,B-2476-T;n:type:ShaderForge.SFN_Tex2d,id:4451,x:31085,y:32500,ptovrint:False,ptlb:disturbance,ptin:_disturbance,cmnt:扰动,varname:_1,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,tex:42da42ce397534e488066298516a599e,ntxv:0,isnm:False|UVIN-2039-UVOUT;n:type:ShaderForge.SFN_Tex2d,id:7000,x:31944,y:32993,ptovrint:False,ptlb:environmentMap,ptin:_environmentMap,varname:_22,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,tex:0dd283584e40a9a4aa32b94ff3ff1bcf,ntxv:0,isnm:False|UVIN-2319-OUT;n:type:ShaderForge.SFN_Append,id:6446,x:31638,y:32755,varname:node_6446,prsc:2|A-9057-OUT,B-9057-OUT;n:type:ShaderForge.SFN_Multiply,id:9057,x:31459,y:32632,varname:node_9057,prsc:2|A-4451-R,B-278-OUT;n:type:ShaderForge.SFN_Slider,id:278,x:31140,y:32774,ptovrint:False,ptlb:NQ,ptin:_NQ,varname:_NQ,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:0.5897436,max:1;n:type:ShaderForge.SFN_Add,id:2319,x:31719,y:32938,varname:node_2319,prsc:2|A-6446-OUT,B-1935-UVOUT;n:type:ShaderForge.SFN_Slider,id:6700,x:32144,y:33188,ptovrint:False,ptlb:QD,ptin:_QD,varname:_QD,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:1,max:2;n:type:ShaderForge.SFN_Add,id:4441,x:32301,y:32993,varname:node_4441,prsc:2|A-7000-RGB,B-8561-OUT;n:type:ShaderForge.SFN_Multiply,id:8561,x:31881,y:32361,varname:node_8561,prsc:2|A-4451-RGB,B-3312-OUT,C-9686-OUT;n:type:ShaderForge.SFN_Slider,id:3312,x:31452,y:32260,ptovrint:False,ptlb:FresmelPower,ptin:_FresmelPower,varname:_node_3312,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:2,max:2;n:type:ShaderForge.SFN_Fresnel,id:9686,x:31187,y:32325,varname:node_9686,prsc:2|NRM-3608-OUT,EXP-9659-OUT;n:type:ShaderForge.SFN_Slider,id:9659,x:30507,y:32433,ptovrint:False,ptlb:QX,ptin:_QX,varname:_QX,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:-2,cur:2.862361,max:10;n:type:ShaderForge.SFN_NormalVector,id:3608,x:30548,y:32248,prsc:2,pt:True;n:type:ShaderForge.SFN_Slider,id:6924,x:32546,y:33242,ptovrint:False,ptlb:TM,ptin:_TM,varname:_TM,prsc:2,glob:False,taghide:False,taghdr:False,tagprd:False,tagnsco:False,tagnrm:False,min:0,cur:1,max:1;n:type:ShaderForge.SFN_ComponentMask,id:1263,x:32637,y:32988,varname:node_1263,prsc:2,cc1:0,cc2:-1,cc3:-1,cc4:-1|IN-203-OUT;n:type:ShaderForge.SFN_Multiply,id:1699,x:32863,y:33018,varname:node_1699,prsc:2|A-1263-OUT,B-6924-OUT;proporder:8891-785-4451-7000-278-6700-3312-9659-6924;pass:END;sub:END;*/

Shader "Shader Forge/Water1" {
    Properties {
        _AddtiveColor ("AddtiveColor", Color) = (1,1,1,1)
        _SPEED ("SPEED", Range(0, 2)) = 1.059829
        _disturbance ("disturbance", 2D) = "white" {}
        _environmentMap ("environmentMap", 2D) = "white" {}
        _NQ ("NQ", Range(0, 1)) = 0.5897436
        _QD ("QD", Range(0, 2)) = 1
        _FresmelPower ("FresmelPower", Range(0, 2)) = 2
        _QX ("QX", Range(-2, 10)) = 2.862361
        _TM ("TM", Range(0, 1)) = 1
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
            Cull Off
            ZWrite Off
            
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #define UNITY_PASS_FORWARDBASE
            #include "UnityCG.cginc"
            #pragma multi_compile_fwdbase
            #pragma only_renderers d3d9 d3d11 glcore gles metal 
            #pragma target 3.0
            uniform float4 _TimeEditor;
            uniform float4 _AddtiveColor;
            uniform float _SPEED;
            uniform sampler2D _disturbance; uniform float4 _disturbance_ST;
            uniform sampler2D _environmentMap; uniform float4 _environmentMap_ST;
            uniform float _NQ;
            uniform float _QD;
            uniform float _FresmelPower;
            uniform float _QX;
            uniform float _TM;
            struct VertexInput {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 texcoord0 : TEXCOORD0;
            };
            struct VertexOutput {
                float4 pos : SV_POSITION;
                float2 uv0 : TEXCOORD0;
                float4 posWorld : TEXCOORD1;
                float3 normalDir : TEXCOORD2;
                float4 screenPos : TEXCOORD3;
            };
            VertexOutput vert (VertexInput v) {
                VertexOutput o = (VertexOutput)0;
                o.uv0 = v.texcoord0;
                o.normalDir = UnityObjectToWorldNormal(v.normal);
                o.posWorld = mul(unity_ObjectToWorld, v.vertex);
                o.pos = UnityObjectToClipPos( v.vertex );
                o.screenPos = o.pos;
                return o;
            }
            float4 frag(VertexOutput i, float facing : VFACE) : COLOR {
                float isFrontFace = ( facing >= 0 ? 1 : 0 );
                float faceSign = ( facing >= 0 ? 1 : -1 );
                #if UNITY_UV_STARTS_AT_TOP
                    float grabSign = -_ProjectionParams.x;
                #else
                    float grabSign = _ProjectionParams.x;
                #endif
                i.normalDir = normalize(i.normalDir);
                i.normalDir *= faceSign;
                i.screenPos = float4( i.screenPos.xy / i.screenPos.w, 0, 0 );
                i.screenPos.y *= _ProjectionParams.x;
                float3 viewDirection = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);
                float3 normalDirection = i.normalDir;
                float2 sceneUVs = float2(1,grabSign)*i.screenPos.xy*0.5+0.5;
////// Lighting:
////// Emissive:
                float4 node_2476 = _Time + _TimeEditor;
                float2 node_2039 = (i.uv0+(_SPEED*node_2476.g)*float2(1,0));
                float4 _disturbance_var = tex2D(_disturbance,TRANSFORM_TEX(node_2039, _disturbance)); // 扰动
                float node_9057 = (_disturbance_var.r*_NQ);
                float2 node_2319 = (float2(node_9057,node_9057)+sceneUVs.rg);
                float4 _environmentMap_var = tex2D(_environmentMap,TRANSFORM_TEX(node_2319, _environmentMap));
                float node_9686 = pow(1.0-max(0,dot(normalDirection, viewDirection)),_QX);
                float3 node_203 = (_AddtiveColor.rgb*(_environmentMap_var.rgb+(_disturbance_var.rgb*_FresmelPower*node_9686))*_QD);
                float3 emissive = node_203;
                float3 finalColor = emissive;
                return fixed4(finalColor,(node_203.r*_TM));
            }
            ENDCG
        }
        Pass {
            Name "ShadowCaster"
            Tags {
                "LightMode"="ShadowCaster"
            }
            Offset 1, 1
            Cull Off
            
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #define UNITY_PASS_SHADOWCASTER
            #include "UnityCG.cginc"
            #include "Lighting.cginc"
            #pragma fragmentoption ARB_precision_hint_fastest
            #pragma multi_compile_shadowcaster
            #pragma only_renderers d3d9 d3d11 glcore gles metal 
            #pragma target 3.0
            struct VertexInput {
                float4 vertex : POSITION;
            };
            struct VertexOutput {
                V2F_SHADOW_CASTER;
            };
            VertexOutput vert (VertexInput v) {
                VertexOutput o = (VertexOutput)0;
                o.pos = UnityObjectToClipPos( v.vertex );
                TRANSFER_SHADOW_CASTER(o)
                return o;
            }
            float4 frag(VertexOutput i, float facing : VFACE) : COLOR {
                float isFrontFace = ( facing >= 0 ? 1 : 0 );
                float faceSign = ( facing >= 0 ? 1 : -1 );
                SHADOW_CASTER_FRAGMENT(i)
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
    CustomEditor "ShaderForgeMaterialInspector"
}
