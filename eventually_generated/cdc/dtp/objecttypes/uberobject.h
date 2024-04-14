#pragma once
#include <cstdint>

namespace dtp {

struct UberObjectProp {
	struct SectionProp {
		uint8_t pad[0x28];
		uint8_t modelIndex; // 28
		uint8_t pad29[0x2F0-0x29];
	};
	struct QueryProp {

	};
	struct Consequence {
		uint32_t targetSection;
		uint32_t kind;
		uint8_t pad[0x3C];
	};
	struct Command {
		uint32_t dword0;
		uint32_t numConsequences; // 4
		Consequence *consequences; // 8
		uint32_t numConditions; // C
		QueryProp **conditions; // 10
	};
	struct CommandMap {
		uint32_t key;
		uint32_t commandIndex;
	};
	struct Unknown {
		uint32_t dword0;
		uint32_t numConditions; // 4
 		QueryProp **conditions; // 8
		uint32_t sectionIndex; // C
		uint32_t dword10;
		uint32_t dword14;
		uint32_t dword18;
		uint32_t dword1C;
		uint8_t byte20;
  		uint16_t objListIndexOverride; // 22
	};
	uint16_t version; // 0
	uint16_t family; // 2
	uint16_t id; // 4
	uint16_t type; // 6

	uint32_t numSections; // 8
	SectionProp *sectionList; // C

	uint32_t numCommandMaps; // 10
	CommandMap *commandMapList; // 14

	uint32_t numCommands; // 18
	Command *commandList; // 1C

	uint32_t numUnknown; // 20
	Unknown *unknownList; // 24

	uint32_t dword28;
	uint32_t dword2C;
	
	uint32_t numQueries; // 30
	QueryProp *queryList; // 34
};

static_assert(sizeof(UberObjectProp::SectionProp) == 0x2F0);
static_assert(sizeof(UberObjectProp::Consequence) == 0x44);

}
