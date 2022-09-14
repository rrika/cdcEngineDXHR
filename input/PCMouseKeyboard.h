#pragma once
#include <windows.h>
#include "InputProducer.h"

namespace cdc {

struct Keybind {
	uint32_t keycode[2];
};

class PCMouseKeyboard : public InputProducer {
public:
	PCMouseKeyboard();
#ifdef _WIN32
	void processWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	void setCursorPos(float x, float y);
	void setupClip();
	void centerCursor(bool);

	static PCMouseKeyboard *create(HWND hwnd);
	static void assignDefaultKeybinds(Keybind *keybinds);

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
	int32_t deltaX = 0;
	int32_t deltaY = 0;
	// uint32_t dwordB8;
	// uint32_t dwordBC;
	// char vkeysC0[257];
	// char char1C1[257];
	// uint8_t gap2C2[2];
	// float float2C4_x;
	// float float2C8_y;
	Keybind keybinds[32]; // 2CC
	// uint32_t dword3CC;
	// uint32_t dword3D0;
	// uint8_t byte3D4;
};

}
