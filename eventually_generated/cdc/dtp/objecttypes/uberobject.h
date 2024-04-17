#pragma once
#include <cstdint>
#include <cstddef>

namespace dtp {

struct MaterialTextureParamUpdate {
	bool assignInstanceParam; // 0
	uint32_t instanceParamIndex; // 4
	uint32_t instanceParamFormat; // 8
	uint32_t instanceParamData[4]; // C

	bool assignTexture; // 1C
	uint32_t texturePrimGroupSelector; // 20
	uint32_t textureSlot; // 24
	uint32_t textureId; // 28

	bool assignMaterial; // 2C
	uint32_t materialPrimGroupSelector; // 30
	uint32_t materialId; // 34
};

struct Conseq8StepAny {
	uint32_t type;
};

struct Conseq8Step0 {
	uint32_t type;
	// TODO
};

struct Conseq8Step1 {
	uint32_t type;
	uint32_t sectionIndex;
	// TODO
};

struct Conseq8Step2 {
	uint32_t type;
	// TODO
};

struct Conseq8Step3 {
	uint32_t type;
	float float4;
};

struct Conseq8Step4 {
	uint32_t type;
	// TODO
};

struct Conseq8Step5 {
	uint32_t type;
	uint32_t commandIndex;
};

struct Conseq8Step6 {
	uint32_t type;
	float float4;
	// TODO
};

struct Conseq8Step7 {
	uint32_t type;
};

struct Conseq8Step8 {
	uint32_t type;
	uint32_t sectionIndex;
	uint32_t renderModelIndex;
	// TODO
};

struct Conseq8Step9 {
	uint32_t type;
	uint32_t sectionIndex;
	// TODO
};

struct Conseq8Step10 {
	uint32_t type;
	uint32_t passIndex;
};

struct Conseq8Step11 {
	uint32_t type;
	uint32_t passIndex;
};

struct Conseq8Step12 {
	uint32_t type;
	// TODO
};

union Conseq8Step {
	Conseq8StepAny stepAny;
	Conseq8Step0 step0;
	Conseq8Step1 step1;
	Conseq8Step2 step2;
	Conseq8Step3 step3;
	Conseq8Step4 step4;
	Conseq8Step5 step5;
	Conseq8Step6 step6;
	Conseq8Step7 step7;
	Conseq8Step8 step8;
	Conseq8Step9 step9;
	Conseq8Step10 step10;
	Conseq8Step11 step11;
	Conseq8Step12 step12;
};

struct Conseq8 {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t numSteps; // 10
	Conseq8Step *steps; // 14
	bool bool18;
};

struct UberObjectProp {
	struct FlagMatcher {
		uint32_t bitIndex;
		bool invert;
	};

	struct Transition {
		uint32_t dword0;
		uint32_t nextState;
		uint32_t dword8;
		uint32_t dwordC;
		uint32_t dword10;
		uint32_t dword14;
		uint32_t dword18;
		uint32_t dword1C;
		uint8_t gap20[8];
		Transition *ptr28;
		uint32_t randIntCutoff; // 2C
		uint32_t dword30;
		uint32_t numFlagMatchers; // 34
		FlagMatcher *flagMatchers; // 38
		uint8_t randFlags; // 3C
	};

	struct Action {
		uint32_t kind;
		uint32_t dword4;
		uint32_t pad[13];
	};

	struct CondAction {
		uint32_t dword0;
		uint32_t dword4;
		Action action;
	};

	struct StateProp {
		uint32_t dword0;
		uint32_t dword4;
		uint32_t dword8;

		uint32_t numTransitions;
		Transition **transitions;

		uint32_t numEntry;
		Action *entry;

		uint32_t numExit;
		Action *exit;

		uint32_t numCondEntry;
		CondAction *condEntry;

		uint32_t numCondExit;
		CondAction *condExit;
	};
	struct SectionProp {
		uint8_t pad0[0xC];
		uint32_t numStates; // C
		StateProp *states; // 10
		uint32_t numTransitions; // 14
		Transition *transitions; // 18
		uint32_t initialState; // 1C
		uint8_t pad20[8];
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
	struct Unknown { // maintenance maybe
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

struct TvSegment {
	uint32_t dtpIndex;
	uint32_t dword4;
};

struct Program {
	uint32_t one;
	uint32_t numSegments;
	TvSegment *segments;
};

struct DoorCon {
	bool bool0;
	uint32_t introId;
};

struct IntroDataUberObject {
	uint32_t type;
	uint8_t gap4[20];
	uint32_t *commandIndices; // 18
	uint8_t gap1C[4];
	uint32_t *effects20;
	uint8_t numCommandIndices; // 24
	uint8_t byte25;
	uint8_t numEffects26;
	uint8_t enable27;
	uint32_t dword28;
	uint8_t byte2C;
	DoorCon *doorCon;
	uint8_t gap34[52];
	uint32_t *connections; // 68
	uint32_t dword6C;
	uint8_t gap70[8];
	uint32_t dword78;
	uint32_t dword7C;
	uint32_t dword80;
	uint32_t dword84;
	uint32_t *defaultPrograms; // 88
	uint8_t gap8C[76];
	uint8_t numConnections; // D8
	uint8_t byteD9;
	uint8_t gapDA[4];
	uint8_t byteDE;
	uint8_t byteDF;
	uint8_t numDefaultPrograms; // E0
};

static_assert(sizeof(UberObjectProp::SectionProp) == 0x2F0);
static_assert(sizeof(UberObjectProp::Consequence) == 0x44);
static_assert(sizeof(IntroDataUberObject) == 0xE4);

}
