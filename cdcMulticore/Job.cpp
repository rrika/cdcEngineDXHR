#include <cstring> // memset
#include "Multicore.h"

namespace cdc {

Job::Job() { // line 18
	memset(this, 0, sizeof(Job));
	m_affinityMask_maybe = -1;
}

void Job::AddUserData(uint64_t userData) { // line 48
	m_userData[m_userDataCount++] = userData;
}

void Job::AddInput(void *buffer, int32_t sizeInBytes) { // line 57
	m_input[m_inputCount++] = { buffer, sizeInBytes };
}

void Job::AddReadOnly(void *buffer, int32_t sizeInBytes) { // line 92
	m_cache[m_cacheCount++] = { buffer, sizeInBytes };
}

void Job::SetOutput(void *buffer, int32_t sizeInBytes) { // line 109
	m_output = { buffer, sizeInBytes };
}

}
