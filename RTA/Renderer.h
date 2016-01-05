#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class Renderer
{
private:

	IDXGISwapChain*					swapchain;
	ID3D11Device*					device;
	ID3D11DeviceContext*			devicecontext;
	D3D11_VIEWPORT					viewport;
	ID3D11RenderTargetView *		RenderTargetView;
	ID3D11Texture2D*				BackBuffer;
	ID3D11Texture2D*				DepthStencilBuffer;
	ID3D11DepthStencilView*			DepthStencilView;
	HWND window;

public:
	Renderer();
	~Renderer();
	bool Init(HWND win);
	bool Run();
	bool Shutdown();
};

