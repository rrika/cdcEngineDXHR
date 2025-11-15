#pragma once
#include <cstdint>

namespace cdc {

class PCDX11AsyncCreateResource {
public:
	uint32_t m_dword4;
	bool m_keepWaiting = false;
	bool m_requested = false;
	virtual void asyncCreate() = 0;
	void request(uint32_t data) {
		if (!m_requested) {
			m_requested = true;
			m_keepWaiting = true;
			// deviceManager->queueAsyncCreation(this, data);
		}
	}
	void awaitResource() {
		// hack
		if (m_keepWaiting) {
			asyncCreate();
			m_keepWaiting = false;
		}
	}
};

}
