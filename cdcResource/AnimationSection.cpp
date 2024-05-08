#include <cstring>
#include "AnimationSection.h"
#include "cdcKit/Animation/anitracker.h"

namespace cdc {

uint32_t AnimationSection::StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	alreadyLoaded = ANITRACKER_ResolveAnimation(sectionId, size) == nullptr;
	return ANITRACKER_FindAnimation(sectionId); // if ResolveAnimation returned the index this wouldn't be needed
}

void AnimationSection::ReleaseResource(uint32_t id) {
	ANITRACKER_FreeAnimation(id);
}

void AnimationSection::HandleResourceData(uint32_t handle, void *src, uint32_t size, uint32_t offset) {
	memcpy(((char*)aniTracker[handle].animationData) + offset, src, size);
}

void *AnimationSection::GetBasePointer(uint32_t handle) {
	return (void*)aniTracker[handle].animationData;
}

uint32_t AnimationSection::FindResource(uint32_t sectionId) {
	return ANITRACKER_FindAnimation(sectionId);
}

}
