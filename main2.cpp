#include <windows.h>
#include "main.h"
#include "main2.h"
#include "spinnycube.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11RenderDevice.h"

using namespace cdc;

LRESULT CALLBACK gameWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
	createPCDX11DeviceManager();
	createWindow();
	createPCDX11RenderDevice(hwnd, 640, 480, 0);

	if (true)
		return spinnyCube(hwnd);

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
