#pragma once
#include "Renderer.h"
//#include <DirectXMath.h>

using namespace DirectX;

struct MatrixHolder
{
	XMFLOAT4X4 * WorldMatrix	=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //matrix space for each object
	XMFLOAT4X4 * viewMatrix		=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //camera
	XMFLOAT4X4 * projMatrix		=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //proj
};


class Camera
{
public:
	Camera();
	~Camera();
	void init();
	void CameraMovement(XMFLOAT4X4 camIn);

	ID3D11RenderTargetView		*	cameraRTV = nullptr;
	ID3D11Buffer				*	cBuffer = nullptr;
	ID3D11DepthStencilView		*	cameraDepthSV = nullptr;
	XMFLOAT4X4 * WorldMatrix	=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //matrix space for each object
	XMFLOAT4X4 * viewMatrix		=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //camera
	XMFLOAT4X4 * projMatrix		=	&XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); //proj
	//MatrixHolder camera;
};
