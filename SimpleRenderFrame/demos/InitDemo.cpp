#include "InitDemo.h"



InitDemo::InitDemo(HINSTANCE hinstance):CoreDevice(hinstance),
m_VB(NULL),m_IB(NULL),m_radius(15.0f),m_theta(1.5f*CoreMath::PI),m_phi(0.1f*CoreMath::PI)
{
	m_enable4xMsaa = true;
}

InitDemo::InitDemo(HINSTANCE hinstance, int width, int height):CoreDevice(hinstance,width,height),
m_VB(NULL), m_IB(NULL), m_radius(15.0f), m_theta(1.5f*CoreMath::PI), m_phi(0.1f*CoreMath::PI)
{
}

InitDemo::~InitDemo()
{
}

bool InitDemo::Init()
{
	if (!CoreDevice::Init())
		return false;

	m_FX = new CoreEffect(m_pDevice, L"hlsl/shape.fxo");

	if (!InitPolygon())
		return false;

	m_matCube.diffuse = XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f);
	m_matCube.ambient= XMFLOAT4(0.5f, 0.5f, 0.0f,1.0f);
	m_matCube.specular = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_matCube.gloss = 25.0f;

	m_matGrid.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_matGrid.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_matGrid.specular = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_matGrid.gloss = 1.0f;

	m_matSkull.ambient= XMFLOAT4(0.5f, 0.5f, 0.5f,1.0f);
	m_matSkull.diffuse= XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_matSkull.specular = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_matSkull.gloss = 15.0f;

	m_dirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLight.diffuse= XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_dirLight.specular= XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_dirLight.dir = XMFLOAT3(1.0f, 1.0f, -1.0f);

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_world, I);
	XMStoreFloat4x4(&m_view, I);
	XMStoreFloat4x4(&m_proj, I);

	OnResize();

	InitSkull();

	return true;
}

void InitDemo::OnResize()
{
	CoreDevice::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*CoreMath::PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_proj, P);
}

void InitDemo::Render()
{
	RenderBegin();

	//SetCullState(RasterizeState::CullNone);
	//SetFrameState(RasterizeState::Wireframe);

	m_pImContext->IASetInputLayout(m_Input);
	m_pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);

	m_FX->SetDirectLight(m_dirLight);
	m_FX->SetEyePos(m_ViewPos);
	m_FX->SetW(world);

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique*tech = m_FX->m_basicTech;
	tech->GetDesc(&techDesc);

	UINT stride = sizeof(Vertex::BaseVertex);
	UINT offset = 0;

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pImContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
		m_pImContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

		//Cube Render
		XMMATRIX model = SetPosition(XMFLOAT3(0.0f,5.0f,0.0f));
		XMMATRIX mvp = model*view*proj;

		m_FX->SetM(model);
		m_FX->SetV(view);
		m_FX->SetP(proj);
		m_FX->SetMVP(mvp);
		m_FX->SetVP(view*proj);
		m_FX->SetEyePos(m_ViewPos);
		m_FX->SetMaterial(m_matCube);

		tech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_CubeIndexCount, m_CubeOffsetIndex, m_CubeOffsetVertex);

		//Grid Render
		model = XMLoadFloat4x4(&m_world);
		mvp = model*view*proj;
		m_FX->SetM(model);
		m_FX->SetV(view);
		m_FX->SetP(proj);
		m_FX->SetVP(view*proj);
		m_FX->SetMVP(mvp);
		m_FX->SetEyePos(m_ViewPos);
		m_FX->SetMaterial(m_matGrid);

		tech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_GridIndexCount, m_GridOffsetIndex, m_GridOffsetVertex);

		//Skull
		m_pImContext->IASetVertexBuffers(0, 1, &m_SkullVB, &stride, &offset);
		m_pImContext->IASetIndexBuffer(m_SkullIB, DXGI_FORMAT_R32_UINT, 0);

		model = SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
		mvp = model*view*proj;
		m_FX->SetMVP(mvp);
		m_FX->SetMaterial(m_matSkull);
		
		tech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_SkullIndexCount, m_SkullOffsetIndex, m_SkullOffsetVertex);
	}

	RenderEnd();
}

