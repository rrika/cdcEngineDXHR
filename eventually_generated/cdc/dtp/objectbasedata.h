#pragma once
#include <cstdint>

namespace cdc {

class ObjectBlobAnim;

}

namespace dtp {

struct AnimGraphRootRef;
struct AnimStateGraph;
struct Model;
struct SoundPlex;

struct ObjectBaseData {

	struct SoundHandle {
		dtp::SoundPlex* m_plex;
		uint16_t word4;
		uint16_t word6;
		uint32_t dword8;
	};

	uint32_t dword0;
		// 00001000 invisible (triggers, etc.), see Instance::IntroduceInstance
		// 00002000 no_dump, see cdc::ObjectManager::ReleaseObject
		// 20000000 see MeshComponent::SetModel
	uint32_t dword4;
		// 00004000 has bones
	uint32_t dword8;
	uint32_t dwordC;
	float float10_lod1_distance_maybe;
	float float14_lod2_distance_maybe;
	uint8_t byte18_lod1_rmi_selector_maybe;
	uint8_t byte19_lod2_rmi_selector_maybe;
	uint8_t f1A[6];
	float float20;
	uint16_t word24;
	uint8_t byte26;
	uint8_t f27;
	float fl28;
	uint8_t f2C[4];
	uint16_t word30;
	uint8_t f32[6];
	uint32_t numAnims; // 38
	dtp::AnimEntry *animations; // 3C
	uint16_t word40;
	uint16_t word42;
	uint32_t dword44;
	uint32_t hasAnimGraph; // 48
	dtp::AnimGraphRootRef *pAnimGraph; // 4C
	uint32_t numHostedAnimGraphs;
	dtp::AnimGraphRootRef *pHostedAnimGraphs;
	uint32_t dword58;
	uint32_t dword5C;
	uint32_t dword60;
	int field_64_IK;
	uint8_t gap68[4];
	int field_6C;
	uint8_t gap70[8];
	int field_78_cloth;
	int field_7C_softBody;
	uint32_t dword80;
	uint32_t numSounds;
	SoundHandle *sounds;
	uint32_t dword8C;
	uint32_t dword90;
	uint32_t dword94_softAccessory;
	uint32_t dword98;
	uint32_t dword9C;
	uint32_t dwordA0;
	uint32_t dwordA4;
	uint32_t dwordA8;
	uint32_t dwordAC;
	uint32_t dwordB0;
	uint32_t dwordB4;
	uint32_t dwordB8;
	void *TuneDataPtr; // BC, override data in object by this
	uint32_t m_scriptTypeID; // C0
	cdc::ObjectBlobAnim **animationsC4;
	uint32_t dwordC8;
	uint32_t dwordCC;
	uint32_t dwordD0;
	uint32_t dwordD4;
	uint32_t dwordD8;
	uint32_t dwordDC;
	uint32_t dwordE0;
	uint32_t dwordE4;
	uint32_t dwordE8;
	uint32_t dwordEC;
	uint32_t dwordF0;
	uint32_t dwordF4;
	uint32_t dwordF8;
	uint16_t numModels; // FC
	uint8_t fFE[2];
	dtp::Model **models;
	uint32_t dword104;
	uint32_t dword108;
	uint32_t dword10C;
	uint32_t dword110;
	uint32_t dword114;
	uint16_t baseObject_objectListIndex;
	uint16_t dword11A;
	uint32_t dword11C;
	uint32_t dword120;
	uint32_t dword124;
	uint32_t dword128;
	uint32_t hasDeferredLightMaybe;
};

static_assert(sizeof(ObjectBaseData) == 0x130);

}
