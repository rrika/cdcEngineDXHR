#include "ScriptObject.h"
#include "ScriptType.h"

namespace cdc {

ScriptObject::ScriptObject(ScriptType *scriptType) :
	scriptType(scriptType),
	m_instance(nullptr)
{
	// TODO
	m_instance = (uint8_t*) new char;
	m_instance[0] = 0;
}

Function *ScriptObject::GetFunction(int32_t state, int32_t vtIndex) {
	return scriptType->GetVFunction(state, vtIndex);
}

void ScriptObject::CallFunction(Function *func, int32_t numArgs, DataValue *args, void *retVal) {
	if (!func || !m_instance)
		return;

	if (func->flags & Function::Flags::NATIVE) {
		func->nativeFunc(this, numArgs, args, retVal);
		return;
	}

	// TODO
}

}
