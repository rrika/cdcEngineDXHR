#pragma once
#include <cstdint>

struct UIActions;

namespace cdc {

class AnimFragment;

struct AniTracker {
	AnimFragment *animationData;
	// uint32_t pad;
};

extern uint32_t gNumLoadedAnimations;
extern AniTracker aniTracker[];

void ANITRACKER_Init();
int ANITRACKER_FindAnimation(uint32_t animID);
// void ANITRACKER_TranslateAnimationList();
uint32_t ANITRACKER_GetRefCount(uint32_t handle);
void *ANITRACKER_ResolveAnimation(uint32_t id, uint32_t size);
uint32_t ANITRACKER_AllocateAnimation(void *animation, uint32_t animID);
void ANITRACKER_FreeAnimation(uint32_t handle);

void ANITRACKER_BuildUI(UIActions& uiact);

}
