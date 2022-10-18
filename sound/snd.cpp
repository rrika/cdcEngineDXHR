#include <cstdio>
#include <cstdlib>
#include "config.h"
#include "snd.h"
#ifdef _WIN32
#define WIN32 // make fmod use stdcall
#endif
#include "../fmod/fmod.hpp"
#include "../fmod/fmod_errors.h"

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

int SND_Init() { // 504
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
	// TODO
	return 1;
}

}
