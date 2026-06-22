#pragma once
#include <cstdint>

struct UpgradeInitialValue {
	uint32_t upgradeType; // 0
	uint32_t data; // 4, either float, uint32 or bool
};

struct UpgradeInfo {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t numInitialValues;
	UpgradeInitialValue *initialValues;
};

struct GlobalPlayerInfo {
	UpgradeInfo *upgradeInfo; // 0
	char pad4[0xE44];
	uint32_t numJensens; // E48
	uint16_t *jensens; // E4C
};
