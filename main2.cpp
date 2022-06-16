#include <windows.h>
#include "rendering/BuiltinResources.h"
#include "imgui/imgui.h"
#include "main.h"
#include "main2.h"
#include "spinnycube.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11RenderDevice.h"

using namespace cdc;

#if ENABLE_IMGUI
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK gameWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

#if ENABLE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
#endif

	// if (uMsg == WM_DEUS_EX_GCS_FEEDBACK) { /*TODO*/ }

	switch (uMsg)
	{
	case WM_CHAR:
		// TODO
		break;
	case WM_SIZE:
		// TODO
		break;
	case WM_ACTIVATE:
		// TODO
		break;
	case WM_KEYUP:
		// TODO
		break;
	case WM_PAINT:
		// TODO
		break;
	case WM_CLOSE:
		// TODO
		break;
	case WM_MOVE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		if (gRenderDevice)
			static_cast<PCDX11RenderDevice*>(gRenderDevice)->handleResize(rect.right, rect.bottom);
		// TODO
		break;
	}
	case 0xBB8:
		// TODO
		break;
	case 0x8000:
		// TODO
		break;
	case WM_SIZING:
		// TODO
		break;
	case WM_SYSKEYDOWN:
		// TODO
		break;
	case WM_SYSCOMMAND:
		// TODO
		break;
	case WM_MOUSEFIRST:
		// TODO
		break;
	case WM_DESTROY: // hack
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

DisplayConfig displayConfig;
HWND hwnd;

void createWindow() {
	WNDCLASSW wndClass = { sizeof(wndClass) };
	wndClass.lpfnWndProc = (WNDPROC)gameWndProc;
	wndClass.hInstance = GetModuleHandleW(0);
	// wndClass.hIcon = LoadIconW(wndClass.hInstance, (LPCWSTR)0x66);
	// if ( !wndClass.hIcon )
	// 	wndClass.hIcon = LoadIconW(0, (LPCWSTR)0x7F00);
	wndClass.lpszClassName = L"DeusExHRDCE";
	// wndClass.hbrBackground = (HBRUSH)GetStockObject(5);
	// wndClass.hCursor = LoadCursorW(0, (LPCWSTR)0x7F00);

	RegisterClassW(&wndClass);

	wchar_t windowName[] = L"Deus Ex: Human Revolution - Director's Cut";

	hwnd = CreateWindowExW(
		0, // 8
		L"DeusExHRDCE",
		windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		nullptr,
		nullptr,
		wndClass.hInstance,
		nullptr);
}

int WinMain2(HINSTANCE hInstance, LPSTR lpCmdLine) {
	loadBuiltinResources();
	auto deviceManager = createPCDX11DeviceManager();
	createWindow();
	cdc::DisplayConfig *displayConfig = deviceManager->getDisplayConfig();
	displayConfig->fullscreenRefreshRate = 60;
	displayConfig->fullscreen = false;
	displayConfig->fullscreenWidth = 1366;
	displayConfig->fullscreenHeight = 768;
	displayConfig->sampleCount = 1;
	displayConfig->sampleQuality = 0;
	displayConfig->enableTripleBuffer = false;
	displayConfig->enableVsync = true;
	displayConfig->lockWindowResolution = false;
	gRenderDevice = createPCDX11RenderDevice(hwnd, 640, 480, 0);

	if (true)
		return spinnyCube(
			hwnd,
			deviceManager->getD3DDevice(),
			deviceManager->getD3DDeviceContext());

	MSG msg;

	while (GetMessageA(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		if (msg.message == WM_QUIT)
			return 0;
		DispatchMessageA(&msg);
	}

	return 0;
}
