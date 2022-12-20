#pragma once
#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"

namespace cdc {

class SoundPlexWave : public SoundPlex {
public:
	SoundPlexWave(
		dtp::SoundPlex::Wave *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner);
};

}