void InitDemo::Update(float deltaTime)
{
	float x = m_radius*sinf(m_phi)*cosf(m_theta);
	float z = m_radius*sinf(m_phi)*sinf(m_theta);
	float y = m_radius*cosf(m_phi);

	m_ViewPos = XMFLOAT3(x, y, z);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR at = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX v = XMMatrixLookAtLH(pos, at, up);
	XMStoreFloat4x4(&m_view, v);
}

void InitDemo::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;

	SetCapture(m_hwnd);
}

void InitDemo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void InitDemo::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_lastMousePos.y));

		// Update angles based on input to orbit camera around box.
		m_theta += dx;
		m_phi += dy;

		// Restrict the angle mPhi.
		m_phi = CoreMath::Clamp(m_phi, 0.1f, CoreMath::PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.01f*static_cast<float>(x - m_lastMousePos.x);
		float dy = 0.01f*static_cast<float>(y - m_lastMousePos.y);

		// Update the camera radius based on input.
		m_radius += dx - dy;

		// Restrict the radius.
		m_radius = CoreMath::Clamp(m_radius, 3.0f, 200.0f);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}

bool InitDemo::InitPolygon()
{
	PolygonBuilder::Mesh cube;
	PolygonBuilder::Mesh grid;

	PolygonBuilder polyGen;
	polyGen.BuildCube(cube);
	polyGen.BuildGrid(100, 100, 101, 101, grid);

	UINT totalVertex = cube.vertices.size() + grid.vertices.size();
	UINT totalIndex = cube.indices.size() + grid.indices.size();

	std::vector<Vertex::BaseVertex> vertices(totalVertex);
	std::vector<UINT> indices(totalIndex);

	UINT k = 0;
	for (size_t i = 0; i < cube.vertices.size(); ++i, ++k)
	{
		vertices[k].pos = cube.vertices[i].position;
		vertices[k].normal = cube.vertices[i].normal;
		vertices[k].tex = cube.vertices[i].texcoord;
	}
	for (size_t i = 0; i < grid.vertices.size(); ++i, ++k)
	{
		vertices[k].pos = grid.vertices[i].position;
		vertices[k].normal = grid.vertices[i].normal;
		vertices[k].tex = grid.vertices[i].texcoord;
	}

	m_CubeOffsetVertex = 0;
	m_GridOffsetVertex = cube.vertices.size();

	m_CubeIndexCount = cube.indices.size();
	m_GridIndexCount = grid.indices.size();

	m_CubeOffsetIndex = 0;
	m_GridOffsetIndex = cube.indices.size();

	D3D11_BUFFER_DESC vbd;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::BaseVertex)*totalVertex;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initVertex;
	initVertex.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &initVertex, &m_VB));

	//indices.insert(indices.end(), cube.indices.begin(), cube.indices.end());
	//indices.insert(indices.end(), grid.indices.begin(), grid.indices.end());

	k = 0;
	for (size_t i = 0; i < cube.indices.size(); ++i,++k)
		indices[k] = cube.indices[i];
	for (size_t i = 0; i < grid.indices.size(); ++i, ++k)
		indices[k] = grid.indices[i];

	D3D11_BUFFER_DESC ibd;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)*totalIndex;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initIndex;
	initIndex.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &initIndex, &m_IB));

	D3DX11_PASS_DESC passDesc;
	m_FX->m_basicTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pDevice->CreateInputLayout(Vertex::BaseInputLayout, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_Input));
	return true;
}

void InitDemo::InitSkull()
{

	std::ifstream fin("models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertex::BaseVertex> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	m_SkullIndexCount = 3 * tcount;
	std::vector<UINT> indices(m_SkullIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::BaseVertex) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &m_SkullVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_SkullIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_SkullIB));


}

