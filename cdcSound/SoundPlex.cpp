#include <cstdio>
#include "cdc/dtp/soundplex.h"
#include "cdcResource/DTPDataSection.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/WaveSection.h"
#include "SoundPlex.h"
#include "SoundPlexAssignment.h"
#include "SoundPlexCinematic.h"
#include "SoundPlexChoiceList.h"
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
#include "Voice.h"

#include "config.h" // for ENABLE_IMGUI
#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

namespace cdc {

SoundPlexCollection gSoundPlexCollection;

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

retry:
	printf("SoundPlexCollection::Create type=%d (%s)\n",
		snd->m_type,
		snd->m_type >= 17 ? "???" : soundPlexNodeNames[snd->m_type]);

	if (Voice::s_voiceCollection.IsOutOfMemory())
		return nullptr;

	switch (snd->m_type) {
	case dtp::SoundPlex::SoundPlexSelector_Silence: // 0
		plex = new SoundPlexTimeIn(*(float*)snd->m_data, nullptr, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference1: // 1
	case dtp::SoundPlex::SoundPlexSelector_Reference2: { // 2
		auto data = *(uint32_t*)snd->m_data;
		snd = (dtp::SoundPlex*)((DTPDataSection*)cdc::g_resolveSections[7])->getPointer(data); // HACK
		if (!snd)
			return nullptr;

		goto retry;
	}

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

	case dtp::SoundPlex::SoundPlexSelector_Assignment: { // 5
		auto *data = (dtp::SoundPlex::Assignment*)snd->m_data;
		plex = new SoundPlexAssignment(data, controls, controls3d, owner);
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Envelope: // 6
		plex = new SoundPlexEnvelope(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_Selector: // 7
		plex = new SoundPlexSelector(snd->m_data, controls, controls3d, owner);
		break;

	case dtp::SoundPlex::SoundPlexSelector_ChoiceList: { // 8
		auto *data = (dtp::SoundPlex::ChoiceList*)snd->m_data;

		// retry m_numSounds times
		for (uint32_t i = 0; i < data->m_numSounds; i++) {
			if (Voice::s_voiceCollection.IsOutOfMemory())
				break;

			plex = SoundPlexChoiceList::GetSoundPlexObject(*data, controls, controls3d, owner);
			if (plex)
				break;
		}
		break;
	}

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

	return plex;
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

	// delay = 2.f; for testing function of SoundPlexTimeIn

	if (delay > 0.0f) {
		plex = new SoundPlexTimeIn(
			delay,
			snd,
			&owner->controls,
			&owner->controls3d,
			owner);

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
		m_plexes.emplace_back(owner);
		// TODO

		return owner; // calls cdc::Handle<SoundOwner>::Handle(SoundOwner*)
	}

	return nullptr;
}

void SoundPlexCollection::Update(float time) {
	auto ownerIt = m_plexes.begin();
	while (ownerIt != m_plexes.end()) {
		auto replacementPlex = (*ownerIt)->plex->Update(time);
		if (replacementPlex) {
			(*ownerIt)->plex = replacementPlex;
			ownerIt++;
		}
		else
			ownerIt = m_plexes.erase(ownerIt);
	}
}

SoundHandle SOUND_StartPaused( // 382
	dtp::SoundPlex *snd,
	float delay
	// TODO
) {
	return gSoundPlexCollection.StartPaused(snd, delay);
}

}

void buildUI(dtp::SoundPlex *snd, std::function<void(cdc::SoundHandle)>* onPlay, std::string indent) {
#if ENABLE_IMGUI
	ImGui::PushID((uint32_t)snd);

	ImGui::Text("%s%s",
		indent.c_str(),
		snd->m_type >= 17 ? "[error]" : cdc::soundPlexNodeNames[snd->m_type]);

	ImGui::SameLine();
	if (ImGui::SmallButton("Play")) {
		auto handle = cdc::SOUND_StartPaused(snd, /*delay=*/ 0.0f);
		if (onPlay) (*onPlay)(handle);
	}

	switch (snd->m_type) {
	case dtp::SoundPlex::SoundPlexSelector_Silence: // 0
		break;

	case dtp::SoundPlex::SoundPlexSelector_Reference1: // 1
	case dtp::SoundPlex::SoundPlexSelector_Reference2: // 2
	{
		auto data = *(uint32_t*)snd->m_data;
		ImGui::SameLine();
		ImGui::Text("%04x", data);
		auto *refplex = (dtp::SoundPlex*)((DTPDataSection*)cdc::g_resolveSections[7])->getPointer(data);
		if (refplex)
			buildUI(refplex, onPlay, indent + "  ");
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Wave: { // 3
		auto *data = (dtp::SoundPlex::Wave*)snd->m_data;
		ImGui::SameLine();
		ImGui::Text("%04x", data->m_id);
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Stream: { // 4
		auto *data = (dtp::SoundPlex::Stream *)snd->m_data;
		ImGui::SameLine();
		ImGui::Text("%s", data->m_streamName);
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Assignment: { // 5
		auto *data = (dtp::SoundPlex::Assignment*)snd->m_data;
		buildUI(data->m_sound, onPlay, indent + "  ");
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Envelope: { // 6
		auto *data = (dtp::SoundPlex**)snd->m_data;
		buildUI(*data, onPlay, indent + "  ");
		// TODO
		break;
	}

	case dtp::SoundPlex::SoundPlexSelector_Selector: // 7
		break;

	case dtp::SoundPlex::SoundPlexSelector_ChoiceList: { // 8
		auto *data = (dtp::SoundPlex::ChoiceList*)snd->m_data;
		for (uint32_t i=0; i<data->m_numSounds; i++) {
			auto *subPlex = data->m_sounds[i];
			buildUI(subPlex, onPlay, indent + "  ");
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
