#include "SoundPlexWave.h"
#include "Voice.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/WaveSection.h"

namespace cdc {

SoundPlexWave::SoundPlexWave(
	dtp::SoundPlex::Wave *data,
	Controls *controls,
	Controls3d *controls3d,
	SoundOwner *owner)
:
	SoundPlex(controls, controls3d, owner)
{
	Wave *wave = ((WaveSection*)g_resolveSections[6])->WaveFind(data->m_id);
	if (wave && wave->sample) {	
		// TODO
		m_voice = Voice::s_voiceCollection.Create(wave->sample);
	}
}

SoundPlex *SoundPlexWave::Update(float time) {
	// TODO
	return nullptr;
}

void SoundPlexWave::End(SoundPlexWave::EndType end) {
	if (m_voice) {
		switch (end) {
		case k0:
		case k2:
		case k3:
			if ((m_state & 4) != 2)
				return;
		case k1:
		case k4:
		case k5:
		case k6:
			delete m_voice; // call VoiceImpl::~VoiceImpl
			m_voice = nullptr;
		default:
			return;
		}
	}
}

}
