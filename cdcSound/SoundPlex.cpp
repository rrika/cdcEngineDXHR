#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"
#include "SoundPlexAssignment.h"
#include "SoundPlexCinematic.h"
#include "SoundPlexEffect.h"
#include "SoundPlexEnvelope.h"
#include "SoundPlexMaterialList.h"
#include "SoundPlexOscillator.h"
#include "SoundPlexOverlap.h"
#include "SoundPlexReaction.h"
#include "SoundPlexSelector.h"
#include "SoundPlexSequence.h"
#include "SoundPlexStitching.h"
#include "SoundPlexStream.h"
#include "SoundPlexTimeIn.h"
#include "SoundPlexWave.h"

namespace cdc {

SoundPlex *SoundPlexCollection::Create(
	dtp::SoundPlex *snd,
	SoundTypes::Controls *controls,
	SoundTypes::Controls3d *controls3d,
	SoundOwner *owner
) {
	SoundPlex *plex = nullptr;

	// TODO
	switch (snd->m_type) {
	case dtp::SoundPlex::SoundPlexSelector_Silence: // 0
		plex = new SoundPlexTimeIn(*(float*)snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference1: // 1
	case dtp::SoundPlex::SoundPlexSelector_Reference2: // 2
		// TODO
		break;

	case dtp::SoundPlex::SoundPlexSelector_Wave: // 3
		// TODO
		plex = new SoundPlexWave(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Stream: // 4
		plex = new SoundPlexStream(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Assignment: // 5
		plex = new SoundPlexAssignment(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Envelope: // 6
		plex = new SoundPlexEnvelope(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Selector: // 7
		plex = new SoundPlexSelector(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_ChoiceList: // 8
		// TODO
		break;

	case dtp::SoundPlex::SoundPlexSelector_MaterialList: // 9
		plex = new SoundPlexMaterialList(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reaction: // 0xA = 10
		plex = new SoundPlexReaction(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Sequence: // 0xB = 11
		plex = new SoundPlexSequence(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Overlap: // 0xC = 12
		plex = new SoundPlexOverlap(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Effect: // 0xD = 13
		plex = new SoundPlexEffect(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Oscillator: // 0xE = 14
		plex = new SoundPlexOscillator(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Cinematic: // 0xF = 15
		plex = new SoundPlexCinematic(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Stitching: // 0x10 = 16
		if (!snd->m_data)
			return nullptr;
		plex = new SoundPlexStitching(snd->m_data, controls, controls3d, owner);
		break;

	}

	if (plex) {
		// TODO
	}

	return nullptr;
}

}
