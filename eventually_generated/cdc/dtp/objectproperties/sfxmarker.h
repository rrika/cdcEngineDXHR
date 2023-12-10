#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "cdcSound/SoundOwner.h"

namespace dtp {

struct SoundPlex;

struct sfxmarker {
	enum EnumCondition {
		EnumCondition_Init = 0x0,
		EnumCondition_Enter = 0x1,
		EnumCondition_Exit = 0x2
	};
	enum EnumAction {
		EnumAction_Start = 0x0,
		EnumAction_End = 0x1,
		EnumAction_SetVariable = 0x2,
		EnumAction_IncVariable = 0x3,
		EnumAction_DecVariable = 0x4
	};

	struct Perimeter { // byte offsets are relative to beginning of Trigger
		float radius; // 4
		// unsigned int invaderID; // 8
		float lastDistance; // 8, HACK
		uint32_t bAlways : 1; // C
		uint32_t bBreached : 1;
		uint32_t nCountFired : 6;
	};
	static_assert(sizeof(Perimeter) == 0xC);

	struct unionStartList { // byte offsets are relative to beginning of Trigger
		char restartType; // 10
		uint16_t numSoundRefs; // 12
		unsigned int *soundRefs; // 14
	};
	struct unionEndList { // byte offsets are relative to beginning of Trigger
		uint16_t endType; // 10 
		uint16_t numSoundRefs; // 12
		unsigned int *soundRefs; // 14
	};
	struct unionMusicVar {
		uint32_t num : 8;
		uint32_t value : 8;
	};
	union UnionAction {
		unionStartList Start;
		unionEndList End;
		unionMusicVar SetVariable;
		unionMusicVar IncVariable;
		unionMusicVar DecVariable;
	};
	static_assert(sizeof(UnionAction) == 0x8);

	struct Trigger {
		uint32_t bEnabled : 1;
		uint32_t condition : 2;
		uint32_t action : 4;
		Perimeter conditionData; // 4
		UnionAction actionData; // 10
	};
	static_assert(sizeof(Trigger) == 0x18);

	cdc::Vector3 position; // 0
	// cdc::Euler rotation; // 10
	cdc::Vector rotation; // 10
	uint32_t dword20; // m_unit or UniqueID or m_streamLayer
	cdc::SoundHandle *soundHandles;    // 24
	SoundPlex **soundData; // 28
	uint32_t numSounds;    // 2C
	uint32_t numTriggers;  // 30
	SoundPlex *sounds;     // 34
	Trigger *triggers;     // 38
};

static_assert(sizeof(sfxmarker) == 0x3C);

}
