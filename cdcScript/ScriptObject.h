#pragma once
#include "cdcSys/RCObject.h"
#include "cdcSys/SList.h"
#include "cdcScript/ScriptType.h" // so RCPtr knows ScriptType is derived from RCObject

namespace cdc {

class ScriptExec;
class ScriptSequence;
class ScriptType;
class Function;
union DataValue;

class ScriptObject /* : public GCObject */ {
	RCPtr<ScriptType> scriptType;
	uint8_t *m_instance; // C

public:
	ScriptObject(ScriptType*);
	virtual ~ScriptObject() = default; // force vtable

	Function* GetFunction(int32_t state, int32_t vtIndex);
	void CallFunction(Function *func, int32_t numArgs, DataValue *args, void *retVal);

	uint8_t GetState() {
		return m_instance ? m_instance[0] : 0;
	}
};

}
