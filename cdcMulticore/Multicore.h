#pragma once
#include <cstdint>

namespace cdc {

struct Job;

struct JobMethod { // line 353
	void (*funcptr)(Job*);
	uint32_t userData;
};

class Job { // line 224
public:
	Job();
	void AddUserData(uint64_t userData);
	void AddInput(void *buffer, int32_t sizeInBytes);
	void AddReadOnly(void *buffer, int32_t sizeInBytes);
	void SetOutput(void *buffer, int32_t sizeInBytes);

	void SetInputWritable() { m_inputWritable_maybe |= 1; } // line 260
	void SetStackSize(int32_t sizeInBytes) { m_stackSize = sizeInBytes; } // line 270
	void SetScratchSize(int32_t sizeInBytes) { m_scratchSize = sizeInBytes; } // line 276
	void SetMethod(JobMethod& method) { m_method = method; } // line 282

protected:
	struct MemoryBuffer { // line 302
		void *buffer;
		int32_t sizeInBytes;
	};

	uint64_t m_userData[26]; // 0
	uint32_t m_userDataCount; // D0

	MemoryBuffer m_input[26]; // D4
	uint32_t m_inputCount; // 1A4

	MemoryBuffer m_cache[4]; // 1A8
	uint32_t m_cacheCount; // 1C8

	MemoryBuffer m_output; // 1CC

	JobMethod m_method; // 1D4
	uint32_t m_stackSize; // 1DC
	uint32_t m_scratchSize; // 1E0
	uint32_t m_affinityMask_maybe; // 1E4
	uint32_t m_inputWritable_maybe; // 1E8
	uint32_t dword1EC;
};

}
