#include <cstdlib>
#include "NativeScriptType.h"
#include "PackageBinaryLoader.h"
#include "ScriptManager.h"
#include "ScriptType.h"

namespace cdc {

void ScriptBinaryLoader::LoadScriptType(ScriptType *ty) {
	// TODO
	m_links.push_back(LinkData { ty, (uint8_t)ty->blob->m_depth });
}

void ScriptBinaryLoader::Link() {
	// sort by inheritance depth
	if (m_links.size() > 2)
		qsort(m_links.data(), m_links.size(), sizeof(LinkData), [](void const *a, void const *b) -> int {
			return ((LinkData*)a)->m_inheritanceDepth - ((LinkData*)b)->m_inheritanceDepth;
		});

	// call InitType and propagate downwards
	for (auto& link : m_links) {
		ScriptType *ty = link.m_script;
		if (ty->nativeScriptType)
			continue;
		auto *nt = ScriptManager::s_instance->GetNativeScriptType(
			ty->blob->m_nativeScriptPackageName,
			ty->blob->m_nativeScriptName);

		if (nt) {
			nt->InitType(ty);

		} else if (auto *pty = ty->getParentType()) {
			// inherit native type from parent
			ty->nativeScriptType = pty->nativeScriptType;

		} else {
			// TODO
		}
	}

	m_links.clear();
}

void ScriptBinaryLoader::RemoveLink(ScriptType *ty) {
	/*TODO*/
}

}
