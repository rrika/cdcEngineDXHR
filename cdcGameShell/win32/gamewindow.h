#pragma once
#include <windows.h>

extern HWND hwnd1; // GameWindow::s_window
extern HWND hwnd2;
extern HWND hwnd3;
extern void *yellowCursor;

namespace GameWindow {

LRESULT CALLBACK RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Create();

}
