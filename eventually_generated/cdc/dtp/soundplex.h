#pragma once
#include <cstdint>

namespace dtp {

struct SoundPlex {
	enum SoundPlexSelector { // 137
		SoundPlexSelector_Silence = 0,
		SoundPlexSelector_Reference1 = 1,
		SoundPlexSelector_Reference2 = 2,
		SoundPlexSelector_Wave = 3,
		SoundPlexSelector_Stream = 4,
		SoundPlexSelector_Assignment = 5,
		SoundPlexSelector_Envelope = 6,
		SoundPlexSelector_Selector = 7,
		SoundPlexSelector_ChoiceList = 8,
		SoundPlexSelector_MaterialList = 9,
		SoundPlexSelector_Reaction = 10,
		SoundPlexSelector_Sequence = 11,
		SoundPlexSelector_Overlap = 12,
		SoundPlexSelector_Effect = 13,
		SoundPlexSelector_Oscillator = 14,
		SoundPlexSelector_Cinematic = 15,
		SoundPlexSelector_Stitching = 16
	};

	struct Assignment {
		SoundPlex *m_sound;
		// TODO
	};

	struct ChoiceList {
		uint32_t m_numSounds; // 0
		SoundPlex **m_sounds; // 4
		SoundPlex *m_soundData; // 8
		int8_t m_sequence; // C
		int8_t m_trackType; // D
		uint32_t m_counter; // 10
	};

	struct Wave {
		uint32_t m_id;
		// TODO
	};

	int32_t m_type;
	void *m_data;
};

}
