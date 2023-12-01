#include <cstdint>
#include <cstring>
#include "anitracker.h"
#include "config.h"

#if ENABLE_IMGUI
#include <cstdio>
#include "imgui/imgui.h"
#include "cdcAnim/Inspector.h"
#endif

namespace cdc {

static uint32_t sNextFreeAnimHint;
uint32_t gNumLoadedAnimations;
static unsigned short sAnimRefCounts[4000];
static unsigned short sAnimIDs[4000];
AniTracker aniTracker[4000];

void ANITRACKER_Init() {
	memset(sAnimRefCounts, 0, sizeof(sAnimRefCounts));
	sNextFreeAnimHint = 0;
	gNumLoadedAnimations = 0;
}

int ANITRACKER_FindAnimation(uint32_t animID) {
	for (uint32_t i=0; i<4000; i++)
		if (sAnimRefCounts[i] > 0 && sAnimIDs[i] == animID)
			return i;
	return -1;
}

uint32_t ANITRACKER_GetRefCount(uint32_t handle) {
	return sAnimRefCounts[handle];
}

void *ANITRACKER_ResolveAnimation(uint32_t id, uint32_t size) {
	int handle = ANITRACKER_FindAnimation(id);
	if (handle == -1) {
		void *data = (void*) new char[size];
		ANITRACKER_AllocateAnimation(data, id);
		return data;
	} else {
		sAnimRefCounts[handle]++;
		return nullptr;
	}
}

uint32_t ANITRACKER_AllocateAnimation(void *animation, uint32_t animID) {
	for (uint32_t i=sNextFreeAnimHint; i<4000; i++)
		if (sAnimRefCounts[i] == 0) {
			++gNumLoadedAnimations;
			sNextFreeAnimHint = i+1;
			sAnimRefCounts[i] = 1;
			aniTracker[i].animationData = (AnimFragment*)animation;
			sAnimIDs[i] = animID;
			return i;
		}
	return -1;
}

void ANITRACKER_FreeAnimation(uint32_t handle) {
	if (--sAnimRefCounts[handle] == 0) {
		delete[] (char*) aniTracker[handle].animationData;
		if (sNextFreeAnimHint > handle)
			sNextFreeAnimHint = handle;
		// TODO
		--gNumLoadedAnimations;
	}
}

void ANITRACKER_BuildUI(UIActions& uiact) {
#if ENABLE_IMGUI
	uint32_t encountered = 0;
	for (uint32_t i=0; i<4000 && encountered < gNumLoadedAnimations; i++) {
		if (sAnimRefCounts[i] != 0) {
			encountered++;
			char label[20];
			snprintf(label, 20, "fragment %04x", sAnimIDs[i]);
			if (ImGui::TreeNode(label)) {
				buildUI(uiact, aniTracker[i].animationData);
				ImGui::TreePop();
			}
		}
	}
#endif
}

}
