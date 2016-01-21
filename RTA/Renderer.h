#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "ConstantBuffers.h"
#include "Camera.h"

//using namespace DirectX;
//
//struct MatrixHolder
//{
//	XMFLOAT4X4 * WorldMatrix = &XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //matrix space for each object
//	XMFLOAT4X4 * viewMatrix = &XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //camera
//	XMFLOAT4X4 * projMatrix = &XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //proj
//};
//
//
//class Camera
//{
//public:
//	Camera();
//	~Camera();
//	void init();
//	void CameraMovement();
//
//	ID3D11RenderTargetView		*	cameraRTV = nullptr;
//	ID3D11Buffer				*	cBuffer = nullptr;
//	ID3D11DepthStencilView		*	cameraDepthSV = nullptr;
//	XMFLOAT4X4 WorldMatrix = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //matrix space for each object
//	XMFLOAT4X4 viewMatrix = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //camera
//	XMFLOAT4X4 projMatrix = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //proj
//	//MatrixHolder camera;
//};

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
class RenderSet;
//class Camera;

class Renderer
{
private:	
	HWND window;
public:
	static IDXGISwapChain*					swapchain;
	static ID3D11Device*					device;
	static ID3D11DeviceContext*				devicecontext;
	static D3D11_VIEWPORT					viewport;
	static ID3D11RenderTargetView *			RenderTargetView;
	static ID3D11Texture2D*					BackBuffer;
	static ID3D11Texture2D*					DepthStencilBuffer;
	static ID3D11DepthStencilView*			DepthStencilView;
	static ID3D11SamplerState*				sampler;


	static Camera*							camera;



	static cbPerObject thePerObjectData;
	static ID3D11Buffer *thePerObjectCBuffer;
	static void BuildPerObjectConstantBuffers();
	static void SetPerObjectData(DirectX::XMFLOAT4X4 *mView, DirectX::XMFLOAT4X4 *mProj, DirectX::XMFLOAT4X4 &mWorld);

	Renderer(){};
	~Renderer(){};
	static bool Init(HWND win);
	static bool Render(RenderSet* set);
	static bool Present();
	static bool Shutdown();
};

