#include "ScriptManager.h"
#include "cdcScript/NativeScriptType.h"

namespace cdc {

ScriptManager *ScriptManager::s_instance = nullptr;

void ScriptManager::RegisterNativeScriptType(NativeScriptType *nt) {
	scriptTypeMap[nt->package][nt->name] = nt;
}

NativeScriptType *ScriptManager::GetNativeScriptType(const char *packageName, const char *nativeName) {
	if (packageName == nullptr || nativeName == nullptr)
		return nullptr;
	return scriptTypeMap[packageName][nativeName];
}

}
