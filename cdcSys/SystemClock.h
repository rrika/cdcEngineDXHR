#pragma once

#ifdef _WIN32
	// TODO: check this
	#include <stdio.h>
	#include <intrin.h>
	#pragma intrinsic(__rdtsc)
#else
	#include <x86intrin.h>
#endif

namespace cdc {

uint64_t GetSystemTicks() { return __rdtsc(); }

}
