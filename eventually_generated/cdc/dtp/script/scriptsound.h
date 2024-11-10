#pragma once
#include "../soundplex.h"

namespace dtp {

struct ScriptSound {
	SoundPlex m_plex;
	uint16_t m_endType;
	uint16_t m_refCount;
};

}
