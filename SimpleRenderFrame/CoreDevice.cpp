#include "CoreDevice.h"
#include<windowsx.h>
#include<sstream>

namespace
{
	CoreDevice*g_device;
}

LRESULT CALLBACK GlobelWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_device->WndProc(hwnd, msg, wParam, lParam);
}

CoreDevice::CoreDevice(HINSTANCE hinstance, int width, int height) :
	m_hwnd(0),
	m_Ins(hinstance),
	m_clientHeight(height),
	m_clientWidth(width),
	m_wndCaption(L"D3D Application"),
	m_pSwapChain(NULL),
	m_pDevice(NULL),
	m_pImContext(NULL),
	m_pRenderTargetView(NULL),
	m_pDepthStencilView(NULL),
	m_BlendOff(0),
	m_BlendOn(0),
	m_CullBack(0),
	m_CullFront(0),
	m_CullNone(0),
	m_WireFrame(0),
	m_SolidFrame(0),
	m_DepthDisable(0),
	m_DepthEnable(0),
	m_driverType(D3D_DRIVER_TYPE_HARDWARE),
	m_maximized(false),
	m_minimized(false),
	m_resizing(false),
	m_enable4xMsaa(false),
	m_4xMsaaQuality(0),
	m_timer()
{
	ZeroMemory(&m_screenViewPort, sizeof(D3D11_VIEWPORT));
	g_device = this;
}

CoreDevice::~CoreDevice()
{
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthBuffer);
	ReleaseCOM(m_BlendOff);
	ReleaseCOM(m_BlendOn);
	ReleaseCOM(m_DepthDisable);
	ReleaseCOM(m_DepthEnable);
	ReleaseCOM(m_CullBack);
	ReleaseCOM(m_CullFront);
	ReleaseCOM(m_CullNone);
	ReleaseCOM(m_WireFrame);
	ReleaseCOM(m_SolidFrame);
	ReleaseCOM(m_pSwapChain);
	if (m_pImContext)
		m_pImContext->ClearState();
	ReleaseCOM(m_pImContext);
	ReleaseCOM(m_pDevice);
}

HINSTANCE CoreDevice::AppInst() const
{
	return m_Ins;
}

HWND CoreDevice::AppMainWnd() const
{
	return m_hwnd;
}

float CoreDevice::AspectRatio() const
{
	return m_clientWidth/ static_cast<float>(m_clientHeight);
}

bool CoreDevice::Init()
{
	if (!InitWindow())
		return false;
	if (!InitD3D11())
		return false;
	return true;
}

int CoreDevice::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	m_timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer.Tick();
			if (!m_appPaused)
			{
				CaculateFPS();
				Update(m_timer.GetDeltaTime());
				Render();
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

void CoreDevice::OnResize()
{
	assert(m_pImContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	//Release current Devices 
	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = NULL;
	}
	if (m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = NULL;
	}
	if (m_pDepthTexture)
	{
		m_pDepthTexture->Release();
		m_pDepthTexture = NULL;
	}

	HRESULT hr = m_pSwapChain->ResizeBuffers(0, m_clientWidth, m_clientHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	if (FAILED(hr))
	{
		MessageBox(0, L"Resize Buffer Failed", L"error", MB_OK);
		return;
	}
	ID3D11Texture2D*backBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Get Buffer Failed", L"error", MB_OK);
		return;
	}
	hr = m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create RTV Failed", L"error", MB_OK);
		return;
	}

	backBuffer->Release();

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_clientWidth;
	desc.Height = m_clientHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (m_enable4xMsaa)
	{
		desc.SampleDesc.Count = 4;
		desc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
	}

	hr = m_pDevice->CreateTexture2D(&desc, 0, &m_pDepthTexture);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create Depth Buffer Failed", L"error", MB_OK);
		return;
	}
	hr = m_pDevice->CreateDepthStencilView(m_pDepthTexture, 0, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create DSV Failed", L"error", MB_OK);
		return;
	}

	m_pImContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	m_screenViewPort.TopLeftX = 0;
	m_screenViewPort.TopLeftY = 0;
	m_screenViewPort.MaxDepth = 1.0f;
	m_screenViewPort.MinDepth = 0.0f;
	m_screenViewPort.Width = static_cast<float>(m_clientWidth);
	m_screenViewPort.Height = static_cast<float>(m_clientHeight);

	m_pImContext->RSSetViewports(1, &m_screenViewPort);
}

