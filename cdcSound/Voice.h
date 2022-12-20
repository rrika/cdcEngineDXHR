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

class VoiceCollection : protected SoundTypes { // 28
	void Add(VoiceImpl *voice);

public:
	FMOD::ChannelGroup *m_groupMaster = nullptr; // 0, assigned in SND_Init
	uint8_t byte4 = 1; // 4
	SList<VoiceImpl*> m_voices; // 8
	FMOD::Channel *m_arrayChannelsToSyncStart[1024]; // C
	uint32_t m_nVoices = 0; // 100C

	Voice *Create(Sample *sample);
	void Update(float seconds);
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
	UpdateCode Update();

	FMOD::Channel *m_channel; // 8
	Sample *m_sample; // C

	UpdateCode hackState = kSyncReady;
};

}
