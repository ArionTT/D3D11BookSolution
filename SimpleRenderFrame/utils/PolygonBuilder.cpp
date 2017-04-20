#include "PolygonBuilder.h"

void PolygonBuilder::BuildQuad(float width, float height, Mesh & meshDate)
{
	meshDate.vertices.resize(4);
	meshDate.indices.resize(6);

	meshDate.vertices[0].position = XMFLOAT3(-width/2.0f, -height/2.0f, 0.0f);
	meshDate.vertices[1].position = XMFLOAT3(-width / 2.0f, height / 2.0f, 0.0f);
	meshDate.vertices[2].position = XMFLOAT3(width / 2.0f, height / 2.0f, 0.0f);
	meshDate.vertices[3].position = XMFLOAT3(width / 2.0f, -height / 2.0f, 0.0f);

	meshDate.vertices[0].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	meshDate.vertices[1].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	meshDate.vertices[2].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	meshDate.vertices[3].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	meshDate.vertices[0].texcoord = XMFLOAT2(0.0f, 1.0f);
	meshDate.vertices[1].texcoord = XMFLOAT2(0.0f, 0.0f);
	meshDate.vertices[2].texcoord = XMFLOAT2(1.0f, 0.0f);
	meshDate.vertices[3].texcoord = XMFLOAT2(1.0f, 1.0f);

	meshDate.indices[0] = 0;
	meshDate.indices[1] = 1;
	meshDate.indices[2] = 2;
	meshDate.indices[3] = 0;
	meshDate.indices[4] = 2;
	meshDate.indices[5] = 3;
}

void PolygonBuilder::BuildFullScreenQuad(Mesh & meshData)
{
}

void PolygonBuilder::BuildGrid(float width, float depth, UINT row, UINT col, Mesh&meshData)
{
	//  z  col =8
	//  |
	//  |   ++++++++   
	//  |   ++++++++
	//  |   ++++++++
	//  |   ++++++++
	//  |   ++++++++      row =5
	// +！！！！！！！！！x
	UINT vertexCount = col*row;
	UINT faceCout = (col - 1)*(row - 1);

	float hW = width / 2.0f;
	float hD = depth / 2.0f;

	float dx = width / (col - 1);
	float dz = depth / (row - 1);

	float du = 1.0f / (col - 1);
	float dv = 1.0f / (row - 1);

	meshData.vertices.resize(vertexCount);
	for (UINT i = 0; i < row; i++)
	{
		float z = -hD + i*dz;
		float u = i*dv;
		for (UINT j = 0; j < col; j++)
		{
			float v = j*du;
			float x = -hW + j*dx;

			meshData.vertices[i*col + j].position = XMFLOAT3(x, 0.0f, z);
			meshData.vertices[i*col + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.vertices[i*col + j].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);
			meshData.vertices[i*col + j].texcoord = XMFLOAT2(u, v);
		}
	}

	meshData.indices.resize(faceCout * 6);
	UINT k = 0;
	for (UINT i = 0; i < row - 1; i++)
	{
		for (UINT j = 0; j < col - 1; j++)
		{
			meshData.indices[k] = (i+1)*col + j;
			meshData.indices[k + 1] = i*col + j+1;
			meshData.indices[k + 2] = i*col + j;

			meshData.indices[k + 3] = (i + 1)*col + j;
			meshData.indices[k + 4] = (i+1)*col + j + 1;
			meshData.indices[k + 5] = i*col + j + 1;

			k += 6;
		}
	}
}

