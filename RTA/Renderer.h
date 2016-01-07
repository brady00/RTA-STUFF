#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "ConstantBuffers.h"

class RenderSet;

class Renderer
{
private:	
	HWND window;
public:
	static IDXGISwapChain*					swapchain;
	static ID3D11Device*					device;
	static ID3D11DeviceContext*			devicecontext;
	static D3D11_VIEWPORT					viewport;
	static ID3D11RenderTargetView *		RenderTargetView;
	static ID3D11Texture2D*				BackBuffer;
	static ID3D11Texture2D*				DepthStencilBuffer;
	static ID3D11DepthStencilView*			DepthStencilView;
	static cbPerObject thePerObjectData;
	static ID3D11Buffer *thePerObjectCBuffer;
	static void BuildPerObjectConstantBuffers();

	Renderer(){};
	~Renderer(){};
	bool Init(HWND win);
	bool Render(RenderSet &set);
	bool Shutdown();
};

