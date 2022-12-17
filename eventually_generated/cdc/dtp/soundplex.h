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

	struct Choice {
		uint32_t m_numSounds;
		SoundPlex **m_sounds;
		SoundPlex *m_soundData;
		// TODO
	};

	struct Wave {
		uint32_t m_id;
		// TODO
	};

	int32_t m_type;
	void *m_data;
};

}