void PolygonBuilder::BuildBox(float width, float height, float depth, Mesh & meshData)
{
	Vertex cube[] =
	{
		{ XMFLOAT3(-width / 2,height / 2,depth / 2),XMFLOAT3(0,1,0),XMFLOAT3(0,0,0),XMFLOAT2(0,0) }, //0
		{ XMFLOAT3(width / 2,height / 2,depth / 2),XMFLOAT3(0,1,0),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },  //1
		{ XMFLOAT3(width / 2,height / 2,-depth / 2),XMFLOAT3(0,1,0),XMFLOAT3(0,0,0),XMFLOAT2(1,1) },  //2
		{ XMFLOAT3(-width / 2,height / 2,-depth / 2),XMFLOAT3(0,1,0),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },  //3

		{ XMFLOAT3(-width / 2,-height / 2,depth / 2),XMFLOAT3(0,-1,0),XMFLOAT3(0,0,0),XMFLOAT2(0,0) },//4
		{ XMFLOAT3(width / 2,-height / 2,depth / 2),XMFLOAT3(0,-1,0),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },//5 
		{ XMFLOAT3(width / 2,-height / 2,-depth / 2),XMFLOAT3(0,-1,0),XMFLOAT3(0,0,0),XMFLOAT2(1,1) },//6
		{ XMFLOAT3(-width / 2,-height / 2,-depth / 2),XMFLOAT3(0,-1,0),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },//7

		{ XMFLOAT3(-width / 2,height / 2,depth / 2),XMFLOAT3(0,0,1),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },//8
		{ XMFLOAT3(width / 2,height / 2,depth / 2),XMFLOAT3(0,0,1),XMFLOAT3(0,0,0),XMFLOAT2(0,0) },//9
		{ XMFLOAT3(-width / 2,-height / 2,depth / 2),XMFLOAT3(0,0,1),XMFLOAT3(0,0,0),XMFLOAT2(1,1)},//10
		{ XMFLOAT3(width / 2,-height / 2,depth / 2),XMFLOAT3(0,0,1),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },//11

		{ XMFLOAT3(width / 2,height / 2,-depth / 2),XMFLOAT3(0,0,-1),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },//12
		{ XMFLOAT3(-width / 2,height / 2,-depth / 2),XMFLOAT3(0,0,-1),XMFLOAT3(0,0,0),XMFLOAT2(0,0) },//13
		{ XMFLOAT3(width / 2,-height / 2,-depth / 2),XMFLOAT3(0,0,-1),XMFLOAT3(0,0,0),XMFLOAT2(1,1) },//14
		{ XMFLOAT3(-width / 2,-height / 2,-depth / 2),XMFLOAT3(0,0,-1),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },//15

		{ XMFLOAT3(-width / 2,height / 2,depth / 2),XMFLOAT3(-1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(0,0) },//16
		{ XMFLOAT3(-width / 2,height / 2,-depth / 2),XMFLOAT3(-1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },//17
		{ XMFLOAT3(-width / 2,-height / 2,depth / 2),XMFLOAT3(-1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },//18
		{ XMFLOAT3(-width / 2,-height / 2,-depth / 2),XMFLOAT3(-1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(1,1) },//19

		{ XMFLOAT3(width / 2,height / 2,depth / 2),XMFLOAT3(1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(1,0) },//20
		{ XMFLOAT3(width / 2,height / 2,-depth / 2),XMFLOAT3(1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(0,0) },//21
		{ XMFLOAT3(width / 2,-height / 2,depth / 2),XMFLOAT3(1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(1,1) },//22
		{ XMFLOAT3(width / 2,-height / 2,-depth / 2),XMFLOAT3(1,0,0),XMFLOAT3(0,0,0),XMFLOAT2(0,1) },//23
	};

	UINT index[] =
	{
		0,1,2,
		0,2,3,

		6,5,4,
		6,4,7,

		11,9,8,
		11,8,10,

		15,13,12,
		15,12,14,

		18,16,17,
		18,17,19,

		23,21,20,
		23,20,22
	};
	meshData.vertices.resize(24);
	meshData.indices.resize(36);

	meshData.vertices.assign(&cube[0], &cube[24]);
	meshData.indices.assign(&index[0], &index[36]);
}

void PolygonBuilder::BuildCube(Mesh & meshData)
{
	BuildBox(10.0f, 10.0f, 10.0f, meshData);
}

void PolygonBuilder::BuildSphere(float radius, UINT sliceCount, UINT stackCount, Mesh & meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.

	Vertex topVertex(XMFLOAT3(0.0f,+radius,0.0f),XMFLOAT3(0.0f,+1.0f,0.0f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT2(0.0f,0.0f));
	Vertex bottomVertex(XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));

	meshData.vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.position.x = radius*sinf(phi)*cosf(theta);
			v.position.y = radius*cosf(phi);
			v.position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.tangent.x = -radius*sinf(phi)*sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.tangent);
			XMStoreFloat3(&v.tangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.texcoord.x = theta / XM_2PI;
			v.texcoord.y = phi / XM_PI;

			meshData.vertices.push_back(v);
		}
	}

	meshData.vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.indices.push_back(0);
		meshData.indices.push_back(i + 1);
		meshData.indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(southPoleIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}

void PolygonBuilder::BuildGeosphere(float radius, UINT numSubdivisions, Mesh & meshData)
{
}

void PolygonBuilder::BuildCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh & meshData)
{
	meshData.vertices.clear();
	meshData.indices.clear();

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*XM_PI / sliceCount;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.position = XMFLOAT3(r*c, y, r*s);

			vertex.texcoord.x = (float)j / sliceCount;
			vertex.texcoord.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.tangent = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent(dr*c, -height, dr*s);

			XMVECTOR T = XMLoadFloat3(&vertex.tangent);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.normal, N);

			meshData.vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.indices.push_back(i*ringVertexCount + j);
			meshData.indices.push_back((i + 1)*ringVertexCount + j);
			meshData.indices.push_back((i + 1)*ringVertexCount + j + 1);

			meshData.indices.push_back(i*ringVertexCount + j);
			meshData.indices.push_back((i + 1)*ringVertexCount + j + 1);
			meshData.indices.push_back(i*ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
}

void PolygonBuilder::BuildCustomPolygon(std::wstring path, Mesh & meshData)
{
}

void PolygonBuilder::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh & meshData)
{
	UINT baseIndex = (UINT)meshData.vertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*XM_PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(XMFLOAT3(x,y,z),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT2(u,v)));
	}

	// Cap center vertex.
	meshData.vertices.push_back(Vertex(XMFLOAT3(0.0f,y,0.0f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT3(1.0f,0.0f,0.0f),XMFLOAT2(0.5f,0.5f)));

	// Index of center vertex.
	UINT centerIndex = (UINT)meshData.vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i + 1);
		meshData.indices.push_back(baseIndex + i);
	}
}

void PolygonBuilder::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh & meshData)
{
	// 
	// Build bottom cap.
	//

	UINT baseIndex = (UINT)meshData.vertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*XM_PI / sliceCount;
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(u, v)));
	}

	// Cap center vertex.
	meshData.vertices.push_back(Vertex(XMFLOAT3(0.0f, y, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.5f)));

	// Cache the index of center vertex.
	UINT centerIndex = (UINT)meshData.vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i);
		meshData.indices.push_back(baseIndex + i + 1);
	}
}

