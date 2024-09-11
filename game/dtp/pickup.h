#pragma once
#include <cstdint>

namespace cdc { class RenderResource; }

namespace dtp {

struct Pickup {
	enum Category : int32_t {
		kPrimaryWeapon = 3,
		kSecondaryWeapon = 4
	};

	uint32_t dword0;
	const char *scaleformPath4;
	uint32_t texture8;
	const char *scaleformPathC;
	uint32_t texture10;
	void *scaleform14;
	uint32_t dword18;
	uint32_t notAddable; // 1C

	uint16_t nameStringIndex; // 20
	uint16_t descriptionStringIndex; // 22

	uint8_t width; // 24
	uint8_t height; // 25
	uint32_t stack; // 28

	uint32_t flags2C; // 1 = do not stack
	Category category; // 30

	uint32_t dword34;
	uint32_t dword38;
	uint32_t dword3C;
	uint32_t dword40;
	uint32_t dword44;

	uint32_t dtpId48; // maybe to link weapon and ammo
};

}
