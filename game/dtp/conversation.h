#pragma once
#include <cstdint>

namespace cdc { class RenderResource; }

namespace dtp {

// nodes

struct BriefingProfile {
	uint16_t nameIndex;
	uint16_t nonPlayer;
	// cdc::RenderResource *texture;
	uint32_t texture;
};

struct BriefingLine {
	BriefingProfile *profile; // 0
	uint32_t dword4;
	uint32_t dword8;
	uint32_t voiceLineIndex; // C
	uint32_t stringIndex; // 10
	const char *untranslated;
};

struct ConversationGraphNode { // made-up name
	const char *count;    // eg. "(099)"
	const char *dtpPath;  // eg. "dtp\conversation\briefingnodes\Briefing_Line.dtp"
	union {
		BriefingLine *line; // 8
	};
	const char *typeName; // eg. "Line"
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t numSuccessors; // 20
	void *successors; // 24
	uint32_t dword28;
};

struct ConversationGraphNodeList {
	uint32_t dword0;
	ConversationGraphNode *nodes;
};

// graphs

struct ConversationBase { // made-up name
	uint16_t word0;
	uint16_t objectListIndex;
};

struct ConversationBriefing : ConversationBase {
	char pad4[0x18-0x4];
	const char *name18;
	char pad1C[0x24-0x1C];
	const char *name24;
	char pad28[0x60-0x28];
	ConversationGraphNodeList nodeList; // 60
};

struct ConversationTier1 : ConversationBase {
	uint32_t pad[(0x60-0x4) / 4]; // 4
	ConversationGraphNodeList nodeList; // 60
};

struct ConversationTier2v2 : ConversationBase {
	uint32_t pad[(0x60-0x4) / 4]; // 4
	ConversationGraphNodeList nodeList; // 60
};

// graph wrappers

struct ConversationGraphBase { // made-up name
	uint32_t dword0;
	uint32_t tier; // 4
};

struct ConversationGraph_Briefing : ConversationGraphBase {
	ConversationBriefing *sub8;
};

struct ConversationGraph : ConversationGraphBase {
	ConversationTier1 *sub8;
};

struct ConversationGraphTier2 : ConversationGraphBase {
	ConversationTier2v2 *sub8;
};

// interactions

struct UberSectionRef {
	uint32_t objectId;
	uint32_t sectionIndex;
	uint32_t shopData;
};

struct Interaction {
	uint32_t dword0;
	ConversationGraphBase *dtpGraph; // 4
	uint32_t conversationData8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint8_t byte1C;
	UberSectionRef shopUberRef;
};

}
