#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexSelector : public SoundPlex {
public:
	SoundPlexSelector(
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
