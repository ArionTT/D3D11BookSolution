#pragma once
#include"../CoreDevice.h"
#include"../utils/PolygonBuilder.h"
#include"../utils/CoreEffect.h"

class texDemo :public CoreDevice
{
public:
	texDemo(HINSTANCE hinstance);
	texDemo(HINSTANCE hinstance, int width, int height);
	~texDemo();

	bool Init();
	void OnResize();

	// Í¨¹ý CoreDevice ¼Ì³Ð
	virtual void Render() override;
	virtual void Update(float deltaTime) override;

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

public:
	bool InitPolygon();
	float CaculateHeight(float x, float z);
	XMFLOAT3 GetHillNormal(float x, float z);

public:
	CoreEffect*m_FX;
	CoreEffect*m_FxGrid;
	CoreEffect*m_FxWater;
	CoreEffect*m_FxFire;

	XMFLOAT4X4 m_lightM;
	XMFLOAT4X4 m_proj;
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_world;
	XMFLOAT4X4 m_shapeM;
	DirectLight m_dirLight;
	Fog m_fog;
	FLOAT m_deltaTime;


	ID3D11ShaderResourceView*m_diffuseMapSRV;
	ID3D11ShaderResourceView*m_grassMapSRV;
	ID3D11ShaderResourceView*m_waterMapSRV;
	ID3D11ShaderResourceView*m_fireMapSRV;

	ID3D11Buffer*m_VB;
	ID3D11Buffer*m_IB;
	ID3D11InputLayout*m_Input;

	XMFLOAT4 m_water_ST;
	XMFLOAT4 m_grid_ST;
	XMFLOAT4 m_fire_ST;

	CoreMaterial m_matCube;
	CoreMaterial m_matGrid;
	CoreMaterial m_matWater;
	CoreMaterial m_matFire;
	XMFLOAT4X4 m_cubeMVP;
	XMFLOAT4X4 m_gridMVP;
	XMFLOAT4X4 m_waterMVP;
	XMFLOAT4X4 m_fireMVP;
	int m_CubeOffsetVertex;
	UINT m_CubeOffsetIndex;
	UINT m_CubeIndexCount;
	int m_GridOffsetVertex;
	UINT m_GridOffsetIndex;
	UINT m_GridIndexCount;
	int m_WaterOffsetVertex;
	UINT m_WaterOffsetIndex;
	UINT m_WaterIndexCount;
	int m_FireOffsetVertex;
	UINT m_FireOffsetIndex;
	UINT m_FireIndexCount;

	XMFLOAT3 m_ViewPos;
	float m_theta;
	float m_phi;
	float m_radius;

	POINT m_lastMousePos;
};
