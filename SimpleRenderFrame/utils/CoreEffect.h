#pragma once
#include"CoreUtils.h"

class CoreEffect
{
public:
	CoreEffect(ID3D11Device* device,const std::wstring filename);
	~CoreEffect();
public:
	void SetMVP(CXMMATRIX mvp);
	void SetM(CXMMATRIX m);
	void SetV(CXMMATRIX v);
	void SetP(CXMMATRIX p);
	void SetW(CXMMATRIX w);
	void SetVP(CXMMATRIX vp);
	void SetEyePos(const XMFLOAT3&v);
	void SetDirectLight(const DirectLight&light);
	void SetMaterial(const CoreMaterial&mat);
	void SetTexture(ID3D11ShaderResourceView*SRV);
	void SetTextureST(const XMFLOAT4&st);
	void SetDeltaTime(const FLOAT&deltaTime);
	void SetFog(const Fog &fog);

public:
	ID3DX11EffectTechnique*m_basicTech;

	ID3DX11EffectMatrixVariable*m_MVP;
	ID3DX11EffectMatrixVariable*m_W;
	ID3DX11EffectMatrixVariable*m_VP;
	ID3DX11EffectMatrixVariable*m_M;
	ID3DX11EffectMatrixVariable*m_V;
	ID3DX11EffectMatrixVariable*m_P;
	ID3DX11EffectVectorVariable*m_ViewPos;
	ID3DX11EffectVectorVariable*m_Texture_ST;
	ID3DX11EffectVariable* m_DirLight;
	ID3DX11EffectVariable* m_Material;
	ID3DX11EffectVariable*m_DeltaTime;
	ID3DX11EffectVariable*m_Fog;
	ID3DX11EffectShaderResourceVariable*m_DiffuseMap;

protected:
	ID3DX11Effect*m_FX;
};
