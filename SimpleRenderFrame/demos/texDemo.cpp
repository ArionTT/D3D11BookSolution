#include "texDemo.h"

texDemo::texDemo(HINSTANCE hinstance) :CoreDevice(hinstance),
m_VB(NULL), m_IB(NULL), m_radius(15.0f), m_theta(1.5f*CoreMath::PI), m_phi(0.1f*CoreMath::PI)
{
}

texDemo::texDemo(HINSTANCE hinstance, int width, int height) : CoreDevice(hinstance, width, height),
m_VB(NULL), m_IB(NULL), m_radius(15.0f), m_theta(1.5f*CoreMath::PI), m_phi(0.1f*CoreMath::PI)
{
}

texDemo::~texDemo()
{
}

bool texDemo::Init()
{
	if (!CoreDevice::Init())
		return false;

	m_FX = new CoreEffect(m_pDevice, L"hlsl/texbox.fxo");
	m_FxGrid = new CoreEffect(m_pDevice, L"hlsl/hills.fxo");
	m_FxWater = new CoreEffect(m_pDevice, L"hlsl/water.fxo");
	m_FxFire = new CoreEffect(m_pDevice, L"hlsl/fire.fxo");

	if (!InitPolygon())
		return false;

	m_matCube.diffuse = XMFLOAT4(0.5f, 0.5f, 0.0f,1.0f);
	m_matCube.ambient = XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f);
	m_matCube.specular = XMFLOAT3(0.5f, 0.5f, 0.0f);
	m_matCube.gloss = 8.0f;

	m_matGrid.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_matGrid.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_matGrid.specular = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_matGrid.gloss = 1.0f;

	m_matWater.diffuse= XMFLOAT4(0.0f, 0.2f, 0.4f, 0.6f);
	m_matWater.ambient = XMFLOAT4(0.0f, 0.2f, 0.4f,1.0f);
	m_matWater.specular = XMFLOAT3(0.0f, 0.2f, 0.4f);
	m_matWater.gloss = 30.0f;

	m_matFire.diffuse= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_matFire.ambient= XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f);
	m_matFire.specular = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_matFire.gloss= 0.0f;

	m_dirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_dirLight.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_dirLight.dir = XMFLOAT3(1.0f, 1.0f, -1.0f);

	m_fog.fogColor = XMFLOAT4(Colors::Silver[0],Colors::Silver[1],Colors::Silver[2], 1.0f);
	m_fog.fogVar = XMFLOAT2(35.0f, 225.0f);

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_world, I);
	XMStoreFloat4x4(&m_view, I);
	XMStoreFloat4x4(&m_proj, I);

	HR(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"textures/WoodCrate01.dds", 0, 0, &m_diffuseMapSRV, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"textures/grass.dds", 0, 0, &m_grassMapSRV, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"textures/water2.dds", 0, 0, &m_waterMapSRV, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"textures/ImgAnime/boom.png", 0, 0, &m_fireMapSRV, 0));

	m_water_ST = XMFLOAT4(5.0f, 5.0f, 0.0f, 0.0f);
	m_grid_ST = XMFLOAT4(5.0f, 5.0f, 1.0f, 1.0f);
	m_fire_ST = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);

	OnResize();

	return true;
}

void texDemo::OnResize()
{
	CoreDevice::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*CoreMath::PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_proj, P);
}

void texDemo::Update(float deltaTime)
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

	//Water wave 
	m_water_ST.z+=0.05f*deltaTime;
	m_water_ST.w += 0.03f*deltaTime;

	//Fire 
	int row = 8;
	int col = 8;
	m_fire_ST.x = 1.0f / row;
	m_fire_ST.y = 1.0f / col;
	static float time = 0;
	static int m = 0;
	static int n = 0;
	time += deltaTime;
	if (time >= 0.01f)
	{
		m_fire_ST.z += 1.0f / row;
		m += 1;
		if (m >= row)
		{
			m_fire_ST.w += 1.0f / col;
			m = 0;
			n += 1;
			if (n >= col)
				n = 0;
		}
		time = 0;
	}


	m_deltaTime = deltaTime;
}

