#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexAssignment : public SoundPlex {
public:
	SoundPlexAssignment(
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
