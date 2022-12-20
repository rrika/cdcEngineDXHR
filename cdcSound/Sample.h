#pragma once
#include <cstdint>
#ifdef _WIN32
#define WIN32 // make fmod use stdcall
#endif
#include "3rdParty/fmod/fmod.hpp"

namespace cdc {

class VoiceCollection;

class Sample {
protected:
	FMOD::Sound *m_soundHandle;    // 0
	FMOD::Sound *m_soundPlay;      // 4
	uint32_t m_referenceCount;     // 8
	uint32_t m_lengthInBytes;      // C
	uint32_t m_loopStartInSamples; // 10
	uint32_t m_loopEndInSamples;   // 14
	uint32_t m_sampleRate;         // 18
	uint8_t byte1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t zeroOneTwo28;

	friend class VoiceCollection;

public:
	static Sample *Create(
		uint32_t zeroOneTwo,
		uint32_t length,
		uint32_t loopStart,
		uint32_t loopEnd,
		uint32_t format);

	uint32_t Upload(
		uint32_t offset,
		void *src,
		uint32_t bytes);

	// HACK
	void Play();
};

}