void texDemo::Render()
{
	RenderBegin();

	if (GetAsyncKeyState('0') & 0x8000)
		SetFrameState(RasterizeState::Wireframe);
	if (GetAsyncKeyState('1') & 0x8000)
		SetFrameState(RasterizeState::Solidframe);
	if (GetAsyncKeyState('2') & 0x8000)
		SetCullState(RasterizeState::CullNone);
	if (GetAsyncKeyState('3') & 0x8000)
		SetCullState(RasterizeState::CullBack);


	m_pImContext->IASetInputLayout(m_Input);
	m_pImContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);

	m_FX->SetDirectLight(m_dirLight);
	m_FX->SetEyePos(m_ViewPos);
	m_FX->SetW(world);
	m_FX->SetTexture(m_diffuseMapSRV);
	m_FX->SetFog(m_fog);

	m_FxGrid->SetDirectLight(m_dirLight);
	m_FxGrid->SetEyePos(m_ViewPos);
	m_FxGrid->SetW(world);
	m_FxGrid->SetTexture(m_grassMapSRV);
	m_FxGrid->SetFog(m_fog);

	m_FxWater->SetDirectLight(m_dirLight);
	m_FxWater->SetEyePos(m_ViewPos);
	m_FxWater->SetW(world);
	m_FxWater->SetTexture(m_waterMapSRV);
	m_FxWater->SetFog(m_fog);

	m_FxFire->SetDirectLight(m_dirLight);
	m_FxFire->SetEyePos(m_ViewPos);
	m_FxFire->SetW(world);
	m_FxFire->SetTexture(m_fireMapSRV);
	m_FxFire->SetFog(m_fog);


	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique*tech = m_FX->m_basicTech;
	tech->GetDesc(&techDesc);

	UINT stride = sizeof(Vertex::BaseVertex);
	UINT offset = 0;
	m_pImContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_pImContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		//Cube Render
		XMMATRIX model = SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
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
	}

	D3DX11_TECHNIQUE_DESC techDesc2;
	ID3DX11EffectTechnique*GridTech = m_FxGrid->m_basicTech;
	GridTech->GetDesc(&techDesc2);
	for (UINT p = 0; p < techDesc2.Passes; p++)
	{
		//Grid Render
		XMMATRIX model = XMLoadFloat4x4(&m_world);
		XMMATRIX mvp = model*view*proj;
		m_FxGrid->SetM(model);
		m_FxGrid->SetV(view);
		m_FxGrid->SetP(proj);
		m_FxGrid->SetVP(view*proj);
		m_FxGrid->SetMVP(mvp);
		m_FxGrid->SetEyePos(m_ViewPos);
		m_FxGrid->SetMaterial(m_matGrid);
		m_FxGrid->SetTextureST(m_grid_ST);

		GridTech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_GridIndexCount, m_GridOffsetIndex, m_GridOffsetVertex);
	}

	D3DX11_TECHNIQUE_DESC techDesc3;
	ID3DX11EffectTechnique*WaterTech = m_FxWater->m_basicTech;
	WaterTech->GetDesc(&techDesc3);
	for (UINT p = 0; p < techDesc3.Passes; p++)
	{
		//Grid Render
		XMMATRIX model = XMLoadFloat4x4(&m_world);
		XMMATRIX mvp = model*view*proj;
		m_FxWater->SetM(model);
		m_FxWater->SetV(view);
		m_FxWater->SetP(proj);
		m_FxWater->SetVP(view*proj);
		m_FxWater->SetMVP(mvp);
		m_FxWater->SetEyePos(m_ViewPos);
		m_FxWater->SetMaterial(m_matWater);
		m_FxWater->SetTextureST(m_water_ST);

		SetBlend(CoreDevice::DeviceState::ON);
		WaterTech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_WaterIndexCount, m_WaterOffsetIndex, m_WaterOffsetVertex);
		SetBlend(CoreDevice::DeviceState::OFF);
	}

	D3DX11_TECHNIQUE_DESC techDesc4;
	ID3DX11EffectTechnique*FireTech = m_FxFire->m_basicTech;
	FireTech->GetDesc(&techDesc4);
	for (UINT p = 0; p < techDesc4.Passes; p++)
	{
		//Grid Render
		XMMATRIX model = SetPosition(XMFLOAT3(0.0f, 25.0f, 0.0f)); 
		XMMATRIX mvp = model*view*proj;
		m_FxFire->SetM(model);
		m_FxFire->SetV(view);
		m_FxFire->SetP(proj);
		m_FxFire->SetVP(view*proj);
		m_FxFire->SetMVP(mvp);
		m_FxFire->SetEyePos(m_ViewPos);
		m_FxFire->SetMaterial(m_matFire);
		m_FxFire->SetTextureST(m_fire_ST);

		FireTech->GetPassByIndex(p)->Apply(0, m_pImContext);
		m_pImContext->DrawIndexed(m_FireIndexCount, m_FireOffsetIndex, m_FireOffsetVertex);


	}

	//D3DX11_TECHNIQUE_DESC techDesc5;
	//ID3DX11EffectTechnique*lightTech = m_FxDirLight->m_basicTech;
	//lightTech->GetDesc(&techDesc5);
	//for (UINT p = 0; p < techDesc5.Passes; p++)
	//{
	//	//Light Render
	//	XMMATRIX model = SetPosition(m_dirLight.dir);
	//	XMMATRIX mvp = model*view*proj;
	//	m_FxFire->SetM(model);
	//	m_FxFire->SetV(view);
	//	m_FxFire->SetP(proj);
	//	m_FxFire->SetVP(view*proj);
	//	m_FxFire->SetMVP(mvp);
	//	m_FxFire->SetEyePos(m_ViewPos);

	//	lightTech->GetPassByIndex(p)->Apply(0, m_pImContext);
	//	m_pImContext->DrawIndexed(m_FireIndexCount, m_FireOffsetIndex, m_FireOffsetVertex);
	//}


	RenderEnd();
}

