#include <cstdio>
#include <cstring>
#include "ScriptSection.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcObjects/ObjectManager.h"
#include "cdcResource/ResolveObject.h"
#include "cdcScript/NativeScriptType.h"
#include "cdcScript/ScriptManager.h"
#include "cdcScript/ScriptType.h"
#include "cdcWorld/Object.h"

namespace cdc {

ScriptType *ScriptSection::FindScript(uint32_t domainId) { // 253
	auto it = s_scripts.find(domainId);
	if (it == s_scripts.end())
		return nullptr;
	return it->second.Get(); // this might run RemReference on the HandleData pointer
}

uint32_t ScriptSection::StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	if (ScriptType *ty = FindScript(sectionId)) {
 		alreadyLoaded = true;
		ty->AddReference();
		return sectionId;
	}

	ScriptType *ty = new ScriptType(size);

	// call Handle::Handle
	Handle<ScriptType> newHandle(ty);

	// call operator[] which may call Handle::Handle
	// then Handle::operator= which may call RemReference (not in practice though, sectionId is not in s_script on this path)
	s_scripts[sectionId] = newHandle;

	// call Handle::~Handle for newHandle which will call RemReference
	return sectionId;
}

void ScriptSection::HandleResourceData(uint32_t domainId, void* src, size_t size, size_t offset) {
	ScriptType *ty = FindScript(domainId);
	memcpy(((char*)ty->blob) + offset, src, size);
	// return size;
}

void ScriptSection::construct(uint32_t domainId, void *) {
	s_loader.Link();

	// HACK
	ScriptType *ty = FindScript(domainId);
	auto *nt = ScriptManager::s_instance->GetNativeScriptType(
		ty->blob->m_nativeScriptPackageName,
		ty->blob->m_nativeScriptName);

	if (nt) {
		nt->InitType(ty);
	}
}

void *ScriptSection::GetBasePointer(uint32_t domainId) {
	return FindScript(domainId);
}

void *ScriptSection::GetResolveBasePointer(uint32_t domainId) {
	if (ScriptType *ty = FindScript(domainId))
		return ty->blob;
	return nullptr;
}

uint32_t ScriptSection::FindResource(uint32_t sectionId) {
	if (FindScript(sectionId))
		return sectionId;
	return ~0u;
}

}
