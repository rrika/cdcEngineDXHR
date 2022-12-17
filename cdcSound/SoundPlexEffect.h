#pragma once
#include "SoundPlex.h"

namespace cdc {

class SoundPlexEffect : public SoundPlexSingleChild {
public:
	SoundPlexEffect(
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
