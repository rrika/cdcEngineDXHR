#include "PCMouseKeyboard.h"
#include <cstdio>
#include <hidusage.h>

namespace cdc {

PCMouseKeyboard::PCMouseKeyboard() {
	assignDefaultKeybinds(keybinds);
}

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

	printf("WM_INPUT with type %lu\n", input.header.dwType);
}

// signature is bool create() in original binary
PCMouseKeyboard *PCMouseKeyboard::create(HWND hwnd) {
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

	return new PCMouseKeyboard();
}

void PCMouseKeyboard::assignDefaultKeybinds(Keybind *keybinds) {
	// Q   W   E   R   T   Y   U   I   O   P
	//  10  11  12  13  14  15  16  17  18  19
	// A   S   D   F   G   H   J   K   L
	//  1E  1F  20  21  22  23  24  25  26
	// Z   X   C   V   B   N   M
	//  
	auto V = MAPVK_VSC_TO_VK;
	keybinds[0] = { MapVirtualKeyW(0x1E, V), VK_LEFT };  // A
	keybinds[1] = { MapVirtualKeyW(0x20, V), VK_RIGHT }; // D
	keybinds[2] = { MapVirtualKeyW(0x11, V), VK_UP };    // W
	keybinds[3] = { MapVirtualKeyW(0x1F, V), VK_DOWN };  // S
	keybinds[4] = { VK_CAPITAL, 0 };
	keybinds[5] = { VK_LCONTROL, 0 };
	keybinds[6] = { MapVirtualKeyW(0x12, V), 0 };        // E (use)
	keybinds[7] = { VK_SPACE, 0 };
	keybinds[8] = { VK_LSHIFT, 0 };
	keybinds[9] = { MapVirtualKeyW(0x2E, V), 0 };        // C (crouch)
	keybinds[10] = { VK_RBUTTON, 0 };
	keybinds[11] = { VK_LBUTTON, 0 };
	keybinds[12] = { MapVirtualKeyW(0x13, V), 0 };       // R (reload)
	keybinds[13] = { MapVirtualKeyW(0x23, V), 0 };       // H (holster)
	keybinds[14] = { VK_MBUTTON, VK_RSHIFT };
	keybinds[15] = { MapVirtualKeyW(0x22, V), 0 };       // G (grenade)
	keybinds[16] = { MapVirtualKeyW(0x10, V), 0 };       // Q (take-down)
	keybinds[17] = { VK_F1, 0 };
	keybinds[18] = { VK_F2, 0 };
	keybinds[19] = { VK_F4, 0 };
	keybinds[20] = { VK_F3, 0 };
	keybinds[21] = { MapVirtualKeyW(0x2C, V), 0 };       // 
	keybinds[22] = { MapVirtualKeyW(0x18, V), 0 };       // O
	keybinds[23] = { MapVirtualKeyW(0x17, V), 0 };       // I
	keybinds[24] = { MapVirtualKeyW(0x16, V), 0 };       // U
	keybinds[25] = { MapVirtualKeyW(0x1A, V), 0 };       // 
	keybinds[26] = { MapVirtualKeyW(0x19, V),            // P
	                 MapVirtualKeyW(0x32, V) };          // 
	keybinds[27] = { MapVirtualKeyW(0x29, V), 0 };       // 
	keybinds[28] = { MapVirtualKeyW(0x0D, V), 0 };       // 
	keybinds[29] = { MapVirtualKeyW(0x0C, V), 0 };       // 
	keybinds[30] = { MapVirtualKeyW(0x14, V), 0 };       // T
	keybinds[31] = { MapVirtualKeyW(0x21, V), 0 };       // F
}

void PCMouseKeyboard::method_4() { /*TODO*/ }
void PCMouseKeyboard::method_14() { /*TODO*/ }
void PCMouseKeyboard::method_18() { /*TODO*/ }

}
