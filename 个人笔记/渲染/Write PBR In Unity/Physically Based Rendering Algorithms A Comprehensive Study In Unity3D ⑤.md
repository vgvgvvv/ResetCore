# Physically Based Rendering Algorithms: A Comprehensive Study In Unity3D ⑤
#读书笔记/渲染/Write PBR In Unity#

## 组装你的PBR Shader Part3：菲涅尔函数（The Fresnel Function）
菲涅尔效应是由第一个描述它的法国物理学家Augustin-Jean Fresnel命名的。这个现象表现在视角改变的情况下表面反射强度的改变。反射强度随着视角与表面切角增大而增强。为了将菲涅尔效应包含到我们的shader当中，我们需要将它应用好几个地方。首先我们需要考虑传统光照模型，然后我们需要考虑BRDF的菲涅尔效应。

为了更近似地计算我们的菲涅尔效应，我们需要考虑法线以及切角的关系。在下面我们会使用粗糙度来计算漫反射菲涅尔反射率以传入我们的菲涅尔方程。为了计算这个值我们使用了Schlick的版本来近似菲涅尔，看起来就是下面的样子：

![](Physically%20Based%20Rendering%20Algorithms%20A%20Comprehensive%20Study%20In%20Unity3D%20%E2%91%A4/B852277E-DB22-4DAC-84DC-E381F7914BBC.png)

```
schlick = x + (1-x) * pow(1-dotProduct,5);
```

公式更进一步转换一下我们可以看到：

```
mix(x,1,pow(1-dotProduct,5));
```

这个写法在一些GPU上可以跑得更快，我们可以把1和x混合的部分提出来单独算，然后用以下的代码来计算我们的diffuse。

```
float MixFunction(float i, float j, float x) {
	 return  j * x + i * (1.0 - x);
}

float SchlickFresnel(float i){
    float x = clamp(1.0-i, 0.0, 1.0);
    float x2 = x*x;
    return x2*x2*x;
}

//normal incidence reflection calculation
float F0 (float NdotL, float NdotV, float LdotH, float roughness){
    float FresnelLight = SchlickFresnel(NdotL); 
    float FresnelView = SchlickFresnel(NdotV);
    float FresnelDiffuse90 = 0.5 + 2.0 * LdotH*LdotH * roughness;
    return  MixFunction(1, FresnelDiffuse90, FresnelLight) * MixFunction(1, FresnelDiffuse90, FresnelView);
}
```


### Schlick Fresnel
Schilck的近似菲涅尔方程也许是他写出来的最有名的公式之一。这个菲涅尔效果的近似算式允许我们计算切角的反光。
```
float3 SchlickFresnelFunction(float3 SpecularColor,float LdotH){
    return SpecularColor + (1 - SpecularColor)* SchlickFresnel(LdotH);
}
```
```
FresnelFunction *=  SchlickFresnelFunction(specColor, LdotH);
```

下一个算法传入了一个特殊的值，而不是高光颜色。这个新的值是折射因子（Index of Refraction ,IOR）。IOR是一个标量，用于描述光穿过一个表面的速度。为了使用这个方法，我们必须加一个新的property和变量到我们的shader当中。

```
_Ior("Ior",  Range(1,4)) = 1.5
```

上面的代码应该放在Shader的Property部分，然后下面的变量放在变量部分。

```
float _Ior;
```

```
float SchlickIORFresnelFunction(float ior ,float LdotH){
    float f0 = pow(ior-1,2)/pow(ior+1, 2);
    return f0 + (1-f0) * SchlickFresnel(LdotH);
}
```

```
FresnelFunction *=  SchlickIORFresnelFunction(_Ior, LdotH);
```

### Spherical-Gaussian Fresnel

Spherical-Gaussian 菲涅尔方程产生了与Schlicks近似式非常相近的结果。其中唯一的区别是它的power值是由Spherical Gaussian算式来算出来的。

```
float SphericalGaussianFresnelFunction(float LdotH,float SpecularColor)
{	
  float power = ((-5.55473 * LdotH) - 6.98316) * LdotH;
  return SpecularColor + (1 - SpecularColor) * pow(2,power);
}
```

```
FresnelFunction *= SphericalGaussianFresnelFunction(LdotH, specColor);
```





