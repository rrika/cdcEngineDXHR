#pragma once
#include <cstdint>
#include "MediaStream.h"

namespace cdc {

class Sample;
class SoundStreamHandler;

typedef struct { // line 69
	uint32_t hertz;             // 0
	uint32_t startLoop;         // 4
	uint32_t endLoop;           // 8
	uint32_t channelCount;      // C
	uint32_t reverbVol;         // 10
	uint32_t startSizeToLoad;   // 14
	uint32_t partialLoop;       // 18
	uint32_t loopAreaSize;      // 1C
	uint32_t hasCinematic;      // 20
	uint32_t hasSubtitles;      // 24
	uint32_t faceFxSize;        // 28
	uint32_t loopStartFileOffset;   // 2C
	uint32_t loopStartBundleOffset; // 30
	uint32_t maxEEBytesPerRead; // 34
	float mediaLength;          // 38
	float volLeft[12];          // 3C
	float volRight[12];         // 6C
	uint32_t loopStartSamplesToSkip[12]; // 9C
	char padding[0x2D0-0xCC];   // CC
} STRHEADER;

enum StreamType { // line 204
	kSoundStream = 0,
	kCinematicStream = 1,
	kMusicStream = 2
};

class MultiplexStream : public MediaStream { // line 71
public:
	static void SetSoundDirectory(const char *dir);
	static void SetCinematicDirectory(const char *dir);
	static MultiplexStream *CreateSoundStream(const char *name, uint8_t priority);
	static MultiplexStream *CreateMusicStream(const char *name);

	inline void Play() override;
	void Stop() override;
};

class MultiplexStreamImpl : public MultiplexStream { // line 239
	friend class MultiplexStream;
	static char s_soundStreamDir[256];
	static char s_cinematicStreamDir[256];

	STRHEADER m_streamHeader; // B0

	SoundStreamHandler *m_soundStreamHandler; // 41C

public:
	MultiplexStreamImpl(); // line 729
	~MultiplexStreamImpl(); // line 790
	void PlayRequest(); // line 1516
	bool Init(StreamType streamType, uint8_t priority, const char *name); // line 2284
};

void MultiplexStream::Play() { // line 434
	auto *impl = static_cast<MultiplexStreamImpl*>(this);
	impl->PlayRequest();
}

}
