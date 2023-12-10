#pragma once
#include <cstdint>

typedef unsigned int FMOD_MODE;
namespace FMOD { class Channel; }

namespace cdc {

int SND_Init();
void SND_GetNextDSPClock(uint32_t clock[2]);
void SND_SystemTimer(float);
void SND_SetFMODModeFlag(FMOD::Channel *channel, FMOD_MODE flags);

}
