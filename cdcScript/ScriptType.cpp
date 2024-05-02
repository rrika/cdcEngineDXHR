#include <cstdio>
#include "ScriptType.h"
#include "NativeScriptType.h"

namespace cdc {

static const char* nameof(ScriptType *ty) {
	if (!ty)
		return "(null)";
	else if (ty->blob->m_nativeScriptName)
		return ty->blob->m_nativeScriptName;
	else
		return "Unknown";
}

ScriptObject *ScriptType::CreateObject() {
	ScriptType *realNativeTy = getClosestNativeAncestor();
	if (!nativeScriptType) {
		printf("Native script type for '%s' not registered, can't instantiate!\n", nameof(realNativeTy));
		return nullptr;
	}
	if (realNativeTy != nativeScriptType->type) {
		printf("Native script type for '%s' not registered, instantiating '%s' instead.\n",
			nameof(realNativeTy),
			nameof(nativeScriptType->type));
	}
	ScriptObject *obj = nativeScriptType->CreateObject(this);
	// obj->OnConstruct();
	return obj;
}

ScriptType *ScriptType::getClosestNativeAncestor() {
	for (ScriptType *t = this; t; t = t->blob->m_superScriptType)
		if (t->blob->m_nativeScriptName)
			return t;

	return nullptr;
}

}
