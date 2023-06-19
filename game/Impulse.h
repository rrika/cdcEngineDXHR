#pragma once
#include <cstdint>

using ImpulseFunc = void (void*, void*);
void impulse_register(uint32_t index, void *key, void *userdata, ImpulseFunc *funcptr, const char *comment=nullptr);
void impulse_call(uint32_t index, void *arg, void *key);
