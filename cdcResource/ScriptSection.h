#pragma once
#include <unordered_map>
#include "cdcResource/ResolveSection.h"
#include "cdcScript/PackageBinaryLoader.h"
#include "cdcSys/RCObject.h" // for Handle<>

namespace cdc {

class ScriptType;

class ScriptSection : public ResolveSection {
	// cdc::HashMap<uint32_t, Handle<ScriptType>> s_scripts;
	std::unordered_map<uint32_t, Handle<ScriptType>> s_scripts;
	ScriptBinaryLoader s_loader;

public:
	ScriptType *FindScript(uint32_t);

	uint32_t realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void fill(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* getWrapped(uint32_t) override; // 0x30
	void* getBlob(uint32_t) override; // 0x34
	uint32_t getDomainId(uint32_t) override;  // 0x44
};

}
