#include "snd.h"
#include "Voice.h"

namespace cdc {

F_STDCALL FMOD_RESULT FmodChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) { // line 159
	void *userdata = nullptr;
	FMOD_Channel_GetUserData(channel, &userdata);
	auto *voice = (VoiceImpl*) userdata;

	if (type == FMOD_CHANNEL_CALLBACKTYPE_END)
		return voice->EndCallback();
	else if (type == FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE)
		return voice->VirtualCallback((bool)commanddata1);

	return FMOD_OK;
}

FMOD_RESULT VoiceImpl::EndCallback() { // line 192
	m_bStoppedFMOD = true;
	return FMOD_OK;
}

FMOD_RESULT VoiceImpl::VirtualCallback(bool nowVirtual) { // line 233
	if (nowVirtual && m_bPreventVol0Virtual) {
		unsigned int position;
		m_channel->getPosition(&position, FMOD_TIMEUNIT_PCM);
		// and then nothing
	}
	return FMOD_OK;
}

Voice::UpdateCode VoiceImpl::Update() { // 498
	if (m_bStoppedFMOD)
		return kNormal;
	// TODO
	bool isVirtual, isPaused;
	m_channel->isVirtual(&isVirtual);
	m_channel->getPaused(&isPaused);
	// TODO
	if (isPaused) {
		if (isVirtual)
			return kSyncStalled;
		else
			return kSyncReady;
	}

	return kNormal;
}

VoiceImpl::~VoiceImpl() {
	// this is called through "delete voice;" in
	//   SoundPlexWave::Update
	//   SoundPlexWave::End
	//   SoundPlexWave::~SoundPlexWave
	//   SoundPlexStitching::Update
	//   SoundPlexStitching::End
	//   SoundPlexStitching::~SoundPlexStitching
	//   ...

	// In Deus Ex
	//   cdc::SoundPlexWave::Update (and others)
	//    -> VoiceImpl::~VoiceImpl
	//        -> Sample::RemoveRef
	//        -> VoiceCollection::Remove
	//        -> FMOD::Channel::stop

	// In Tomb Raider
	//   cdc::SoundPlexWave::Update (and others)
	//    -> cdc::Voice::Release
	//        -> VoiceCollection::Destroy
	//            -> Sample::RemoveRef
	//            -> VoiceCollection::Remove
	//            -> VoiceImpl::~VoiceImpl
	//                -> FMOD::Channel::stop

	m_sample->RemoveRef();
	Voice::s_voiceCollection.Remove(this);
	m_channel->stop(); // this is FMOD::Channel::stop
	Voice::s_voiceCollection.byte4 = 1;
}

void VoiceCollection::Add(VoiceImpl *voice) { // line 1239

	m_nVoices++;

	if (m_voices.empty()) {

		// head    [BEFORE]
		//   | `---.
		//   v      \
		// link1 -> voice1
		//  ...

		m_voices.push_front(voice);
		voice->it = m_voices.begin();

		// head    [BETWEEN]
		//   | `----------.
		//   v      \      \
		// link2 -> voice2  )
		//   |             /
		//   v            /
		// link1 -> voice1
		//  ...

		auto it = m_voices.begin(); ++it;
		it->it = it;

		// head     [AFTER]
		//   | `---.
		//   v      \
		// link2 -> voice2
		//   | `---.
		//   v      \
		// link1 -> voice1
		//  ...

	} else {

		// head    [BEFORE]
		//   |
		//   v
		// nullptr

		m_voices.push_front(voice);
		voice->it = m_voices.begin();

		// head     [AFTER]
		//   | `---.
		//   v      \
		// link1 -> voice1
		//   |
		//   v
		// nullptr
	}
}

void VoiceCollection::Remove(VoiceImpl *voice) { // line 1260
	auto nextIt = voice->it; ++nextIt;

	if (nextIt == m_voices.end()) {

		// iterator
		//   | `---.
		//   v      \
		// link1 -> voice1
		//   |
		//   v
		// nullptr

		m_voices.erase(voice->it);

		// iterator
		//   |
		//   v     (voice1)
		// nullptr

	} else {

		// iterator
		//   | `---.
		//   v      \
		// link1 -> voice1
		//   | `---.
		//   v      \
		// link0 -> voice0
		//  ...

		m_voices.erase(voice->it);

		// iterator <- (voice1)
		//   |
		//   v
		// link0 -> voice0
		//  ...

		nextIt->it = voice->it;

		// iterator <- (voice1)
		//   | `---.
		//   v      \
		// link0 -> voice0
		//  ...

	}
}

void VoiceCollection::Update(float seconds) { // line 1283
	uint32_t numPending = 0;
	bool noneStalled = true;

	for (auto *voice : m_voices) {
		switch (voice->Update()) {
		case Voice::kNormal: break;
		case Voice::kSyncReady:
			m_arrayChannelsToSyncStart[numPending++] = voice->m_channel;
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
	channel->setUserData(voice);
	channel->setCallback(&FmodChannelCallback);

	Add(voice);

	// TODO
	return voice;
}

VoiceCollection Voice::s_voiceCollection;

}