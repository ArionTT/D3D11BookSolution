

cbuffer LightConst:register(b0)
{
	float4 LightColor;
	float3 LightPos;
	float3 LightDir;
	float3 ViewPos;
};

cbuffer Material:register(b1)
{
	float glossness;
	float4 MatColor;
}


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD0;
};


float4  pixel(PS_INPUT input):SV_Target
{
	float3 worldNormal = normalize(input.Normal);
	float3 worldLightDir = normalize(LightDir.xyz);
	float3 viewDir = normalize(ViewPos.xyz - input.Pos);
	float3 reflectDir = normalize(reflect(-worldLightDir, worldNormal));

	//Ambient
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	//Diffuse
	float3 diffuse = LightColor.rgb*MatColor.rgb*saturate(dot(worldNormal, worldLightDir));

	//Specular
	float3 specular = LightColor.rgb * MatColor.rgb * pow(saturate(dot(reflectDir, viewDir)), glossness);

	float alpha = 1.0f;
	return float4(ambient + diffuse + specular,alpha);
}