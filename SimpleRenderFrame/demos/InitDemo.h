#pragma once

#include"../CoreDevice.h"
#include"../utils/PolygonBuilder.h"
#include"../utils/CoreEffect.h"

class InitDemo :public CoreDevice
{
public:
	InitDemo(HINSTANCE hinstance);
	InitDemo(HINSTANCE hinstance, int width, int height);
	~InitDemo();

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
	void InitSkull();

public:
	CoreEffect*m_FX;
	XMFLOAT4X4 m_lightM;
	XMFLOAT4X4 m_proj;
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_world;
	DirectLight m_dirLight;

	ID3D11Buffer*m_VB;
	ID3D11Buffer*m_IB;
	ID3D11InputLayout*m_Input;

	ID3D11Buffer*m_SkullVB;
	ID3D11Buffer*m_SkullIB;
	ID3D11InputLayout*m_skullInput;

	CoreMaterial m_matSkull;
	XMFLOAT4X4 m_skullMVP;
	int m_SkullOffsetVertex;
	UINT m_SkullOffsetIndex;
	UINT m_SkullIndexCount;

	CoreMaterial m_matCube;
	CoreMaterial m_matGrid;
	CoreMaterial m_matSphere;
	CoreMaterial m_matCylinder;
	XMFLOAT4X4 m_shapeM;
	XMFLOAT4X4 m_cubeMVP;
	XMFLOAT4X4 m_gridMVP;
	XMFLOAT4X4 m_sphereMVP;
	XMFLOAT4X4 m_cylinderMVP;
	int m_CubeOffsetVertex;
	UINT m_CubeOffsetIndex;
	UINT m_CubeIndexCount;
	int m_GridOffsetVertex;
	UINT m_GridOffsetIndex;
	UINT m_GridIndexCount;
	int m_SphereOffsetVertex;
	UINT m_SphereOffsetIndex;
	UINT m_SphereIndexCount;
	int m_CylinderOffsetVertex;
	UINT m_CylinderOffsetIndex;
	UINT m_CylinderIndexCount;

	XMFLOAT3 m_ViewPos;
	float m_theta;
	float m_phi;
	float m_radius;

	POINT m_lastMousePos;
};
