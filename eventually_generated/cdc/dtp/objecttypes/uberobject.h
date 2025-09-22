#pragma once
#include <cstdint>

namespace dtp {

struct UberObjectProp {
	struct SectionProp {
		uint8_t pad[0x28];
		uint8_t modelIndex; // 28
		uint8_t pad29[0x2F0-0x29];
	};
	uint16_t version; // 0
	uint16_t family; // 2
	uint16_t id; // 4
	uint16_t type; // 6
	uint32_t numSections; // 8
	SectionProp *sectionList; // C
};

struct TvShow{
	struct Dummy0 { uint32_t soundPlexDtp; };
	struct Dummy1 { uint32_t dword0; Dummy0 *ptr; };
	struct Dummy2 { Dummy1 *ptr; };

	uint32_t pad0[3];
	Dummy2 *ptr; // C
};

struct TvProgramEntry {
	uint32_t showDtpId;
	uint32_t dword4;
};

struct TvProgram {
	uint32_t one;
	uint32_t numEntries;
	TvProgramEntry *entries;
};

struct LootItem {
	uint32_t pickupDtpIndex; // 0
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
};

struct Loot {
	uint32_t sectionIndex;
	uint32_t numItems;
	LootItem *items;
};

struct EMail {
	uint16_t stridx_from;
	uint16_t stridx_to;
	uint16_t stridx_subject;
	uint16_t stridx_body;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t ptr10;
};

struct Keycode {
	uint32_t localized;
	union {
		const char *code;
		struct {
			uint16_t padding;
			uint16_t stridx_code;
		};
	};
};

struct IntroDataUberObject {
	uint8_t gap0[0x44];
	uint32_t numEMail; // 44
	uint32_t *email; // 48, email_database.drm must be loaded separately to retrieve
	uint32_t numConditionalEMail; // 4C
	void *conditionalEMail; // 50
	uint8_t gap54[0x5C-0x54];
	uint32_t keycode; // 5C
	uint8_t gap60[0x6C-0x60];
	Loot *loot; // 6C
	uint8_t gap70[0x88-0x70];
	uint32_t *defaultPrograms; // 88
	uint8_t gap8C[0xD9-0x8C];
	uint8_t numLoot; // D9
	uint8_t gapDA[0xE0-0xDA];
	uint8_t numDefaultPrograms; // E0
};

static_assert(sizeof(UberObjectProp::SectionProp) == 0x2F0);

}
