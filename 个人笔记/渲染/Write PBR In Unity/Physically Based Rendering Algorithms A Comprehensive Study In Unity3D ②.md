# Physically Based Rendering Algorithms: A Comprehensive Study In Unity3D ②
#读书笔记/渲染/Write PBR In Unity#

## 编写一个PBR Shader：光滑平面
### PBR Shader的属性有哪些
在大多数的PBR光照模型中，我们可以看到几乎有着相同的属性来影响他们的渲染。现代PBR Shader中最常见的两种属性分类是光滑度（或者粗糙度）以及金属度。这两种属性都是在0~1之间进行取值。有很多方式去写不同的PBR Shader，有些Shader可以使用BRDF产生更多的效果，例如[迪士尼的PBR管线](http://blog.selfshadow.com/publications/s2012-shading-course/burley/s2012_pbs_disney_brdf_slides_v2.pdf)，每一种效果都由一个特定的属性进行影响。
让我们开始把这些属性在Shader中写出来，如果你还没有看过我之前写的[Writing Shaders in Unity](http://www.jordanstevenstechart.com/writing-shaders-in-unity)，现在是时候去从头读一下了。

```
Shader "Physically-Based-Lighting" {
    Properties { 
    _Color ("Main Color", Color) = (1,1,1,1)                    //diffuse Color
    _SpecularColor ("Specular Color", Color) = (1,1,1,1)        //Specular Color (Not Used)
    _Glossiness("Smoothness",Range(0,1)) = 1                    //My Smoothness
    _Metallic("Metalness",Range(0,1)) = 0                    //My Metal Value      

    }
    SubShader {
	Tags {
            "RenderType"="Opaque"  "Queue"="Geometry"
        } 
        Pass {
            Name "FORWARD"
            Tags {
                "LightMode"="ForwardBase"
            }
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #define UNITY_PASS_FORWARDBASE
            #include "UnityCG.cginc"
            #include "AutoLight.cginc"
            #include "Lighting.cginc"
            #pragma multi_compile_fwdbase_fullshadows      
            #pragma target 3.0

float4 _Color;
float4 _SpecularColor;
float _Glossiness;
float _Metallic;
```

在这我们在UnityShader中定义了我们的公开属性。我们会在后面加上更多的属性，这是一个好的开头。在属性的下方是我们Shader的初始化结构。我们会使用#pragma指令进行引用，让我们在后面的函数中加上更多的功能。

### 顶点程序
我们的顶点程序非常类似于我在[Writing Shaders in Unity](http://www.jordanstevenstechart.com/writing-shaders-in-unity)中进行编写的顶点程序。在我们的顶点中我们会用到法线（normal）、切线（tangent）、以及副切线（bitangent）信息，所以我们在程序中也会有所体现。
```
struct VertexInput {
    float4 vertex : POSITION;       //local vertex position
    float3 normal : NORMAL;         //normal direction
    float4 tangent : TANGENT;       //tangent direction    
    float2 texcoord0 : TEXCOORD0;   //uv coordinates
    float2 texcoord1 : TEXCOORD1;   //lightmap uv coordinates
};

struct VertexOutput {
    float4 pos : SV_POSITION;              //screen clip space position and depth
    float2 uv0 : TEXCOORD0;                //uv coordinates
    float2 uv1 : TEXCOORD1;                //lightmap uv coordinates

//below we create our own variables with the texcoord semantic. 
    float3 normalDir : TEXCOORD3;          //normal direction   
    float3 posWorld : TEXCOORD4;          //normal direction   
    float3 tangentDir : TEXCOORD5;
    float3 bitangentDir : TEXCOORD6;
    LIGHTING_COORDS(7,8)                   //this initializes the unity lighting and shadow
    UNITY_FOG_COORDS(9)                    //this initializes the unity fog
};

VertexOutput vert (VertexInput v) {
     VertexOutput o = (VertexOutput)0;           
     o.uv0 = v.texcoord0;
     o.uv1 = v.texcoord1;
     o.normalDir = UnityObjectToWorldNormal(v.normal);
     o.tangentDir = normalize( mul( _Object2World, float4( v.tangent.xyz, 0.0 ) ).xyz );
     o.bitangentDir = normalize(cross(o.normalDir, o.tangentDir) * v.tangent.w);
     o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
     o.posWorld = mul(_Object2World, v.vertex);
     UNITY_TRANSFER_FOG(o,o.pos);
     TRANSFER_VERTEX_TO_FRAGMENT(o)
     return o;
}
```

### 片段程序
在我们的片段程序中，我们会将我们之后用到算法当中的所有值都提前算出来：
```
float4 frag(VertexOutput i) : COLOR {

//normal direction calculations
     float3 normalDirection = normalize(i.normalDir);

     float3 lightDirection = normalize(lerp(_WorldSpaceLightPos0.xyz, _WorldSpaceLightPos0.xyz
 - i.posWorld.xyz,_WorldSpaceLightPos0.w));

     float3 lightReflectDirection = reflect( -lightDirection, normalDirection );

     float3 viewDirection = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);

     float3 viewReflectDirection = normalize(reflect( -viewDirection, normalDirection ));

     float3 halfDirection = normalize(viewDirection+lightDirection); 

     float NdotL = max(0.0, dot( normalDirection, lightDirection ));

     float NdotH =  max(0.0,dot( normalDirection, halfDirection));

     float NdotV =  max(0.0,dot( normalDirection, viewDirection));

     float VdotH = max(0.0,dot( viewDirection, halfDirection));

     float LdotH =  max(0.0,dot(lightDirection, halfDirection));
 
     float LdotV = max(0.0,dot(lightDirection, viewDirection)); 

     float RdotV = max(0.0, dot( lightReflectDirection, viewDirection ));

     float attenuation = LIGHT_ATTENUATION(i);

     float3 attenColor = attenuation * _LightColor0.rgb;
```

以上的这些变量我们会通过Unity给我们的数据算出来，Unity中数据的含义都在[Unity Shader Tutorial](http://www.jordanstevenstechart.com/writing-shaders-in-unity)中进行总结了。这些变量会重复的出现在我们的Shader中以帮助我们编写BRDF。
### 粗糙度（Roughness）
在我的做法当中，我重新映射了粗糙度，我这么做的原因其实更偏向于个人喜好，因为我发现重定向之后的粗糙度在进行一下处理之后更符合物理规律。
```
float roughness = 1- (_Glossiness * _Glossiness);   // 1 - smoothness*smoothness
roughness = roughness * roughness;
```
### 金属度（Metallic）
在PBR Shader中使用金属度的时候有太多的东西需要关注了。你会发现没有一个算法是对其负责的，所以我们使用一个完全不同的方式来描述它。
金属度是一个用于控制一个材质接近金属的程度（一个非金属：金属度为0，一个完全金属：金属度为1）因此，为了给我们的材质赋值一个正确的金属度，我们会让其决定我们的漫反射颜色以及高光颜色。所以一个完全的金属不会表现出任何的漫反射，它会显示一个完全黑色的漫反射，并且其高光颜色会改变为这个物体表面的颜色。代码如下：
```
float3 diffuseColor = _Color.rgb * (1-_Metallic) ;
float3 specColor = lerp(_SpecularColor.rgb, _Color.rgb, _Metallic * 0.5);
```
 
## Shader的内脏
下面所展示的就是我们要编写的shader的基本格式。注意Shader中的注释，因为它们会帮助我们组织代码并且告诉我们在哪边插入我们的代码。
```
Shader "Physically-Based-Lighting" {
    Properties { 
    _Color ("Main Color", Color) = (1,1,1,1)                    //diffuse Color
    _SpecularColor ("Specular Color", Color) = (1,1,1,1)        //Specular Color (Not Used)
    _Glossiness("Smoothness",Range(0,1)) = 1                    //My Smoothness
    _Metallic("Metalness",Range(0,1)) = 0                    //My Metal Value      


// future shader properties will go here!! Will be referred to as Shader Property Section
    }

    SubShader {
	Tags {
            "RenderType"="Opaque"  "Queue"="Geometry"
        } 
        Pass {
            Name "FORWARD"
            Tags {
                "LightMode"="ForwardBase"
            }
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #define UNITY_PASS_FORWARDBASE
            #include "UnityCG.cginc"
            #include "AutoLight.cginc"
            #include "Lighting.cginc"
            #pragma multi_compile_fwdbase_fullshadows  
            #pragma target 3.0

float4 _Color;
float4 _SpecularColor;
float _Glossiness;
float _Metallic;

//future public variables will go here! Public Variables Section

struct VertexInput {
    float4 vertex : POSITION;       //local vertex position
    float3 normal : NORMAL;         //normal direction
    float4 tangent : TANGENT;       //tangent direction    
    float2 texcoord0 : TEXCOORD0;   //uv coordinates
    float2 texcoord1 : TEXCOORD1;   //lightmap uv coordinates
};

struct VertexOutput {
    float4 pos : SV_POSITION;              //screen clip space position and depth
    float2 uv0 : TEXCOORD0;                //uv coordinates
    float2 uv1 : TEXCOORD1;                //lightmap uv coordinates

//below we create our own variables with the texcoord semantic. 
    float3 normalDir : TEXCOORD3;          //normal direction   
    float3 posWorld : TEXCOORD4;          //normal direction   
    float3 tangentDir : TEXCOORD5;
    float3 bitangentDir : TEXCOORD6;
    LIGHTING_COORDS(7,8)                   //this initializes the unity lighting and shadow
    UNITY_FOG_COORDS(9)                    //this initializes the unity fog
};

VertexOutput vert (VertexInput v) {
     VertexOutput o = (VertexOutput)0;           
     o.uv0 = v.texcoord0;
     o.uv1 = v.texcoord1;
     o.normalDir = UnityObjectToWorldNormal(v.normal);
     o.tangentDir = normalize( mul( _Object2World, float4( v.tangent.xyz, 0.0 ) ).xyz );
     o.bitangentDir = normalize(cross(o.normalDir, o.tangentDir) * v.tangent.w);
     o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
     o.posWorld = mul(_Object2World, v.vertex);
     UNITY_TRANSFER_FOG(o,o.pos);
     TRANSFER_VERTEX_TO_FRAGMENT(o)
     return o;
}    

//helper functions will go here!!! Helper Function Section


//algorithms we build will be placed here!!! Algorithm Section



float4 frag(VertexOutput i) : COLOR {

//normal direction calculations
     float3 normalDirection = normalize(i.normalDir);

     float3 lightDirection = normalize(lerp(_WorldSpaceLightPos0.xyz, _WorldSpaceLightPos0.xyz - i.posWorld.xyz,_WorldSpaceLightPos0.w));

     float3 lightReflectDirection = reflect( -lightDirection, normalDirection );

     float3 viewDirection = normalize(_WorldSpaceCameraPos.xyz - i.posWorld.xyz);

     float3 viewReflectDirection = normalize(reflect( -viewDirection, normalDirection ));

     float3 halfDirection = normalize(viewDirection+lightDirection); 

     float NdotL = max(0.0, dot( normalDirection, lightDirection ));

     float NdotH =  max(0.0,dot( normalDirection, halfDirection));

     float NdotV =  max(0.0,dot( normalDirection, viewDirection));

     float VdotH = max(0.0,dot( viewDirection, halfDirection));

     float LdotH =  max(0.0,dot(lightDirection, halfDirection));
 
     float LdotV = max(0.0,dot(lightDirection, viewDirection)); 

     float RdotV = max(0.0, dot( lightReflectDirection, viewDirection ));

     float attenuation = LIGHT_ATTENUATION(i);

     float3 attenColor = attenuation * _LightColor0.rgb;

     float roughness = 1- (_Glossiness * _Glossiness);   // 1 - smoothness*smoothness
     
     roughness = roughness * roughness;

     float3 diffuseColor = _Color.rgb * (1-_Metallic) ;
    
     float3 specColor = lerp(_SpecularColor.rgb, _Color.rgb, _Metallic * 0.5);
      
    //future code will go here!    Fragment Section

     return float4(1,1,1,1);
}
ENDCG
}
}
FallBack "Legacy Shaders/Diffuse"
}
```
在Unity中创建物体并且附上材质，使用这个Shader，会显示一个白色的物体。我们会在shader中添加各种属性、变量、方法以及算法来扩充这个函数。













