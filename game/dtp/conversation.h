#pragma once
#include <cstdint>

namespace dtp {

// nodes

struct ConversationGraphNode { // made-up name
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	const char *typeName;
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
	uint32_t pad[(0x60-0x4) / 4]; // 4
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
