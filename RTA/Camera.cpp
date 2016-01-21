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
	//	D3D11_TEXTURE2D_DESC tDesc;
	//	memset(&tDesc, NULL, sizeof(tDesc));
	//	tDesc.Width = 400;
	//	tDesc.Height = 400;
	//	tDesc.MipLevels = 1;
	//	tDesc.ArraySize = 1;
	//	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//	tDesc.SampleDesc.Count = 1;
	//	tDesc.Usage = D3D11_USAGE_DEFAULT;
	//	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	//	//	tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//
	//	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	//	memset(&rtDesc, NULL, sizeof(rtDesc));
	//	rtDesc.Format = tDesc.Format;
	//	rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//
	//	ID3D11Texture2D * ptext;
	//
	//	//HRESULT workingResult = Renderer::device->CreateTexture2D(&tDesc, nullptr, &ptext);
	//	//workingResult = Renderer::device->CreateRenderTargetView(ptext, &rtDesc, &Renderer::RenderTargetView);
	////	ptext->Release();
	//
	//	memset(&tDesc, NULL, sizeof(tDesc));
	//	tDesc.Width = 500;
	//	tDesc.Height = 500;
	//	tDesc.MipLevels = 1;
	//	tDesc.ArraySize = 1;
	//	tDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//	tDesc.SampleDesc.Count = 1;
	//	tDesc.Usage = D3D11_USAGE_DEFAULT;
	//	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//
	//	//workingResult = Renderer::device->CreateTexture2D(&tDesc, nullptr, &ptext);
	//	//workingResult = Renderer::device->CreateDepthStencilView(ptext, nullptr, &Renderer::DepthStencilView);
	//
	////	ptext->Release();
	//
	//
	//#pragma region Camera Buffer
	//
	//	D3D11_BUFFER_DESC sCBuff;
	//	memset(&sCBuff, NULL, sizeof(sCBuff));
	//	sCBuff.Usage = D3D11_USAGE_DYNAMIC;
	//	sCBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	sCBuff.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	sCBuff.ByteWidth = sizeof(MatrixHolder);
	//
	//	D3D11_SUBRESOURCE_DATA scs;
	//	memset(&scs, NULL, sizeof(scs));
	//	scs.pSysMem = &camera;

	//camera.projMatrix = XMMatrixIdentity();

#pragma endregion
	//workingResult = Renderer::device->CreateBuffer(&sCBuff, &scs, &cBuffer);

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



//GCamera::GCamera(void)
//{
//	mPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);
//	mTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
//	mUp = GMathVF(GMathFV(mPosition) + GMathFV(XMFLOAT3(0, 1, 0)));
//	this->init&viewmatrix();
//
//	mAngle = 0.0f;
//	mClientWidth = 0.0f;
//	mClientHeight = 0.0f;
//	mNearest = 0.0f;
//	mFarthest = 0.0f;
//
//	XMStoreFloat4x4(&mView, XMMatrixIdentity());
//	XMStoreFloat4x4(&mProj, XMMatrixIdentity());
//	XMStoreFloat4x4(&mOrtho, XMMatrixIdentity());
//}
//
//GCamera::GCamera(const GCamera& camera)
//{
//	*this = camera;
//}

