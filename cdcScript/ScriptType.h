#pragma once
#include "cdcSys/RCObject.h"

namespace cdc {

class NativeScriptType;
class ScriptType;

struct ScriptTypeStreamData { // 692
	uint32_t    scriptTypeVersion;
	uint16_t    field_4;
	uint16_t    field_6;
	uint16_t    id8;
	uint16_t    idA;
	char       *path; // 0C
	char       *shortName;
	// TODO
};

class ScriptType : public RCObject { // 749
public:
	ScriptTypeStreamData* blob;
	NativeScriptType* nativeScriptType;
public:
	ScriptType(uint32_t size) {
		blob = (ScriptTypeStreamData*) new char[size];
	}
	virtual ~ScriptType() {
		// TODO
		delete[] (char*)blob;
	}
	virtual void finalize() { /*TODO*/ }
};

}
