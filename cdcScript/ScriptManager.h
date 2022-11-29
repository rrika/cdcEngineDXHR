#pragma once
#include <string>
#include <unordered_map>
#include "cdcSys/RCObject.h"

namespace cdc {

class NativeScriptType;

class ScriptManager : public RCObject {
public:
	static ScriptManager *s_instance;

	// using NativeScriptTypes = HashMap<ScriptSymbol, ArrayMap<ScriptSymbol, RCPtr<NativeScriptType> > >; // 175
	using NativeScriptTypes = std::unordered_map<std::string, std::unordered_map<std::string, NativeScriptType* > >; // 175

	NativeScriptTypes scriptTypeMap; // 18
	// NativeScriptType *defaultNativeScript; // 2C	
	// NativeScriptType *defaultNativeType;   // 30
	// ...

	ScriptManager() {
		// TODO
	}

	void RegisterNativeScriptType(NativeScriptType *nt);
	NativeScriptType *GetNativeScriptType(const char *packageName, const char *nativeName);
};

}
