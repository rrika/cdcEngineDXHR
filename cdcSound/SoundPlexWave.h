#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexWave : public SoundPlex {
public:
	SoundPlexWave(
		void *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlex(controls, controls3d, owner)
	{
		// TODO
	}
};

}
