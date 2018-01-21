#ifndef ROLIGHTINGBASIC_INCLUDED
#define ROLIGHTINGBASIC_INCLUDED 



v2f vert(a2v v) {  

	v2f o = (v2f)0;  
	  
	o.pos = UnityObjectToClipPos(v.vertex);
	o.uv0 = v.texcoord0;

#ifdef UV2
	o.uv1 = v.texcoord1;
#endif//UV2

#ifndef FRAG_NONORMAL
	//1.calc normal
	o.normal = normalize(mul((float3x3)unity_ObjectToWorld, SCALED_NORMAL));
#endif

	return CustomVert(v, o);  
} 


fixed4 frag(v2f i) : COLOR 
{  
	fixed4 _BasicColor = CustomFrag(i);
	fixed4 c=_BasicColor;
	return c;  
} 


#endif