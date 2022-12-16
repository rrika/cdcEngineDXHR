#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexReaction : public SoundPlex {
public:
	SoundPlexReaction(
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
