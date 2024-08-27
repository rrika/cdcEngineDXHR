#pragma once
#include <cstdint>

namespace dtp { struct Intro; }

namespace IntroManager {

dtp::Intro *FindIntroUniqueID(int32_t, int32_t*);

}

inline dtp::Intro *INSTANCE_FindIntroUniqueID(int32_t introUniqueID, int32_t *streamUnitID) { // line 182
	return IntroManager::FindIntroUniqueID(introUniqueID, streamUnitID);
}

