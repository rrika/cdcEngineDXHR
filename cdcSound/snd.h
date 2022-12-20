#pragma once
#include <cstdint>

namespace cdc {

int SND_Init();
void SND_GetNextDSPClock(uint32_t clock[2]);
void SND_SystemTimer(float);

}
