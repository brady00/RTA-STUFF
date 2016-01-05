#include "Renderer.h"
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

bool Renderer::Init(HWND win)
{
	window = win;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.BufferDesc.RefreshRate = { 1, 60 };
	desc.BufferDesc.Height = 500;
	desc.BufferDesc.Width = 500;
	desc.SampleDesc.Count = 1;
	desc.OutputWindow = window;
	desc.Windowed = true;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &desc, &swapchain, &device, NULL, &devicecontext);
	swapchain->GetBuffer(0, __uuidof(BackBuffer), reinterpret_cast<void**>(&BackBuffer));
	device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
//	D3D11_RENDER_TARGET_VIEW_DESC viewdesc;
	swapchain->GetDesc(&desc);
	viewport.Height = (float)desc.BufferDesc.Height;
	viewport.Width = (float)desc.BufferDesc.Width;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	D3D11_TEXTURE2D_DESC textdesc;
	ZeroMemory(&textdesc, sizeof(textdesc));
	textdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textdesc.Format = DXGI_FORMAT_D32_FLOAT;
	textdesc.Width = 500;
	textdesc.Height = 500;
	textdesc.ArraySize = 1;
	textdesc.SampleDesc = { 1, 0 };
	textdesc.MipLevels = 1;
	device->CreateTexture2D(&textdesc, NULL, &DepthStencilBuffer);
	D3D11_DEPTH_STENCIL_VIEW_DESC stenview;
	ZeroMemory(&stenview, sizeof(stenview));
	stenview.Format = DXGI_FORMAT_D32_FLOAT;
	stenview.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(DepthStencilBuffer, &stenview, &DepthStencilView);
	return true;
}

bool Renderer::Run()
{
	devicecontext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	
	float color[4] = { 0, 0, 1, 0 };
	devicecontext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	devicecontext->ClearRenderTargetView(RenderTargetView, color);
	swapchain->Present(0, 0);
	return false;
}

bool Renderer::Shutdown()
{
	devicecontext->ClearState();

	ReleaseCOM(DepthStencilView);
	ReleaseCOM(RenderTargetView);
	ReleaseCOM(BackBuffer);
	ReleaseCOM(DepthStencilBuffer);
	ReleaseCOM(swapchain);
	ReleaseCOM(devicecontext);
	ReleaseCOM(device);

	return true;
}