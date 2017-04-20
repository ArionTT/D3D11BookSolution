
cbuffer ConstantBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VS_INPUT
{
    float4 position:POSITION;
    float3 normal:NORMAL;
    float2 texcoord:TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD0;
};



PS_INPUT  vert(VS_INPUT i)
{
    PS_INPUT o;
	float4x4 mvp = mul(mul(projection, view), world);
    o.Pos=mul(mvp,i.position);
	o.Normal = mul(i.normal,world);
    return o;
}