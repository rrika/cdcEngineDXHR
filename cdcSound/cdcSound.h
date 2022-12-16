#pragma once
#include "cdc/dtp/soundgroup.h"
#include "cdc/dtp/controls3d.h"

namespace cdc {

class SoundTypes {
public:
	struct Controls3d : public dtp::Controls3d { // 355

	};

	struct Controls : public dtp::SoundGroup::Controls { // 438

	};
};

}

	