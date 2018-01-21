// Simplified Diffuse shader. Differences from regular Diffuse one:
// - no Main Color
// - fully supports only 1 directional light. Other lights can affect it, but it will be per-vertex/SH.

Shader "Custom/Scene/DiffuseEmisSin" {
Properties {
	_MainTex ("Base (RGB)", 2D) = "white" {}
	_MaskTex ("Mask (B)", 2D) = "white" {}
	_Emission("EmisColor(RGB) Intensity(A)",COLOR)=(0,0,0,1)
	_EmisVector("X:Speed Y:Min  Z:Distance  W:Direction   ",vector)=(1,1,0,1)
}
SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 150

CGPROGRAM
#pragma surface surf Lambert noforwardadd

sampler2D _MainTex;
sampler2D _MaskTex;
fixed4 _Emission;
half4 _EmisVector;

struct Input {
	float2 uv_MainTex;
	float3 worldPos;
};	

void surf (Input IN, inout SurfaceOutput o) {
	fixed4 c = tex2D(_MainTex, IN.uv_MainTex);
	fixed4 mask = tex2D(_MaskTex, IN.uv_MainTex);
	o.Albedo = c.rgb;
	float SinPos= sin(_Time.y*_EmisVector.x+lerp(IN.worldPos.x,IN.worldPos.z,saturate( _EmisVector.w))*0.01*_EmisVector.z)*0.5+0.5;


	o.Emission=_Emission*_Emission.a*mask.b*min((SinPos+_EmisVector.y),1);
	o.Alpha = c.a;
}
ENDCG
}

Fallback "Mobile/VertexLit"
}
