#pragma once
#include "ResolveSection.h"
#include "cdcSound/RingBuffer.h"
#include "cdcSound/Sample.h"

namespace cdc {

struct WaveSectionEntry {
	uint32_t refCount; // 0
	Sample *sample; // 4
	uint32_t size; // 8
	uint8_t byteC;
	uint8_t byteD;
	uint32_t dword10;
	uint32_t dword14;
};

struct SoundBlobHeader {
	uint32_t sampleRate;
	uint32_t loopStart;
	uint32_t loopEnd;
	uint32_t dwordC;
};

class WaveSection : public ResolveSection {
	uint32_t dword4;
	WaveSectionEntry entries[4096]; // 8
	SoundBlobHeader soundBlobHeader; // 18008
	char *readTarget; // 18018
	uint32_t readAmount; // 1801C

	RingBuffer ringbuffer; // 18020
	uint32_t loadingId; // 1803C
	uint32_t loadingState; // 18040
	uint32_t totalSoundBytes; // 18044

	WaveSectionEntry *table[4096]; // dword18048
	uint32_t dword1C048;

	uint32_t dword1C04C;
	uint32_t dword1C050;
	uint32_t dword1C054;

	WaveSectionEntry *lookupEntry(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded);

public:
	uint32_t realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void *getWrapped(uint32_t) override; // 0x30
	uint32_t getDomainId(uint32_t) override;  // 0x44

	// HACK
	void playSound(uint32_t);
};

}
