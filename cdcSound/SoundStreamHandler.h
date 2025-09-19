#pragma once
#include "MediaStream.h"
#include "MultiplexStream.h"

namespace cdc {

class Voice;

class SoundStreamHandler : public MediaStreamHandler {
protected:
	SoundStreamHandler(MediaStream*);
public:
	static SoundStreamHandler *Create(MediaStream*);
	int ChannelCount() const;
	Voice *GetVoice(int channelIndex);

	void hackLoad(STRHEADER*, const char *path);

	void Start() override;
	bool OnStreamStop() override;
};

}
