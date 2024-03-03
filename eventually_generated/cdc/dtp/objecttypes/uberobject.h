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

static_assert(sizeof(UberObjectProp::SectionProp) == 0x2F0);

}
