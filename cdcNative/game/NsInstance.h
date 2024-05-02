#pragma once
#include "cdcNative/game/NsGameBase.h"

class Instance;

class NsInstance : public NsGameBase {
public:
	Instance *m_instance = nullptr; // 20

	NsInstance(cdc::ScriptType *ty) : NsGameBase(ty) {}
};
