#pragma once
#include "ResolveSection.h"
#include "cdcSound/RingBuffer.h"
#include "cdcSound/Sample.h"

namespace cdc {

struct Wave {
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
	Wave entries[4096]; // 8
	SoundBlobHeader soundBlobHeader; // 18008
	char *readTarget; // 18018
	uint32_t readAmount; // 1801C

	RingBuffer ringbuffer; // 18020
	uint32_t loadingId; // 1803C
	uint32_t loadingState; // 18040
	uint32_t totalSoundBytes; // 18044

	Wave *table[4096]; // dword18048
	uint32_t dword1C048;

	uint32_t dword1C04C;
	uint32_t dword1C050;
	uint32_t dword1C054;

	Wave *lookupEntry(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded);

public:
	uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void HandleResourceEnd(uint32_t id, void *drmSectionHeader) override;
	void *GetBasePointer(uint32_t) override; // 0x30
	uint32_t FindResource(uint32_t) override;  // 0x44

	virtual Wave *WaveFind(uint32_t); // 0x64, called from SoundPlexWave::SoundPlexWave

	// HACK
	void playSound(uint32_t);
};

}
