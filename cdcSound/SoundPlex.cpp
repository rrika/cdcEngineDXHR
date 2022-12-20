#include <cstdio>
#include "cdc/dtp/soundplex.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/WaveSection.h"
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

#include "config.h" // for ENABLE_IMGUI
#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

namespace cdc {

static SoundPlexCollection gSoundPlexCollection;

static const char *soundPlexNodeNames[] = {
	"Silence",
	"Reference1",
	"Reference2",
	"Wave",
	"Stream",
	"Assignment",
	"Envelope",
	"Selector",
	"ChoiceList",
	"MaterialList",
	"Reaction",
	"Sequence",
	"Overlap",
	"Effect",
	"Oscillator",
	"Cinematic",
	"Stitching"
};

SoundPlex *SoundPlexCollection::Create( // line 126
	dtp::SoundPlex *snd,
	SoundTypes::Controls *controls,
	SoundTypes::Controls3d *controls3d,
	SoundOwner *owner
) {
	SoundPlex *plex = nullptr;

	// TODO

	printf("SoundPlexCollection::Create type=%d (%s)\n",
		snd->m_type,
		snd->m_type >= 17 ? "???" : soundPlexNodeNames[snd->m_type]);

	switch (snd->m_type) {
	case dtp::SoundPlex::SoundPlexSelector_Silence: // 0
		plex = new SoundPlexTimeIn(*(float*)snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference1: // 1
	case dtp::SoundPlex::SoundPlexSelector_Reference2: // 2
		// TODO
		break;

	case dtp::SoundPlex::SoundPlexSelector_Wave: { // 3
		// TODO
		auto *data = (dtp::SoundPlex::Wave*)snd->m_data;
		if (!data)
			return nullptr;

		Wave *wave = ((WaveSection*)g_resolveSections[6])->WaveFind(data->m_id);
		if (!wave)
			return nullptr;

		if (!wave->sample)
			return nullptr;

		plex = new SoundPlexWave(data, controls, controls3d, owner);
		break;
	}

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

SoundHandle SoundPlexCollection::StartPaused( // 178
	dtp::SoundPlex *snd,
	float delay)
{
	// if (option no sound) return;
	if (!snd) return nullptr;
	// if (!globalSoundInfo) return;
	// if (!cdc::gFMOD) return;

	auto *owner = new SoundOwner();
	SoundPlex *plex;

	if (delay > 0.0f) {
		// plex = ...

	} else {
		plex = SoundPlexCollection::Create(
			snd,
			&owner->controls,
			&owner->controls3d,
			owner
		);
	}

	owner->plex = plex;

	if (plex) {
		// TODO
		m_plexes.push_back(owner);
		// TODO

		return owner; // calls cdc::Handle<SoundOwner>::Handle(SoundOwner*)
	}

	return nullptr;
}

SoundHandle SOUND_StartPaused( // 382
	dtp::SoundPlex *snd,
	float delay
	// TODO
) {
	return gSoundPlexCollection.StartPaused(snd, delay);
}

}

void buildUI(dtp::SoundPlex *snd, std::string indent) {
#if ENABLE_IMGUI
	ImGui::PushID((uint32_t)snd);

	ImGui::Text("%s%s",
		indent.c_str(),
		snd->m_type >= 17 ? "[error]" : cdc::soundPlexNodeNames[snd->m_type]);

	ImGui::SameLine();
	if (ImGui::SmallButton("Play")) {
		cdc::SOUND_StartPaused(snd, /*delay=*/ 0.0f);
	}

	switch (snd->m_type) {
	case dtp::SoundPlex::SoundPlexSelector_Silence: // 0
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference1: // 1
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference2: // 2
		break;

	case dtp::SoundPlex::SoundPlexSelector_Wave: { // 3
		auto *data = (dtp::SoundPlex::Wave*)snd->m_data;
		ImGui::SameLine();
		ImGui::Text("%04x", data->m_id);
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Stream: // 4
		break;

	case dtp::SoundPlex::SoundPlexSelector_Assignment: { // 5
		auto *data = (dtp::SoundPlex::Assignment*)snd->m_data;
		buildUI(data->m_sound, indent + "  ");
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Envelope: // 6
		break;

	case dtp::SoundPlex::SoundPlexSelector_Selector: // 7
		break;

	case dtp::SoundPlex::SoundPlexSelector_ChoiceList: { // 8
		auto *data = (dtp::SoundPlex::Choice*)snd->m_data;
		for (uint32_t i=0; i<data->m_numSounds; i++) {
			auto *subPlex = data->m_sounds[i];
			buildUI(subPlex, indent + "  ");
		}
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_MaterialList: // 9
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reaction: // 0xA = 10
		break;

	case dtp::SoundPlex::SoundPlexSelector_Sequence: // 0xB = 11
		break;

	case dtp::SoundPlex::SoundPlexSelector_Overlap: // 0xC = 12
		break;

	case dtp::SoundPlex::SoundPlexSelector_Effect: // 0xD = 13
		break;

	case dtp::SoundPlex::SoundPlexSelector_Oscillator: // 0xE = 14
		break;

	case dtp::SoundPlex::SoundPlexSelector_Cinematic: // 0xF = 15
		break;

	case dtp::SoundPlex::SoundPlexSelector_Stitching: // 0x10 = 16
		break;

	}

	ImGui::PopID();
#endif
}
