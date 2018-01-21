Shader "Custom/Scene/ DiffuseEmission_FlowVC_UV2" {
Properties {
	_Color ("Main Color(RGB)   EmiAreaLumen(A)", Color) = (1,1,1,1)
	_MainTex ("Base (RGB) Gloss (A)", 2D) = "white" {}
	_Illum ("Illumin (R)", 2D) = "white" {}
	_EmissionColor ("(RGB)FlowLightColor  (A)ColorIntensity", Color) = (1,1,1,0.5)   
	_FlowDir("x:U Speed y:V Speed  z:FlowLightInte w:FlowLightPow",Vector)= (1.0,1.0,1.0,10)
}
SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 200
	
CGPROGRAM
#pragma surface surf Lambert

sampler2D _MainTex;
sampler2D _Illum;
fixed4 _Color;
fixed4 _EmissionColor;
float4 _FlowDir;

struct Input {
	float4 color : COLOR;
	float2 uv_MainTex;
	float2 uv2_Illum;
	//float2 uv2_Illum;
};

void surf (Input IN, inout SurfaceOutput o) {
	fixed4 tex = tex2D(_MainTex, IN.uv_MainTex)*_Color;
	fixed4 Illu = tex2D(_Illum, IN.uv_MainTex);
	fixed4 c = tex * _Color;
float a=abs(fmod(IN.uv2_Illum.x-fmod(-_Time.x*_FlowDir.x,1),1));
		float f=1-a*(1-a)*4;
	o.Albedo = c.rgb*_Color*min(1,1-Illu.x+_Color.a);
	o.Emission = saturate(pow(f,_FlowDir.w))*_EmissionColor*Illu.x*IN.color*_EmissionColor.a*2;

}
ENDCG
} 
FallBack "Self-Illumin/VertexLit"
}
