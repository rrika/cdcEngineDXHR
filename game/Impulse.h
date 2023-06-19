#pragma once
#include <cstdint>

using ImpulseFunc = void (void*, void*);
void impulse_register(uint32_t index, void *key, void *userdata, ImpulseFunc *funcptr, const char *comment=nullptr);
void impulse_call(uint32_t index, void *arg, void *key);

struct IXYZ {
	uint32_t x;
	uint32_t y;
	void *z;
};

inline void impulse_call_10Z(uint32_t index, void *arg, void *key=nullptr) {
	IXYZ ixyz { 1, 0, arg };
	impulse_call(index, &ixyz, key);
}
