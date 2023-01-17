#pragma once
#include <cstdint>

struct Specialisation {
	static void BlockingChange(uint32_t mask);
	static void StartChange(uint32_t mask);
	static void ContinueChange();

	static uint32_t s_targetMask;
	static uint32_t s_phase; // 0, 1 or 2
};
