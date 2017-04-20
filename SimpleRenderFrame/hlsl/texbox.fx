
#include"EffectTools.fx"


Texture2D gDiffuseMap;

SamplerState anisoFilter
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
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
	o.pos = float4(i.pos, 1.0f);
	o.pos = mul(o.pos, MATRIX_MVP);
	o.normal = mul(i.normal, MATRIX_M);
	o.posW = mul(i.pos, MATRIX_M);
	o.tex = i.texcoord;
	return o;
}

float4 PS(PS_INPUT i) :SV_Target
{
	float4 sampleColor = gDiffuseMap.Sample(anisoFilter,i.tex);

	float3 WorldNormal = normalize(i.normal);
	float3 LightDir = normalize(mul(float4(gDirectLight.dir,0.0f), MATRIX_W)).xyz;
	float3 ReflectDir = normalize(reflect(-LightDir, WorldNormal));
	float3 ViewDir = normalize(gViewPos - i.posW);
	float gloss = OBJECT_MAT.Specular.w;
	float3 ambient = gDirectLight.ambient.xyz*OBJECT_MAT.Ambient.xyz;
	float3 diffuse = gDirectLight.diffuse.xyz*OBJECT_MAT.Diffuse.xyz*saturate(dot(WorldNormal, LightDir));
	float3 specular = gDirectLight.specular.xyz*OBJECT_MAT.Specular.xyz*pow(max(dot(ReflectDir, ViewDir),0.0f), gloss);

	sampleColor = float4(FogCaculate(sampleColor, gFog.fogColor, FogParameter(gViewPos, i.posW, gFog.start, gFog.range)), 1.0f);
	//return float4(ambient + diffuse + specular, 1.0f);
	return sampleColor;
}

technique11 texShape
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}