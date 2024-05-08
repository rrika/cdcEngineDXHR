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

	uint32_t StartResource(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) override; // 0x04
	void ReleaseResource(uint32_t id) override; // 0x14
	void HandleResourceData(uint32_t id, void* src, uint32_t size, uint32_t offset) override; // 0x18
	void HandleResourceEnd(uint32_t id, void *drmSectionHeader) override; // 0x1C
	void construct(uint32_t id, void *drmSectionHeaderMaybe) override; // 0x24
	void* GetBasePointer(uint32_t) override; // 0x30
	void* GetResolveBasePointer(uint32_t) override; // 0x34
	uint32_t FindResource(uint32_t) override;  // 0x44
};

}
