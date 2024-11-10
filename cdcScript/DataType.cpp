#include "DataType.h"
#include "NativeScriptType.h"
#include "ScriptType.h"

namespace cdc {

ScriptType* DataType::GetScriptType() {
	if (type & 0x80)
		return m_compound->m_script;
	else
		return m_script;
}

NativeScriptType* DataType::GetNativeType() {
	return GetScriptType()->nativeScriptType;
}

}
