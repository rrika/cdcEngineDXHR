#pragma once
#include <cstdint>

namespace cdc {

class GCObject;
class ScriptType;

union DataValue { // 399
	uint8_t int8Value;
	uint16_t int16Value;
	uint32_t int32Value;
	int8_t boolValue;
	int32_t intValue;
	float floatValue;
	void *nativeValue;
	const char *stringValue;
	ScriptType *script;
	GCObject *object;
	uint8_t *address;
};

using CallbackCallFunction = void(void*, int32_t, DataValue*, void*);

}