LRESULT CoreDevice::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_appPaused = true;
			m_timer.Stop();
		}
		else
		{
			m_appPaused = false;
			m_timer.Start();
		}
		return 0;
	case WM_SIZE:
		// Save the new client area dimensions.
		m_clientWidth = LOWORD(lParam);
		m_clientHeight = HIWORD(lParam);
		if (m_pDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_appPaused = true;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_appPaused = false;
				m_minimized = false;
				m_maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				// Restoring from minimized state?
				if (m_minimized)
				{
					m_appPaused = false;
					m_minimized = false;
					OnResize();
				}
				// Restoring from maximized state?
				else if (m_maximized)
				{
					m_appPaused = false;
					m_maximized = false;
					OnResize();
				}
				else if (m_resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		m_appPaused = true;
		m_resizing = true;
		m_timer.Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		m_appPaused = false;
		m_resizing = false;
		m_timer.Start();
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CoreDevice::RenderBegin()
{
	assert(m_pImContext);
	m_pImContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Silver);
	m_pImContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CoreDevice::RenderEnd()
{
	assert(m_pSwapChain);
	m_pSwapChain->Present(0, 0);
}

void CoreDevice::OnMouseDown(WPARAM btnState, int x, int y)
{
}

void CoreDevice::OnMouseUp(WPARAM btnState, int x, int y)
{
}

void CoreDevice::OnMouseMove(WPARAM btnState, int x, int y)
{
}

bool CoreDevice::InitWindow()
{
	wchar_t*classname = L"MyClass";

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = m_Ins;
	wc.lpfnWndProc = GlobelWndProc;
	wc.style= CS_HREDRAW | CS_VREDRAW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = classname;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	RECT rc = { 0,0,m_clientWidth,m_clientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, NULL);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	m_hwnd = CreateWindow(classname, m_wndCaption.c_str(), WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, m_Ins, NULL);

	if (!m_hwnd)
	{
		MessageBox(0, L"Create Window Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

bool CoreDevice::InitD3D11()
{
	// Create the device and device context.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		m_driverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_pDevice,
		&featureLevel,
		&m_pImContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	//assert(m4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_clientWidth;
	sd.BufferDesc.Height = m_clientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if (m_enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Interface Faild", L"error", MB_OK);
		return 0;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Adapter Faild", L"error", MB_OK);
		return 0;
	}

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"Query Factory Faild", L"error", MB_OK);
		return 0;
	}

	hr = dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"Create Swap Chain Faild", L"error", MB_OK);
		return 0;
	}

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	OnResize();

	InitState();

	return true;
}

bool CoreDevice::InitState()
{
	//*****Cull and Frame state
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_CullBack));

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_CullFront));


	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_CullNone));

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_WireFrame));


	//*****Blend state
	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	// Create the blend state using the description.
	HR(m_pDevice->CreateBlendState(&blendStateDescription, &m_BlendOn));
	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	HR(m_pDevice->CreateBlendState(&blendStateDescription, &m_BlendOff));

	return true;
}

void CoreDevice::CaculateFPS()
{
	static int AVEN_FPS = 0;
	static float SpendSecond = 0.0f;

	++AVEN_FPS;
	if ((m_timer.GetTotalTime() - SpendSecond) >= 1.0f)
	{
		int FPS = (float)AVEN_FPS;

		std::wostringstream outs;
		outs.precision(4);
		outs << m_wndCaption << L"  FPS:"
			<< FPS<<L"   DeltaTime:"<<m_timer.GetDeltaTime();

		SetWindowText(m_hwnd, outs.str().c_str());

		AVEN_FPS = 0;
		SpendSecond += 1.0f;
	}
}

XMMATRIX CoreDevice::SetPosition(XMFLOAT3 pos)
{
	return XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
}

XMMATRIX CoreDevice::SetScale(XMFLOAT3 scale)
{
	return XMMATRIX();
}

XMMATRIX CoreDevice::SetRotate(XMFLOAT3 rotate)
{
	return XMMATRIX();
}

void CoreDevice::SetBlend(DeviceState state)
{
	float factor[4] = { 0.0f,0.0f,0.0f,0.0f };
	switch (state)
	{
	case CoreDevice::OFF:
		m_pImContext->OMSetBlendState(m_BlendOff, factor, 0xffffffff);
		break;
	case CoreDevice::ON:
		m_pImContext->OMSetBlendState(m_BlendOn, factor, 0xffffffff);
		break;
	default:
		break;
	}
}

void CoreDevice::SetCullState(RasterizeState state)
{
	switch (state)
	{
	case CoreDevice::CullBack:
		m_pImContext->RSSetState(m_CullBack);
		break;
	case CoreDevice::CullFront:
		m_pImContext->RSSetState(m_CullFront);
		break;
	case CoreDevice::CullNone:
		m_pImContext->RSSetState(m_CullNone);
		break;
	default:
		break;
	}
}

void CoreDevice::SetFrameState(RasterizeState state)
{
	switch (state)
	{
	case CoreDevice::Wireframe:
		m_pImContext->RSSetState(m_WireFrame);
		break;
	case CoreDevice::Solidframe:
		m_pImContext->RSSetState(m_CullBack);
		break;
	default:
		break;
	}
}

void CoreDevice::SetDepthState(DeviceState state)
{
	switch (state)
	{
	case CoreDevice::OFF:
		break;
	case CoreDevice::ON:
		break;
	default:
		break;
	}
}
