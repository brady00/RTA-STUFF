#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
	delete projMatrix;
	delete viewMatrix;
	delete invViewMatrix;
}

void Camera::init()
{
	
	viewMatrix = new XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -20, 1);
	invViewMatrix = new XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -20, 1);
	DirectX::XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(DirectX::XMLoadFloat4x4(viewMatrix)), DirectX::XMLoadFloat4x4(viewMatrix)));
	projMatrix = new XMFLOAT4X4();
	XMStoreFloat4x4(projMatrix,XMMatrixPerspectiveFovLH(3.14f/2, 1, 0.1f, 1000));
}

void Camera::CameraMovement()
{
	//camera.&viewmatrix = camIn;
	GetCursorPos(&prevCursor);
	float x = 0, y = 0, z = 0;

	if (GetAsyncKeyState('D'))
		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixTranslation(-0.002f, 0, 0), XMLoadFloat4x4(viewMatrix)));
	if (GetAsyncKeyState('A'))
		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixTranslation(0.002f, 0, 0), XMLoadFloat4x4(viewMatrix)));
	if (GetAsyncKeyState('W'))
		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixTranslation(0, 0, -0.002f), XMLoadFloat4x4(viewMatrix)));
	if (GetAsyncKeyState('S'))
		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixTranslation(0, 0, 0.002f), XMLoadFloat4x4(viewMatrix)));

	//if (GetAsyncKeyState(RI_MOUSE_LEFT_BUTTON_DOWN))
	//{
	//	POINT mouse;
	//	GetCursorPos(&mouse);
	//	int x = mouse.x - prevCursor.x;
	//	int y = mouse.y - prevCursor.y;
	//	if (x != 0)
	//		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixRotationY(x), XMLoadFloat4x4(viewMatrix)));
	//	if (y != 0)																  
	//		XMStoreFloat4x4(viewMatrix, DirectX::XMMatrixMultiply(XMMatrixRotationX(y), XMLoadFloat4x4(viewMatrix)));
	//
	//}
	DirectX::XMStoreFloat4x4(invViewMatrix, DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(DirectX::XMLoadFloat4x4(viewMatrix)), DirectX::XMLoadFloat4x4(viewMatrix)));
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			invViewMatrix->m[i][j] = 0;
			if (i == j)
				invViewMatrix->m[i][j] = 1;
		}
	}
}

