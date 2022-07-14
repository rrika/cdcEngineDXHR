#pragma once
#include "../ResolveSection.h"
#include "../../sound/RingBuffer.h"

namespace cdc {

struct Sound {
	void *fmodSoundBank;
	void *fmodSound;
	uint32_t dword8;
	uint32_t length; // C
	uint32_t loopStart; // 10
	uint32_t loopEnd; // 14
	uint32_t dword18;
	uint8_t byte1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
};

struct WaveSectionEntry {
	uint32_t refCount;
	Sound *sound; // 4
	uint32_t size; // 8
	uint8_t byteC;
	uint8_t byteD;
	uint32_t dword10;
	uint32_t dword14;
};

struct SoundBlobHeader {
	uint32_t dword0;
	uint32_t loopStart;
	uint32_t loopEnd;
	uint32_t dwordC;
};

class WaveSection : public ResolveSection {
	uint32_t dword4;
	WaveSectionEntry entries[4096];
	SoundBlobHeader soundBlobHeader;
	char *readTarget; // 18018
	uint32_t readAmount; // 1801C

	RingBuffer ringbuffer; // 18020
	uint32_t loadingId; // 1803C
	uint32_t loadingState; // 18040
	uint32_t dword18044;

	WaveSectionEntry *table[4096];
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
};

}
