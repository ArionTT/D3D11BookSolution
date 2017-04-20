#pragma once

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include"utils\CoreUtils.h"
#include"utils\Timer.h"

using std::wstring;


class CoreDevice
{
public:
	CoreDevice(HINSTANCE hinstance, int width = 800, int height = 600);
	~CoreDevice();
	HINSTANCE AppInst()const;
	HWND AppMainWnd() const;
	float AspectRatio()const;


public:
	virtual bool Init();
	virtual int Run();
	virtual void OnResize();
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void Render() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	//virtual void OnKeyDown();
	//virtual void OnKeyUp();

protected:
	bool InitWindow();
	bool InitD3D11();
	bool InitState();
	void CaculateFPS();

	XMMATRIX SetPosition(XMFLOAT3 pos);
	XMMATRIX SetScale(XMFLOAT3 scale);
	XMMATRIX SetRotate(XMFLOAT3 rotate);

protected:
	enum DeviceState
	{
		OFF = 0, ON = 1
	};
	enum RasterizeState
	{
		Wireframe, Solidframe, CullBack, CullFront, CullNone
	};
	void SetBlend(DeviceState state);
	void SetCullState(RasterizeState state);
	void SetFrameState(RasterizeState state);
	void SetDepthState(DeviceState state);

protected:
	HWND m_hwnd;
	HINSTANCE m_Ins;
	wstring m_wndCaption;
	bool m_appPaused;
	bool m_paused;
	bool m_minimized;
	bool m_maximized;
	bool m_resizing;
	int m_clientWidth;
	int m_clientHeight;
	int m_wndWidth;
	int m_wndHeight;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device*m_pDevice;
	ID3D11DeviceContext*m_pImContext;
	ID3D11RenderTargetView*m_pRenderTargetView;
	ID3D11DepthStencilView*m_pDepthStencilView;
	ID3D11Texture2D*m_pDepthBuffer;
	D3D11_VIEWPORT m_screenViewPort;
	ID3D11Texture2D*m_pRenderTexture;
	ID3D11Texture2D*m_pDepthTexture;
	ID3D11BlendState*m_BlendOn;
	ID3D11BlendState*m_BlendOff;
	ID3D11RasterizerState*m_CullBack;
	ID3D11RasterizerState*m_CullFront;
	ID3D11RasterizerState*m_CullNone;
	ID3D11RasterizerState*m_WireFrame;
	ID3D11RasterizerState*m_SolidFrame;
	ID3D11DepthStencilState*m_DepthEnable;
	ID3D11DepthStencilState*m_DepthDisable;
	D3D_DRIVER_TYPE m_driverType;
	UINT m_4xMsaaQuality;
	bool m_enable4xMsaa;

	Timer m_timer;
};

