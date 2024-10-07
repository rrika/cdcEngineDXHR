#include "gamewindow.h"
#include "config.h"
#include "imgui/imgui.h"
#include "cdcRender/PCDX11RenderDevice.h"

using namespace cdc;

HWND hwnd1;
HWND hwnd2;
HWND hwnd3;
void *yellowCursor;

#if ENABLE_IMGUI
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace GameWindow {

LRESULT CALLBACK RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

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
		if (g_renderDevice)
			static_cast<PCDX11RenderDevice*>(g_renderDevice)->handleResize(rect.right, rect.bottom);
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

void Create() {
	WNDCLASSW wndClass = { sizeof(wndClass) };
	wndClass.lpfnWndProc = (WNDPROC)RealWindowProc;
	wndClass.hInstance = GetModuleHandleW(0);
	wndClass.hIcon = LoadIconW(wndClass.hInstance, (LPCWSTR)0x66);
	// if ( !wndClass.hIcon )
	// 	wndClass.hIcon = LoadIconW(0, (LPCWSTR)0x7F00);
	wndClass.lpszClassName = L"DeusExHRDCE";
	// wndClass.hbrBackground = (HBRUSH)GetStockObject(5);
	wndClass.hCursor = LoadCursorW(0, (LPCWSTR)0x7F00); // normal cursor

	if (true) { // HACK
		ICONINFO ii;
		auto pointerIcon = LoadIconA(wndClass.hInstance, (LPCSTR)109);
		GetIconInfo(pointerIcon, &ii);
		ii.fIcon = 0;
		ii.xHotspot = 5;
		ii.yHotspot = 5;
		wndClass.hCursor = CreateIconIndirect(&ii); // new and improved (yellow) cursor
	}

	yellowCursor = wndClass.hCursor;

	RegisterClassW(&wndClass);

	wchar_t windowName[] = L"Deus Ex: Human Revolution - Director's Cut";

	hwnd1 = hwnd2 = hwnd3 = CreateWindowExW(
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

}
