#include "CoreEffect.h"

CoreEffect::CoreEffect(ID3D11Device * device, const std::wstring filename):
	m_FX(NULL)
{
	std::ifstream fin(filename, std::ios::binary);
	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compilerShader(size);

	fin.read(&compilerShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compilerShader[0], size, 0, device, &m_FX));

	m_Material = m_FX->GetVariableByName("OBJECT_MAT");
	m_DirLight = m_FX->GetVariableByName("gDirectLight");
	m_Fog = m_FX->GetVariableByName("gFog");
	m_DeltaTime = m_FX->GetVariableByName("gDeltaTime");
	m_MVP = m_FX->GetVariableByName("MATRIX_MVP")->AsMatrix();
	m_M = m_FX->GetVariableByName("MATRIX_M")->AsMatrix();
	m_V = m_FX->GetVariableByName("MATRIX_V")->AsMatrix();
	m_P = m_FX->GetVariableByName("MATRIX_P")->AsMatrix();
	m_VP = m_FX->GetVariableByName("MATRIX_VP")->AsMatrix();
	m_W = m_FX->GetVariableByName("MATRIX_W")->AsMatrix();
	m_ViewPos = m_FX->GetVariableByName("gViewPos")->AsVector();
	m_Texture_ST = m_FX->GetVariableByName("_Texture_ST")->AsVector();
	m_DiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_basicTech = m_FX->GetTechniqueByIndex(0);
}

CoreEffect::~CoreEffect()
{
	ReleaseCOM(m_FX);
}

void CoreEffect::SetMVP(CXMMATRIX mvp)
{
	m_MVP->SetMatrix(reinterpret_cast<const float*>(&mvp));
}

void CoreEffect::SetM(CXMMATRIX m)
{
	m_M->SetMatrix(reinterpret_cast<const float*>(&m));
}

void CoreEffect::SetV(CXMMATRIX v)
{
	m_V->SetMatrix(reinterpret_cast<const float*>(&v));
}

void CoreEffect::SetP(CXMMATRIX p)
{
	m_P->SetMatrix(reinterpret_cast<const float*>(&p));
}

void CoreEffect::SetW(CXMMATRIX w)
{
	m_W->SetMatrix(reinterpret_cast<const float*>(&w));
}

void CoreEffect::SetVP(CXMMATRIX vp)
{
	m_VP->SetMatrix(reinterpret_cast<const float*>(&vp));
}

void CoreEffect::SetEyePos(const XMFLOAT3 & v)
{
	m_ViewPos->SetRawValue(&v, 0, sizeof(XMFLOAT3));
}

void CoreEffect::SetDirectLight(const DirectLight & light)
{
	m_DirLight->SetRawValue(&light, 0, sizeof(DirectLight));
}

void CoreEffect::SetMaterial(const CoreMaterial & mat)
{
	m_Material->SetRawValue(&mat, 0, sizeof(CoreMaterial));
}

void CoreEffect::SetTexture(ID3D11ShaderResourceView * SRV)
{
	m_DiffuseMap->SetResource(SRV);
}

void CoreEffect::SetTextureST(const XMFLOAT4 &st)
{
	m_Texture_ST->SetRawValue(&st, 0, sizeof(XMFLOAT4));
}

void CoreEffect::SetDeltaTime(const FLOAT & deltaTime)
{
	m_DeltaTime->SetRawValue(&deltaTime, 0, sizeof(FLOAT));
}

void CoreEffect::SetFog(const Fog & fog)
{
	m_Fog->SetRawValue(&fog, 0, sizeof(Fog));
}


