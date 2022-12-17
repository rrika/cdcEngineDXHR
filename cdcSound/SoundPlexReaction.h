#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexReaction : public SoundPlexSingleChild {
public:
	SoundPlexReaction(
		void *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner)
	:
		SoundPlexSingleChild(controls, controls3d, owner)
	{
		// TODO
	}
};

}
