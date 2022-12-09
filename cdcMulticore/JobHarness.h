#pragma once
#include "Multicore.h"

namespace cdc {

class JobHarness {
public:
	virtual void Execute() = 0;
protected:
	uint32_t dword4;
	Job job; // 8
	void *m_scratch; // 1F8
	uint32_t dword1FC;
};

}
