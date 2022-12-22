#include "snd.h"
#include "Sound.h"
#include "SoundPlex.h"
#include "Voice.h"

namespace cdc {

void SOUND_Update(float seconds) { // 222
	// TODO
	gSoundPlexCollection.Update(seconds);
	Voice::s_voiceCollection.Update(seconds);
	// TODO
	SND_SystemTimer(seconds);
}

void SOUND_Init() { // 312
	SND_Init();
}

}
