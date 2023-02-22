#include <cstring>
#include "Sample.h"

#include <cstdio>
#include "../fmod/fmod_errors.h"

namespace cdc {

extern FMOD::System *gFMOD;

Sample *Sample::Create(
	uint32_t zeroOneTwo,
	uint32_t length,
	uint32_t loopStart,
	uint32_t loopEnd,
	uint32_t sampleRate)
{
	// if (option_nosound.isActive)
	//.	return nullptr;
	// if (!format)
	// 	format = fmod_get_default_sound_format();
	auto *sound = new Sample;
	sound->zeroOneTwo28 = zeroOneTwo;
	sound->m_lengthInBytes = length;
	sound->m_sampleRate = sampleRate;
	sound->m_referenceCount = 0;
	sound->m_soundHandle = 0;
	sound->m_soundPlay = 0;
	sound->dword24 = 0;
	sound->dword20 = 0;
	sound->byte1C = 0;
	sound->m_loopStartInSamples = loopStart;
	sound->m_loopEndInSamples = loopEnd;
	return sound;
}

uint32_t Sample::Upload(
	uint32_t offset,
	void *src,
	uint32_t bytes)
{
	(void) offset;
	(void) bytes;

	uint32_t mode = 0xA040A40;
	if (m_loopEndInSamples)
		mode = 0xA040A42;

	FMOD_CREATESOUNDEXINFO createInfo;
	memset(&createInfo, 0, 0x6C); // do not clear the last four bytes?
	createInfo.length = m_lengthInBytes;
	createInfo.defaultfrequency = m_sampleRate;
	createInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); // = 0x70
	createInfo.numchannels = 1;
	createInfo.userdata = (void*)this;
	createInfo.suggestedsoundtype = FMOD_SOUND_TYPE_FSB; // = 8
	FMOD_RESULT result = gFMOD->createSound((const char*) src, mode, &createInfo, &m_soundHandle);
	if (result != FMOD_OK) {
		printf("Upload: FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return 0;
	}

	m_soundHandle->getSubSound(0, &m_soundPlay);

	return m_lengthInBytes;
}

// HACK
void Sample::Play() {
	FMOD::Channel *channel = 0;
	FMOD_RESULT result = gFMOD->playSound(FMOD_CHANNEL_FREE, m_soundPlay, 0, &channel);
	if (result != FMOD_OK)
		printf("Play: FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

}
