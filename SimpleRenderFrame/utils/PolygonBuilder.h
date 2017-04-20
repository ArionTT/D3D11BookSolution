#pragma once
#include<D3DX11.h>
#include<xnamath.h>
#include<vector>

class PolygonBuilder
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(XMFLOAT3&p, XMFLOAT3&n, XMFLOAT3 &tan, XMFLOAT2&tex) :position(p), normal(n), tangent(tan), texcoord(tex) {}
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT2 texcoord;
	};
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;
	};

public:
	void BuildQuad(float width,float height,Mesh&meshDate);
	void BuildFullScreenQuad(Mesh&meshData);
	void BuildGrid(float width,float depth,UINT m,UINT n,Mesh&meshData);
	void BuildBox(float width,float height,float depth,Mesh&meshData);
	void BuildCube(Mesh&meshData);
	void BuildSphere(float radius, UINT sliceCount, UINT stackCount, Mesh& meshData);
	void BuildGeosphere(float radius, UINT numSubdivisions, Mesh& meshData);
	void BuildCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& meshData);
	void BuildCustomPolygon(std::wstring path,Mesh&meshData);

protected:
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& meshData);
};
