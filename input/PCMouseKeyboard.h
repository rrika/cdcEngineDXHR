#pragma once
#include <windows.h>
#include "Input.h"

#ifdef __linux__
#include <vector>
union SDL_Event;
#endif

namespace cdc {

struct Keybind {
	uint32_t keycode[2] = {0, 0};
};

struct Keybinds {
	Keybind keybinds[32];

	void assignDefaultKeybinds();
	bool IsDown(uint32_t index, char *keys);
	bool IsPressed(uint32_t index, char *keys, char *keysPrev);
};

class PCMouseKeyboard : public InputProducer {
public:
	PCMouseKeyboard();
#ifdef _WIN32
	void processWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#ifdef __linux__
	void processSDLEvent(SDL_Event *event);
#endif
	void setCursorPos(float x, float y);
	void setupClip();
	void centerCursor(bool);
	bool isPressedWhileCursorGrab(EKeyboard key);

	static PCMouseKeyboard *create(HWND hwnd);

	void setCursorGrab(bool active) override;
	void update() override;
	void method_18() override;

	// uint8_t gap8C[4];
	// uint8_t byte90;
	// uint8_t byte91;
	// uint8_t gap92[2];
	// uint32_t dword94;
	// float float98;
	RECT m_rect; // 9C
	// uint32_t dwordAC;
	int32_t deltaX = 0; // B0
	int32_t deltaY = 0; // B4
	int32_t deltaWheel = 0; // B8
	// uint32_t dwordBC;
#ifdef _WIN32
	char vkeys[257]; // C0
	char vkeysPrev[257]; // 1C1
#endif
#ifdef __linux__
	std::vector<char> vkeysPrev;
#endif
	// uint8_t gap2C2[2];
	// float float2C4_x;
	// float float2C8_y;
	Keybinds keybinds; // 2CC
	// uint32_t dword3CC;
	// uint32_t dword3D0;
	// uint8_t byte3D4;
};

}
