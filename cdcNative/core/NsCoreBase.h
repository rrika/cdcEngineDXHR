#pragma once
#include "cdcScript/ScriptObject.h"

class ObjectThread;

class NsCoreBase : public cdc::ScriptObject {
	ObjectThread *m_thread = nullptr;
public:
	NsCoreBase(cdc::ScriptType *ty) : cdc::ScriptObject(ty) {}
};
