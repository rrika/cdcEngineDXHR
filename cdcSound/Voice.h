#pragma once
#ifdef _WIN32
#define WIN32 // make fmod use stdcall
#endif
#include "3rdParty/fmod/fmod.hpp"
#include "cdcSound.h"
#include "cdcSys/SList.h"
#include "Sample.h"

namespace cdc {

class Voice;
class VoiceImpl;

class VoiceCollection : protected SoundTypes { // line 28
	void Add(VoiceImpl *voice);

public:
	FMOD::ChannelGroup *m_groupMaster = nullptr; // 0, assigned in SND_Init
	uint8_t byte4 = 1; // 4
	SList<VoiceImpl*> m_voices; // 8
	FMOD::Channel *m_arrayChannelsToSyncStart[1024]; // C
	uint32_t m_nVoices = 0; // 100C

	Voice *Create(Sample *sample);
	void Remove(VoiceImpl *voice);
	void Update(float seconds);
	inline bool IsOutOfMemory() { // line 42
		return !byte4 || m_nVoices >= 0x200;
	}
};

class Voice { // 74
public:
	enum UpdateCode { // 127
		kNormal = 0,
		kSyncReady = 1,
		kSyncStalled = 2
	};

	static VoiceCollection s_voiceCollection;
};

class VoiceImpl : public Voice {
public:
	void SetControls(
		SoundTypes::Controls *controls,
		SoundTypes::Controls3d *controls3d);
	UpdateCode Update();
	float Update3D(float& directOcclusion, float& reverbOcclusion);
	FMOD_RESULT EndCallback();
	FMOD_RESULT VirtualCallback(bool);
	~VoiceImpl();

	cdc::SList<VoiceImpl*>::iterator it; // 4
	FMOD::Channel *m_channel; // 8
	Sample *m_sample; // C
	SoundTypes::Controls3d *m_controls3d = nullptr; // 20
	bool m_bPreventVol0Virtual = false; // 25
	bool m_bStoppedFMOD = false; // 26
};

}
