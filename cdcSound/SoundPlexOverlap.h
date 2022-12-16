#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexOverlap : public SoundPlex {
public:
	SoundPlexOverlap(
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
