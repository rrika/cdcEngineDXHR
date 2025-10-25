#define NOMINMAX
#include "PCMouseKeyboard.h"
#include <cstdio>
#include <algorithm>

#ifdef _WIN32
#include <hidusage.h>
#endif

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

extern HWND hwnd2;

float g_mouseXSensitivity2 = 0.00135f;
float g_mouseYSensitivity2 = 0.00135f;

namespace cdc {

PCMouseKeyboard::PCMouseKeyboard() {
	assignDefaultKeybinds(keybinds);
}

#ifdef _WIN32
void PCMouseKeyboard::processWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_SIZE: break; // TODO
		case WM_SETFOCUS: break; // TODO
		case WM_KILLFOCUS: break; // TODO
		case WM_CHAR: break; // TODO
		case WM_LBUTTONUP: break; // TODO
		case WM_INPUT: break; // handled below
		default: return;
	}
	// handle WM_INPUT here
	if (wParam)
		return;

	RAWINPUT input; {
		UINT inputSize = sizeof(RAWINPUT);
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &inputSize, sizeof(RAWINPUTHEADER));
	}

	// printf("WM_INPUT with type %lu\n", input.header.dwType);

	if (input.header.dwType == RIM_TYPEMOUSE) {
		deltaX += input.data.mouse.lLastX;
		deltaY += input.data.mouse.lLastY;
	}
}
#endif

#ifdef __linux__
void PCMouseKeyboard::processSDLEvent(SDL_Event *event) {
	switch (event->type) {
	case SDL_MOUSEMOTION: {
		deltaX += event->motion.xrel;
		deltaY += event->motion.yrel;
	}
	default:
		break;
	}
}
#endif

void PCMouseKeyboard::setCursorPos(float x, float y) {
#ifdef _WIN32
	if (cursorGrab) {
		RECT rect;
		GetClientRect(hwnd2, &rect);
		POINT point;
		point.x = rect.right * std::clamp(x, 0.0f, 1.0f);
		point.y = rect.bottom * std::clamp(y, 0.0f, 1.0f);
		ClientToScreen(hwnd2, &point);
		SetCursorPos(point.x, point.y);
	}
#endif
}

void PCMouseKeyboard::setupClip() {
#ifdef _WIN32
	if (!cursorGrab) {
		ClipCursor(nullptr);
		return;
	}

	if (true /* !fullscreen */) {
		GetClientRect(hwnd2, &m_rect);
		ClientToScreen(hwnd2, reinterpret_cast<POINT*>(&m_rect.left));
		ClientToScreen(hwnd2, reinterpret_cast<POINT*>(&m_rect.right));
		ClipCursor(&m_rect);
	}

#elif defined(__linux__)
	SDL_SetWindowGrab((SDL_Window*) hwnd2, (SDL_bool)cursorGrab);
	SDL_SetRelativeMouseMode((SDL_bool)cursorGrab);
#endif
}

void PCMouseKeyboard::centerCursor(bool unknown) {
	// TODO
	if (cursorGrab) {
		if (unknown) {
			setCursorPos(0.5f, 0.5f);
		}
	}
	setupClip();
}

