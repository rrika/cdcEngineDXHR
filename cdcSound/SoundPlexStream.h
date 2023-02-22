#pragma once
#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"
#include "MultiplexStream.h"

namespace cdc {

class SoundPlexStream : public SoundPlexSingleChild {
	MultiplexStream *multiplexStream = nullptr; // 18
	dtp::SoundPlex::Stream *dtpStream = nullptr; // 24

public:
	SoundPlexStream(
		void *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner);

	uint32_t Init();
	SoundPlex *Update(float) override;
	void End(EndType) override;
};

}
