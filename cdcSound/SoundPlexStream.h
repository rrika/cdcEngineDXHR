#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexStream : public SoundPlexSingleChild {
public:
	SoundPlexStream(
		void *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlexSingleChild(controls, controls3d, owner)
	{
		// TODO
	}

	SoundPlex *Update(float) override { /*TODO*/ return nullptr; }
	void End(EndType) override { /*TODO*/ }
};

}
