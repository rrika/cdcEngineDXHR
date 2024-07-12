#include <cstdio>
#include "ScriptObject.h"
#include "ScriptType.h"

namespace cdc {

ScriptObject::ScriptObject(ScriptType *scriptType) :
	GCObject(),
	scriptType(scriptType),
	m_instance(nullptr)
{
	ConstructInstance(); // HACK
}

void ScriptObject::ConstructInstance() {
	// HACK
	m_instance = (uint8_t*) new char;
	m_instance[0] = 0;
}

void ScriptObject::DestructInstance() {
	// HACK
	delete m_instance;
	m_instance = nullptr;
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

ScriptObject::~ScriptObject() {
	// TODO
	DestructInstance(); // HACK
}

void ScriptObject::MarkChildren() {
	// TODO
}

void ScriptObject::Finalize() {
	// TODO
}

const char* ScriptObject::GetGCName() {
	static char scriptobject_string_buffer[32];
	sprintf(scriptobject_string_buffer, "Script: %d", (int)scriptType->blob->m_name.nocaseStr.m_id);
	return scriptobject_string_buffer;
}

}
