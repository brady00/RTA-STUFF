#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#include "RenderSet.h"
#include "RenderNode.h"
#include "Renderer.h"

ID3D11Device * Renderer::device = 0;
ID3D11DeviceContext * Renderer::devicecontext = 0;
IDXGISwapChain * Renderer::swapchain = 0;
ID3D11RenderTargetView * Renderer::RenderTargetView = 0;
ID3D11Texture2D * Renderer::BackBuffer = 0;
ID3D11Texture2D * Renderer::DepthStencilBuffer = 0;
ID3D11DepthStencilView * Renderer::DepthStencilView = 0;
ID3D11Buffer * Renderer::thePerObjectCBuffer = 0;
cbPerObject Renderer::thePerObjectData;
D3D11_VIEWPORT Renderer::viewport;
ID3D11SamplerState* Renderer::sampler = 0;

bool Renderer::Init(HWND win)
{
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
	desc.OutputWindow = win;
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
	BuildPerObjectConstantBuffers();
	D3D11_SAMPLER_DESC desc2;
	//anisoWrapSampler
	desc2.Filter = D3D11_FILTER_ANISOTROPIC;
	desc2.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc2.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc2.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc2.MipLODBias = 0.0f;
	desc2.MaxAnisotropy = 4;
	desc2.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc2.BorderColor[0] = desc2.BorderColor[1] = desc2.BorderColor[2] = desc2.BorderColor[3] = 0;
	desc2.MinLOD = -FLT_MAX;
	desc2.MaxLOD = FLT_MAX;
	Renderer::device->CreateSamplerState(&desc2, &sampler);
	return true;
}

bool Renderer::Render(RenderSet *set)
{
	devicecontext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	
	float color[4] = { 0, 0, 1, 0 };
	devicecontext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	devicecontext->ClearRenderTargetView(RenderTargetView, color);
	devicecontext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	devicecontext->RSSetViewports(1, &viewport);
	devicecontext->PSSetSamplers(0, 1, &sampler);

	RenderNode *pCurrent = set->GetHead();

	while (0 != pCurrent)
	{
		pCurrent->RenderProcess();
		pCurrent = pCurrent->GetNext();
	}

	return true;
}

bool Renderer::Present()
{
	swapchain->Present(0, 0);
	return true;
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
	ReleaseCOM(thePerObjectCBuffer);
	ReleaseCOM(sampler);


	return true;
}

void Renderer::BuildPerObjectConstantBuffers()
{
	// per object CBuffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(cbPerObject);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&desc, nullptr, &thePerObjectCBuffer);

}

void Renderer::SetPerObjectData(DirectX::XMFLOAT4X4 &mMVP, DirectX::XMFLOAT4X4 &mWorld)
{
	thePerObjectData.gMVP = mMVP;
	thePerObjectData.gWorld = mWorld;

	D3D11_MAPPED_SUBRESOURCE edit;
	devicecontext->Map(thePerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &edit);
	memcpy(edit.pData, &thePerObjectData, sizeof(thePerObjectData));
	devicecontext->Unmap(thePerObjectCBuffer, 0);

	devicecontext->VSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
	devicecontext->PSSetConstantBuffers(cbPerObject::REGISTER_SLOT, 1, &thePerObjectCBuffer);
}
