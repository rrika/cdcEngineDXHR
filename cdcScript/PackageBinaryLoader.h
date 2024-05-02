#pragma once
#include <cstdint>
#include <vector>

namespace cdc {

class ScriptType;

class ScriptBinaryLoader {
	struct LinkData {
		ScriptType *m_script;
		uint8_t m_inheritanceDepth;
	};
	std::vector<LinkData> m_links;
public:
	void LoadScriptType(ScriptType *ty);
	void Link();
	void RemoveLink(ScriptType *ty);
};

}
