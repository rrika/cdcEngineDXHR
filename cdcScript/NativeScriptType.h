#pragma once
#include <cstdint>
#include "cdcScript/ScriptType.h"
#include "cdcSys/RCObject.h"

namespace cdc {

class ScriptObject;
class ScriptType;

class NativeScriptType : public RCObject {
public:
	const char *package; // 8
	const char *name; // C

	uint32_t size; // 14
	uint32_t align; // 18

	ScriptType *type; // 1C
	// uint8_t success; // 20

public:
	NativeScriptType(const char *package, const char *name) :
		package(package), name(name), size(0), align(0), type(nullptr)
	{}

	virtual bool InitType(ScriptType *_type) { // C
		type = _type;
		type->nativeScriptType = this;
		// success = true;
		return true;
	}

	void InitFunction(int32_t index, CallbackCallFunction *nativeFunc) {
		type->blob->m_functions[index].nativeFunc = nativeFunc;
	}
};

}
