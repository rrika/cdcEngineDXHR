#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexEffect : public SoundPlex {
public:
	SoundPlexEffect(
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