//GCamera& GCamera::operator=(const GCamera& camera)
//{
//	mPosition = camera.mPosition;
//	mTarget = camera.mTarget;
//	mUp = camera.mUp;
//
//	mAngle = camera.mAngle;
//	mClientWidth = camera.mClientWidth;
//	mClientHeight = camera.mClientHeight;
//	mNearest = camera.mNearest;
//	mFarthest = camera.mFarthest;
//
//	mView = camera.mView;
//	mProj = camera.mProj;
//	mOrtho = camera.mOrtho;
//	return *this;
//}
//
//void GCamera::init&viewmatrix()
//{
//	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&mTarget),
//		XMLoadFloat3(&this->Up())));
//}
//
//void GCamera::InitProjMatrix(const float angle, const float client_width, const float client_height,
//	const float near_plane, const float far_plane)
//{
//	mAngle = angle;
//	mClientWidth = client_width;
//	mClientHeight = client_height;
//	mNearest = near_plane;
//	mFarthest = far_plane;
//	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(angle, client_width / client_height,
//		near_plane, far_plane));
//}
//
//void GCamera::Move(XMFLOAT3 direction)
//{
//	mPosition = GMathVF(XMVector3Transform(GMathFV(mPosition),
//		XMMatrixTranslation(direction.x, direction.y, direction.z)));
//	mTarget = GMathVF(XMVector3Transform(GMathFV(mTarget),
//		XMMatrixTranslation(direction.x, direction.y, direction.z)));
//	mUp = GMathVF(XMVector3Transform(GMathFV(mUp),
//		XMMatrixTranslation(direction.x, direction.y, direction.z)));
//
//	this->init&viewmatrix();
//}
//
//void GCamera::Rotate(XMFLOAT3 axis, float degrees)
//{
//	if (XMVector3Equal(GMathFV(axis), XMVectorZero()) ||
//		degrees == 0.0f)
//		return;
//
//	// rotate vectors
//	XMFLOAT3 look_at_target = GMathVF(GMathFV(mTarget) - GMathFV(mPosition));
//	XMFLOAT3 look_at_up = GMathVF(GMathFV(mUp) - GMathFV(mPosition));
//	look_at_target = GMathVF(XMVector3Transform(GMathFV(look_at_target),
//		XMMatrixRotationAxis(GMathFV(axis), XMConvertToRadians(degrees))));
//	look_at_up = GMathVF(XMVector3Transform(GMathFV(look_at_up),
//		XMMatrixRotationAxis(GMathFV(axis), XMConvertToRadians(degrees))));
//
//	// restore vectors's end points mTarget and mUp from new rotated vectors
//	mTarget = GMathVF(GMathFV(mPosition) + GMathFV(look_at_target));
//	mUp = GMathVF(GMathFV(mPosition) + GMathFV(look_at_up));
//
//	this->init&viewmatrix();
//}
//
//void GCamera::Target(XMFLOAT3 new_target)
//{
//	if (XMVector3Equal(GMathFV(new_target), GMathFV(mPosition)) ||
//		XMVector3Equal(GMathFV(new_target), GMathFV(mTarget)))
//		return;
//
//	XMFLOAT3 old_look_at_target = GMathVF(GMathFV(mTarget) - GMathFV(mPosition));
//	XMFLOAT3 new_look_at_target = GMathVF(GMathFV(new_target) - GMathFV(mPosition));
//	float angle = XMConvertToDegrees(XMVectorGetX(
//		XMVector3AngleBetweenNormals(XMVector3Normalize(GMathFV(old_look_at_target)),
//		XMVector3Normalize(GMathFV(new_look_at_target)))));
//	if (angle != 0.0f  angle != 360.0f  angle != 180.0f)
//	{
//		XMVECTOR axis = XMVector3Cross(GMathFV(old_look_at_target), GMathFV(new_look_at_target));
//		Rotate(GMathVF(axis), angle);
//	}
//	mTarget = new_target;
//	this->init&viewmatrix();
//}

// Set camera position
//void GCamera::Position(XMFLOAT3& new_position)
//{
//	XMFLOAT3 move_vector = GMathVF(GMathFV(new_position) - GMathFV(mPosition));
//	XMFLOAT3 target = mTarget;
//	this->Move(move_vector);
//	this->Target(target);
//}
//
//void GCamera::Angle(float angle)
//{
//	mAngle = angle;
//	InitProjMatrix(mAngle, mClientWidth, mClientHeight, mNearest, mFarthest);
//}
//
//void GCamera::NearestPlane(float nearest)
//{
//	mNearest = nearest;
//	OnResize(mClientWidth, mClientHeight);
//}
//
//void GCamera::FarthestPlane(float farthest)
//{
//	mFarthest = farthest;
//	OnResize(mClientWidth, mClientHeight);
//}
//
//void GCamera::InitOrthoMatrix(const float clientWidth, const float clientHeight,
//	const float nearZ, const float fartherZ)
//{
//	XMStoreFloat4x4(&mOrtho, XMMatrixOrthographicLH(clientWidth, clientHeight, 0.0f, fartherZ));
//}
//
//void GCamera::OnResize(uint32_t new_width, uint32_t new_height)
//{
//	mClientWidth = new_width;
//	mClientHeight = new_height;
//	InitProjMatrix(mAngle, static_cast<float>(new_width), static_cast<float>(new_height), mNearest, mFarthest);
//	InitOrthoMatrix(static_cast<float>(new_width), static_cast<float>(new_height), 0.0f, mFarthest);
//}
