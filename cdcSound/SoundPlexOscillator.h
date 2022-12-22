#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexOscillator : public SoundPlex {
public:
	SoundPlexOscillator(
		void *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlex(controls, controls3d, owner)
	{
		// TODO
	}

	SoundPlex *Update(float) override { /*TODO*/ return nullptr; }
	void End(EndType) override { /*TODO*/ }
};

}
