#pragma once
#include "cdc/dtp/soundplex.h"
#include "SoundPlex.h"

namespace cdc {

class SoundPlexAssignment : public SoundPlexSingleChild {
public:
	SoundPlexAssignment(
		dtp::SoundPlex::Assignment *data,
		Controls *controls,
		Controls3d *controls3d,
		SoundOwner *owner);

	SoundPlex *Update(float) override;
	void End(EndType) override;
};

}
