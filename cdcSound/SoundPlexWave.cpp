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
		Voice::s_voiceCollection.Create(wave->sample);
	}
}

}
