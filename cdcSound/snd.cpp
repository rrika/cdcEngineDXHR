#include <cstdio>
#include <cstdlib>
#include "config.h"
#include "snd.h"
#ifdef _WIN32
#define WIN32 // make fmod use stdcall
#endif
#include "3rdParty/fmod/fmod.hpp"
#include "3rdParty/fmod/fmod_errors.h"
#include "Voice.h"

static void ERRCHECK(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	} else {
		puts("ok");
	}
}

namespace cdc {

FMOD::System *gFMOD;

int SND_Init() { // line 504
	FMOD_RESULT result;
	// TODO
	result = FMOD_System_Create((FMOD_SYSTEM**)&gFMOD);
	ERRCHECK(result);
	// TODO
#if __linux__
	result = gFMOD->setOutput(FMOD_OUTPUTTYPE_ALSA);
	ERRCHECK(result);
#else

#endif
	// TODO
	result = gFMOD->init(
		512,
		FMOD_INIT_VOL0_BECOMES_VIRTUAL | FMOD_INIT_SOFTWARE_OCCLUSION /*0x88*/,
		0);
	ERRCHECK(result);

	gFMOD->getMasterChannelGroup(&Voice::s_voiceCollection.m_groupMaster);

	// TODO
	return 1;
}

void SND_GetNextDSPClock(uint32_t clock[2]) { // line 776
	gFMOD->getDSPClock(&clock[0], &clock[1]);
	uint32_t bufferLength;
	int32_t numBuffer;
	gFMOD->getDSPBufferSize(&bufferLength, &numBuffer);
	uint32_t nlo = clock[1] + bufferLength * numBuffer;
	if (nlo < clock[1]) clock[0]++; // overflow into 'hi'
	clock[1] = nlo;
}

void SND_SystemTimer(float seconds) { // line 846
	// if (...) return;
	static float t = 0.0f;
	t += seconds;
	t -= 0.016666668f;

	gFMOD->update();
}

void SND_SetFMODModeFlag(FMOD::Channel *channel, FMOD_MODE flags) { // line 916
	FMOD_MODE mode;
	channel->getMode(&mode);
	if (flags & 0x00000007)
		mode &= 0xFFFFFFF8;
	if (flags & 0x000C0000)
		mode &= 0xFFF3FFFF;
	if (flags & 0x04300000)
		mode &= 0xFBCFFFFF;
	if (flags & 0x00000018)
		mode &= 0xFFFFFFE7;
	if (flags & 0x00000060)
		mode &= 0xFFFFFF9F;
	channel->setMode(mode | flags);
}

}
