#include "snd.h"
#include "Voice.h"

namespace cdc {

Voice::UpdateCode VoiceImpl::Update() { // 498
	// TODO
	return hackState;
}

void VoiceCollection::Add(VoiceImpl *voice) { // line 1239
	m_nVoices++;
	m_voices.push_front(voice);
	// TODO
}

void VoiceCollection::Update(float seconds) { // line 1283
	uint32_t numPending = 0;
	bool noneStalled = true;

	for (auto *voice : m_voices) {
		switch (voice->Update()) {
		case Voice::kNormal: break;
		case Voice::kSyncReady:
			m_arrayChannelsToSyncStart[numPending++] = voice->m_channel;
			voice->hackState = Voice::kNormal;
			break;
		case Voice::kSyncStalled:
			noneStalled = false;
			break;
		}
	}

	if (noneStalled && numPending > 0) {
		uint32_t nextClock[2];
		SND_GetNextDSPClock(nextClock);
		while (numPending) {
			auto *channel = m_arrayChannelsToSyncStart[--numPending];
			channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_START /*1*/, nextClock[0], nextClock[1]);
			channel->setPaused(0);
		}
	}

	// TODO
}

Voice *VoiceCollection::Create(Sample *sample) { // line 1348

	if (m_nVoices >= 0x200) {
		// ++
		return nullptr;
	}

	FMOD::Channel *channel = 0;
	FMOD_RESULT result = gFMOD->playSound(FMOD_CHANNEL_FREE, sample->m_soundPlay, /*paused=*/ 1, &channel);
	if (result != FMOD_OK) {
		byte4 = 0;
		return nullptr;
	}

	channel->setSpeakerMix(1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0);
	byte4 = 1;
	auto *voice = new VoiceImpl;
	voice->m_channel = channel;
	voice->m_sample = sample;

	Add(voice);

	// TODO
	return voice;
}

VoiceCollection Voice::s_voiceCollection;

}
