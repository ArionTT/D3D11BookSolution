//***************************************************************************************
//Struct 
//***************************************************************************************

struct Material
{
	float4 Ambient;
	float4 Diffuse;  //w componet is alpha value
	float4 Specular; //w componet is gloss value
};

struct DirectLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 dir;
};

struct Fog
{
	float4 fogColor;
	float start;
	float range;
};

cbuffer ConstBufferScene
{
	DirectLight gDirectLight;
	Fog gFog;
	float3 gViewPos;
	float gDeltaTime;
	float4x4 MATRIX_W;
};

cbuffer ConstBufferObject
{
	float4x4 MATRIX_M;
	float4x4 MATRIX_V;
	float4x4 MATRIX_P;
	float4x4 MATRIX_VP;
	float4x4 MATRIX_MVP;
	Material OBJECT_MAT;
};

struct PointLight
{

};

struct SpotLight
{

};



//***************************************************************************************
//Function
//***************************************************************************************

void CaculateColor(float3 lightPos, float normal, Material mat, out float4 ambient, out float4 diffuse, out float4 specular)
{

}

float RGB2Gray(float4 color)
{
	return color.x*0.2154 + color.y*0.7154 + color.z*0.0721;
}

float FogParameter(float3 viewPos,float3 pointPos,float fogStart,float fogRange)
{
	return saturate((length(viewPos - pointPos) - fogStart) / fogRange);
}

float3 FogCaculate(float3 litColor, float3 fogColor, float FogParameter)
{
	return lerp(litColor, fogColor, FogParameter);
}