// signature is bool create() in original binary
PCMouseKeyboard *PCMouseKeyboard::create(HWND hwnd) {
#ifdef _WIN32
	RAWINPUTDEVICE rawInputDevices[2];

	rawInputDevices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rawInputDevices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rawInputDevices[0].dwFlags = 0;
	rawInputDevices[0].hwndTarget = hwnd;

	rawInputDevices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rawInputDevices[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rawInputDevices[1].dwFlags = 0;
	rawInputDevices[1].hwndTarget = hwnd;

	RegisterRawInputDevices(rawInputDevices, 2, sizeof(RAWINPUTDEVICE));
#endif
	return new PCMouseKeyboard();
}

void PCMouseKeyboard::assignDefaultKeybinds(Keybind *keybinds) {
#ifdef _WIN32
	// Q   W   E   R   T   Y   U   I   O   P
	//  10  11  12  13  14  15  16  17  18  19
	// A   S   D   F   G   H   J   K   L
	//  1E  1F  20  21  22  23  24  25  26
	// Z   X   C   V   B   N   M
	//  
	auto V = MAPVK_VSC_TO_VK;
	keybinds[/* 0*/ Keyboard_MovementLeft]      = { MapVirtualKeyW(0x1E, V), VK_LEFT };  // A
	keybinds[/* 1*/ Keyboard_MovementRight]     = { MapVirtualKeyW(0x20, V), VK_RIGHT }; // D
	keybinds[/* 2*/ Keyboard_MovementForward]   = { MapVirtualKeyW(0x11, V), VK_UP };    // W
	keybinds[/* 3*/ Keyboard_MovementBackward]  = { MapVirtualKeyW(0x1F, V), VK_DOWN };  // S
	keybinds[/* 4*/ Keyboard_MovementRunToggle] = { VK_CAPITAL, 0 };
	keybinds[/* 5*/ Keyboard_MovementWalk]      = { VK_LCONTROL, 0 };
	keybinds[/* 6*/ Keyboard_Action]            = { MapVirtualKeyW(0x12, V), 0 };        // E (use)
	keybinds[/* 7*/ Keyboard_Jump]              = { VK_SPACE, 0 };
	keybinds[/* 8*/ Keyboard_Sprint]            = { VK_LSHIFT, 0 };
	keybinds[/* 9*/ Keyboard_Crouch]            = { MapVirtualKeyW(0x2E, V), 0 };        // C (crouch)
	keybinds[/*10*/ Keyboard_Cover]             = { VK_RBUTTON, 0 };
	keybinds[/*11*/ Keyboard_WeaponFire]        = { VK_LBUTTON, 0 };
	keybinds[/*12*/ Keyboard_WeaponReload]      = { MapVirtualKeyW(0x13, V), 0 };       // R (reload)
	keybinds[/*13*/ Keyboard_WeaponHolster]     = { MapVirtualKeyW(0x23, V), 0 };       // H (holster)
	keybinds[/*14*/ Keyboard_WeaponIronSight]   = { VK_MBUTTON, VK_RSHIFT };
	keybinds[/*15*/ Keyboard_ThrowDetonate]     = { MapVirtualKeyW(0x22, V), 0 };       // G (grenade)
	keybinds[/*16*/ Keyboard_Takedown]          = { MapVirtualKeyW(0x10, V), 0 };       // Q (take-down)
	keybinds[/*17*/ Keyboard_Cloak]             = { VK_F1, 0 };
	keybinds[/*18*/ Keyboard_Claymore]          = { VK_F2, 0 };
	keybinds[/*19*/ Keyboard_MoveSilently]      = { VK_F4, 0 };
	keybinds[/*20*/ Keyboard_VisionEnhancer]    = { VK_F3, 0 };
	keybinds[/*21*/ Keyboard_SpecialUpgrade]    = { MapVirtualKeyW(0x2C, V), 0 };       // 
	keybinds[/*22*/ Keyboard_TechTree]          = { MapVirtualKeyW(0x18, V), 0 };       // O
	keybinds[/*23*/ Keyboard_Inventory]         = { MapVirtualKeyW(0x17, V), 0 };       // I
	keybinds[/*24*/ Keyboard_Objectives]        = { MapVirtualKeyW(0x16, V), 0 };       // U
	keybinds[/*25*/ Keyboard_Logs]              = { MapVirtualKeyW(0x1A, V), 0 };       // 
	keybinds[/*26*/ Keyboard_Map]               = { MapVirtualKeyW(0x19, V),            // P
	                                                MapVirtualKeyW(0x32, V) };          // 
	keybinds[/*27*/ Keyboard_QuickBarShow]      = { MapVirtualKeyW(0x29, V), 0 };       // 
	keybinds[/*28*/ Keyboard_QuickBarNext]      = { MapVirtualKeyW(0x0D, V), 0 };       // 
	keybinds[/*29*/ Keyboard_QuickBarPrev]      = { MapVirtualKeyW(0x0C, V), 0 };       // 
	keybinds[/*30*/ Keyboard_MarkAndTrack]      = { MapVirtualKeyW(0x14, V), 0 };       // T
	keybinds[/*31*/ Keyboard_TriggerCommentary] = { MapVirtualKeyW(0x21, V), 0 };       // F
#endif
}

void PCMouseKeyboard::setCursorGrab(bool active) {
	cursorGrab = active;
	centerCursor(false); // so, don't actually center
}

void PCMouseKeyboard::update() {
	state = InputState();

	// TODO

	state.deltaX = deltaX *	g_mouseXSensitivity2;
	state.deltaY = deltaY * g_mouseYSensitivity2;

	// TODO

	deltaX = 0;
	deltaY = 0;
}

void PCMouseKeyboard::method_18() { /*TODO*/ }

}
