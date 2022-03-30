#pragma once
#include "../types.h"

namespace cdc {

class PCDX11AsyncCreateResource {
public:
	uint m_dword4;
	bool m_keepWaiting = false;
	bool m_requested = false;
	virtual void asyncCreate() = 0;
	void request(uint data) {}
	void await() {}
};

}
