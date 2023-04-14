#pragma once
#include <cstdint>

namespace dtp {

struct SoundGroup { // line 55
	struct Controls { // line 60
		float volume;
		float pitch;
		float filter;
		float reverb;
		uint32_t flags;
	};
	// TODO
};

}
