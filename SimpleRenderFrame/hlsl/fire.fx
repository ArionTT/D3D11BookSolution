
#include"EffectTools.fx"


Texture2D gDiffuseMap;

SamplerState anisoFilter
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

cbuffer cbAdd
{
	float4 _Texture_ST;
};



struct VS_INPUT
{
	float3 pos:POSITION;
	float3 normal:NORMAL;
	float2 texcoord:TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float3 normal:NORMAL;
	float3 posW:TEXCOORD0;
	float2 tex:TEXCOORD1;
};

PS_INPUT VS(VS_INPUT i)
{
	PS_INPUT o;

	float3 center = float3(0.0f, 0.0f, 0.0f);
	float3 normalDir = mul(MATRIX_M, float4(gViewPos,1.0f)).xyz - center;
	normalDir = normalize(normalDir);
	float3 upDir = abs(normalDir.y) > 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(0.0f, 1.0f, 0.0f);
	float3 rightDir = normalize(cross( normalDir, upDir));
	upDir = normalize(cross(rightDir, normalDir));
	float3 centerOffset = i.pos - center;
	float3 localPos = center + centerOffset.x*rightDir + centerOffset.y*upDir + centerOffset.z*normalDir;

	o.pos = float4(localPos, 1.0f);
	o.pos = mul(o.pos, MATRIX_MVP);
	o.normal = mul(normalDir, MATRIX_M);
	o.posW = mul(i.pos, MATRIX_M);
	o.tex = i.texcoord.xy*_Texture_ST.xy+_Texture_ST.zw;
	return o;
}

float4 PS(PS_INPUT i) :SV_Target
{
	float4 sampleColor = float4(1.0f,1.0f,1.0f,1.0f);
	sampleColor = gDiffuseMap.Sample(anisoFilter, i.tex);

	clip(sampleColor.a - 0.3f);

	float4 color = float4(sampleColor.xyz,OBJECT_MAT.Diffuse.w);

	color = float4(FogCaculate(color, gFog.fogColor, FogParameter(gViewPos, i.posW, gFog.start, gFog.range)), 1.0f);

	return color;
}

technique11 fireTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}