void texDemo::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;

	SetCapture(m_hwnd);
}

void texDemo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void texDemo::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_lastMousePos.y));

		// Update angles based on input to orbit camera around box.
		m_theta -= dx;
		m_phi -= dy;

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

bool texDemo::InitPolygon()
{
	PolygonBuilder::Mesh cube;
	PolygonBuilder::Mesh grid;
	PolygonBuilder::Mesh water;
	PolygonBuilder::Mesh fire;

	PolygonBuilder polyGen;
	polyGen.BuildCylinder(5, 5, 20, 20, 20, cube);
	polyGen.BuildGrid(300, 300, 201, 201, grid);
	polyGen.BuildGrid(300, 300, 201, 201, water);
	polyGen.BuildQuad(10, 10, fire);


	UINT totalVertex = cube.vertices.size() + grid.vertices.size()+water.vertices.size()+fire.vertices.size();
	UINT totalIndex = cube.indices.size() + grid.indices.size() + water.indices.size()+fire.indices.size();

	std::vector<Vertex::BaseVertex> vertices(totalVertex);
	std::vector<UINT> indices(totalIndex);

	UINT k = 0;
	for (size_t i = 0; i < cube.vertices.size(); ++i, ++k)
	{
		vertices[k].pos = cube.vertices[i].position;
		vertices[k].normal = cube.vertices[i].normal;
		vertices[k].tex = cube.vertices[i].texcoord;
	}

	float y = 0;
	for (size_t i = 0; i < grid.vertices.size(); ++i, ++k)
	{
		y = CaculateHeight(grid.vertices[i].position.x, grid.vertices[i].position.z);
		vertices[k].pos = XMFLOAT3(grid.vertices[i].position.x, y, grid.vertices[i].position.z);
		vertices[k].normal = GetHillNormal(grid.vertices[i].position.x, grid.vertices[i].position.z);
		vertices[k].tex = grid.vertices[i].texcoord;
	}

	for (size_t i = 0; i < water.vertices.size(); ++i, ++k)
	{
		vertices[k].pos = water.vertices[i].position;
		vertices[k].normal = water.vertices[i].normal;
		vertices[k].tex = water.vertices[i].texcoord;
	}

	for (size_t i = 0; i < fire.vertices.size(); ++i, ++k)
	{
		vertices[k].pos = fire.vertices[i].position;
		vertices[k].normal = fire.vertices[i].normal;
		vertices[k].tex = fire.vertices[i].texcoord;
	}

	m_CubeOffsetVertex = 0;
	m_GridOffsetVertex = cube.vertices.size();
	m_WaterOffsetVertex = grid.vertices.size()+m_GridOffsetVertex;
	m_FireOffsetVertex = m_WaterOffsetVertex + water.vertices.size();

	m_CubeIndexCount = cube.indices.size();
	m_GridIndexCount = grid.indices.size();
	m_WaterIndexCount = water.indices.size();
	m_FireIndexCount = fire.indices.size();

	m_CubeOffsetIndex = 0;
	m_GridOffsetIndex = m_CubeIndexCount;
	m_WaterOffsetIndex = m_GridOffsetIndex+m_GridIndexCount;
	m_FireOffsetIndex = m_WaterOffsetIndex + m_WaterIndexCount;

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
	for (size_t i = 0; i < cube.indices.size(); ++i, ++k)
		indices[k] = cube.indices[i];
	for (size_t i = 0; i < grid.indices.size(); ++i, ++k)
		indices[k] = grid.indices[i];
	for (size_t i = 0; i < water.indices.size(); ++i, ++k)
		indices[k] = water.indices[i];
	for (size_t i = 0; i < fire.indices.size(); ++i, ++k)
		indices[k] = fire.indices[i];

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

float texDemo::CaculateHeight(float x,float z)
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}

XMFLOAT3 texDemo::GetHillNormal(float x, float z)
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}

