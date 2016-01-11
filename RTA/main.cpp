#include <iostream>
#include <ctime>
#include "Renderer.h"
#include "RenderSet.h"
#include <fbxsdk.h>
#include "FBXLoader.h"
#include <vector>

Renderer renderer;
HINSTANCE application;
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

	renderer.Init(window);

}

bool Run()
{
	//RenderSet* renderset = nullptr;
	//renderer.Render(renderset);
	FileInfo::ExporterHeader file;
	std::vector<FileInfo::MyVertex> positions;
	file.FBXLoad("Box_Idle.fbx", &positions);
	return true;
}

void ShutDown()
{
	renderer.Shutdown();
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