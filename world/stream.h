#pragma once
#include <cstdint>

namespace cdc { class ResolveObject; }

struct StreamUnitPortal {
	const char name[128];
	uint16_t word80;
	uint16_t signal_index;
	uint16_t word84;
	uint16_t patchIntoSignalDwordC;
	uint32_t dword88;
	uint32_t dword8C;
	cdc::ResolveObject *resolveObject90_maybe;
	uint32_t dword94;
	uint32_t dword98;
	uint32_t dword9C;
	uint32_t dwordA0;
	uint32_t dwordA4;
	uint32_t dwordA8;
	uint32_t dwordAC;
	uint32_t dwordB0;
	uint32_t dwordB4;
	uint32_t dwordB8;
	uint32_t dwordBC;
	uint32_t paddingC0[16];
};
