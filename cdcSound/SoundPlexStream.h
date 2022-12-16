#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexStream : public SoundPlex {
public:
	SoundPlexStream(
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
