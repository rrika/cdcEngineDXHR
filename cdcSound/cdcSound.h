#pragma once
#include "cdc/dtp/soundgroup.h"
#include "cdc/dtp/controls3d.h"

namespace FMOD { class System; }

namespace cdc {

extern FMOD::System *gFMOD;

class SoundTypes {
public:
	enum EndType { // 314
		k0 = 0,
		k1 = 1,
		k2 = 2,
		k3 = 3,
		k4 = 4,
		k5 = 5,
		k6 = 6
	};

	struct Controls3d : public dtp::Controls3d { // 355

	};

	struct Controls : public dtp::SoundGroup::Controls { // 438

	};
};

}

	