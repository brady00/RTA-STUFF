#include <iostream>
#include <ctime>
#include "Renderer.h"
#include "RenderSet.h"
#include "RenderContext.h"
#include "RenderMaterial.h"
#include <fbxsdk.h>
#include "FBXLoader.h"
#include <vector>
#include "RenderShape.h"
#include "Camera.h"

HINSTANCE application;
RenderSet* renderset;
Camera* camera;
DirectX::XMFLOAT4X4 cameraMatrix;

void Init(HINSTANCE hinst, WNDPROC proc)
{
	application = hinst;
	WNDPROC appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, 500, 500 };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	HWND window = CreateWindow(L"DirectXApplication", L"TITLE", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, nullptr);

	ShowWindow(window, SW_SHOW);

	Renderer::Init(window);
	FileInfo::ExporterHeader file;
	std::vector<FileInfo::MyVertex> verticies;
	file.FBXLoad("Teddy_Idle.fbx", &verticies);
	file.FBXSave("Teddy_Idle.bin", verticies);

	renderset = new RenderSet;
	RenderContext* renderContext = new RenderContext;
	D3D11_BUFFER_DESC VertDesc;
	ZeroMemory(&VertDesc, sizeof(VertDesc));
	VertDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertDesc.ByteWidth = sizeof(FileInfo::MyVertex) * verticies.size();
	VertDesc.StructureByteStride = sizeof(FileInfo::MyVertex);
	D3D11_SUBRESOURCE_DATA VertData;
	ZeroMemory(&VertData, sizeof(VertData));
	VertData.pSysMem = &verticies[0];
	renderContext->setVertexBuffer(VertDesc, VertData, sizeof(FileInfo::MyVertex), 0);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	renderContext->setVertexShader("VertexShader.cso");
	renderContext->setPixelShader("PixelShader.cso");
	renderContext->setInputLayout(layout, 3);
	renderContext->RenderFunc = renderContext->RenderFunction;
	renderContext->CreateRenderMaterials();
	RenderMaterial* renderMaterial = new RenderMaterial;
	renderMaterial->CreateTexture(L"Teddy_D.dds");
	renderMaterial->RenderFunc = renderMaterial->RenderFunction;
	renderMaterial->CreateRenderShapes();
	renderContext->AddRenderMaterials((RenderNode*)renderMaterial);
	RenderShape* renderShape = new RenderShape;
	
	DirectX::XMMATRIX matrix2 = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(3.14), DirectX::XMLoadFloat4x4(&DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 100, 200, 1)));
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, matrix2);
	renderShape->SetWorldMatrix(world);
	renderShape->RenderFunc = renderShape->RenderFunction;
	renderShape->setNumPrimitives(verticies.size());
	renderShape->setPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderShape->setstartIndex(0);
	renderShape->setStartVertex(0);
	DirectX::XMFLOAT4X4 matrix;

	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(300), 1, 0.1f, 1000.0f));
	renderShape->SetViewProjMatrix(matrix);
	cameraMatrix = matrix;

	renderMaterial->AddRenderShapes((RenderNode*)renderShape);
	
	renderset->AddRenderNode((RenderNode*)renderContext);
	camera->init();

}

bool Run()
{
	
	Renderer::Render(renderset);
	camera->CameraMovement(cameraMatrix);
	

	//float aspectRatio = 500.0f / 500.0f;
	//
	//camera->camera.viewMatrix = DirectX::XMMatrixIdentity();
	//camera->camera.WorldMatrix = DirectX::XMMatrixIdentity();
	//camera->camera.projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(75), aspectRatio, 0.1f, 1000.0f);

	Renderer::Present();
	return true;
}

void ShutDown()
{
	Renderer::Shutdown();
	delete renderset;
	UnregisterClass(L"DirectXApplication", application);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	Init(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	ShutDown();
	
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message)
	{
	case (WM_DESTROY) : { PostQuitMessage(0); }
						break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}