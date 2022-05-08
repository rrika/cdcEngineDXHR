#pragma once
#include "../types.h"

namespace cdc {

class PCDX11AsyncCreateResource {
public:
	uint m_dword4;
	bool m_keepWaiting = false;
	bool m_requested = false;
	virtual void asyncCreate() = 0;
	void request(uint data) {
		if (!m_requested) {
			m_requested = true;
			m_keepWaiting = true;
			// deviceManager->queueAsyncCreation(this, data);
		}
	}
	void awaitResource() {
		// hack
		asyncCreate();
		m_keepWaiting = false;
	}
};

}
