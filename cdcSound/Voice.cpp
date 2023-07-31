#include "cdcMath/Math.h"
#include "Microphone.h"
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

void VoiceImpl::SetControls(
	SoundTypes::Controls *controls,
	SoundTypes::Controls3d *controls3d)
{
	// TODO
	m_controls3d = controls3d;
	// TODO
}

Voice::UpdateCode VoiceImpl::Update() { // 498
	if (m_bStoppedFMOD)
		return kNormal;
	float directOcclusion, reverbOcclusion;
	float volume3d = Update3D(directOcclusion, reverbOcclusion);
	// TODO
	bool isVirtual, isPaused;
	m_channel->setVolume(volume3d);
	m_channel->set3DOcclusion(directOcclusion, reverbOcclusion);
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

// should be read from globalsoundinfo.drm
static uint8_t volumeCurve[] = {0xFF, 0xD2, 0xA5, 0x7F, 0x64, 0x4F, 0x40, 0x34, 0x2B, 0x24, 0x1D, 0x16, 0x10, 0x0B, 0x05, 0x00};
static uint8_t directCurve[] = {0x00, 0x11, 0x22, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x97, 0xA8, 0xB9, 0xCA, 0xDB, 0xEC, 0xFC};
static uint8_t reverbCurve[] = {0xAF, 0x9B, 0x86, 0x71, 0x5F, 0x51, 0x47, 0x40, 0x3A, 0x35, 0x31, 0x2D, 0x29, 0x26, 0x22, 0x1E};

static float samplePiecewiseLinear(uint8_t *curve, uint32_t steps, float t) {
	float u = t * (steps-1);
	uint32_t i = u;
	uint32_t j = i < steps-1 ? i + 1 : steps-1;
	float f = u - i;
	return (float(curve[i]) + (float(curve[j])-float(curve[i])) * f) / 255.0;
}

static float distanceCurve(float distance, float maxDistance, uint8_t *curve, uint32_t steps) {
	float t = distance / maxDistance;
	if (distance >= maxDistance) t = 1.0f;
	float value = samplePiecewiseLinear(curve, steps, t);
	if (value > 1.0)
		return 1.0;
	if (value < 0.0)
		return 0.0;
	return value;
}

float VoiceImpl::Update3D(float& directOcclusion, float& reverbOcclusion) {
	directOcclusion = 0.0f;
	reverbOcclusion = 0.0f;
	if (!m_controls3d)
		return 1.0f;

	// TODO

	uint32_t playbackType = m_controls3d->playbackType & 0xFF;
	switch (playbackType) {
	case 1:
		if (m_controls3d->playbackType & 0x100) {
			SND_SetFMODModeFlag(m_channel, FMOD_3D);
			Vector4 direction;
			float distance;
			g_microphone.GetRelativePositionVector(m_controls3d, &direction, &distance);
			float volume = distanceCurve(distance, 4000.f, volumeCurve, 16);
			directOcclusion = distanceCurve(distance, 4000.f, directCurve, 16);
			reverbOcclusion = distanceCurve(distance, 4000.f, reverbCurve, 16);
			// printf("distance %f -> volume %f direct %f reverb %f\n", distance, volume, directOcclusion, reverbOcclusion);

			FMOD_VECTOR pos {direction.x, direction.y, direction.z};
			FMOD_VECTOR vel {0, 0, 0};
			m_channel->set3DAttributes(&pos, &vel);
			return volume;
		}
		return 1.0f;

	default:
		SND_SetFMODModeFlag(m_channel, FMOD_2D);
		return 1.0f;
	}
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
