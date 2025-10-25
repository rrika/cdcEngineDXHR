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
float g_mouseWheelDivider = 120.f;

namespace cdc {

bool Keybinds::IsDown(uint32_t index, char *keys) {
	uint32_t *codes = keybinds[index].keycode;
	if (codes[0] && keys[codes[0]])
		return true;
	if (codes[1] && keys[codes[1]])
		return true;
	return false;
}

bool Keybinds::IsPressed(uint32_t index, char *keys, char *keysPrev) {
	return IsDown(index, keys) && !IsDown(index, keysPrev);
}

PCMouseKeyboard::PCMouseKeyboard() {
	keybinds.assignDefaultKeybinds();
#ifdef _WIN32
	memset(vkeys, 0, 257);
#endif
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

	if (input.header.dwType == RIM_TYPEKEYBOARD) {
		// HACK
		auto flags = input.data.keyboard.Flags;
		bool isPressed = !(flags & RI_KEY_BREAK);
		bool e0 = !!(flags & RI_KEY_E0);
		auto vkey = input.data.keyboard.VKey;
		switch (vkey) {
		case /*0x0D*/ VK_RETURN:  vkey = e0 ? VK_RETURN : 0x100; break;
		case /*0x11*/ VK_CONTROL: vkey = e0 ? VK_RCONTROL : VK_LCONTROL; break;
		case /*0x12*/ VK_MENU:    vkey = e0 ? VK_RMENU : VK_LMENU; break;
		case /*0x10*/ VK_SHIFT: {
			auto vscRShift = MapVirtualKeyExW(/*0xA1*/ VK_RSHIFT, MAPVK_VK_TO_VSC, 0);
			if (input.data.keyboard.MakeCode == vscRShift)
				vkey = /*0xA1*/ VK_RSHIFT;
			else
				vkey = /*0xA0*/ VK_LSHIFT;
			break;
		}
		default: break;
		}
		vkeys[vkey] = isPressed;
		return;
	}

	if (input.header.dwType == RIM_TYPEMOUSE) {
		static const uint16_t index[] = {1, 2, 4, 5, 6};
		static const uint16_t press[] = {0x1, 0x4, 0x10, 0x40, 0x100};
		static const uint16_t release[] = {0x2, 0x8, 0x20, 0x80, 0x200};
		for (int i=0; i<5; i++) {
			if (input.data.mouse.ulButtons & press[i])
				vkeys[index[i]] = true;
			if (input.data.mouse.ulButtons & release[i])
				vkeys[index[i]] = false;
		}

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

bool PCMouseKeyboard::isPressedWhileCursorGrab(EKeyboard key) {
	// TODO
#ifdef __linux__
	char *vkeys = (char*)SDL_GetKeyboardState(nullptr);
	char *vkeysPrev = this->vkeysPrev.data();
#endif
	return cursorGrab && keybinds.IsPressed(key, vkeys, vkeysPrev);
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

void Keybinds::assignDefaultKeybinds() {
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

#ifdef __linux__
	keybinds[/* 0*/ Keyboard_MovementLeft]      = {SDL_SCANCODE_A};
	keybinds[/* 1*/ Keyboard_MovementRight]     = {SDL_SCANCODE_D};
	keybinds[/* 2*/ Keyboard_MovementForward]   = {SDL_SCANCODE_W};
	keybinds[/* 3*/ Keyboard_MovementBackward]  = {SDL_SCANCODE_S};
	keybinds[/* 4*/ Keyboard_MovementRunToggle] = {SDL_SCANCODE_CAPSLOCK};
	keybinds[/* 5*/ Keyboard_MovementWalk]      = {SDL_SCANCODE_LCTRL};
	keybinds[/* 6*/ Keyboard_Action]            = {SDL_SCANCODE_E};
	keybinds[/* 7*/ Keyboard_Jump]              = {SDL_SCANCODE_SPACE};
	keybinds[/* 8*/ Keyboard_Sprint]            = {SDL_SCANCODE_LSHIFT};
	keybinds[/* 9*/ Keyboard_Crouch]            = {SDL_SCANCODE_C};
	keybinds[/*10*/ Keyboard_Cover]             = {};
	keybinds[/*11*/ Keyboard_WeaponFire]        = {};
	keybinds[/*12*/ Keyboard_WeaponReload]      = {SDL_SCANCODE_R};
	keybinds[/*13*/ Keyboard_WeaponHolster]     = {SDL_SCANCODE_H};
	keybinds[/*14*/ Keyboard_WeaponIronSight]   = {SDL_SCANCODE_RSHIFT};
	keybinds[/*15*/ Keyboard_ThrowDetonate]     = {SDL_SCANCODE_G};
	keybinds[/*16*/ Keyboard_Takedown]          = {SDL_SCANCODE_Q};
	keybinds[/*17*/ Keyboard_Cloak]             = {SDL_SCANCODE_F1};
	keybinds[/*18*/ Keyboard_Claymore]          = {SDL_SCANCODE_F2};
	keybinds[/*19*/ Keyboard_MoveSilently]      = {SDL_SCANCODE_F3};
	keybinds[/*20*/ Keyboard_VisionEnhancer]    = {SDL_SCANCODE_F4};
	keybinds[/*21*/ Keyboard_SpecialUpgrade]    = {};
	keybinds[/*22*/ Keyboard_TechTree]          = {SDL_SCANCODE_O};
	keybinds[/*23*/ Keyboard_Inventory]         = {SDL_SCANCODE_I};
	keybinds[/*24*/ Keyboard_Objectives]        = {SDL_SCANCODE_U};
	keybinds[/*25*/ Keyboard_Logs]              = {SDL_SCANCODE_LEFTBRACKET};
	keybinds[/*26*/ Keyboard_Map]               = {SDL_SCANCODE_P};
	keybinds[/*27*/ Keyboard_QuickBarShow]      = {SDL_SCANCODE_RIGHTBRACKET};
	keybinds[/*28*/ Keyboard_QuickBarNext]      = {SDL_SCANCODE_EQUALS};
	keybinds[/*29*/ Keyboard_QuickBarPrev]      = {SDL_SCANCODE_MINUS};
	keybinds[/*30*/ Keyboard_MarkAndTrack]      = {SDL_SCANCODE_T};
	keybinds[/*31*/ Keyboard_TriggerCommentary] = {SDL_SCANCODE_F};
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
	state.deltaWheel = deltaWheel / g_mouseWheelDivider;

	// TODO

#ifdef _WIN32
	char vkeysLocal[257];
	char vkeysPrevLocal[257];
	memcpy(vkeysLocal, vkeys, 257);
	memcpy(vkeysPrevLocal, vkeysPrev, 257);
#endif
#ifdef __linux__
	int numKeys;
	char *vkeysLocal = (char*)SDL_GetKeyboardState(&numKeys);
#endif

	// TODO

	float ad = 0.0, ws = 0.0;
	if (keybinds.IsDown(0, vkeysLocal)) // A
		ad -= 1.0;
	if (keybinds.IsDown(1, vkeysLocal)) // D
		ad += 1.0;
	if (keybinds.IsDown(2, vkeysLocal)) // W
		ws -= 1.0;
	if (keybinds.IsDown(3, vkeysLocal)) // S
		ws += 1.0;

	float length = sqrt(ad*ad + ws*ws);
	if (length > 0.0) {
		ad /= length;
		ws /= length;
	}

	state.keys[Input_MovementAD] = (char)(127 * ad);
	state.keys[Input_MovementWS] = (char)(127 * ws);

	for (uint32_t i=6; i<32; i++) {
		state.keys[Input_KeyboardOffset + i] = keybinds.IsDown(i, vkeysLocal) ? 127 : 0;
	}

	// TODO

#ifdef _WIN32
	memcpy(vkeysPrev, vkeys, 257);
#endif
#ifdef __linux__
	vkeysPrev.resize(numKeys);
	memcpy((void*)vkeysPrev.data(), (void*)vkeysLocal, numKeys);
#endif
	deltaX = 0;
	deltaY = 0;
	deltaWheel = 0;
}

void PCMouseKeyboard::method_18() { /*TODO*/ }

}
